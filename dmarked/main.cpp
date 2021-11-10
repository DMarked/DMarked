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

#include "mainwindow.h"
#include "application.h"

#include <DApplication>
#include <DWidgetUtil>
#include <DApplicationSettings>
#include <DGuiApplicationHelper>
#include <DMainWindow>
#include <DLog>


DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Translator for qmarkdowntextedit
    QTranslator translator;
    if (translator.load(QString(":/3drtrans/qmarkdowntextedit/qmarkdowntextedit_%1").arg(QLocale::system().name())))
        app.installTranslator(&translator);

    app.loadTranslator();
    app.setOrganizationName("deepin");
    app.setApplicationName("DMarked");
    app.setApplicationVersion(DApplication::buildVersion("0.0.1"));
    app.setProductIcon(QIcon(":/images/dmarked.svg"));
    app.setProductName(DApplication::translate("AppMain", "Markdown Editor"));
    app.setApplicationDisplayName(DApplication::translate("AppMain", "Markdown Editor"));
    app.setApplicationDescription(DApplication::translate("AppMain",
                                                           "Markdown Editor is a powerful tool for viewing and editing Markdown files."));
    app.setApplicationAcknowledgementPage("https://github.com/wineee/DMarked");
    app.setQuitOnLastWindowClosed(true);

    qputenv("DTK_USE_SEMAPHORE_SINGLEINSTANCE", "1");
    //qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "7777");

    DApplicationSettings settings;

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();
    app.activateWindow();

    return app.exec();
}
