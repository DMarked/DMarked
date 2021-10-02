#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "document.h"

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
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

    Ui::MainWindow *ui;
    QString m_filePath;
    Document m_content;
};

#endif // MAINWINDOW_H
