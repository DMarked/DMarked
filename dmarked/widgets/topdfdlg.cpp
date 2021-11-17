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
