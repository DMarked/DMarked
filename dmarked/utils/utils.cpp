#include "utils.h"

#include <DSettingsOption>

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
