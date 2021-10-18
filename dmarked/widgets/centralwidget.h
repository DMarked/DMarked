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

#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <DWidget>
#include <DSplitter>
#include <QLayout>

#include <qmarkdowntextedit.h>
#include "preview-widget/previewwidget.h"


DWIDGET_USE_NAMESPACE

class CentralWidget : public DWidget
{
    Q_OBJECT

    friend class MainWindow;
public:
    explicit CentralWidget(DWidget *parent = nullptr);

private:
    QMarkdownTextEdit *m_editor_widget;
    PreviewWidget *m_preview_widget;
    DSplitter *m_splitter;
    QLayout *m_central_layout;
};

#endif // CENTRALWIDGET_H
