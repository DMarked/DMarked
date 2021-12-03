/*
 * DMarked
 *
 * Copyright (C) 2021 DMarked.
 * Authors: Lu Hongxu <lhongxu@outlook.com>
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
 *
 */

#include "utils.h"

#include <DSettingsOption>
#include <QDir>

QString Utils::getKeyshortcut(QKeyEvent *keyEvent)
{
    QStringList keys;
    Qt::KeyboardModifiers modifiers = keyEvent->modifiers();
    if (modifiers != Qt::NoModifier){
        if (modifiers.testFlag(Qt::MetaModifier)) {
            keys.append("Meta");
        }

        if (modifiers.testFlag(Qt::ControlModifier)) {
            keys.append("Ctrl");
        }

        if (modifiers.testFlag(Qt::AltModifier)) {
            keys.append("Alt");
        }

        if (modifiers.testFlag(Qt::ShiftModifier)) {
            keys.append("Shift");
        }
    }

    if(keyEvent->key() !=0 && keyEvent->key() != Qt::Key_unknown){
        keys.append(QKeySequence(keyEvent->key()).toString());
    }

    for (int i = 0;i < keys.count();i++) {
        if (keys.value(i).contains("Return")) {
            keys.replace(i,"Enter");
        }
    }

    return keys.join("+");
}

QString Utils::getKeyshortcutFromKeymap(Settings* settings, const QString &keyCategory, const QString &keyName)
{
    return settings->settings->option(QString("shortcuts.%1.%2").arg(keyCategory).arg(keyName))->value().toString();
}

bool Utils::ensurePathExist(const QString &filePath)
{
    // 确保创建文件前，目录存在
    int id = filePath.lastIndexOf('/');
    if (id == -1)
        return true;
    QDir path(filePath.left(id));
    if (!path.exists())
        return path.mkpath(path.path());
    return true;
}
