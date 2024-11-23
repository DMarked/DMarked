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

#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "editor-widget/editorwidget.h"
#include "preview-widget/previewwidget.h"

#include <DWidget>
#include <DSplitter>
#include <QLayout>
#include <DFileSystemWatcher>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

class CentralWidget : public DWidget
{
    Q_OBJECT

    friend class MainWindow;
public:
    explicit CentralWidget(DWidget *parent = nullptr);
    void setMode(const QString &mode);
    void setSync(bool enable) { m_isSync = enable; }

    void setFontSize(int size);
    void setFontFamily(const QString &fontName);
    void setTabSpaceNumber(int num);
    void updateFont();

    void setFilePath(const QString &path);
    const QString &getFilePath() { return m_filePath; }

public Q_SLOTS:
    void onFileModified(const QString &path, const QString &name);

private:
    EditorWidget *m_editorWidget;
    PreviewWidget *m_previewWidget;
    DSplitter *m_splitter;
    QHBoxLayout *m_centralLayout;

    QString m_filePath;
    bool m_isSync = false;

    QString m_fontName;
    int m_fontSize;
    int m_tabSpaceNumber;

    DFileSystemWatcher *m_fileWatcher;
};

#endif // CENTRALWIDGET_H
