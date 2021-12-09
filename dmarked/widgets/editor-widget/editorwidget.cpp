/*
 * DMarked
 *
 * Copyright (C) 2021 DMarked.
 * Authors: Lu Hongxu <lhongxu@outlook.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "editorwidget.h"
#include "fakevimproxy.h"

#include "mainwindow.h"
#include <FakeVim/fakevimhandler.h>

#include <DGuiApplicationHelper>
using DTK_NAMESPACE::Gui::DGuiApplicationHelper;

EditorWidget::EditorWidget(QWidget *parent):
    QMarkdownTextEdit (parent)
{
    setContentsMargins(0, 0, 0, 0);
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setLineNumbersCurrentLineColor(QColor::fromRgb(255, 0, 0));
    setLineNumbersOtherLineColor(QColor::fromRgb(0, 0, 255));

    bool isDark = DGuiApplicationHelper::instance()->applicationPalette().color(QPalette::Background).lightness() < 128;
    m_highlightLineColor = isDark ? QColor(Qt::darkGray).darker(160)
                                  : QColor(Qt::darkGray).lighter(160);
    connect(this, &EditorWidget::cursorPositionChanged, this, &EditorWidget::highlightCurrentLine);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            [this](DGuiApplicationHelper::ColorType themeType) {
        bool isDark = themeType == DGuiApplicationHelper::ColorType::DarkType;
        m_highlightLineColor = isDark ? QColor(Qt::darkGray).darker(160)
                                      : QColor(Qt::darkGray).lighter(160);
        highlightCurrentLine();
    });

    setExtraSelections({});
}

#include <QStandardPaths>
void EditorWidget::initFakeVim(MainWindow *mw)
{
    // Create FakeVimHandler instance which will emulate Vim behavior in editor widget.
    auto handler = new FakeVim::Internal::FakeVimHandler(this, /*parent=*/mw);
    // Connect slots to FakeVimHandler signals.
    FakeVimProxy *proxy = FakeVimProxy::connectSignals(handler, mw, this);

    // regular signal
    QObject::connect(proxy, &FakeVimProxy::handleInput, handler,
                     [handler](const QString &text) { handler->handleInput(text); });

    // Load vimrc if it exists
    QString vimRcPath = QString("%1/%2/.vimrc")
                            .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
                            .arg(qApp->applicationName());
    if (QFile::exists(vimRcPath)) {
        handler->handleCommand(QLatin1String("source ") + vimRcPath);
    } else {
        // Set some Vim options.
        handler->handleCommand(QLatin1String("set expandtab"));
        handler->handleCommand(QLatin1String("set shiftwidth=8"));
        handler->handleCommand(QLatin1String("set tabstop=16"));
        handler->handleCommand(QLatin1String("set autoindent"));
        handler->handleCommand(QLatin1String("set smartindent"));
    }
    // Initialize FakeVimHandler.
    handler->handleCommand(QLatin1String("set nopasskeys"));
    handler->handleCommand(QLatin1String("set nopasscontrolkey"));
    handler->installEventFilter();
    handler->setupWidget();

    // Clear undo and redo queues.
    setUndoRedoEnabled(false);
    setUndoRedoEnabled(true);

    setFocus();
}

void EditorWidget::setHighlightCurrentLineEnabled(bool enable)
{
    m_bHighlightCurrentLine = enable;
    if (!enable) {
        setExtraSelections({});
    } else {
        highlightCurrentLine();
    }
}

void EditorWidget::highlightCurrentLine()
{
    if (m_bHighlightCurrentLine) {
        QList<QTextEdit::ExtraSelection> extraSelections = this->extraSelections();

        if (!isReadOnly()) {
            QTextEdit::ExtraSelection selection;
            selection.format.setBackground(m_highlightLineColor);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = textCursor();
            selection.cursor.clearSelection();
            extraSelections.append(selection);
        }

        setExtraSelections(extraSelections);
    }
}
