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
#include "settings.h"

#include <QFontDatabase>
#include <QColor>
#include <QScrollBar>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <DSettingsOption>

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
      m_editor_widget->setLineNumberEnabled(Settings::instance()->settings->option("base.font.showlinenumber")->value().toBool());
      QColor a = QColor::fromRgb(255, 0, 0);
      m_editor_widget->setLineNumbersCurrentLineColor(a);

      m_preview_widget->setGeometry(0, 0, 600, 740);

      connect(m_editor_widget, &QMarkdownTextEdit::textChanged, [this]() {
          m_preview_widget->setText(m_editor_widget->toPlainText());
      });

      connect(m_editor_widget->verticalScrollBar(), &QScrollBar::valueChanged,
              m_preview_widget, [this](int val) {
          if (!m_is_sync) return;
          int editMax = m_editor_widget->verticalScrollBar()->maximum();
          //int editPos = m_editor_widget->verticalScrollBar()->sliderPosition();
          double factor = 1.0 * val / editMax;
          QString method = QString("window.scrollTo(0,document.documentElement.scrollHeight*%1)").arg(factor);
          m_preview_widget->page()->runJavaScript(method);
      });


      m_splitter->addWidget(m_editor_widget);
      m_splitter->addWidget(m_preview_widget);

      //m_splitter->setContentsMargins(200, 0, 200, 0);

      m_central_layout = new QHBoxLayout;
      m_central_layout->addStretch(1);
      m_central_layout->addWidget(m_splitter, 5);
      m_central_layout->addStretch(1);
      m_central_layout->setStretch(0, 0);
      m_central_layout->setStretch(2, 0);
      setLayout(m_central_layout);
}

void CentralWidget::setFilePath(const QString &path)
{
    m_file_path = path;
}

const QString & CentralWidget::getFilePath()
{
    return  m_file_path;
}

void CentralWidget::setSync(bool enable)
{
    m_is_sync = enable;
}

void CentralWidget::setMode(const QString &mode)
{
    if (mode == tr("Read Mode") || mode == tr("Write Mode")) {
        m_central_layout->setStretch(0, 1);
        m_central_layout->setStretch(2, 1);
        m_editor_widget->setFrameStyle(QFrame::NoFrame); // 去除控件边框线
        if (mode == tr("Read Mode")) {
            m_editor_widget->hide();
            m_preview_widget->show();
        } else if (mode == tr("Write Mode")) {
            m_editor_widget->show();
            m_preview_widget->hide();
        }
    } else {
        m_central_layout->setStretch(0, 0);
        m_central_layout->setStretch(2, 0);
        m_editor_widget->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
        m_editor_widget->show();
        m_preview_widget->show();
        if (mode ==tr("Preview Mode(S)")) {
            setSync(true);
        } else if (mode == tr("Preview Mode(N)")) {
            setSync(false);
        }
    }
}

