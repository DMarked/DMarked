/*
 * DMarked
 *
 * Copyright (C) 2021. Lu Hongxu
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmarkdowntextedit.h"
#include <QString>
#include <DTitlebar>
#include <DSearchEdit>
#include <DMainWindow>
#include <DGuiApplicationHelper>
#include <QAction>
#include <QEventLoop>
#include <QTimer>

#include "widgets/centralwidget.h"
#include "widgets/bottombar.h"

DWIDGET_USE_NAMESPACE

enum CLI_STATE {
    NONE,
    PDF,
    HTML
};

struct CliTool {
    QEventLoop loop;
    QTimer timer;
    QString topath;
    QPageLayout pageLayout;
    CLI_STATE state;
};

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void openFile(const QString &path);

    bool md2html(QString mdpath, QString htmlpath);
    bool md2pdf(QString mdpath, QString pdfpath, QPageLayout pageLayout);

private slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onToPdf();
    void onToHtml();

public:
    bool isModified() const;
    bool queryClose();
    void setNoGui();

private:
    void setupAction();

    DSearchEdit *m_search_edit;
    CentralWidget *m_central_widget;
    BottomBar *m_bottom_bar;
    CliTool ct;
};

#endif // MAINWINDOW_H
