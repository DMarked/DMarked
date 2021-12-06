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

#ifndef FAKEVIMPROXY_H
#define FAKEVIMPROXY_H

#include <QObject>
#include <QTextEdit>

class MainWindow;
class QWidget;
class QTextDocument;
class QTextCursor;

namespace FakeVim {
namespace Internal {
class ExCommand;
}
}    // namespace FakeVim

class FakeVimProxy : public QObject {
    Q_OBJECT

   public:
    FakeVimProxy(QWidget *widget, MainWindow *mw, QObject *parent = 0);

   signals:
    void handleInput(const QString &keys);

   public slots:
    void changeStatusData(const QString &info);

    void highlightMatches(const QString &pattern);

    void changeStatusMessage(const QString &contents, int cursorPos, int anchorPos, int messageLevel);

    void changeExtraInformation(const QString &info);

    void updateStatusBar();

    void handleExCommand(bool *handled,
                         const FakeVim::Internal::ExCommand &cmd);

    void requestSetBlockSelection(const QTextCursor &tc);

    void requestDisableBlockSelection();

    void updateBlockSelection();

    void requestHasBlockSelection(bool *on);

    void indentRegion(int beginBlock, int endBlock, QChar typedChar);

    void checkForElectricCharacter(bool *result, QChar c);

   private:
    static int firstNonSpace(const QString &text);

    void updateExtraSelections();

    bool wantSaveAndQuit(const FakeVim::Internal::ExCommand &cmd);

    bool wantSave(const FakeVim::Internal::ExCommand &cmd);

    bool wantQuit(const FakeVim::Internal::ExCommand &cmd);

    bool save();

    void cancel();

    void invalidate();

    bool hasChanges();

    QTextDocument *document() const;

    QString content() const;

    QWidget *m_widget;
    MainWindow *m_mainWindow;
    QString m_statusMessage;
    QString m_statusData;

    QList<QTextEdit::ExtraSelection> m_searchSelection;
    QList<QTextEdit::ExtraSelection> m_clearSelection;
    QList<QTextEdit::ExtraSelection> m_blockSelection;
    typedef QLatin1String _;
};

#endif // FAKEVIMPROXY_H
