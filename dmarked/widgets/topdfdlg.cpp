/*
 * DMarked
 *
 * Copyright (C) 2021 DMarked.
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

#include "topdfdlg.h"
#include "ui_topdfdlg.h"
#include "utils/mappagesize.h"

#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QPageSize>
#include <QMetaEnum>

ToPdfDlg::ToPdfDlg(QWidget *parent) :
    QDialog(parent)
  , ui(new Ui::ToPdfDlg)
{
    ui->setupUi(this);
    QButtonGroup *pageLayout_bg = new QButtonGroup(this);
    pageLayout_bg->addButton(ui->checkBox_Portrait);
    pageLayout_bg->addButton(ui->checkBox_Landscape);
    ui->comboBox_PageSize->addItems(QStringList(MapPageSize::getInstance()->mapPageSize.keys()));
    ui->comboBox_PageSize->setCurrentIndex(5);
}

ToPdfDlg::~ToPdfDlg()
{
    delete ui;
}

QPageSize ToPdfDlg::getPageSize()
{
    return QPageSize(MapPageSize::getInstance()->mapPageSize.value(ui->comboBox_PageSize->currentText()));
}

QPageLayout::Orientation ToPdfDlg::getOrientation()
{
    return ui->checkBox_Portrait->isChecked() ? QPageLayout::Portrait : QPageLayout::Landscape;
}

double ToPdfDlg::getMarginLeft()
{
    return ui->doubleSpinBox_Left->value();
}

double ToPdfDlg::getMarginTop()
{
    return ui->doubleSpinBox_Top->value();
}

double ToPdfDlg::getMarginRight()
{
    return ui->doubleSpinBox_Right->value();
}

double ToPdfDlg::getMarginButtom()
{
    return ui->doubleSpinBox_Buttom->value();
}
