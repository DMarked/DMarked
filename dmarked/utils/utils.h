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

#ifndef UTILS_H
#define UTILS_H

#include "settings.h"
#include <QKeyEvent>

class Utils
{
public:
    Utils() = default;
    static QString getKeyshortcut(QKeyEvent *keyEvent);
    static QString getKeyshortcutFromKeymap(Settings* settings, const QString &keyCategory, const QString &keyName);

    static bool ensurePathExist(const QString &filePath);

    static QString getDefaultDlgFilePath(const QString &nowFilePath="");
};

#endif // UTILS_H
