/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2018 Deepin, Inc.
 * Copyright (C) 2021 DMarked.
 *
 * Author:     Wang Yong <wangyong@deepin.com>
 * Maintainer: Rekols    <rekols@foxmail.com>
 * Maintainer：Lu Hongxu <lhongxu@outlook.com>
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

#include "settings.h"

#include "dthememanager.h"
#include <DSettings>
#include <DSettingsGroup>
#include <DSettingsWidgetFactory>
#include <DSettingsOption>
#include <QStyleFactory>
#include <QFontDatabase>
#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

Settings *Settings::s_pSetting = nullptr;
Settings::Settings(QWidget *parent)
    : QObject(parent)
{
    QString strConfigPath = QString("%1/%2/%3/config.conf")
                            .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
                            .arg(qApp->organizationName())
                            .arg(qApp->applicationName());

    m_backend = new QSettingBackend(strConfigPath);

    settings = DSettings::fromJsonFile(":/settings.json");
    settings->setBackend(m_backend);

    auto fontFamliy = settings->option("base.font.family");
    connect(fontFamliy, &Dtk::Core::DSettingsOption::valueChanged, this, [ = ](QVariant value) {
        emit sigAdjustFont(value.toString());
    });

    auto fontSize = settings->option("base.font.size");
    connect(fontSize, &Dtk::Core::DSettingsOption::valueChanged, this, [ = ](QVariant value) {
        emit sigAdjustFontSize(value.toInt());
    });

    auto showLineNumber = settings->option("base.font.showlinenumber");
    connect(showLineNumber, &Dtk::Core::DSettingsOption::valueChanged, this, [ = ](QVariant value) {
        emit sigSetLineNumberShow(value.toBool());
    });

    //hightlightcurrentline
    auto hightlightCurrentLine = settings->option("base.font.hightlightcurrentline");
    connect(hightlightCurrentLine, &Dtk::Core::DSettingsOption::valueChanged, this, [ = ](QVariant value) {
        emit sigHightLightCurrentLine(value.toBool());
    });

    auto tabSpaceNumber = settings->option("advance.editor.tabspacenumber");
    connect(tabSpaceNumber, &Dtk::Core::DSettingsOption::valueChanged, this, [ = ](QVariant value) {
        emit sigAdjustTabSpaceNumber(value.toInt());
    });

    auto keymap = settings->option("shortcuts.keymap.keymap");
    QMap<QString, QVariant> keymapMap;
    keymapMap.insert("keys", QStringList() << "standard" << "customize");
    keymapMap.insert("values", QStringList() << tr("Standard") << tr("Customize"));
    keymap->setData("items", keymapMap);

    connect(keymap, &Dtk::Core::DSettingsOption::valueChanged, this, [ = ](QVariant value) {
        // Update all key's display value with user select keymap.
        updateAllKeysWithKeymap(value.toString());
    });

    //only used by new window
    auto windowState = settings->option("advance.window.windowstate");
    connect(windowState, &Dtk::Core::DSettingsOption::valueChanged, this, [=] (QVariant value) {
        emit sigChangeWindowSize(value.toString());
    });

    QMap<QString, QVariant> windowStateMap;
    windowStateMap.insert("keys", QStringList() << "window_normal" << "window_maximum" << "fullscreen");
    windowStateMap.insert("values", QStringList() << tr("Normal") << tr("Maximum") << tr("Fullscreen"));
    windowState->setData("items", windowStateMap);

    connect(settings, &Dtk::Core::DSettings::valueChanged, this, [ = ](const QString & key, const QVariant & value) {

        // Change keymap to customize once user change any keyshortcut.
        if (!m_bUserChangeKey && key.startsWith("shortcuts.") && key != "shortcuts.keymap.keymap" && !key.contains("_keymap_")) {
            m_bUserChangeKey = true;

            QString currentKeymap = settings->option("shortcuts.keymap.keymap")->value().toString();

            QStringList keySplitList = key.split(".");
            keySplitList[1] = QString("%1_keymap_customize").arg(keySplitList[1]);
            QString customizeKey = keySplitList.join(".");

            // Just update customize key user input, don't change keymap.
            if (currentKeymap == "customize") {
                settings->option(customizeKey)->setValue(value);
            }
            // If current kemap is not "customize".
            // Copy all customize keys from current keymap, and then update customize key just user input.
            // Then change keymap name.
            else {
                copyCustomizeKeysFromKeymap(currentKeymap);
                settings->option(customizeKey)->setValue(value);
                keymap->setValue("customize");
            }
            m_bUserChangeKey = false;
        }
    });
}

