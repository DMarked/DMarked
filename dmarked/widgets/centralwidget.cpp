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
      m_editor_widget = new EditorWidget;
      m_preview_widget = new PreviewWidget;
      m_splitter = new QSplitter;

      m_splitter->setOrientation(Qt::Horizontal);
      m_splitter->setOpaqueResize(true);
      m_splitter->setHandleWidth(0);
      m_splitter->setChildrenCollapsible(true);

      m_font_name = Settings::instance()->settings->option("base.font.family")->value().toString();
      m_font_size = Settings::instance()->settings->option("base.font.size")->value().toInt();
      m_tab_space_number = Settings::instance()->settings->option("advance.editor.tabspacenumber")->value().toInt();
      updateFont();

      m_editor_widget->setGeometry(0, 0, 600, 740);
      m_editor_widget->setLineNumberEnabled(Settings::instance()->settings->option("base.font.showlinenumber")->value().toBool());
      m_editor_widget->setHighlightCurrentLineEnabled(Settings::instance()->settings->option("base.font.hightlightcurrentline")->value().toBool());

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
      switch (Settings::instance()->settings->option("advance.editor.editor_mode")->value().toInt()) {
      case 0: setMode(tr("Read Mode")); break;
      case 1: setMode(tr("Write Mode")); break;
      case 2: setMode(tr("Preview Mode(S)")); break;
      case 3: setMode(tr("Preview Mode(N)")); break;
      }
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
            Settings::instance()->settings->option("advance.editor.editor_mode")->setValue(0);
            m_editor_widget->hide();
            m_preview_widget->show();
        } else if (mode == tr("Write Mode")) {
            Settings::instance()->settings->option("advance.editor.editor_mode")->setValue(1);
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
            Settings::instance()->settings->option("advance.editor.editor_mode")->setValue(2);
            setSync(true);
        } else if (mode == tr("Preview Mode(N)")) {
            Settings::instance()->settings->option("advance.editor.editor_mode")->setValue(3);
            setSync(false);
        }
    }
}

void CentralWidget::setFontSize(int size)
{
    m_font_size = size;
    updateFont();
}

void CentralWidget::setFontFamily(const QString &fontName)
{
    m_font_name = fontName;
    updateFont();
}

void CentralWidget::setTabSpaceNumber(int num)
{
    m_tab_space_number = num;
    updateFont();
}

void CentralWidget::updateFont()
{
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setFixedPitch(true);
    font.setPointSize(m_font_size);
    font.setFamily(m_font_name);
    m_editor_widget->setFont(font);
    m_preview_widget->setFontSize(m_font_size);
    m_preview_widget->setFontFamily(m_font_name);
    m_editor_widget->setTabStopDistance(m_tab_space_number * QFontMetrics(font).width(QChar(0x2192)));
}
