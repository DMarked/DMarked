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

#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <qmarkdowntextedit.h>
class MainWindow;

class EditorWidget : public QMarkdownTextEdit
{
public:
    explicit EditorWidget(QWidget *parent = nullptr);
    void setHighlightCurrentLineEnabled(bool enable);
    void initFakeVim(MainWindow *mw);

    // Sync From CentralWidget
    void syncFilePath(const QString &path);

public Q_SLOTS:
    void highlightCurrentLine();

private:
    bool m_bHighlightCurrentLine;
    QColor m_highlightLineColor;
    int m_lastSelectionId = -1;
    void removeLastSelection();

    QString m_filePath;

    // QWidget interface
protected:
    virtual void dropEvent(QDropEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dragMoveEvent(QDragMoveEvent *e) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *e) override;
};

#endif // EDITORWIDGET_H
