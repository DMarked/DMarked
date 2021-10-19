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

PreviewWidget::PreviewWidget(QWidget *parent) : QWebEngineView(parent)
{
    m_page = new PreviewPage(this);
    m_page->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
    setPage(m_page);
    m_channel = new QWebChannel(this);
    m_channel->registerObject(QStringLiteral("content"), &m_content);
    m_page->setWebChannel(m_channel);

    setUrl(QUrl("qrc:/index.html"));

    connect(this, &QWebEngineView::loadFinished, [this](bool success) {
        if (success) {
            bool isDark = DGuiApplicationHelper::instance()->applicationPalette().color(QPalette::Background).lightness() < 128;
            setMdTheme(isDark ? MdTheme::dark_current_theme : MdTheme::light_current_theme);
        }
    });

    connect(m_page, &QWebEnginePage::pdfPrintingFinished,
              this, &PreviewWidget::pdfPrintingFinished);

    setContentsMargins(0, 0, 0, 0);
    setContextMenuPolicy(Qt::NoContextMenu);
    setFocusPolicy(Qt::NoFocus);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void PreviewWidget::setText(const QString &content) {
    m_content.setText(content);
}

void PreviewWidget::setMdTheme(const QString &theme) {
    QString method = "setMdTheme(\'" + theme + "\')";
    m_page->runJavaScript(method);
}

void PreviewWidget::printToPdf(const QString &filePath, const QPageLayout &layout) {
    m_page->printToPdf(filePath, layout);
}

void PreviewWidget::pdfPrintingFinished(const QString &filePath, bool success) {
    if (success) {
        DDesktopServices::showFileItem(filePath);
    } else {
        DMessageBox::warning(this, tr("Warn"), tr("fail to PDF"));
    }
}

