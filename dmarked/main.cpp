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
