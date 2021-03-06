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
#include "mdtheme.h"
#include "settings.h"

#include <DSettingsOption>

const QStringList MdTheme::light_themes = { "apollo",        "infoq",      "markdown", "simple",  "less"
                                          , "wecatch-code"   "ocean",      "style",    "wecatch", "erye"
                                          , "list-writing",  "sanyuesha",  "typing",   "xiaolai", "clearness"
                                          , "solarized-light","github" };

const QStringList MdTheme::dark_themes = { "darkdown", "solarized-dark", "clearness-dark", "byword-dark" };

QString MdTheme::getCurrentLightTheme()
{
    return Settings::instance()->settings->option("advance.editor.mdtheme_light")->value().toString();
}

QString MdTheme::getCurrentDarkTheme()
{
    return Settings::instance()->settings->option("advance.editor.mdtheme_dark")->value().toString();
}

void MdTheme::setCurrentLightTheme(const QString &theme)
{
    Settings::instance()->settings->option("advance.editor.mdtheme_light")->setValue(theme);
}

void MdTheme::setCurrentDarkTheme(const QString &theme)
{
    Settings::instance()->settings->option("advance.editor.mdtheme_dark")->setValue(theme);
}