Settings::~Settings()
{
}

void Settings::setSettingDialog(DSettingsDialog *settingsDialog)
{
    m_pSettingsDialog = settingsDialog;
}

// This function is workaround, it will remove after DTK fixed SettingDialog theme bug.
/*
void Settings::dtkThemeWorkaround(QWidget *parent, const QString &theme)
{
    parent->setStyle(QStyleFactory::create(theme));

    for (auto obj : parent->children()) {
        auto w = qobject_cast<QWidget *>(obj);
        if (!w) {
            continue;
        }

        dtkThemeWorkaround(w, theme);
    }
}*/

// QWidget *Settings::createFontComBoBoxHandle(QObject *obj)
QPair<QWidget *, QWidget *> Settings::createFontComBoBoxHandle(QObject *obj)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(obj);

    QComboBox *comboBox = new QComboBox;
    //QWidget *optionWidget = DSettingsWidgetFactory::createTwoColumWidget(option, comboBox);
    QPair<QWidget *, QWidget *> optionWidget = DSettingsWidgetFactory::createStandardItem(QByteArray(), option, comboBox);

    QFontDatabase fontDatabase;
    comboBox->addItems(fontDatabase.families());
    //comboBox->setItemDelegate(new FontItemDelegate);
    //comboBox->setFixedSize(240, 36);

    if (option->value().toString().isEmpty()) {
        option->setValue(QFontDatabase::systemFont(QFontDatabase::FixedFont).family());
    }

    // init.
    comboBox->setCurrentText(option->value().toString());

    connect(option, &DSettingsOption::valueChanged, comboBox, [ = ](QVariant var) {
        comboBox->setCurrentText(var.toString());
    });

    option->connect(comboBox, &QComboBox::currentTextChanged, option, [ = ](const QString & text) {
        option->setValue(text);
    });

    return optionWidget;
}

