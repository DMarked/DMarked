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
#include <QPageLayout>
#include "utils/mappagesize.h"

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

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption write_md_file(QStringList() << "w" << "write", "open markdown file in DMarked", "file");
    parser.addOption(write_md_file);
    //
    QCommandLineOption out_put_format(QStringList() << "f" << "format", "convert markdown file to pdf/html", "pdf");
    parser.addOption(out_put_format);
    QCommandLineOption use_landscape(QStringList() << "L" << "use-landscape", "Set orientation to Landscape, which Portrait");
    parser.addOption(use_landscape);
    QCommandLineOption page_size(QStringList() << "s" << "page-size", "Set paper size to: A4, Letter, etc.", "A4");
    parser.addOption(page_size);
    QCommandLineOption margin_left(QStringList() << "l" << "margin-left", "Set left margin", "10");
    parser.addOption(margin_left);
    QCommandLineOption margin_top(QStringList() << "t" << "margin-top", "Set top margin", "10");
    parser.addOption(margin_top);
    QCommandLineOption margin_right(QStringList() << "r" << "margin-right", "Set right margin", "10");
    parser.addOption(margin_right);
    QCommandLineOption margin_bottom(QStringList() << "b" << "margin-bottom", "Set bottom margin", "10");
    parser.addOption(margin_bottom);
    QCommandLineOption max_depth(QStringList() << "d" << "max-depth", "Maximum depth of search directory", "1");
    parser.addOption(max_depth);
    //
    parser.addPositionalArgument("source", DApplication::translate("AppMain", "Source file to copy."));
    parser.addPositionalArgument("[destination]", DApplication::translate("AppMain", "Destination directory."));
    parser.process(app);

    /*      case0:  open a markdown file.        */
    if (parser.isSet(write_md_file)) {
        QString wfile = parser.value("w");
        app.activateWindow();
        app.mainWindow()->openFile(wfile);
        return app.exec();
    }

    /*      case1:  convert a markdown file.        */
    if (parser.isSet(out_put_format)) {
        QString format = parser.value("f");
        int mDepth = parser.isSet("d") ? parser.value("d").toInt() : 1;
        const QStringList args = parser.positionalArguments();
        if (args.empty()) {
            dError() << "Input File is Empty";
            return -1;
        }
        bool isfile = args[0].right(3) == ".md";
        if (isfile)
            mDepth = 0;
        QString soure(args[0]), destination;
        if (args.length() < 2) {
            destination = isfile ? args[0].left(args[0].length()-3)+"."+format : args[0];
        } else {
            destination = args[1];
        }
        //
        if (format == "html") {
            app.activateWindow(false);
            app.md2html(soure, destination, mDepth);
            dWarning() << "to html";
        }
        else if (format == "pdf") {
            bool isLandscape = parser.isSet("L");
            QString pSize = parser.isSet("s") ? parser.value("s") : "A4";

            int mL = parser.isSet("l") ? parser.value("l").toInt() : 10;
            int mT = parser.isSet("t") ? parser.value("t").toInt() : 10;
            int mR = parser.isSet("r") ? parser.value("r").toInt() : 10;
            int mB = parser.isSet("b") ? parser.value("b").toInt() : 10;
            app.activateWindow(false);
            QPageLayout pageLayout(QPageSize(MapPageSize::getInstance()->mapPageSize.value(pSize))
                                 , isLandscape ? QPageLayout::Landscape : QPageLayout::Portrait
                                 , QMarginsF(mL, mT, mR, mB));
            app.md2pdf(soure, destination, pageLayout, mDepth);
            dWarning() << "toPdf: " << isLandscape << " " << pSize << mL << mT << mR << mB << soure << destination << mDepth;
        } else {
            dError() << "Don't support format: " << format;
            return -1;
        }
        return 0;
    }

    app.activateWindow();
    return app.exec();
}
