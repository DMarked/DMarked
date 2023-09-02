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

#include "mainwindow.h"
#include "widgets/topdfdlg.h"
#include "utils/utils.h"

#include <QFile>
#include <DFileDialog>
#include <DMessageBox>
#include <QTextStream>
#include <QLayout>
#include <DLog>
#include <DSettingsDialog>
#include <DSettingsOption>
#include <DWidgetUtil>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
    DMainWindow(parent),
    m_searchEdit(new DSearchEdit(this)),
    m_centralWidget(new CentralWidget(this)),
    m_bottomBar(new BottomBar(this)),
    m_settings(Settings::instance()),
    m_autoSaveTimer(new QTimer)
{

    resize(1200, 740);
    setWindowIcon(QIcon(":/images/dmarked.jpeg"));

    titlebar()->setCustomWidget(m_searchEdit);
    m_searchEdit->setFixedWidth(400);
    connect(m_searchEdit, &DSearchEdit::textChanged/*editingFinished*/, [this]() {
        QString context = m_searchEdit->text();
        if (!context.isEmpty())
           m_centralWidget->m_editorWidget->doSearch(context);
    });

    // Make editor widget get focus when BottomBar lost it
    connect(m_bottomBar, &BottomBar::bottombarLostFocus, [this]() {
      m_centralWidget->m_editorWidget->setFocus();
    });
    // update WordCount in BottomBar
    connect(m_centralWidget->m_editorWidget, &QMarkdownTextEdit::textChanged, [this]() {
        m_bottomBar->updateWordCount(m_centralWidget->m_editorWidget->toPlainText().length());
    });
    // update Position in BottomBar
    connect(m_centralWidget->m_editorWidget, &QMarkdownTextEdit::cursorPositionChanged, [this]() {
        QTextCursor cursor = m_centralWidget->m_editorWidget->textCursor();
        m_bottomBar->updatePosition(cursor.blockNumber()+1, cursor.columnNumber()+1);
    });
    // onTextChanged
    connect(m_centralWidget->m_editorWidget, &QMarkdownTextEdit::textChanged, this, &MainWindow::onTextChanged);
    // Change Markdown Theme by DropDownMenu
    connect(m_bottomBar, &BottomBar::currentMdThemeChanged, [this](const QString &theme) {
       m_centralWidget->m_previewWidget->setMarkdownTheme(theme);
    });
    // Change Read/Write Mode by DropDownMenu
    connect(m_bottomBar, &BottomBar::currentModeChanged, [this](const QString &mode) {
        m_centralWidget->setMode(mode);
    });
    /***        Layout          ***/
    QVBoxLayout *m_layout = new QVBoxLayout;
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_centralWidget);
    m_layout->addWidget(m_bottomBar);
    DWidget *base_widget = new DWidget;
    base_widget->setLayout(m_layout);
    setCentralWidget(base_widget);
    setupAction();

    /***        setting         ***/
    connect(m_settings, &Settings::sigAdjustFont, [this](QString fontName) {
        m_centralWidget->setFontFamily(fontName);
    });
    m_font_size = SettingsHelper::get("base.font.size").toInt();
    connect(m_settings, &Settings::sigAdjustFontSize, [this](int size) {
        m_font_size = size;
        m_centralWidget->setFontSize(size);
    });
    connect(m_settings, &Settings::sigSetLineNumberShow, [this](bool bIsShow) {
        m_centralWidget->m_editorWidget->setLineNumberEnabled(bIsShow);
    });
    connect(m_settings, &Settings::sigAdjustTabSpaceNumber, [this](int number) {
        m_centralWidget->setTabSpaceNumber(number);
    });
    connect(m_settings, &Settings::sigHightLightCurrentLine, [this](bool enable) {
        m_centralWidget->m_editorWidget->setHighlightCurrentLineEnabled(enable);
    });
    connect(m_settings, &Settings::sigChangeAutoSaveOption, [this]() {
        setupAutoSave();
    });
    connect(m_settings, &Settings::sigChangeWindowSize, [this](QString mode) {
        if (mode == "fullscreen") {
            this->showFullScreen();
        } else if (mode == "window_maximum") {
            this->showNormal();
            this->showMaximized();
        } else {
            this->showNormal();
        }
    });

    /***        autoSave         ***/
    connect(m_autoSaveTimer, &QTimer::timeout, m_autoSaveTimer, [this] {
        if (m_centralWidget->getFilePath().isEmpty()) {
            return;
        }
        this->onFileSave();
    });

    setupAutoSave();

    /***        init FakeVim!         ***/
    bool enableFakeVim = SettingsHelper::get("base.fakevim.enable").toBool();
    if (enableFakeVim)
        m_centralWidget->m_editorWidget->initFakeVim(this);

    /***        code about convert files in cli         ***/
    ct.state = CLI_STATE::NONE;
    connect(m_centralWidget->m_previewWidget, &PreviewWidget::markdownLoadFinished,
             [this]() {
        if (ct.state == CLI_STATE::PDF)
            m_centralWidget->m_previewWidget->convert2Pdf(ct.topath, ct.pageLayout);
        else if (ct.state == CLI_STATE::HTML)
            m_centralWidget->m_previewWidget->convert2Html(ct.topath);
    });
    ct.timer.setSingleShot(true);
    connect(&ct.timer, &QTimer::timeout, &ct.loop, &QEventLoop::quit);  // 异步调用超时退出
    connect(m_centralWidget->m_previewWidget, &PreviewWidget::convert2PdfFinish, &ct.loop, &QEventLoop::quit);  // 异步调用完成退出
    connect(m_centralWidget->m_previewWidget, &PreviewWidget::convert2HtmlFinish, &ct.loop, &QEventLoop::quit);

    // open a default file
    QString history = SettingsHelper::get("advance.editor.browsing_history_file").toString();
    QFile defaultFile(history.isEmpty() ? ":/default.md" : history);
    defaultFile.open(QIODevice::ReadOnly);
    m_centralWidget->m_editorWidget->setPlainText(defaultFile.readAll());
    m_centralWidget->setFilePath(history);
    defaultFile.close();
}

