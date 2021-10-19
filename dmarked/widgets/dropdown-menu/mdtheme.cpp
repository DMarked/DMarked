/*
 * DMarked
 *
 * Copyright (C) 2021. Lu Hongxu
 * Authors: Lu Hongxu <lhongxu@outlook.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "mdtheme.h"

const QStringList MdTheme::light_themes = { "apollo",        "infoq",      "markdown", "simple",  "less"
                                          , "wecatch-code"   "ocean",      "style",    "wecatch", "erye"
                                          , "list-writing",  "sanyuesha",  "typing",   "xiaolai", "clearness"
                                          , "solarized-light","github" };

const QStringList MdTheme::dark_themes = { "darkdown", "solarized-dark", "clearness-dark", "byword-dark" };

QString MdTheme::light_current_theme = "xiaolai";
QString MdTheme::dark_current_theme = "darkdown";

