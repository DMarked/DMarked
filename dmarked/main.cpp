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
    const char *descriptionText = QT_TRANSLATE_NOOP(
        "MainWindow", "Markdown Editor is a powerful tool for viewing and editing Markdown files.");

    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    // TODO: app.loadTranslator();
    app.setOrganizationName("deepin");
    app.setApplicationName("DMarked");
    app.setApplicationDisplayName("Markdown Editor");
    app.setApplicationVersion(DApplication::buildVersion("0.0.1"));
    // TODO 临时借用一下 logo
    app.setProductIcon(QIcon::fromTheme("deepin-editor"));
    app.setProductName(DApplication::translate("MainWindow", "Markdown Editor"));
    app.setApplicationDescription(DApplication::translate("MainWindow", descriptionText) + "\n");
    app.setApplicationAcknowledgementPage("https://github.com/wineee/DMarked");
    app.setQuitOnLastWindowClosed(false);

    qputenv("DTK_USE_SEMAPHORE_SINGLEINSTANCE", "1");

    /*
    DGuiApplicationHelper::instance()->setSingleInstanceInterval(-1);
    if (!DGuiApplicationHelper::instance()->setSingleInstance(
                app.applicationName(),
                DGuiApplicationHelper::UserScope)) {
        return 0;
    }*/

    DApplicationSettings settings;

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();
    app.activateWindow();

    return app.exec();
}
