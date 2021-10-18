/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 * Maintainer： rewine <lhongxu@outlook.com>
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

#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H

#include "dropdown-menu/ddropdownmenu.h"

#include <DLabel>
#include <DApplicationHelper>
#include <DFontSizeManager>
#include <QPainterPath>

DWIDGET_USE_NAMESPACE

class EditWrapper;
class BottomBar : public QWidget
{
    Q_OBJECT

public:
    explicit BottomBar(QWidget *parent = nullptr);
    ~BottomBar();

    void updatePosition(int row, int column);
    void updateWordCount(int charactorCount);
    void setCursorStatus(const QString &text);
    void setPalette(const QPalette &palette);
    void updateSize(int size);
    void setChildEnabled(bool enabled);
    void setChildrenFocus(bool ok,QWidget* preOrderWidget = nullptr);

    DDropdownMenu *getThemeMenu();

protected:
    void paintEvent(QPaintEvent *);

private:
    DLabel *m_pPositionLabel {nullptr};
    DLabel *m_pCharCountLabel {nullptr};
    DLabel *m_pCursorStatus {nullptr};
    DDropdownMenu *m_pThemeMenu {nullptr};
    QString m_rowStr {QString()};
    QString m_columnStr {QString()};
    QString m_chrCountStr {QString()};

public slots:
    // 编码按钮/文本类型按钮失去焦点后，设置光标回到文本框里
    void slotSetTextEditFocus();

signals:
    void currentMdThemeChanged(const QString&);
};

#endif