MainWindow::~MainWindow()
{
    m_autoSaveTimer->deleteLater();
}

void MainWindow::setupAutoSave()
{
    if (SettingsHelper::isAutoSave()) {
        dError() << SettingsHelper::getAutoSaveIntervalType() << SettingsHelper::getAutoSaveInterval();
        m_autoSaveTimer->setSingleShot(SettingsHelper::getAutoSaveIntervalType() != "without_modification");
        m_autoSaveTimer->setInterval(SettingsHelper::getAutoSaveInterval());
        m_autoSaveTimer->start();
    } else {
        m_autoSaveTimer->stop();
    }
}

void MainWindow::setupAction()
{
    QMenu *menu, *convertMenu;
    QAction *actionNew, *actionOpen, *actionSave, *actionSaveAs, *actionSetting, *actionHelp;
    QAction *action2Pdf, *action2Html;

    actionNew = new QAction(tr("New"));
    actionOpen = new QAction(tr("Open"));
    actionSave = new QAction(tr("Save"));
    actionSaveAs = new QAction(tr("SaveAs"));
    action2Pdf = new QAction(tr("Pdf"));
    action2Html = new QAction(tr("Html"));
    actionSetting = new QAction(tr("Setting"));
    actionHelp = new QAction(tr("Help"));
    m_recentFilesMenu = new QMenu(tr("Recent files"));

    menu = new QMenu;
    menu->addAction(actionNew);
    menu->addAction(actionOpen);
    convertMenu = new QMenu(tr("Convert"));
    convertMenu->addAction(action2Pdf);
    convertMenu->addAction(action2Html);
    menu->addMenu(convertMenu);
    menu->addAction(actionSave);
    menu->addAction(actionSaveAs);
    menu->addAction(actionSetting);
    menu->addAction(actionHelp);
    menu->addMenu(m_recentFilesMenu);

    titlebar()->setMenu(menu);

    connect(actionNew, &QAction::triggered, this, &MainWindow::onFileNew);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::onFileOpen);
    connect(actionSave, &QAction::triggered, this, &MainWindow::onFileSave);
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::onFileSaveAs);
    connect(m_centralWidget->m_editorWidget->document(), &QTextDocument::modificationChanged,
          actionSave, &QAction::setEnabled);
    connect(actionSetting, &QAction::triggered, this, &MainWindow::popupSettingsDialog);
    connect(actionHelp, &QAction::triggered, this, &MainWindow::onOpenHelpFile);
    connect(action2Pdf, &QAction::triggered, this, &MainWindow::onToPdf);
    connect(action2Html, &QAction::triggered, this, &MainWindow::onToHtml);
    setupRecentFilesMenu();
}

