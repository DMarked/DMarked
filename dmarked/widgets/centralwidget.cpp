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

#include "centralwidget.h"
#include "settings.h"

#include <QFontDatabase>
#include <QColor>
#include <QScrollBar>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <DSettingsOption>
#include <DLog>
#include <DMessageBox>

CentralWidget::CentralWidget(DWidget *parent)
    : DWidget (parent),
      m_editorWidget(new EditorWidget),
      m_previewWidget(new PreviewWidget),
      m_splitter(new QSplitter),
      m_centralLayout(new QHBoxLayout),
      m_fileWatcher(new DFileSystemWatcher)
{
      m_splitter->setOrientation(Qt::Horizontal);
      m_splitter->setOpaqueResize(true);
      m_splitter->setHandleWidth(0);
      m_splitter->setChildrenCollapsible(false);

      m_fontName = SettingsHelper::get("base.font.family").toString();
      m_fontSize = SettingsHelper::get("base.font.size").toInt();
      m_tabSpaceNumber = SettingsHelper::get("advance.editor.tabspacenumber").toInt();
      updateFont();

      m_editorWidget->setGeometry(0, 0, 600, 740);
      m_editorWidget->setLineNumberEnabled(SettingsHelper::get("base.font.showlinenumber").toBool());
      m_editorWidget->setHighlightCurrentLineEnabled(SettingsHelper::get("base.font.hightlightcurrentline").toBool());

      m_previewWidget->setGeometry(0, 0, 600, 740);

      connect(m_editorWidget, &QMarkdownTextEdit::textChanged, [this]() {
          m_previewWidget->setText(m_editorWidget->toPlainText());
      });

      connect(m_editorWidget->verticalScrollBar(), &QScrollBar::valueChanged,
              m_previewWidget, [this](int val) {
          if (!m_isSync) return;
          int editMax = m_editorWidget->verticalScrollBar()->maximum();
          //int editPos = m_editorWidget->verticalScrollBar()->sliderPosition();
          double factor = 1.0 * val / editMax;
          QString method = QString("window.scrollTo(0,document.documentElement.scrollHeight*%1)").arg(factor);
          m_previewWidget->page()->runJavaScript(method);
      });

      m_splitter->addWidget(m_editorWidget);
      m_splitter->addWidget(m_previewWidget);
      //m_splitter->setContentsMargins(200, 0, 200, 0);

      m_centralLayout->addStretch(1);
      m_centralLayout->addWidget(m_splitter, 5);
      m_centralLayout->addStretch(1);
      m_centralLayout->setStretch(0, 0);
      m_centralLayout->setStretch(2, 0);
      setLayout(m_centralLayout);
      switch (SettingsHelper::get("advance.editor.editor_mode").toInt()) {
      case 1: setMode(tr("Read Mode")); break;
      case 2: setMode(tr("Write Mode")); break;
      case 3: setMode(tr("Preview Mode(S)")); break;
      case 4: setMode(tr("Preview Mode(N)")); break;
      }

      // 监视文件修改
      connect(m_fileWatcher, &DFileSystemWatcher::fileModified, this, &CentralWidget::onFileModified);
}

void CentralWidget::setFilePath(const QString &path)
{
    m_editorWidget->syncFilePath(path);
    if (m_filePath != path) {
        if (!m_filePath.isEmpty())
            m_fileWatcher->removePath(m_filePath);
        if (!path.isEmpty())
            m_fileWatcher->addPath(path);
    }
    m_filePath = path;
    m_previewWidget->syncFilePath(path); // let dmarked.js get the file path
}

void CentralWidget::onFileModified(const QString &path, const QString &name)
{
    dInfo() << QString("File Modified: path: %1, name: %2").arg(path).arg(name);
    //auto ret = DMessageBox::question(this, tr("reload"), tr("file has modified, reload?"));
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return;
    QString newContent = f.readAll();
    if (newContent != m_editorWidget->toPlainText())
        m_editorWidget->setPlainText(newContent);
}

void CentralWidget::setMode(const QString &mode)
{

    if (mode == tr("Read Mode") || mode == tr("Write Mode")) {
        m_centralLayout->setStretch(0, 1);
        m_centralLayout->setStretch(2, 1);
        m_editorWidget->setFrameStyle(QFrame::NoFrame); // 去除控件边框线
        if (mode == tr("Read Mode")) {
            SettingsHelper::set("advance.editor.editor_mode", 1);
            m_editorWidget->hide();
            m_previewWidget->show();
        } else if (mode == tr("Write Mode")) {
            SettingsHelper::set("advance.editor.editor_mode", 2);
            m_editorWidget->show();
            m_previewWidget->hide();
        }
    } else {
        m_centralLayout->setStretch(0, 0);
        m_centralLayout->setStretch(2, 0);
        m_editorWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
        m_editorWidget->show();
        m_previewWidget->show();
        if (mode ==tr("Preview Mode(S)")) {
            SettingsHelper::set("advance.editor.editor_mode", 3);
            setSync(true);
        } else if (mode == tr("Preview Mode(N)")) {
            SettingsHelper::set("advance.editor.editor_mode", 4);
            setSync(false);
        }
    }
}

void CentralWidget::setFontSize(int size)
{
    m_fontSize = size;
    updateFont();
}

void CentralWidget::setFontFamily(const QString &fontName)
{
    m_fontName = fontName;
    updateFont();
}

void CentralWidget::setTabSpaceNumber(int num)
{
    m_tabSpaceNumber = num;
    updateFont();
}

void CentralWidget::updateFont()
{
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setFixedPitch(true);
    font.setPointSize(m_fontSize);
    font.setFamily(m_fontName);
    m_editorWidget->setFont(font);
    m_previewWidget->setFontSize(m_fontSize);
    m_previewWidget->setFontFamily(m_fontName);

#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
    m_editorWidget->setTabStopWidth(m_tabSpaceNumber * QFontMetrics(font).width(' '));
#else
    m_editorWidget->setTabStopDistance(m_tabSpaceNumber * QFontMetrics(font).horizontalAdvance(QLatin1Char(' ')));
#endif
}
