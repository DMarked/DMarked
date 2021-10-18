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

#include "centralwidget.h"
#include <QFontDatabase>

CentralWidget::CentralWidget(DWidget *parent): DWidget (parent)
{
      m_editor_widget = new QMarkdownTextEdit;
      m_preview_widget = new PreviewWidget;
      m_splitter = new QSplitter;

      m_splitter->setOrientation(Qt::Horizontal);
      m_splitter->setOpaqueResize(true);
      m_splitter->setHandleWidth(0);
      m_splitter->setChildrenCollapsible(true);

      m_editor_widget->setGeometry(0, 0, 600, 740);
      m_editor_widget->setContentsMargins(0, 0, 0, 0);
      m_editor_widget->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
      m_editor_widget->setFocusPolicy(Qt::StrongFocus);
      m_editor_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      m_editor_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

      m_preview_widget->setGeometry(0, 0, 600, 740);

      connect(m_editor_widget, &QMarkdownTextEdit::textChanged, [this]() {
          m_preview_widget->setText(m_editor_widget->toPlainText());
      });

      m_splitter->addWidget(m_editor_widget);
      m_splitter->addWidget(m_preview_widget);

      m_central_layout = new QHBoxLayout;
      m_central_layout->addWidget(m_splitter);
      setLayout(m_central_layout);
}

