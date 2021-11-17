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

#include <QIcon>
#include <QDebug>
#include <QDir>
#include <QMouseEvent>
#include <QAbstractButton>
#include <DWidgetUtil>
#include <DGuiApplicationHelper>
#include <QCommandLineParser>

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
            Dtk::Widget::moveToCenter(m_qspMainWnd.get());
            m_qspMainWnd->show();
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
        m_qspMainWnd->md2html(from, to);
    }
}

void Application::md2pdf(QString from, QString to, int depth)
{
    if (depth == 0) {
        m_qspMainWnd->md2pdf(from, to);
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
        //让所有按钮响应回车 DPushButton不响应回车 DIconButton会默认响应
        QKeyEvent *keyevent = static_cast<QKeyEvent *>(event);
        if ((object->inherits("QAbstractButton")) && (keyevent->key() == Qt::Key_Return || keyevent->key() == Qt::Key_Enter)) {
            QAbstractButton *pushButton = dynamic_cast<QAbstractButton *>(object);
            if (pushButton) {
                emit pushButton->clicked(!pushButton->isChecked());
                return true;
            }
        }

        //alt+m 模拟右击菜单
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
