#include "mainwindow.h"
#include "previewpage.h"

#include <QFile>
#include <DFileDialog>
#include <QFontDatabase>
#include <DMessageBox>
#include <QTextStream>
#include <QWebChannel>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent) :
    DMainWindow(parent)
{
    resize(800, 500);
    m_eidtor_widget = new DPlainTextEdit;
    m_splitter = new QSplitter;
    m_preview_widget = new QWebEngineView;

    m_splitter->addWidget(m_eidtor_widget);
    m_splitter->addWidget(m_preview_widget);

    m_central_layout = new QHBoxLayout;
    m_central_layout->addWidget(m_splitter);
    m_central_widget = new DWidget;
    m_central_widget->setLayout(m_central_layout);

    setCentralWidget(m_central_widget);
    setAllAction();

    m_eidtor_widget->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    m_preview_widget->setContextMenuPolicy(Qt::NoContextMenu);

    PreviewPage *page = new PreviewPage(this);
    m_preview_widget->setPage(page);

    connect(m_eidtor_widget, &DPlainTextEdit::textChanged, [this]() {
        m_content.setText(m_eidtor_widget->toPlainText());
    });

    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), &m_content);
    page->setWebChannel(channel);

    m_preview_widget->setUrl(QUrl("qrc:/index.html"));

    QFile defaultTextFile(":/default.md");
    defaultTextFile.open(QIODevice::ReadOnly);
    m_eidtor_widget->setPlainText(defaultTextFile.readAll());
}

MainWindow::~MainWindow() {
}

void MainWindow::setAllAction() {
    QMenu *menu;
    QAction *actionNew, *actionOpen, *actionSave, *actionSaveAs;

    actionNew = new QAction(tr("New"));
    actionOpen = new QAction(tr("Open"));
    actionSave = new QAction(tr("Save"));
    actionSaveAs = new QAction(tr("SaveAs"));
    menu = new QMenu;
    menu->addAction(actionNew);
    menu->addAction(actionOpen);
    menu->addAction(actionSave);
    menu->addAction(actionSaveAs);

    titlebar()->setMenu(menu);

    connect(actionNew, &QAction::triggered, this, &MainWindow::onFileNew);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::onFileOpen);
    connect(actionSave, &QAction::triggered, this, &MainWindow::onFileSave);
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::onFileSaveAs);
    connect(m_eidtor_widget->document(), &QTextDocument::modificationChanged,
          actionSave, &QAction::setEnabled);
    //connect(titlebar()->, &MainWindow::close, this, &MainWindow::onExit);
}

void MainWindow::openFile(const QString &path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        DMessageBox::warning(this, windowTitle(),
                             tr("Could not open file %1: %2").arg(
                                 QDir::toNativeSeparators(path), f.errorString()));
        return;
    }
    m_filePath = path;
    m_eidtor_widget->setPlainText(f.readAll());
}

bool MainWindow::isModified() const {
    return m_eidtor_widget->document()->isModified();
}

void MainWindow::onFileNew() {
    if (isModified()) {
        DMessageBox::StandardButton button = DMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to create a new document anyway?"));
        if (button != DMessageBox::Yes)
            return;
    }

    m_filePath.clear();
    m_eidtor_widget->setPlainText(tr("## New document"));
    m_eidtor_widget->document()->setModified(false);
}

void MainWindow::onFileOpen() {
    if (isModified()) {
        DMessageBox::StandardButton button = DMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to open a new document anyway?"));
        if (button != DMessageBox::Yes)
            return;
    }

    QString path = DFileDialog::getOpenFileName(this,
        tr("Open MarkDown File"), "", tr("MarkDown File (*.md)"));
    if (path.isEmpty())
        return;

    openFile(path);
}

void MainWindow::onFileSave() {
    if (m_filePath.isEmpty()) {
        onFileSaveAs();
        return;
    }

    QFile f(m_filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))  {
        DMessageBox::warning(this, windowTitle(),
                             tr("Could not write to file %1: %2").arg(
                                 QDir::toNativeSeparators(m_filePath), f.errorString()));
        return;
    }
    QTextStream str(&f);
    str << m_eidtor_widget->toPlainText();

    m_eidtor_widget->document()->setModified(false);
}

void MainWindow::onFileSaveAs() {
    QString path = QFileDialog::getSaveFileName(this,
        tr("Save MarkDown File"), "", tr("MarkDown File (*.md, *.markdown)"));
    if (path.isEmpty())
        return;
    m_filePath = path;
    onFileSave();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (isModified()) {
        DMessageBox::StandardButton button = DMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to exit anyway?"));
        if (button != DMessageBox::Yes) {
            event->ignore();
            return;
        }
    }
    event->accept();
}