QPair<QWidget *, QWidget *> Settings::createKeySequenceEditHandle(QObject *obj)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(obj);
    KeySequenceEdit *shortCutLineEdit = new KeySequenceEdit(option);

    shortCutLineEdit->ShortcutDirection(Qt::AlignLeft);
    shortCutLineEdit->setFocusPolicy(Qt::StrongFocus);
    if (option->value().toString().isEmpty()) {
        //option->setValue();
    }

    // init.
    shortCutLineEdit->setKeySequence(QKeySequence(option->value().toString()));
    QPair<QWidget *, QWidget *> optionWidget = DSettingsWidgetFactory::createStandardItem(QByteArray(), option, shortCutLineEdit);

    option->connect(shortCutLineEdit, &DKeySequenceEdit::editingFinished, [ = ](const QKeySequence & sequence) {

        if (sequence.toString() == "Enter") qDebug() << "==========Enter";

        QString checkName = option->key();
        QString reason;
        bool bIsConflicts = false;
        auto keymap = instance()->settings->option("shortcuts.keymap.keymap");
        QStringList keySplitList = option->key().split(".");
        keySplitList[1] = QString("%1_keymap_%2").arg(keySplitList[1]).arg(keymap->value().toString());

        if (!instance()->checkShortcutValid(checkName, sequence.toString(), reason, bIsConflicts)) {
            instance()->m_pDialog = instance()->createDialog(reason, "", bIsConflicts);
            instance()->m_pDialog->exec();
            shortCutLineEdit->setKeySequence(QKeySequence(instance()->settings->value(keySplitList.join(".")).toString()));
            keymap->setValue("customize");
            return;
        }

        bool bIsCustomize = false;
        QString conflictsKeys;
        QString originalKeys;

        if (keymap->value().toString() != "customize") {
            instance()->m_bUserChangeKey = true;

            for (auto option : instance()->settings->group("shortcuts.window_keymap_customize")->options()) {
                QStringList keySplitList = option->key().split(".");
                keySplitList[1] = QString("window_keymap_%1").arg(keymap->value().toString());

                if (option->value().toString() == sequence.toString()) {

                    if (checkName.contains(keySplitList.last())) {
                        keymap->setValue("customize");
                        //return;
                    } else {
                        bIsConflicts = true;
                        keySplitList[1] = QString("window_keymap_%1").arg("customize");
                        conflictsKeys = keySplitList.join(".");
                    }
                }
            }

            for (auto option : instance()->settings->group("shortcuts.editor_keymap_customize")->options()) {
                QStringList keySplitList = option->key().split(".");
                keySplitList[1] = QString("editor_keymap_%1").arg(keymap->value().toString());

                if (option->value().toString() == sequence.toString()) {

                    if (checkName.contains(keySplitList.last())) {
                        keymap->setValue("customize");
                        //return;
                    } else {
                        bIsConflicts = true;
                        keySplitList[1] = QString("editor_keymap_%1").arg("customize");
                        conflictsKeys = keySplitList.join(".");
                    }
                }
            }

            instance()->m_bUserChangeKey = false;
        }  else {
            bIsCustomize = true;
            instance()->m_bUserChangeKey = true;
            for (auto option : instance()->settings->group("shortcuts.window_keymap_customize")->options()) {
                QStringList keySplitList = option->key().split(".");
                keySplitList[1] = QString("window_keymap_%1").arg(keymap->value().toString());

                if (option->value().toString() == sequence.toString()) {

                    if (checkName.contains(keySplitList.last())) {
                        //return;
                    } else {
                        bIsConflicts = true;
                        conflictsKeys = keySplitList.join(".");
                    }
                }
            }

            for (auto option : instance()->settings->group("shortcuts.editor_keymap_customize")->options()) {
                QStringList keySplitList = option->key().split(".");
                keySplitList[1] = QString("editor_keymap_%1").arg(keymap->value().toString());

                if (option->value().toString() == sequence.toString()) {

                    if (checkName.contains(keySplitList.last())) {
                        //return;
                    } else {
                        bIsConflicts = true;
                        conflictsKeys = keySplitList.join(".");
                    }
                }
            }
            instance()->m_bUserChangeKey = false;
        }

        keySplitList = option->key().split(".");
        keySplitList[1] = QString("%1_keymap_%2").arg(keySplitList[1]).arg(keymap->value().toString());
        QString qstrSequence = sequence.toString();

        if (sequence.toString().contains("<")) {
            qstrSequence.replace(qstrSequence.indexOf("<"), 1, "&lt;");
        }

        if (sequence.toString().contains("Return")) {
            qstrSequence.replace(qstrSequence.indexOf("Return"), 6, "Enter");
        }

        QString style = QString("<span style=\"color: rgba(255, 87, 54, 1);\">[%1]</span>").arg(qstrSequence);

        if (bIsConflicts || sequence.toString() == "Alt+M") {
            if (sequence.toString() == "Alt+M") {
                instance()->m_pDialog = instance()->createDialog(tr("This shortcut conflicts with system shortcut %1").arg(style), "", bIsConflicts);
            } else {
                instance()->m_pDialog = instance()->createDialog(tr("This shortcut conflicts with %1, click on Replace to make this shortcut effective immediately").arg(style), "", bIsConflicts);
            }

            int mode = instance()->m_pDialog->exec();

            // click cancel button.
            if (mode == -1 || mode == 0) {
                shortCutLineEdit->setKeySequence(QKeySequence(instance()->settings->value(keySplitList.join(".")).toString()));
                keymap->setValue("customize");
                return;
            } else {
                keySplitList = option->key().split(".");
                keySplitList[1] = QString("%1_keymap_customize").arg(keySplitList[1]);

                if (!bIsCustomize) {
                    instance()->settings->option(keySplitList.join("."))->setValue(sequence.toString());
                    instance()->settings->option(conflictsKeys)->setValue("");
                    shortCutLineEdit->setKeySequence(QKeySequence(instance()->settings->value(checkName).toString()));
                } else {
                    instance()->settings->option(keySplitList.join("."))->setValue(sequence.toString());
                    instance()->settings->option(conflictsKeys)->setValue("");
                }
                keymap->setValue("customize");
                return;
            }
        }

        if (!bIsCustomize) {
            keySplitList = option->key().split(".");
            keySplitList[1] = QString("%1_keymap_customize").arg(keySplitList[1]);
            instance()->settings->option(keySplitList.join("."))->setValue(sequence.toString());
        } else {
            instance()->settings->option(keySplitList.join("."))->setValue(sequence.toString());
        }
        keymap->setValue("customize");
    });

    // 配置修改
    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged, shortCutLineEdit, [ = ](const QVariant & value) {
        QString keyseq = value.toString();

        if (keyseq.isEmpty()) {
            shortCutLineEdit->clear();
            return;
        }

        shortCutLineEdit->setKeySequence(QKeySequence(keyseq));
    });

    return optionWidget;
}

