/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
* Copyright (C) 2021 DMarked.
*
* Author:      V4fr3e <V4fr3e@deepin.io>
* Maintainer： Lu Hongxu <lhongxu@outlook.com>
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

#include "application.h"
#include "mainwindow.h"
#include "utils/utils.h"

#include <QIcon>
#include <QDebug>
#include <QDir>
#include <QMouseEvent>
#include <QAbstractButton>
#include <DWidgetUtil>
#include <DGuiApplicationHelper>
#include <QCommandLineParser>
#include <DLog>
#include <DPathBuf>

Application::Application(int &argc, char **argv)
    : DApplication(argc, argv)
{
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::newProcessInstance, this, &Application::onNewProcessInstance);
}

Application::~Application()
{
}

void Application::activateWindow(bool willShow)
{
    //Init Normal window at first time
    if (nullptr == m_qspMainWnd.get()) {
        m_qspMainWnd.reset(new MainWindow());
        //Should be called befor show
        if (willShow) {
            m_qspMainWnd->showCenterWindow(true);
        } else {
            m_qspMainWnd->setNoGui();
        }
    } else {
        m_qspMainWnd->setWindowState(Qt::WindowActive);
        m_qspMainWnd->activateWindow();
    }
}

MainWindow *Application::mainWindow() const
{
    return m_qspMainWnd.get();
}

void Application::md2html(QString from, QString to, int depth)
{
    if (depth == 0) {
        if (from.right(3) != ".md")
            return;
        dInfo() << "2html: " << from << " " << to;
        if (!Utils::ensurePathExist(to)) {
            dError() << QString("Path:%1 don't exist").arg(to);
            return;
        }
        m_qspMainWnd->md2html(from, to);
        return;
    }

    QDir dir(from);
    DPathBuf topath(to), frompath(from);

    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::Readable);
    QStringList fileList = dir.entryList();
    for (const QString &file : fileList) {
        if (file.right(3) == ".md") {
            QString tofile = file.left(file.length()-3)+".html";
            md2html((frompath/file).toString(), (topath/tofile).toString(), 0);
        }
    }

    dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable);
    QStringList subDirs = dir.entryList();
    if (depth > 1) {
        for (const QString &sdir : subDirs) {
            dWarning() << sdir;
            md2html((frompath/sdir).toString(), (topath/sdir).toString(), depth-1);
        }
    }
}

void Application::md2pdf(QString from, QString to, QPageLayout pageLayout, int depth)
{
    if (depth == 0) {
        dInfo() << "2pdf: " << from << " " << to;
        if (from.right(3) != ".md")
            return;
        if (!Utils::ensurePathExist(to)) {
            dError() << QString("Path:%1 don't exist").arg(to);
            return;
        }
        m_qspMainWnd->md2pdf(from, to, pageLayout);
        return;
    }

    QDir dir(from);
    DPathBuf topath(to), frompath(from);

    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::Readable);
    QStringList fileList = dir.entryList();
    for (const QString &file : fileList) {
        if (file.right(3) == ".md") {
            QString tofile = file.left(file.length()-3)+".pdf";
            md2pdf((frompath/file).toString(), (topath/tofile).toString(), pageLayout, 0);
        }
    }

    dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable);
    QStringList subDirs = dir.entryList();
    if (depth > 1) {
        for (const QString &sdir : subDirs) {
            dWarning() << sdir;
            md2pdf((frompath/sdir).toString(), (topath/sdir).toString(), pageLayout, depth-1);
        }
    }
}

void Application::onNewProcessInstance(qint64 pid, const QStringList &arguments)
{
    Q_UNUSED(pid);
    Q_UNUSED(arguments);
    activateWindow();
}

void Application::handleQuitAction()
{
    if (mainWindow()->queryClose()) {
        exit(0);
    }
}

bool Application::notify(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyevent = static_cast<QKeyEvent *>(event);

        // alt+m 模拟右击菜单
        if ((keyevent->modifiers() == Qt::AltModifier) && keyevent->key() == Qt::Key_M) {
            // 光标中心点
            QPoint pos = QPoint(static_cast<int>(qApp->inputMethod()->cursorRectangle().x() + qApp->inputMethod()->cursorRectangle().width() / 2),
                                static_cast<int>(qApp->inputMethod()->cursorRectangle().y() + qApp->inputMethod()->cursorRectangle().height() / 2));

            // QPoint(0,0) 表示无法获取光标位置
            if (pos != QPoint(0, 0)) {
                QMouseEvent event(QEvent::MouseButtonPress, pos, Qt::RightButton, Qt::NoButton, Qt::NoModifier);
                QCoreApplication::sendEvent(object, &event);
            }
        }
    }

    return DApplication::notify(object, event);
}
