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
    mapPageSize.insert("B5", QPageSize::PageSizeId::B5);
    mapPageSize.insert("Letter/AnsiA", QPageSize::PageSizeId::Letter);
    mapPageSize.insert("Legal/AnsiB", QPageSize::PageSizeId::Legal);
    mapPageSize.insert("Executive", QPageSize::PageSizeId::Executive);
    mapPageSize.insert("A0", QPageSize::PageSizeId::A0);
    mapPageSize.insert("A1", QPageSize::PageSizeId::A1);
    mapPageSize.insert("A2", QPageSize::PageSizeId::A2);
    mapPageSize.insert("A3", QPageSize::PageSizeId::A3);
    mapPageSize.insert("A5", QPageSize::PageSizeId::A5);
    mapPageSize.insert("A6", QPageSize::PageSizeId::A6);
    mapPageSize.insert("A7", QPageSize::PageSizeId::A7);
    mapPageSize.insert("A8", QPageSize::PageSizeId::A8);
    mapPageSize.insert("A9", QPageSize::PageSizeId::A9);
    mapPageSize.insert("B0", QPageSize::PageSizeId::B0);
    mapPageSize.insert("B1", QPageSize::PageSizeId::B1);
    mapPageSize.insert("B10", QPageSize::PageSizeId::B10);
    mapPageSize.insert("B2", QPageSize::PageSizeId::B2);
    mapPageSize.insert("B3", QPageSize::PageSizeId::B3);
    mapPageSize.insert("B4", QPageSize::PageSizeId::B4);
    mapPageSize.insert("B6", QPageSize::PageSizeId::B6);
    mapPageSize.insert("B7", QPageSize::PageSizeId::B7);
    mapPageSize.insert("B8", QPageSize::PageSizeId::B8);
    mapPageSize.insert("B9", QPageSize::PageSizeId::B9);
    mapPageSize.insert("C5E/EnvelopeC5", QPageSize::PageSizeId::C5E);
    mapPageSize.insert("Comm10E/Envelope10", QPageSize::PageSizeId::Comm10E);
    mapPageSize.insert("DLE/EnvelopeDL", QPageSize::PageSizeId::DLE);
    mapPageSize.insert("Folio", QPageSize::PageSizeId::Folio);
    mapPageSize.insert("Ledger", QPageSize::PageSizeId::Ledger);
    mapPageSize.insert("Tabloid", QPageSize::PageSizeId::Tabloid);
    mapPageSize.insert("Custom", QPageSize::PageSizeId::Custom);
    // New values derived from PPD standard

    mapPageSize.insert("A3Extra", QPageSize::PageSizeId::A3Extra);
    mapPageSize.insert("A4Extra", QPageSize::PageSizeId::A4Extra);
    mapPageSize.insert("A4Plus", QPageSize::PageSizeId::A4Plus);
    mapPageSize.insert("A4Small", QPageSize::PageSizeId::A4Small);
    mapPageSize.insert("A5Extra", QPageSize::PageSizeId::A5Extra);
    mapPageSize.insert("B5Extra", QPageSize::PageSizeId::B5Extra);
    mapPageSize.insert("JisB0", QPageSize::PageSizeId::JisB0);
    mapPageSize.insert("JisB1", QPageSize::PageSizeId::JisB1);
    mapPageSize.insert("JisB2", QPageSize::PageSizeId::JisB2);
    mapPageSize.insert("JisB3", QPageSize::PageSizeId::JisB3);
    mapPageSize.insert("JisB4", QPageSize::PageSizeId::JisB4);
    mapPageSize.insert("JisB5", QPageSize::PageSizeId::JisB5);
    mapPageSize.insert("JisB6", QPageSize::PageSizeId::JisB6);
    mapPageSize.insert("JisB7", QPageSize::PageSizeId::JisB7);
    mapPageSize.insert("JisB8", QPageSize::PageSizeId::JisB8);
    mapPageSize.insert("JisB9", QPageSize::PageSizeId::JisB9);
    mapPageSize.insert("JisB10", QPageSize::PageSizeId::JisB10);
    // AnsiA = Letter
    // AnsiB = Ledger
    mapPageSize.insert("AnsiC", QPageSize::PageSizeId::AnsiC);
    mapPageSize.insert("AnsiD", QPageSize::PageSizeId::AnsiD);
    mapPageSize.insert("AnsiE", QPageSize::PageSizeId::AnsiE);
    mapPageSize.insert("LegalExtra", QPageSize::PageSizeId::LegalExtra);
    mapPageSize.insert("LetterExtra", QPageSize::PageSizeId::LetterExtra);
    mapPageSize.insert("LetterPlus", QPageSize::PageSizeId::LetterPlus);
    mapPageSize.insert("LetterSmall", QPageSize::PageSizeId::LetterSmall);
    mapPageSize.insert("TabloidExtra", QPageSize::PageSizeId::TabloidExtra);
    mapPageSize.insert("ArchA", QPageSize::PageSizeId::ArchA);
    mapPageSize.insert("ArchB", QPageSize::PageSizeId::ArchB);
    mapPageSize.insert("ArchC", QPageSize::PageSizeId::ArchC);
    mapPageSize.insert("ArchD", QPageSize::PageSizeId::ArchD);
    mapPageSize.insert("ArchE", QPageSize::PageSizeId::ArchE);
    mapPageSize.insert("Imperial7x9", QPageSize::PageSizeId::Imperial7x9);
    mapPageSize.insert("Imperial8x10", QPageSize::PageSizeId::Imperial8x10);
    mapPageSize.insert("Imperial9x11", QPageSize::PageSizeId::Imperial9x11);
    mapPageSize.insert("Imperial9x12", QPageSize::PageSizeId::Imperial9x12);
    mapPageSize.insert("Imperial10x11", QPageSize::PageSizeId::Imperial10x11);
    mapPageSize.insert("Imperial10x13", QPageSize::PageSizeId::Imperial10x13);
    mapPageSize.insert("Imperial10x14", QPageSize::PageSizeId::Imperial10x14);
    mapPageSize.insert("Imperial12x11", QPageSize::PageSizeId::Imperial12x11);
    mapPageSize.insert("Imperial15x11", QPageSize::PageSizeId::Imperial15x11);
    mapPageSize.insert("ExecutiveStandard", QPageSize::PageSizeId::ExecutiveStandard);
    mapPageSize.insert("Note", QPageSize::PageSizeId::Note);
    mapPageSize.insert("Quarto", QPageSize::PageSizeId::Quarto);
    mapPageSize.insert("Statement", QPageSize::PageSizeId::Statement);
    mapPageSize.insert("SuperA", QPageSize::PageSizeId::SuperA);
    mapPageSize.insert("SuperB", QPageSize::PageSizeId::SuperB);
    mapPageSize.insert("Postcard", QPageSize::PageSizeId::Postcard);
    mapPageSize.insert("DoublePostcard", QPageSize::PageSizeId::DoublePostcard);
    mapPageSize.insert("Prc16K", QPageSize::PageSizeId::Prc16K);
    mapPageSize.insert("Prc32K", QPageSize::PageSizeId::Prc32K);
    mapPageSize.insert("Prc32KBig", QPageSize::PageSizeId::Prc32KBig);
    mapPageSize.insert("FanFoldUS", QPageSize::PageSizeId::FanFoldUS);
    mapPageSize.insert("FanFoldGerman", QPageSize::PageSizeId::FanFoldGerman);
    mapPageSize.insert("FanFoldGermanLegal", QPageSize::PageSizeId::FanFoldGermanLegal);
    mapPageSize.insert("EnvelopeB4", QPageSize::PageSizeId::EnvelopeB4);
    mapPageSize.insert("EnvelopeB5", QPageSize::PageSizeId::EnvelopeB5);
    mapPageSize.insert("EnvelopeB6", QPageSize::PageSizeId::EnvelopeB6);
    mapPageSize.insert("EnvelopeC0", QPageSize::PageSizeId::EnvelopeC0);
    mapPageSize.insert("EnvelopeC1", QPageSize::PageSizeId::EnvelopeC1);
    mapPageSize.insert("EnvelopeC2", QPageSize::PageSizeId::EnvelopeC2);
    mapPageSize.insert("EnvelopeC3", QPageSize::PageSizeId::EnvelopeC3);
    mapPageSize.insert("EnvelopeC4", QPageSize::PageSizeId::EnvelopeC4);
    // EnvelopeC5 = C5E
    mapPageSize.insert("EnvelopeC6", QPageSize::PageSizeId::EnvelopeC6);
    mapPageSize.insert("EnvelopeC65", QPageSize::PageSizeId::EnvelopeC65);
    mapPageSize.insert("EnvelopeC7", QPageSize::PageSizeId::EnvelopeC7);
    // EnvelopeDL = DLE
    mapPageSize.insert("Envelope9", QPageSize::PageSizeId::Envelope9);
    // Envelope10 = Comm10E
    mapPageSize.insert("Envelope11", QPageSize::PageSizeId::Envelope11);
    mapPageSize.insert("Envelope12", QPageSize::PageSizeId::Envelope12);
    mapPageSize.insert("Envelope14", QPageSize::PageSizeId::Envelope14);
    mapPageSize.insert("EnvelopeMonarch", QPageSize::PageSizeId::EnvelopeMonarch);
    mapPageSize.insert("EnvelopePersonal", QPageSize::PageSizeId::EnvelopePersonal);
    mapPageSize.insert("EnvelopeChou3", QPageSize::PageSizeId::EnvelopeChou3);
    mapPageSize.insert("EnvelopeChou4", QPageSize::PageSizeId::EnvelopeChou4);
    mapPageSize.insert("EnvelopeInvite", QPageSize::PageSizeId::EnvelopeInvite);
    mapPageSize.insert("EnvelopeItalian", QPageSize::PageSizeId::EnvelopeItalian);
    mapPageSize.insert("EnvelopeKaku2", QPageSize::PageSizeId::EnvelopeKaku2);
    mapPageSize.insert("EnvelopeKaku3", QPageSize::PageSizeId::EnvelopeKaku3);
    mapPageSize.insert("EnvelopePrc1", QPageSize::PageSizeId::EnvelopePrc1);
    mapPageSize.insert("EnvelopePrc2", QPageSize::PageSizeId::EnvelopePrc2);
    mapPageSize.insert("EnvelopePrc3", QPageSize::PageSizeId::EnvelopePrc3);
    mapPageSize.insert("EnvelopePrc4", QPageSize::PageSizeId::EnvelopePrc4);
    mapPageSize.insert("EnvelopePrc5", QPageSize::PageSizeId::EnvelopePrc5);
    mapPageSize.insert("EnvelopePrc6", QPageSize::PageSizeId::EnvelopePrc6);
    mapPageSize.insert("EnvelopePrc7", QPageSize::PageSizeId::EnvelopePrc7);
    mapPageSize.insert("EnvelopePrc8", QPageSize::PageSizeId::EnvelopePrc8);
    mapPageSize.insert("EnvelopePrc9", QPageSize::PageSizeId::EnvelopePrc9);
    mapPageSize.insert("EnvelopePrc10", QPageSize::PageSizeId::EnvelopePrc10);
    mapPageSize.insert("EnvelopeYou4", QPageSize::PageSizeId::EnvelopeYou4);


    ui->comboBox_PageSize->addItems(QStringList(mapPageSize.keys()));
    ui->comboBox_PageSize->setCurrentIndex(5);
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
