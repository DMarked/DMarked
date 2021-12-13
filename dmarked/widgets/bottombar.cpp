/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 * Copyright (C) 2021 DMarked.
 *
 * Author:     rekols <rekols@foxmail.com>
 * Maintainer： Lu Hongxu <lhongxu@outlook.com>
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
 */

#include "bottombar.h"
#include "dropdown-menu/mdtheme.h"

#include <QPainter>
#include <QHBoxLayout>
#include <DMenu>
#include <DVerticalLine>

BottomBar::BottomBar(QWidget *parent)
    : QWidget(parent),
      m_pPositionLabel(new DLabel),
      m_pCharCountLabel(new DLabel),
      m_pFakeVimStatus(new DLabel),
      m_pThemeMenu(DDropdownMenu::createThemeMenu()),
      m_pModeMenu(DDropdownMenu::createModeMenu()),
      m_rowStr(tr("Row")),
      m_columnStr(tr("Column")),
      m_chrCountStr(tr("Characters %1"))
{
    QFont font;
    font.setFamily("SourceHanSansSC-Normal");
    m_pPositionLabel->setFont(font);
    m_pCharCountLabel->setFont(font);
    m_pFakeVimStatus->setFont(font);

    DFontSizeManager::instance()->bind(m_pPositionLabel, DFontSizeManager::T9);
    DFontSizeManager::instance()->bind(m_pCharCountLabel, DFontSizeManager::T9);
    DFontSizeManager::instance()->bind(m_pFakeVimStatus, DFontSizeManager::T9);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(29, 1, 10, 0);
    layout->addWidget(m_pPositionLabel);
    layout->addStretch();
    layout->addSpacerItem(new QSpacerItem(110, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));
    layout->addWidget(m_pCharCountLabel);

    m_pPositionLabel->setText(QString("%1 %2  %3 %4").arg(m_rowStr, "1", m_columnStr, "1"));

    m_pCharCountLabel->setText(m_chrCountStr.arg("0"));

    DVerticalLine *pVerticalLine1 = new DVerticalLine();
    DVerticalLine *pVerticalLine2 = new DVerticalLine();
    pVerticalLine1->setFixedSize(1, 15);
    pVerticalLine2->setFixedSize(1, 15);

    layout->addStretch();
    m_pFakeVimStatus->setText("INSERT");
    layout->addWidget(m_pFakeVimStatus);
    layout->addSpacing(10);
    layout->addWidget(pVerticalLine1);
    layout->addWidget(m_pThemeMenu);
    layout->addSpacing(10);
    layout->addWidget(pVerticalLine2);
    layout->addWidget(m_pModeMenu);

    setFixedHeight(32);

    // 切换 Markdown Theme
    connect(m_pThemeMenu, &DDropdownMenu::currentActionChanged, [this](QAction* pAct) {
        m_pThemeMenu->setCurrentTextOnly(pAct->text());
        bool isDark = DGuiApplicationHelper::instance()->applicationPalette().color(QPalette::Background).lightness() < 128;
        if (isDark)
            MdTheme::setCurrentDarkTheme(pAct->text());
        else
            MdTheme::setCurrentLightTheme(pAct->text());
        emit this->currentMdThemeChanged(pAct->text());
    });

    connect(m_pModeMenu, &DDropdownMenu::currentActionChanged, [this](QAction *pAct) {
        m_pModeMenu->setCurrentTextOnly(pAct->text());
        emit this->currentModeChanged(pAct->text());
    });

    // 按钮失去焦点后，设置光标回到文本框里
    connect(m_pModeMenu, &DDropdownMenu::sigSetTextEditFocus, this, &BottomBar::bottombarLostFocus);
    connect(m_pThemeMenu, &DDropdownMenu::sigSetTextEditFocus, this, &BottomBar::bottombarLostFocus);
}

BottomBar::~BottomBar()
{
    if (m_pThemeMenu != nullptr) {
        delete m_pThemeMenu;
        m_pThemeMenu = nullptr;
    }
    if (m_pModeMenu != nullptr) {
        delete m_pModeMenu;
        m_pModeMenu = nullptr;
    }
}

void BottomBar::updatePosition(int row, int column)
{
    m_pPositionLabel->setText(QString("%1 %2  %3 %4").arg(m_rowStr, QString::number(row),
                                                          m_columnStr, QString::number(column)));
}

