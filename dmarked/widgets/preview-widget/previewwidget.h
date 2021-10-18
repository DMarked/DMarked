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

#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include "document.h"
#include "previewpage.h"

#include <QWebChannel>
#include <QWebEngineView>
#include <DApplicationHelper>

DWIDGET_USE_NAMESPACE

class PreviewWidget : public QWebEngineView
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = nullptr);

    void setText(const QString &content);

public slots:
    void setMdTheme(const QString &theme);

private:
    Document m_content;
    PreviewPage *m_page;
    QWebChannel *m_channel;
};

#endif // PREVIEWWIDGET_H
