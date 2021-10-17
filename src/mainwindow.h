#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmarkdowntextedit.h"
#include <QString>
#include <DTitlebar>
#include <DSearchEdit>
#include <DMainWindow>
#include <DGuiApplicationHelper>
#include <QAction>

#include "widgets/centralwidget.h"
//#include "bottombar.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void openFile(const QString &path);

private slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();

private:
    bool isModified() const;
    void setupAction();

    DSearchEdit *m_search_edit;
    CentralWidget *m_central_widget;
    QString m_file_path;

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
