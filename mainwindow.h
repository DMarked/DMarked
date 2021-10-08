#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "document.h"

#include <QMainWindow>
#include <QString>
#include <DWidgetUtil> // 加入此头文件方可使用moveToCenter
#include <DSearchEdit>
#include <DTitlebar>
#include <DMainWindow>
#include <DGuiApplicationHelper> // 用来适配深色模式
#include <DLabel>
#include <DPlainTextEdit>
#include <DSplitter>
#include <QWebEngineView>
#include <QAction>

DWIDGET_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void openFile(const QString &path);

private slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onExit();

private:
    bool isModified() const;
    void setAllAction();

    DPlainTextEdit *m_eidtor_widget;
    QSplitter *m_splitter;
    QWebEngineView *m_preview_widget;

    QLayout *m_central_layout;
    DWidget *m_central_widget;

    QString m_filePath;
    Document m_content;
};

#endif // MAINWINDOW_H
