/*
 * DMarked
 *
 * Copyright (C) 2021 DMarked.
 * Authors: Lu Hongxu <lhongxu@outlook.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "editorwidget.h"

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
        QList<QTextEdit::ExtraSelection> extraSelections;

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
