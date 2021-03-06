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

#ifndef TOPDFDLG_H
#define TOPDFDLG_H

#include <QDialog>
#include <QPageLayout>

namespace Ui {
class ToPdfDlg;
}

class ToPdfDlg : public QDialog
{
    Q_OBJECT
public:
    explicit ToPdfDlg(QWidget *parent = nullptr);
    ~ToPdfDlg();

    QPageSize getPageSize();
    QPageLayout::Orientation getOrientation();
    double getMarginLeft();
    double getMarginTop();
    double getMarginRight();
    double getMarginButtom();

private:
    Ui::ToPdfDlg *ui;
};

#endif // TOPDFDLG_H