Settings *Settings::instance()
{
    if (s_pSetting == nullptr) {
        s_pSetting = new Settings;
    }
    return s_pSetting;
}

void Settings::updateAllKeysWithKeymap(QString keymap)
{
    m_bUserChangeKey = true;

    for (auto option : settings->group("shortcuts.window")->options()) {
        QStringList keySplitList = option->key().split(".");
        keySplitList[1] = QString("%1_keymap_%2").arg(keySplitList[1]).arg(keymap);
        option->setValue(settings->option(keySplitList.join("."))->value().toString());
    }

    for (auto option : settings->group("shortcuts.editor")->options()) {
        QStringList keySplitList = option->key().split(".");
        keySplitList[1] = QString("%1_keymap_%2").arg(keySplitList[1]).arg(keymap);
        option->setValue(settings->option(keySplitList.join("."))->value().toString());
    }

    m_bUserChangeKey = false;
}

void Settings::copyCustomizeKeysFromKeymap(QString keymap)
{
    m_bUserChangeKey = true;

    for (auto option : settings->group("shortcuts.window_keymap_customize")->options()) {
        QStringList keySplitList = option->key().split(".");
        keySplitList[1] = QString("window_keymap_%1").arg(keymap);
        option->setValue(settings->option(keySplitList.join("."))->value().toString());
    }

    for (auto option : settings->group("shortcuts.editor_keymap_customize")->options()) {
        QStringList keySplitList = option->key().split(".");
        keySplitList[1] = QString("editor_keymap_%1").arg(keymap);
        option->setValue(settings->option(keySplitList.join("."))->value().toString());
    }

    m_bUserChangeKey = false;
}

bool Settings::checkShortcutValid(const QString &Name, QString Key, QString &Reason, bool &bIsConflicts)
{
    Q_UNUSED(Name);

    if (Key.contains("<")) {
        Key.replace(Key.indexOf("<"), 1, "&lt;");
    }

    QString style = QString("<span style=\"color: rgba(255, 87, 54, 1);\">[%1]</span>").arg(Key);
    // 单键
    if (Key.count("+") == 0) {
        //F1-F12是允许的，这个正则不够精确，但是没关系。
        QRegExp regexp("^F[0-9]{1,2}$");
        if (!Key.contains(regexp)) {
            Reason = tr("The shortcut %1 is invalid, please set another one.").arg(style);
            bIsConflicts = false;
            return  false;
        }
    }
    // 小键盘单键都不允许
    QRegExp regexpNum("^Num+.*");
    if (Key.contains(regexpNum)) {
        Reason = tr("The shortcut %1 is invalid, please set another one.").arg(style);
        bIsConflicts = false;
        return  false;
    }

//    // 与设置里的快捷键冲突检测
//    if (isShortcutConflict(Name, Key)) {
//        Reason = tr("This shortcut key conflicts with %1, click add to make this shortcut key take effect immediately").arg(style);
//        bIsConflicts = true;
//        return  false;
//    }

//    bIsConflicts = true;
    return true;
}

bool Settings::isShortcutConflict(const QString &Name, const QString &Key)
{
    for (QString tmpKey : settings->keys()) {
        if (settings->value(tmpKey).toString() == Key/* && tmpKey.contains("customize")*/) {
            if (Name != tmpKey) {
                return  true;
            }
        }
    }
    return false;
}

DDialog *Settings::createDialog(const QString &title, const QString &content, const bool &bIsConflicts)
{
    DDialog *dialog = new DDialog(title, content, m_pSettingsDialog);
    dialog->setWindowFlags(dialog->windowFlags() | Qt::WindowStaysOnBottomHint);
    dialog->setIcon(QIcon(":/images/dmarked.svg"));

    if (bIsConflicts) {
        dialog->addButton(QString(tr("Cancel")), true, DDialog::ButtonNormal);
        dialog->addButton(QString(tr("Replace")), false, DDialog::ButtonRecommend);
    } else {
        dialog->addButton(QString(tr("OK")), true, DDialog::ButtonRecommend);
    }

    return dialog;
}
