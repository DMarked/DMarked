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
#include "previewwidget.h"
#include "widgets/dropdown-menu/mdtheme.h"
#include "settings.h"

#include <DMessageBox>
#include <QFile>
#include <QDir>
#include <DSettingsOption>
#include <QAction>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <DDesktopServices>

PreviewWidget::PreviewWidget(QWidget *parent) : QWebEngineView(parent)
{
    m_page = new PreviewPage(this);

    this->settings()->setDefaultTextEncoding("utf-8");
    this->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    this->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    this->page()->action(QWebEnginePage::Reload)->setVisible(false);
    this->page()->action(QWebEnginePage::ReloadAndBypassCache)->setVisible(false);
    this->page()->action(QWebEnginePage::OpenLinkInThisWindow)->setVisible(false);
    this->page()->action(QWebEnginePage::OpenLinkInNewWindow)->setVisible(false);
    this->page()->action(QWebEnginePage::ViewSource)->setVisible(false);
    this->page()->action(QWebEnginePage::SavePage)->setVisible(false);

    m_page->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_page->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);

    setPage(m_page);
    m_channel = new QWebChannel(this);
    m_channel->registerObject(QStringLiteral("content"), &m_content);
    m_channel->registerObject(QStringLiteral("config"), &m_config);
    m_page->setWebChannel(m_channel);

    setUrl(QUrl("qrc:/index.html"));

    connect(&m_content, &Document::markdownThemeChanged, this, &PreviewWidget::markdownThemeChanged);
    connect(&m_content, &Document::markdownLoadFinished, this, &PreviewWidget::markdownLoadFinished);

    connect(this, &QWebEngineView::loadFinished, [this](bool success) {
        if (success) {
            bool isDark = DGuiApplicationHelper::instance()->applicationPalette().color(QPalette::Window).lightness() < 128;
            setMarkdownTheme(isDark ? MdTheme::getCurrentDarkTheme() : MdTheme::getCurrentLightTheme());
            setHighlightTheme(isDark ? "monokai-sublime" : "default.min");
            setMarkedIsDark(isDark);
        }
    });

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            [this](DGuiApplicationHelper::ColorType themeType) {
        bool isDark = themeType == DGuiApplicationHelper::ColorType::DarkType;
        setHighlightTheme(isDark ? "monokai-sublime" : "default.min");
        setMarkedIsDark(isDark);
        // MarkdownTheme update at ddropdownmenu.cpp
    });

    connect(m_page, &QWebEnginePage::pdfPrintingFinished,
              this, &PreviewWidget::pdfPrintingFinished);

    setContentsMargins(0, 0, 0, 0);
    setContextMenuPolicy(Qt::DefaultContextMenu);
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

void PreviewWidget::setMarkedIsDark(bool isDark)
{
    m_page->runJavaScript(QString("setMarkedIsDark(%1)").arg(isDark));
}

void PreviewWidget::setNoGui()
{
    /* Call this function when running on the command line */
    isGui = false;
    m_content.setNoGui();
}

/**
 * @brief PreviewWidget::convert2Html
 * @param filePath
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

        QRegExp ipjs(" *<script src=\".*\"></script> *\n");
        QString output = html;
        output.remove(ipjs);

        bool isDark = DGuiApplicationHelper::instance()->applicationPalette().color(QPalette::Window).lightness() < 128;
        QString mdtheme = isDark ? MdTheme::getCurrentDarkTheme() : MdTheme::getCurrentLightTheme();
        QFile mdthemeFile(QString(":/themes/%1.css").arg(mdtheme));
        mdthemeFile.open(QIODevice::ReadOnly);
        output = QString("<style>").append(mdthemeFile.readAll()).append("</style>\n").append(output);

        QTextStream str(&f);
        str << output;
        str.flush();
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

void PreviewWidget::setFontSize(int size)
{
    QWebEngineSettings *setting = m_page->settings();
    setting->setFontSize(QWebEngineSettings::DefaultFontSize, size);
}

void PreviewWidget::setFontFamily(const QString &fontName)
{
    QWebEngineSettings *setting = m_page->settings();
    setting->setFontFamily(QWebEngineSettings::StandardFont, fontName);
}



