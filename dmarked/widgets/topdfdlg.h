#ifndef TOPDFDLG_H
#define TOPDFDLG_H

#include <QDialog>
#include <QPageLayout>
#include <QMap>

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
    QMap<QString, QPageSize::PageSizeId> mapPageSize;

    Ui::ToPdfDlg *ui;
};

#endif // TOPDFDLG_H