void MainWindow::setupRecentFilesMenu()
{
    m_recentFilesMenu->clear();
    QStringList paths = SettingsHelper::get("advance.editor.recent_files_list").toStringList();
    foreach (const QString &path, paths) {
        QAction *filePathAction = new QAction(path);
        connect(filePathAction, &QAction::triggered, this, [this, filePathAction]() {
            openFile(filePathAction->text());
        });
        m_recentFilesMenu->addAction(filePathAction);
    }
}

void MainWindow::popupSettingsDialog()
{
    DSettingsDialog *dialog = new DSettingsDialog(this);
    dialog->widgetFactory()->registerWidget("fontcombobox", Settings::createFontComBoBoxHandle);
    dialog->widgetFactory()->registerWidget("keySequenceEdit", Settings::createKeySequenceEditHandle);

    m_settings->setSettingDialog(dialog);
    dialog->updateSettings(m_settings->settings);
    dialog->exec();

    delete dialog;
    m_settings->settings->sync();
}

void MainWindow::setNoGui()
{
    /* Call this function when running on the command line */
    m_centralWidget->m_previewWidget->setNoGui();
}

void MainWindow::updateStatusBarMessage(const QString &msg)
{
    m_bottomBar->updateVimMessage(msg);
}

void MainWindow::storeUpdatedNotesToDisk()
{
    onFileSave();
}

bool MainWindow::md2html(QString mdpath, QString htmlpath) {
    ct.state = CLI_STATE::HTML;
    ct.topath = htmlpath;
    QFile f(mdpath);
    if (!f.open(QIODevice::ReadOnly)) {
        dDebug() << tr("Could not open file %1: %2").arg(QDir::toNativeSeparators(mdpath), f.errorString());
        return false;
    }

    m_centralWidget->setFilePath(mdpath);
    m_centralWidget->m_editorWidget->setPlainText(f.readAll());

    // wait for finish
    const int timeout = 30 * 1000;
    ct.timer.start(timeout);
    ct.loop.exec();
    ct.timer.stop();
    return true;
}

bool MainWindow::md2pdf(QString mdpath, QString pdfpath, QPageLayout pageLayout) {
    //dError() << "In2pdf: " << mdpath << pdfpath;
    ct.state = CLI_STATE::PDF;
    ct.topath = pdfpath;
    ct.pageLayout = pageLayout;

    QFile f(mdpath);
    if (!f.open(QIODevice::ReadOnly)) {
        dDebug() << tr("Could not open file %1: %2").arg(QDir::toNativeSeparators(mdpath), f.errorString());
        return false;
    }
    m_centralWidget->setFilePath(mdpath);
    m_centralWidget->m_editorWidget->setPlainText(f.readAll());

    // wait for finish
    const int timeout = 30 * 1000;
    ct.timer.start(timeout);
    ct.loop.exec();
    ct.timer.stop();
    //dError() << "Out2pdf: " << mdpath << pdfpath;
    return true;
}

void MainWindow::openFile(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        DMessageBox::warning(this
                           , windowTitle()
                           , tr("Could not open file %1: %2").arg(
                                 QDir::toNativeSeparators(path), f.errorString()));
        return;
    }
    m_centralWidget->setFilePath(path);
    m_centralWidget->m_editorWidget->setPlainText(f.readAll());
    SettingsHelper::set("advance.editor.browsing_history_file", path);
    if (SettingsHelper::addRecentFiles(path))
        setupRecentFilesMenu();
}

bool MainWindow::isModified() const
{
    return m_centralWidget->m_editorWidget->document()->isModified();
}

bool MainWindow::queryClose()
{
    if (isModified()) {
        DMessageBox::StandardButton button = DMessageBox::question(this, windowTitle()
                            , tr("You have unsaved changes. Do you want to exit anyway?"));
        return button == DMessageBox::Yes;
    }
    return true;
}

