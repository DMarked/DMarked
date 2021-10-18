#ifndef MDTHEME_H
#define MDTHEME_H

#include <QStringList>

class MdTheme
{
public:
    MdTheme() = default;
    static const QStringList light_themes;
    static const QStringList dark_themes;
    static QString light_current_theme;
    static QString dark_current_theme;
};

#endif // MDTHEME_H
