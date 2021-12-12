/*
 * DMarked
 *
 * Copyright (C) 2021 DMarked.
 * Authors: Lu Hongxu <lhongxu@outlook.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
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
#include "widgets/centralwidget.h"
#include "widgets/bottombar.h"
#include "settings.h"

#include <QString>
#include <DTitlebar>
#include <DSearchEdit>
#include <DMainWindow>
#include <DGuiApplicationHelper>
#include <QAction>
#include <QEventLoop>
#include <QTimer>
#include <DSettingsWidgetFactory>
#include <DSettingsGroup>
#include <DSettings>
#include <DSettingsOption>
#include <QProcess>

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
    void popupSettingsDialog();

    bool md2html(QString mdpath, QString htmlpath); // only use in cli
    bool md2pdf(QString mdpath, QString pdfpath, QPageLayout pageLayout);

private Q_SLOTS:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onToPdf();
    void onToHtml();
    void onOpenHelpFile();

public:
    bool isModified() const;
    bool queryClose();
    void setNoGui();
    void showCenterWindow(bool bIsCenter);

    void toggleFullscreen();
    void resetFontSize();
    void incrementFontSize();
    void decrementFontSize();
    void displayShortcuts();

    // FakeVim Need
    void updateStatusBarMessage(const QString &msg);
    void storeUpdatedNotesToDisk();

    void setupAutoSave(bool enable, bool asingleShot, int intervalMSec);
private:
    void setupAction();

    DSearchEdit *m_search_edit;
    CentralWidget *m_central_widget;
    BottomBar *m_bottom_bar;
    Settings *m_settings;
    QTimer *m_autoSaveTimer;
    CliTool ct;

    int m_font_size;
    QProcess *m_shortcutViewProcess = nullptr;

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H