void MainWindow::onToPdf()
{
    ToPdfDlg dlg(this);
    int res = dlg.exec();
    if (res != QDialog::Accepted)
        return;

    QPageLayout pageLayout(dlg.getPageSize()
                         , dlg.getOrientation()
                         , QMarginsF(dlg.getMarginLeft(),
                                     dlg.getMarginTop(),
                                     dlg.getMarginRight(),
                                     dlg.getMarginButtom()));

    QString path = DFileDialog::getSaveFileName(this
                                              , tr("Convert to PDF")
                                              , Utils::getDefaultDlgFilePath(m_centralWidget->getFilePath())
                                              , tr("PDF File (*.pdf)"));
    if (path.isEmpty())
        return;
    m_centralWidget->m_previewWidget->convert2Pdf(path, pageLayout);
}

void MainWindow::onToHtml()
{
    QString path = DFileDialog::getSaveFileName(this
                                              , tr("Convert to HTML")
                                              , Utils::getDefaultDlgFilePath(m_centralWidget->getFilePath())
                                              , tr("HTML File (*.html)"));
    if (path.isEmpty())
        return;
    m_centralWidget->m_previewWidget->convert2Html(path);
}

void MainWindow::onFileNew()
{
    if (isModified()) {
        DMessageBox::StandardButton button = DMessageBox::question(this, windowTitle()
                            , tr("You have unsaved changes. Do you want to create a new document anyway?"));
        if (button != DMessageBox::Yes)
            return;
    }

    m_centralWidget->setFilePath("");
    m_centralWidget->m_editorWidget->setPlainText(tr("## New document"));
    m_centralWidget->m_editorWidget->document()->setModified(false);
}

void MainWindow::onFileOpen()
{
    if (isModified()) {
        DMessageBox::StandardButton button = DMessageBox::question(this, windowTitle()
                            , tr("You have unsaved changes. Do you want to open a new document anyway?"));
        if (button != DMessageBox::Yes)
            return;
    }

    QString path = DFileDialog::getOpenFileName(this
                                              , tr("Open MarkDown File")
                                              , Utils::getDefaultDlgFilePath(m_centralWidget->getFilePath())
                                              , "MarkDown File (*.markdown, *.md)");
    if (path.isEmpty())
        return;

    openFile(path);
}

void MainWindow::onFileSave()
{
    if (m_centralWidget->getFilePath().isEmpty()) {
        onFileSaveAs();
        return;
    }

    QFile f(m_centralWidget->getFilePath());
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))  {
        DMessageBox::warning(this
                           , windowTitle()
                           , tr("Could not write to file %1: %2").arg(
                                 QDir::toNativeSeparators(m_centralWidget->getFilePath()), f.errorString()));
        return;
    }
    QTextStream str(&f);
    str << m_centralWidget->m_editorWidget->toPlainText();
    m_centralWidget->m_editorWidget->document()->setModified(false);
}

void MainWindow::onFileSaveAs()
{

    QString path = DFileDialog::getSaveFileName(this
                                              , tr("Save MarkDown File")
                                              , Utils::getDefaultDlgFilePath(m_centralWidget->getFilePath())
                                              , "MarkDown File (*.markdown, *.md)");
    if (path.isEmpty())
        return;
    m_centralWidget->setFilePath(path);
    SettingsHelper::set("advance.editor.browsing_history_file", path);
    if (SettingsHelper::addRecentFiles(path))
        setupRecentFilesMenu();
    onFileSave();
}

void MainWindow::onOpenHelpFile()
{
    QFile helpFile(":/default.md");
    helpFile.open(QIODevice::ReadOnly);
    m_centralWidget->setFilePath("");
    m_centralWidget->m_editorWidget->setPlainText(helpFile.readAll());
}

void MainWindow::onTextChanged()
{
    if (SettingsHelper::isAutoSave() && SettingsHelper::getAutoSaveIntervalType() != "without_modification" &&
           (!m_autoSaveTimer->isActive() || SettingsHelper::getAutoSaveIntervalType() == "after_the_last_modification"))
       {
           m_autoSaveTimer->start();
       }
}

