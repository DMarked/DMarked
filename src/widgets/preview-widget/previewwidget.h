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

#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include "document.h"
#include "previewpage.h"

#include <QWebChannel>
#include <QWebEngineView>
#include <DApplicationHelper>
#include <QWebEngineSettings>

DWIDGET_USE_NAMESPACE

class PreviewWidget : public QWebEngineView
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = nullptr);

    void setText(const QString &content);
    void convert2Pdf(const QString &filePath, const QPageLayout &layout = QPageLayout(QPageSize(QPageSize::A4), QPageLayout::Portrait, QMarginsF()));
    void convert2Html(const QString &filePath);
    void setNoGui();

    void setFontSize(int size);
    void setFontFamily(const QString &fontName);
    void syncFilePath(const QString &filePath) { m_content.setFilePath(filePath); }

public Q_SLOTS:
    void setMarkdownTheme(const QString &theme);
    void setHighlightTheme(const QString &theme);
    //void setScrollbarsTheme(const QString &theme);
    void setMarkedIsDark(bool isDark);
    void pdfPrintingFinished(const QString &filePath, bool success);

Q_SIGNALS:
    void markdownLoadFinished();
    void convert2HtmlFinish(bool success);
    void convert2PdfFinish(bool success);

private:
    Document m_content;
    DmarkedConfig m_config;
    PreviewPage *m_page;
    QWebChannel *m_channel;
    bool isGui = true;
};

#endif // PREVIEWWIDGET_H
