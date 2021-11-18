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

#include "previewwidget.h"
#include "../dropdown-menu/mdtheme.h"

#include <QWebEngineSettings>
#include <DDesktopServices>
#include <DMessageBox>
#include <QFile>
#include <QDir>

PreviewWidget::PreviewWidget(QWidget *parent) : QWebEngineView(parent)
{
    m_page = new PreviewPage(this);
    //m_page->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
    setPage(m_page);
    m_channel = new QWebChannel(this);
    m_channel->registerObject(QStringLiteral("content"), &m_content);
    m_page->setWebChannel(m_channel);

    setUrl(QUrl("qrc:/index.html"));
    connect(&m_content, &Document::markdownThemeChanged, this, &PreviewWidget::markdownThemeChanged);
    connect(&m_content, &Document::markdownLoadFinished, this, &PreviewWidget::markdownLoadFinished);

    connect(this, &QWebEngineView::loadFinished, [this](bool success) {
        if (success) {
            bool isDark = DGuiApplicationHelper::instance()->applicationPalette().color(QPalette::Background).lightness() < 128;
            setMarkdownTheme(isDark ? MdTheme::dark_current_theme : MdTheme::light_current_theme);
            setHighlightTheme(isDark ? "monokai-sublime" : "default.min");
        }
    });

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            [this](DGuiApplicationHelper::ColorType themeType){
        bool isDark = themeType == DGuiApplicationHelper::ColorType::DarkType;
        setHighlightTheme(isDark ? "monokai-sublime" : "default.min");
        // MarkdownTheme update at ddropdownmenu.cpp
    });

    connect(m_page, &QWebEnginePage::pdfPrintingFinished,
              this, &PreviewWidget::pdfPrintingFinished);

    setContentsMargins(0, 0, 0, 0);
    setContextMenuPolicy(Qt::NoContextMenu);
    setFocusPolicy(Qt::NoFocus);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void PreviewWidget::setText(const QString &content)
{
    m_content.setText(content);
}

void PreviewWidget::setMarkdownTheme(const QString &theme)
{
    QString method = "setMarkdownTheme(\'" + theme + "\')";
    m_page->runJavaScript(method);
}

void PreviewWidget::setHighlightTheme(const QString &theme)
{
    QString method = "setHighlightTheme(\'" + theme + "\')";
    m_page->runJavaScript(method);
}

void PreviewWidget::setNoGui()
{
    /* Call this function when running on the command line */
    isGui = false;
}

/**
 * @brief PreviewWidget::convert2Html
 * @param filePath
 * @todo add css theme
 * @todo rm code about qtchannel
 */
void PreviewWidget::convert2Html(const QString &filePath) {
    PreviewWidget *pw = this;
    QWebEngineCallback<const QString &> resultCallback([filePath, pw](const QString &html) {
        //DMessageBox::warning(nullptr, tr("Warn"), html);
        QFile f(filePath);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QString warn_info = tr("Could not write to file %1: %2").arg(
                        QDir::toNativeSeparators(filePath), f.errorString());
            if (pw->isGui)
                DMessageBox::warning(nullptr, tr("toHtml"), warn_info);
            Q_EMIT pw->convert2HtmlFinish(false);
            return;
        }
        QTextStream str(&f);
        str << html;
        if (pw->isGui)
            DDesktopServices::showFileItem(filePath);
        Q_EMIT pw->convert2HtmlFinish(true);
    });
    m_page->toHtml(resultCallback);
}

void PreviewWidget::convert2Pdf(const QString &filePath, const QPageLayout &layout)
{
    m_page->printToPdf(filePath, layout);
}

void PreviewWidget::pdfPrintingFinished(const QString &filePath, bool success)
{
    if (isGui) {
        if (success) {
            DDesktopServices::showFileItem(filePath);
        } else {
            DMessageBox::warning(this, tr("Warning"), tr("fail convert to PDF!"));
        }
    }
    Q_EMIT convert2PdfFinish(success);
}