void MainWindow::showCenterWindow(bool bIsCenter)
{
    // Init window state with config.
    QString windowState = SettingsHelper::get("advance.window.windowstate").toString();

    if (bIsCenter) {
        Dtk::Widget::moveToCenter(this);
    }
    // init window state.
    if (windowState == "window_maximum") {
        showMaximized();
    } else if (windowState == "fullscreen") {
        showFullScreen();
    } else {
        showNormal();
    }
}

void MainWindow::toggleFullscreen()
{
    if (!window()->windowState().testFlag(Qt::WindowFullScreen)) {
        window()->setWindowState(windowState() | Qt::WindowFullScreen);
    } else {
        window()->setWindowState(windowState() & ~Qt::WindowFullScreen);
    }
}

void MainWindow::resetFontSize()
{
    SettingsHelper::set("base.font.size", m_settings->m_iDefaultFontSize);
}

void MainWindow::decrementFontSize()
{
    int size = std::max(m_font_size - 1, m_settings->m_iMinFontSize);
    SettingsHelper::set("base.font.size", size);
}

void MainWindow::incrementFontSize()
{
    int size = std::min(m_font_size + 1, m_settings->m_iMaxFontSize);
    SettingsHelper::set("base.font.size", size);
}

void MainWindow::displayShortcuts()
{
    QRect rect = window()->geometry();
    QPoint pos(rect.x() + rect.width() / 2,
               rect.y() + rect.height() / 2);

    /* Show Window Keymaps */
    QStringList windowKeymaps;
    windowKeymaps << "savefile" << "openfile" << "incrementfontsize" << "decrementfontsize"
                  << "resetfontsize" << "togglefullscreen" << "find" << "replace" << "escape"
                  << "print";

    QJsonObject shortcutObj;
    QJsonArray jsonGroups;

    QJsonObject windowJsonGroup;
    windowJsonGroup.insert("groupName", QObject::tr("Window"));
    QJsonArray windowJsonItems;

    for (const QString &keymap : windowKeymaps) {
        auto option = m_settings->settings->group("shortcuts.window")->option(QString("shortcuts.window.%1").arg(keymap));
        QJsonObject jsonItem;
        jsonItem.insert("name", QObject::tr(option->name().toUtf8().data()));
        if (keymap != "incrementfontsize" && keymap != "decrementfontsize") {
            jsonItem.insert("value", option->value().toString().replace("Meta", "Super"));
        } else if (keymap == "incrementfontsize") {
            QString strIncrementfontValue = QString("Ctrl+'='");
            jsonItem.insert("value", strIncrementfontValue.replace("Meta", "Super"));
        } else if (keymap == "decrementfontsize" && option->value().toString() == "Ctrl+-") {
            QString strDecrementfontValue = QString("Ctrl+'-'");
            jsonItem.insert("value", strDecrementfontValue.replace("Meta", "Super"));
        }

        windowJsonItems.append(jsonItem);
    }

    windowJsonGroup.insert("groupItems", windowJsonItems);
    jsonGroups.append(windowJsonGroup);

    /* Show Editor  */
    QStringList editorKeymaps;
    editorKeymaps << "indentline" << "backindentline"
                  << "forwardchar" << "backwardchar"
                  << "forwardword" << "backwardword"
                  << "nextline" << "prevline"
                  << "newline" << "opennewlinebelow" << "killline"
                  << "swaplineup" << "swaplinedown"
                  << "scrollup" << "scrolldown"
                  << "movetoendofline" << "movetostartofline"
                  << "movetoend" << "movetostart"
                  << "selectall" << "copy" << "cut"
                  << "paste" << "undo" << "redo";
    QJsonObject editorJsonGroup;
    editorJsonGroup.insert("groupName", tr("Editor"));
    QJsonArray editorJsonItems;

    for (const QString &keymap : editorKeymaps) {
        auto option = m_settings->settings->group("shortcuts.editor")->option(QString("shortcuts.editor.%1").arg(keymap));
        QJsonObject jsonItem;
        jsonItem.insert("name", QObject::tr(option->name().toUtf8().data()));
        jsonItem.insert("value", option->value().toString().replace("Meta", "Super"));
        editorJsonItems.append(jsonItem);
    }
    editorJsonGroup.insert("groupItems", editorJsonItems);
    jsonGroups.append(editorJsonGroup);


    /* Show Setting Keymaps */
    QStringList setupKeymaps;
    setupKeymaps << "help" << "displayshortcuts";

    QJsonObject setupJsonGroup;
    setupJsonGroup.insert("groupName", tr("Settings"));
    QJsonArray setupJsonItems;

    for (const QString &keymap : setupKeymaps) {
        auto option = m_settings->settings->group("shortcuts.window")->option(QString("shortcuts.window.%1").arg(keymap));
        QJsonObject jsonItem;
        jsonItem.insert("name", QObject::tr(option->name().toUtf8().data()));
        jsonItem.insert("value", option->value().toString().replace("Meta", "Super"));
        setupJsonItems.append(jsonItem);
    }
    setupJsonGroup.insert("groupItems", setupJsonItems);
    jsonGroups.append(setupJsonGroup);

    shortcutObj.insert("shortcut", jsonGroups);

    QJsonDocument doc(shortcutObj);
    QStringList shortcutString;
    QString param1 = "-j=" + QString(doc.toJson().data());
    QString param2 = "-p=" + QString::number(pos.x()) + "," + QString::number(pos.y());
    shortcutString << param1 << param2;

    m_shortcutViewProcess = new QProcess();
    m_shortcutViewProcess->startDetached("deepin-shortcut-viewer", shortcutString);

    connect(m_shortcutViewProcess, SIGNAL(finished(int)), m_shortcutViewProcess, SLOT(deleteLater()));
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    QString key = Utils::getKeyshortcut(e);
    do {
        if (key == Utils::getKeyshortcutFromKeymap(m_settings, "window", "savefile")) {
            onFileSave();
            break;
        }
        if (key == Utils::getKeyshortcutFromKeymap(m_settings, "window", "saveasfile")) {
            onFileSaveAs();
            break;
        }
        if (key == Utils::getKeyshortcutFromKeymap(m_settings, "window", "openfile")) {
            onFileOpen();
            break;
        }
        if (key == Utils::getKeyshortcutFromKeymap(m_settings, "window", "incrementfontsize")) {
            incrementFontSize();
            break;
        }
        if (key == Utils::getKeyshortcutFromKeymap(m_settings, "window", "decrementfontsize")) {
            decrementFontSize();
            break;
        }
        if (key == Utils::getKeyshortcutFromKeymap(m_settings, "window", "resetfontsize")) {
            resetFontSize();
            break;
        }
        if (key == Utils::getKeyshortcutFromKeymap(m_settings, "window", "togglefullscreen")) {
            DIconButton *minBtn = titlebar()->findChild<DIconButton *>("DTitlebarDWindowMinButton");
            DIconButton *quitFullBtn = titlebar()->findChild<DIconButton *>("DTitlebarDWindowQuitFullscreenButton");
            quitFullBtn->setFocusPolicy(Qt::TabFocus);
            DIconButton *maxBtn = titlebar()->findChild<DIconButton *>("DTitlebarDWindowMaxButton");
            if (minBtn->hasFocus() || maxBtn->hasFocus()) {
                toggleFullscreen();
                quitFullBtn->setFocus();
            } else {
                toggleFullscreen();
            }
            break;
        }
        if (key == Utils::getKeyshortcutFromKeymap(m_settings, "window", "displayshortcuts")) {
            displayShortcuts();
            break;
        }
        if (key == Utils::getKeyshortcutFromKeymap(m_settings, "window", "print")) {
            onToPdf();
            break;
        }
        if (key == Utils::getKeyshortcutFromKeymap(m_settings, "window", "help")) {
            onOpenHelpFile();
            break;
        }
        // Post event to window widget if match Alt+0 ~ Alt+9
        /*
        QRegularExpression re("^Alt\\+\\d");
        QRegularExpressionMatch match = re.match(key);
        if (match.hasMatch()) {
            auto tabIndex = key.replace("Alt+", "").toInt();
            if (tabIndex == 9) {
                 if (m_tabbar->count() > 1) {
                     activeTab(m_tabbar->count() - 1);
                 }
            } else {
                 if (tabIndex <= m_tabbar->count()) {
                     activeTab(tabIndex - 1);
                 }
            }
        }*/
    } while(false);
}
