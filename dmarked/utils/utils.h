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
};

#endif // UTILS_H