void BottomBar::updateWordCount(int charactorCount)
{
    m_pCharCountLabel->setText(m_chrCountStr.arg(QString::number(charactorCount-1)));
}

void BottomBar::updateVimMessage(const QString &msg)
{
    m_pFakeVimStatus->setText(msg);
}

void BottomBar::setPalette(const QPalette &palette)
{
    DPalette paPositionLabel  = DApplicationHelper::instance()->applicationPalette();
    DPalette paCharCountLabel = DApplicationHelper::instance()->applicationPalette();
    DPalette paFakeVimStatus = DApplicationHelper::instance()->applicationPalette();
    DPalette paThemeMenu = DApplicationHelper::instance()->applicationPalette();
    DPalette paModeMenu = DApplicationHelper::instance()->applicationPalette();

    QColor colorFont = paPositionLabel.textTips().color();

    paPositionLabel.setColor(DPalette::WindowText, colorFont);
    paCharCountLabel.setColor(DPalette::WindowText, colorFont);
    paFakeVimStatus.setColor(DPalette::WindowText, colorFont);
    paThemeMenu.setColor(DPalette::WindowText, colorFont);
    paModeMenu.setColor(DPalette::WindowText, colorFont);

    m_pPositionLabel->setPalette(paPositionLabel);
    m_pCharCountLabel->setPalette(paCharCountLabel);
    m_pFakeVimStatus->setPalette(paFakeVimStatus);
    m_pThemeMenu->getButton()->setPalette(paThemeMenu);
    m_pModeMenu->getButton()->setPalette(paModeMenu);

    QString theme = (palette.color(QPalette::Background).lightness() < 128) ? "dark" : "light";
    m_pThemeMenu->setTheme(theme);
    m_pModeMenu->setTheme(theme);

    QWidget::setPalette(palette);
}

void BottomBar::updateSize(int size)
{
    setFixedHeight(size);
}

void BottomBar::setChildEnabled(bool enabled)
{
    m_pThemeMenu->setEnabled(enabled);
    m_pThemeMenu->setRequestMenu(enabled);

    m_pModeMenu->setEnabled(enabled);
    m_pModeMenu->setRequestMenu(enabled);
}

void BottomBar::setChildrenFocus(bool ok,QWidget* preOrderWidget)
{
     m_pThemeMenu->setChildrenFocus(ok);
     m_pModeMenu->setChildrenFocus(ok);
    if(ok) {
        if(preOrderWidget) setTabOrder(preOrderWidget, m_pThemeMenu->getButton());
        setTabOrder(m_pThemeMenu->getButton(),m_pModeMenu->getButton());
    }
}

DDropdownMenu *BottomBar::getThemeMenu()
{
    return m_pThemeMenu;
}

DDropdownMenu *BottomBar::getModeMenu()
{
    return m_pModeMenu;
}

void BottomBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setOpacity(1);

    QColor backgroundColor = palette().color(QPalette::Background);
    QColor bottombarBackgroundColor;
    if (backgroundColor.lightness() < 128) {
        bottombarBackgroundColor = palette().base().color();
        if (bottombarBackgroundColor.name() != "#202020") {
            bottombarBackgroundColor = QColor("#202020");
        }
        bottombarBackgroundColor.setAlphaF(0.7);

    } else {
        bottombarBackgroundColor = palette().base().color();
        if (bottombarBackgroundColor.name() != "#ffffff") {
            bottombarBackgroundColor = QColor("#ffffff");
        }
        bottombarBackgroundColor.setAlphaF(0.7);
    }

    QPainterPath path;
    path.addRect(rect());
    painter.fillPath(path, bottombarBackgroundColor);

    QColor splitLineColor;
    if (backgroundColor.lightness() < 128) {
        splitLineColor = palette().brightText().color();
        if (splitLineColor.name() != "#ffffff") {
            splitLineColor = QColor("#ffffff");
        }
        splitLineColor.setAlphaF(0.5);
    } else {
        splitLineColor = palette().brightText().color();
        if (splitLineColor.name() != "#000000") {
            splitLineColor = QColor("#000000");
        }
        splitLineColor.setAlphaF(0.5);
    }
}
