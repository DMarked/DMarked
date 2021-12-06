/*
    Copyright (c) 2017, Lukas Holecek <hluk@email.cz>

    This file is part of CopyQ.

    CopyQ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CopyQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CopyQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "fakevimproxy.h"

#include <FakeVim/fakevimactions.h>
#include <FakeVim/fakevimhandler.h>

#include <QtCore/QTemporaryFile>
#include <QtCore/QTextStream>
#include <QtGui/QTextBlock>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
    #include<QRegularExpression>
#else
    #include<QRegExp>
#endif

FakeVimProxy::FakeVimProxy(QWidget *widget, MainWindow *mw, QObject *parent)
    : QObject(parent), m_widget(widget), m_mainWindow(mw) {}

void FakeVimProxy::changeStatusData(const QString &info) {
    m_statusData = info;
    updateStatusBar();
}

void FakeVimProxy::changeExtraInformation(const QString &info) {
    QMessageBox::information(m_widget, tr("Information"), info);
}

void FakeVimProxy::highlightMatches(const QString &pattern) {
    QPlainTextEdit *ed = qobject_cast<QPlainTextEdit *>(m_widget);
    if (!ed) return;

    QTextCursor cur = ed->textCursor();

    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(Qt::yellow);
    selection.format.setForeground(Qt::black);

    // Highlight matches.
    QTextDocument *doc = ed->document();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
    QRegularExpression re(pattern);
#else
    QRegExp re(pattern);
#endif
    cur = doc->find(re);

    m_searchSelection.clear();

    int a = cur.position();
    while (!cur.isNull()) {
        if (cur.hasSelection()) {
            selection.cursor = cur;
            m_searchSelection.append(selection);
        } else {
            cur.movePosition(QTextCursor::NextCharacter);
        }
        cur = doc->find(re, cur);
        int b = cur.position();
        if (a == b) {
            cur.movePosition(QTextCursor::NextCharacter);
            cur = doc->find(re, cur);
            b = cur.position();
            if (a == b) break;
        }
        a = b;
    }

    updateExtraSelections();
}

void FakeVimProxy::changeStatusMessage(const QString &contents, int cursorPos, int anchorPos, int messageLevel) {
    m_statusMessage =
        cursorPos == -1
            ? contents
            : contents.left(cursorPos) + QChar(10073) + contents.mid(cursorPos);
    updateStatusBar();
}

void FakeVimProxy::updateStatusBar() {
    int slack = 80 - m_statusMessage.size() - m_statusData.size();
    QString msg =
        m_statusMessage + QString(slack, QLatin1Char(' ')) + m_statusData;
    ////m_mainWindow->statusBar()->showMessage(msg);
}

void FakeVimProxy::handleExCommand(bool *handled,
                                   const FakeVim::Internal::ExCommand &cmd) {
    if (wantSaveAndQuit(cmd)) {
        // :wq
        if (save()) cancel();
    } else if (wantSave(cmd)) {
        save();    // :w
    } else if (wantQuit(cmd)) {
        if (cmd.hasBang)
            invalidate();    // :q!
        else
            cancel();    // :q
    } else {
        *handled = false;
        return;
    }

    *handled = true;
}

void FakeVimProxy::requestSetBlockSelection(const QTextCursor &tc) {
    auto *ed = qobject_cast<QPlainTextEdit *>(m_widget);
    if (!ed) return;

    QPalette pal = ed->parentWidget() != nullptr ? ed->parentWidget()->palette()
                                                 : QApplication::palette();

    m_blockSelection.clear();
    m_clearSelection.clear();

    QTextCursor cur = tc;

    QTextEdit::ExtraSelection selection = QTextEdit::ExtraSelection();
    selection.format.setBackground(pal.color(QPalette::Base));
    selection.format.setForeground(pal.color(QPalette::Text));
    selection.cursor = cur;
    m_clearSelection.append(selection);

    selection.format.setBackground(pal.color(QPalette::Highlight));
    selection.format.setForeground(pal.color(QPalette::HighlightedText));

    int from = cur.positionInBlock();
    int to = cur.anchor() - cur.document()->findBlock(cur.anchor()).position();
    const int min = qMin(cur.position(), cur.anchor());
    const int max = qMax(cur.position(), cur.anchor());
    for (QTextBlock block = cur.document()->findBlock(min);
         block.isValid() && block.position() < max; block = block.next()) {
        cur.setPosition(block.position() + qMin(from, block.length()));
        cur.setPosition(block.position() + qMin(to, block.length()),
                        QTextCursor::KeepAnchor);
        selection.cursor = cur;
        m_blockSelection.append(selection);
    }

    disconnect(ed, &QPlainTextEdit::selectionChanged, this,
               &FakeVimProxy::updateBlockSelection);
    ed->setTextCursor(tc);
    connect(ed, &QPlainTextEdit::selectionChanged, this,
            &FakeVimProxy::updateBlockSelection);

    QPalette pal2 = ed->palette();
    pal2.setColor(QPalette::Highlight, Qt::transparent);
    pal2.setColor(QPalette::HighlightedText, Qt::transparent);
    ed->setPalette(pal2);

    updateExtraSelections();
}

void FakeVimProxy::requestDisableBlockSelection() {
    auto *ed = qobject_cast<QPlainTextEdit *>(m_widget);
    if (!ed) return;

    QPalette pal = ed->parentWidget() != nullptr ? ed->parentWidget()->palette()
                                                 : QApplication::palette();

    m_blockSelection.clear();
    m_clearSelection.clear();

    ed->setPalette(pal);

    disconnect(ed, &QPlainTextEdit::selectionChanged, this,
               &FakeVimProxy::updateBlockSelection);

    updateExtraSelections();
}

void FakeVimProxy::updateBlockSelection() {
    auto *ed = qobject_cast<QPlainTextEdit *>(m_widget);
    if (!ed) return;

    requestSetBlockSelection(ed->textCursor());
}

void FakeVimProxy::requestHasBlockSelection(bool *on) {
    *on = !m_blockSelection.isEmpty();
}

void FakeVimProxy::indentRegion(int beginBlock, int endBlock, QChar typedChar) {
    auto *ed = qobject_cast<QPlainTextEdit *>(m_widget);
    if (!ed) return;

    const qint64 indentSize = FakeVim::Internal::fakeVimSettings()->shiftWidth.value();

    QTextDocument *doc = ed->document();
    QTextBlock startBlock = doc->findBlockByNumber(beginBlock);

    // Record line lengths for mark adjustments
    QVector<int> lineLengths(endBlock - beginBlock + 1);
    QTextBlock block = startBlock;

    for (int i = beginBlock; i <= endBlock; ++i) {
        const auto line = block.text();
        lineLengths[i - beginBlock] = line.length();
        if (typedChar.unicode() == 0 && line.simplified().isEmpty()) {
            // clear empty lines
            QTextCursor cursor(block);
            while (!cursor.atBlockEnd()) cursor.deleteChar();
        } else {
            const auto previousBlock = block.previous();
            const auto previousLine =
                previousBlock.isValid() ? previousBlock.text() : QString();

            qint64 indent = firstNonSpace(previousLine);
            if (typedChar == '}')
                indent = std::max(0, int(indent - indentSize));
            else if (previousLine.endsWith(QLatin1String("{")))
                indent += indentSize;
            const auto indentString = QStringLiteral(" ").repeated(indent);

            QTextCursor cursor(block);
            cursor.beginEditBlock();
            cursor.movePosition(QTextCursor::StartOfBlock);
            cursor.movePosition(QTextCursor::NextCharacter,
                                QTextCursor::KeepAnchor, firstNonSpace(line));
            cursor.removeSelectedText();
            cursor.insertText(indentString);
            cursor.endEditBlock();
        }
        block = block.next();
    }
}

void FakeVimProxy::checkForElectricCharacter(bool *result, QChar c) {
    *result = c == '{' || c == '}';
}

int FakeVimProxy::firstNonSpace(const QString &text) {
    int indent = 0;
    while (indent < text.length() && text.at(indent) == ' ') ++indent;
    return indent;
}

void FakeVimProxy::updateExtraSelections() {
    auto *ed = qobject_cast<QPlainTextEdit *>(m_widget);
    if (ed)
        ed->setExtraSelections(m_clearSelection + m_searchSelection +
                               m_blockSelection);
}

bool FakeVimProxy::wantSaveAndQuit(const FakeVim::Internal::ExCommand &cmd) {
    return cmd.cmd == QLatin1String("wq");
}

bool FakeVimProxy::wantSave(const FakeVim::Internal::ExCommand &cmd) {
    return cmd.matches(QStringLiteral("w"), QStringLiteral("write")) ||
           cmd.matches(QStringLiteral("wa"), QStringLiteral("wall"));
}

bool FakeVimProxy::wantQuit(const FakeVim::Internal::ExCommand &cmd) {
    return cmd.matches(QStringLiteral("q"), QStringLiteral("quit")) ||
           cmd.matches(QStringLiteral("qa"), QStringLiteral("qall"));
}

void FakeVimProxy::cancel() { invalidate(); }

bool FakeVimProxy::save() {
    //// m_mainWindow->storeUpdatedNotesToDisk();
    return true;
}

void FakeVimProxy::invalidate() { QApplication::quit(); }

bool FakeVimProxy::hasChanges() { return false; }

QTextDocument *FakeVimProxy::document() const {
    QTextDocument *doc = nullptr;
    if (auto *ed = qobject_cast<QPlainTextEdit *>(m_widget))
        doc = ed->document();
    else if (auto *ed = qobject_cast<QPlainTextEdit *>(m_widget))
        doc = ed->document();
    return doc;
}

QString FakeVimProxy::content() const { return document()->toPlainText(); }
