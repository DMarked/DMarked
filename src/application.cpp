/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Maintainer： rewine <lhongxu@outlook.com>
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
#include <DMessageBox>
#include <DWidgetUtil>
#include <DGuiApplicationHelper>
#include <DMessageBox>

Application::Application(int &argc, char **argv)
    : DApplication(argc, argv)
{
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::newProcessInstance, this, &Application::onNewProcessInstance);
}

Application::~Application()
{
}

void Application::activateWindow() {
    //Init Normal window at first time
    if (nullptr == m_qspMainWnd.get()) {
        m_qspMainWnd.reset(new MainWindow());
        /*m_qspMainWnd->setMinimumSize(DEFAULT_WINDOWS_WIDTH, DEFAULT_WINDOWS_HEIGHT);
        QByteArray mainWindowSize = setting::instance()->getOption(VNOTE_MAINWND_SZ_KEY).toByteArray();
        if (!mainWindowSize.isEmpty()) {
            m_qspMainWnd->restoreGeometry(mainWindowSize);
        }*/

        //Should be called befor show
        Dtk::Widget::moveToCenter(m_qspMainWnd.get());

        m_qspMainWnd->show();
    } else {
        /*
        if (m_qspMainWnd->needShowMax()) {
            m_qspMainWnd->setWindowState(Qt::WindowMaximized);
        } else {
            m_qspMainWnd->setWindowState(Qt::WindowActive);
        }*/
        m_qspMainWnd->activateWindow();
    }
}

MainWindow *Application::mainWindow() const {
    return m_qspMainWnd.get();
}

void Application::onNewProcessInstance(qint64 pid, const QStringList &arguments) {
    Q_UNUSED(pid);
    Q_UNUSED(arguments);

    //TODO:
    //Parase comandline here

    activateWindow();
}

void Application::handleQuitAction() {
    if (mainWindow()->queryClose()) {
        //QEvent event(QEvent::Close);
        //DApplication::sendEvent(mainWindow(), &event);
        exit(0);
    }
}

bool Application::notify(QObject *object, QEvent *event) {
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
