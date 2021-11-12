#include "topdfdlg.h"
#include "ui_topdfdlg.h"

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
    /*
     * Why Can't use QMetaEnum?
     *
    QMetaObject enumTriggerMode = QPageSize::staticMetaObject;
    int index = enumTriggerMode.indexOfEnumerator("PageSizeId");
    QMetaEnum metaEnum = enumTriggerMode.enumerator(index);
    //QMetaEnum ametaEnum = QMetaEnum::fromType<QPageSize::PageSizeId>();
    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        ui->comboBox_PageSize->addItem(metaEnum.key(i), metaEnum.value(i));
    }
    */

    mapPageSize.insert("A4", QPageSize::PageSizeId::A4);
    mapPageSize.insert("Letter", QPageSize::PageSizeId::Letter);
    mapPageSize.insert("Executive", QPageSize::PageSizeId::Executive);
    mapPageSize.insert("A0", QPageSize::PageSizeId::A0);
    mapPageSize.insert("A5", QPageSize::PageSizeId::A5);


 /*
    ,
    ,
    ,
    ,
    A1,
    A2,
    A3,
    A5,
    A6,
    A7,
    A8,
    A9,
    B0,
    B1,
    B10,
    B2,
    B3,
    B4,
    B6,
    B7,
    B8,
    B9,
    C5E,
    Comm10E,
    DLE,
    Folio,
    Ledger,
    Tabloid,
    Custom,*/
    ui->comboBox_PageSize->addItems(QStringList(mapPageSize.keys()));
    ui->comboBox_PageSize->setCurrentIndex(0);
}

ToPdfDlg::~ToPdfDlg()
{
    delete ui;
}

QPageSize ToPdfDlg::getPageSize()
{
    return QPageSize(mapPageSize.value(ui->comboBox_PageSize->currentText()));
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
