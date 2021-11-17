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
#include "widgets/topdfdlg.h"

#include <QFile>
#include <DFileDialog>
#include <DMessageBox>
#include <QTextStream>
#include <QLayout>
#include <DLog>


MainWindow::MainWindow(QWidget *parent) :
    DMainWindow(parent)
{
    resize(1200, 740);
    setWindowIcon(QIcon(":/images/dmarked.svg"));

    m_search_edit = new DSearchEdit(this);
    titlebar()->setCustomWidget(m_search_edit);
    m_search_edit->setFixedWidth(400);

    m_central_widget = new CentralWidget;
    m_bottom_bar = new BottomBar;
    connect(m_central_widget->m_editor_widget, &QMarkdownTextEdit::textChanged, [this]() {
        m_bottom_bar->updateWordCount(m_central_widget->m_editor_widget->toPlainText().length());
    });
    connect(m_central_widget->m_editor_widget, &QMarkdownTextEdit::cursorPositionChanged, [this]() {
        QTextCursor cursor = m_central_widget->m_editor_widget->textCursor();
        m_bottom_bar->updatePosition(cursor.blockNumber()+1, cursor.columnNumber()+1);
    });
    connect(m_bottom_bar, &BottomBar::currentMdThemeChanged, [this](const QString &theme) {
       m_central_widget->m_preview_widget->setMarkdownTheme(theme);
    });
    connect(m_bottom_bar, &BottomBar::currentModeChanged, [this](const QString &mode) {
        m_central_widget->setMode(mode);
    });

    QVBoxLayout *m_layout = new QVBoxLayout;
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_central_widget);
    m_layout->addWidget(m_bottom_bar);
    DWidget *base_widget = new DWidget;
    base_widget->setLayout(m_layout);
    setCentralWidget(base_widget);
    setupAction();

    QFile defaultTextFile(":/default.md");
    defaultTextFile.open(QIODevice::ReadOnly);
    m_central_widget->m_editor_widget->setPlainText(defaultTextFile.readAll());
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupAction()
{
    QMenu *menu, *convert_menu;
    QAction *actionNew, *actionOpen, *actionSave, *actionSaveAs;
    QAction *action2Pdf, *action2Html;

    actionNew = new QAction(tr("New"));
    actionOpen = new QAction(tr("Open"));
    actionSave = new QAction(tr("Save"));
    actionSaveAs = new QAction(tr("SaveAs"));
    action2Pdf = new QAction(tr("Pdf"));
    action2Html = new QAction(tr("Html"));

    menu = new QMenu;
    menu->addAction(actionNew);
    menu->addAction(actionOpen);
    convert_menu = new QMenu(tr("Convert"));
    convert_menu->addAction(action2Pdf);
    convert_menu->addAction(action2Html);
    menu->addMenu(convert_menu);
    menu->addAction(actionSave);
    menu->addAction(actionSaveAs);

    titlebar()->setMenu(menu);

    connect(actionNew, &QAction::triggered, this, &MainWindow::onFileNew);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::onFileOpen);
    connect(actionSave, &QAction::triggered, this, &MainWindow::onFileSave);
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::onFileSaveAs);
    connect(m_central_widget->m_editor_widget->document(), &QTextDocument::modificationChanged,
          actionSave, &QAction::setEnabled);

    connect(action2Pdf, &QAction::triggered, this, &MainWindow::onToPdf);
    connect(action2Html, &QAction::triggered, this, &MainWindow::onToHtml);
}

bool MainWindow::md2html(QString mdpath, QString htmlpath) {
    QFile f(mdpath);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << tr("Could not open file %1: %2").arg(
                                 QDir::toNativeSeparators(mdpath), f.errorString());
        return false;
    }
    m_central_widget->setFilePath(mdpath);
    m_central_widget->m_editor_widget->setPlainText(f.readAll());
    connect(m_central_widget->m_preview_widget, &PreviewWidget::markdownThemeChanged,
            [this, htmlpath]() {
        m_central_widget->m_preview_widget->convert2Html(htmlpath);
    });
    return true;
}

bool MainWindow::md2pdf(QString mdpath, QString pdfpath) {
    QFile f(mdpath);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << tr("Could not open file %1: %2").arg(
                                 QDir::toNativeSeparators(mdpath), f.errorString());
        return false;
    }
    m_central_widget->setFilePath(mdpath);
    m_central_widget->m_editor_widget->setPlainText(f.readAll());
    connect(m_central_widget->m_preview_widget, &PreviewWidget::markdownThemeChanged,
             [this, pdfpath]() {
        m_central_widget->m_preview_widget->convert2Pdf(pdfpath);
    });
    return true;
}

void MainWindow::openFile(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        DMessageBox::warning(this, windowTitle(),
                             tr("Could not open file %1: %2").arg(
                                 QDir::toNativeSeparators(path), f.errorString()));
        return;
    }
    m_central_widget->setFilePath(path);
    m_central_widget->m_editor_widget->setPlainText(f.readAll());
}

bool MainWindow::isModified() const
{
    return m_central_widget->m_editor_widget->document()->isModified();
}

bool MainWindow::queryClose()
{
    if (isModified()) {
        DMessageBox::StandardButton button = DMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to exit anyway?"));
        return button == DMessageBox::Yes;
    }
    return true;
}

void MainWindow::onToPdf()
{
    ToPdfDlg dlg(this);
    int res = dlg.exec();
    if (res != QDialog::Accepted) {
        return;
    }

    QPageLayout pageLayout(dlg.getPageSize()
                         , dlg.getOrientation()
                         , QMarginsF(dlg.getMarginLeft(),
                                     dlg.getMarginTop(),
                                     dlg.getMarginRight(),
                                     dlg.getMarginButtom()));

    QString path = DFileDialog::getSaveFileName(this,
        tr("Convert to PDF"), "", tr("PDF File (*.pdf)"));
    if (path.isEmpty())
        return;
    m_central_widget->m_preview_widget->convert2Pdf(path, pageLayout);
}

void MainWindow::onToHtml()
{
    QString path = DFileDialog::getSaveFileName(this,
        tr("Convert to HTML"), "", tr("HTML File (*.html)"));
    if (path.isEmpty())
        return;
    m_central_widget->m_preview_widget->convert2Html(path);
}

void MainWindow::onFileNew()
{
    if (isModified()) {
        DMessageBox::StandardButton button = DMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to create a new document anyway?"));
        if (button != DMessageBox::Yes)
            return;
    }

    m_central_widget->setFilePath("");
    m_central_widget->m_editor_widget->setPlainText(tr("## New document"));
    m_central_widget->m_editor_widget->document()->setModified(false);
}

void MainWindow::onFileOpen()
{
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

void MainWindow::onFileSave()
{
    if (m_central_widget->getFilePath().isEmpty()) {
        onFileSaveAs();
        return;
    }

    QFile f(m_central_widget->getFilePath());
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))  {
        DMessageBox::warning(this, windowTitle(),
                             tr("Could not write to file %1: %2").arg(
                                 QDir::toNativeSeparators(m_central_widget->getFilePath()), f.errorString()));
        return;
    }
    QTextStream str(&f);
    str << m_central_widget->m_editor_widget->toPlainText();
    m_central_widget->m_editor_widget->document()->setModified(false);
}

void MainWindow::onFileSaveAs()
{
    QString path = DFileDialog::getSaveFileName(this,
        tr("Save MarkDown File"), "", tr("MarkDown File (*.md, *.markdown)"));
    if (path.isEmpty())
        return;
    m_central_widget->setFilePath(path);
    onFileSave();
}
