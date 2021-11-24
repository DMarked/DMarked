/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 * Copyright (C) 2021 DMarked.
 *
 * Author:     rekols <rekols@foxmail.com>
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

#include "ddropdownmenu.h"
#include "mdtheme.h"
#include "settings.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <DApplication>
#include <QPainter>
#include <DSettingsOption>
#include <QDebug>
#include <DFontSizeManager>
#include <DLabel>
#include <DApplicationHelper>
#include <QtSvg/QSvgRenderer>
#include <QActionGroup>
#include <DSettingsOption>

using namespace Dtk::Core;

DDropdownMenu::DDropdownMenu(QWidget *parent)
    : QFrame(parent)
    , m_pToolButton(new DToolButton(this))
    , m_menu(new DMenu)
{
    //设置toobutton属性
    m_pToolButton->setFocusPolicy(Qt::StrongFocus);
    m_pToolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_pToolButton->setArrowType(Qt::NoArrow);
    m_pToolButton->setFixedHeight(28);
    m_pToolButton->installEventFilter(this);
    //this->installEventFilter(this);
    // 设置图标
    QString theme =  (DGuiApplicationHelper::instance()->applicationPalette().color(QPalette::Background).lightness() < 128) ? "dark" : "light";
    QString arrowSvgPath = QString(":/images/arrow_%1.svg").arg(theme);

    // 装换图片
    int scaled =qApp->devicePixelRatio() == 1.25 ? 2 : 1;
    QSvgRenderer svg_render(arrowSvgPath);
    QPixmap pixmap(QSize(8,5)*scaled);
    pixmap.fill(Qt::transparent);
    pixmap.setDevicePixelRatio(scaled);
    QPainter painter(&pixmap);
    svg_render.render(&painter,QRect(0,0,8,5));
    m_arrowPixmap = pixmap;
    m_pToolButton->setIcon(createIcon());

    // 设置字体
    int fontsize =DFontSizeManager::instance()->fontPixelSize(DFontSizeManager::T9);
    m_font.setPixelSize(fontsize);
    m_font.setFamily("SourceHanSansSC-Normal");

    // 添加布局
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(m_pToolButton);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    connect(this, &DDropdownMenu::requestContextMenu, this, &DDropdownMenu::slotRequestMenu);

    // 设置字体自适应大小 设置界面大小根据内容大小自适应 梁卫东 2020.7.7
    connect(qApp, &DApplication::fontChanged,this,&DDropdownMenu::OnFontChangedSlot);
}

DDropdownMenu::~DDropdownMenu()
{
    deleteMenuActionGroup();
    deleteMenu();
}

DDropdownMenu *DDropdownMenu::createThemeMenu()
{
    DDropdownMenu *m_pThemeMenu = new DDropdownMenu();
    DMenu *m_pMenu = new DMenu;
    m_pThemeMenu->m_actionGroup = new QActionGroup(m_pMenu);

    QAction *noHlAction = m_pMenu->addAction("none");
    m_pThemeMenu->m_actionGroup->addAction(noHlAction);
    noHlAction->setCheckable(true);
    noHlAction->setChecked(false);

    bool isDark = DGuiApplicationHelper::instance()->applicationPalette().color(QPalette::Background).lightness() < 128;
    foreach(const QString &theme, MdTheme::light_themes) {
        QAction *act= m_pMenu->addAction(theme);
        m_pThemeMenu->m_actionGroup->addAction(act);
        act->setCheckable(true);
        act->setVisible(!isDark);
        if (!isDark && theme == MdTheme::getCurrentLightTheme())
            act->setChecked(true);
    }
    foreach(const QString &theme, MdTheme::dark_themes) {
        QAction *act= m_pMenu->addAction(theme);
        m_pThemeMenu->m_actionGroup->addAction(act);
        act->setCheckable(true);
        act->setVisible(isDark);
        if (isDark && theme == MdTheme::getCurrentDarkTheme())
            act->setChecked(true);
    }

    m_pThemeMenu->setMenu(m_pMenu);
    m_pThemeMenu->setCurrentTextOnly(isDark ? MdTheme::getCurrentDarkTheme() : MdTheme::getCurrentLightTheme());


    connect(m_pMenu, &DMenu::triggered, m_pThemeMenu, [m_pThemeMenu](QAction *action) {
        if (m_pThemeMenu->m_text != action->text()) {
            m_pThemeMenu->setCurrentTextOnly(action->text());
            emit m_pThemeMenu->currentActionChanged(action);
        }
    });

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            [m_pThemeMenu](DGuiApplicationHelper::ColorType themeType){
        bool isDark = themeType == DGuiApplicationHelper::ColorType::DarkType;
        bool isUnknown = themeType == DGuiApplicationHelper::ColorType::UnknownType;
        foreach (QAction *action, m_pThemeMenu->m_menu->actions()) {
            if (action->text() == "none") continue;
            if (MdTheme::light_themes.contains(action->text()))
                action->setVisible(isUnknown || !isDark);
            else
                action->setVisible(isUnknown || isDark);
            if ((!isDark && action->text() == MdTheme::getCurrentLightTheme())
                    || (isDark && action->text() == MdTheme::getCurrentDarkTheme())) {
                m_pThemeMenu->setCurrentTextOnly(action->text());
                emit m_pThemeMenu->currentActionChanged(action);
            }
        }
    });

    return m_pThemeMenu;
}

DDropdownMenu *DDropdownMenu::createModeMenu() {
    DDropdownMenu *m_pModeMenu = new DDropdownMenu();
    DMenu *m_pMenu = new DMenu;
    m_pModeMenu->m_actionGroup = new QActionGroup(m_pMenu);

    QAction *act1 = m_pMenu->addAction(tr("Read Mode"));
    m_pModeMenu->m_actionGroup->addAction(act1);
    act1->setCheckable(true);

    QAction *act2 = m_pMenu->addAction(tr("Write Mode"));
    m_pModeMenu->m_actionGroup->addAction(act2);
    act2->setCheckable(true);

    QAction *act3 = m_pMenu->addAction(tr("Preview Mode(S)"));
    m_pModeMenu->m_actionGroup->addAction(act3);
    act3->setCheckable(true);

    QAction *act4 = m_pMenu->addAction(tr("Preview Mode(N)"));
    m_pModeMenu->m_actionGroup->addAction(act4);
    act4->setCheckable(true);

    m_pModeMenu->setMenu(m_pMenu);

    QAction *act = nullptr;
    switch (Settings::instance()->settings->option("advance.editor.editor_mode")->value().toInt()) {
    case 0: act = act1; break;
    case 1: act = act2; break;
    case 2: act = act3; break;
    default: act = act4;
    }
    act->setChecked(true);
    m_pModeMenu->setCurrentTextOnly(act->text());

    connect(m_pMenu, &DMenu::triggered, m_pModeMenu, [m_pModeMenu](QAction *action) {
        if (m_pModeMenu->m_text != action->text()) {
            m_pModeMenu->setCurrentTextOnly(action->text());
            emit m_pModeMenu->currentActionChanged(action);
        }
    });

    return m_pModeMenu;
}

void DDropdownMenu::setFontEx(const QFont& font)
{
    m_pToolButton->setFont(font);
    m_font = font;
}

void DDropdownMenu::setCurrentAction(QAction *pAct)
{
    if(pAct){
        QList<QAction*> menuList = m_menu->actions();
        pAct->setChecked(true);
        for (int i = 0; i < menuList.size(); i++) {
            QList<QAction*> acts = menuList[i]->menu()->actions();
            for (int j = 0; j < acts.size(); j++) {
                if(acts[j] != pAct) acts[j]->setChecked(false);
            }
        }
        setText(pAct->text());
    }
}

void DDropdownMenu::setCurrentTextOnly(const QString &name)
{
   QList<QAction*> menuList = m_menu->actions();

   for (int i = 0; i < menuList.size(); i++) {
       if (menuList[i]->menu()){
           QList<QAction*> acts = menuList[i]->menu()->actions();
           if (acts.size() == 0) continue;
           for (int j = 0; j < acts.size(); j++) {
           if (acts[j]->text() != name) {
               acts[j]->setCheckable(false);
               acts[j]->setChecked(false);
           }
           else {
               acts[j]->setCheckable(true);
               acts[j]->setChecked(true);
           }
        }
      }
   }
   setText(name);
}

void DDropdownMenu::slotRequestMenu(bool request)
{
    if (request) {
        // 如果鼠标点击清除 focus
        m_pToolButton->clearFocus();
    }
    QPoint center = this->mapToGlobal(this->rect().center());
    int menuHeight = m_menu->sizeHint().height();
    int menuWidth = m_menu->sizeHint().width();
    center.setY(center.y() - menuHeight - this->rect().height() / 2);
    center.setX(center.x() - menuWidth / 2);
    m_menu->move(center);
    m_menu->exec();
    // 清除 focus
    m_pToolButton->clearFocus();
    QEvent event(QEvent::HoverLeave);
    QApplication::sendEvent(m_pToolButton, &event);
    emit sigSetTextEditFocus();
}

void DDropdownMenu::setText(const QString &text)
{
    m_text = text;
    // 重新绘制icon　设置宽度
    m_pToolButton->setIcon(createIcon());
}

void DDropdownMenu::setMenu(DMenu *menu)
{
    deleteMenu();
    m_menu = menu;
}

void DDropdownMenu::deleteMenu()
{
    if (m_menu != nullptr) {
        delete m_menu;
        m_menu = nullptr;
    }
}

void DDropdownMenu::setMenuActionGroup(QActionGroup *actionGroup)
{
    deleteMenuActionGroup();
    m_actionGroup = actionGroup;
}

void DDropdownMenu::deleteMenuActionGroup()
{
    if (m_actionGroup != nullptr) {
        delete m_actionGroup;
        m_actionGroup = nullptr;
    }
}

void DDropdownMenu::setTheme(const QString &theme)
{
    QString arrowSvgPath = QString(":/images/arrow_%1.svg").arg(theme);
    // 装换图片
    int scaled =qApp->devicePixelRatio() == 1.25 ? 2 : 1;
    QSvgRenderer svg_render(arrowSvgPath);

    QPixmap pixmap(QSize(8,5)*scaled);
    pixmap.fill(Qt::transparent);
    pixmap.setDevicePixelRatio(scaled);

    QPainter painter(&pixmap);
    svg_render.render(&painter,QRect(0,0,8,5));

    m_arrowPixmap = pixmap;
    m_pToolButton->setIcon(createIcon());
}

void DDropdownMenu::setChildrenFocus(bool ok)
{
    if(ok)  m_pToolButton->setFocusPolicy(Qt::StrongFocus);
    else  {
        m_pToolButton->clearFocus();
        m_pToolButton->setFocusPolicy(Qt::NoFocus);
    }
}

void DDropdownMenu::setRequestMenu(bool request)
{
    isRequest = request;
}

DToolButton *DDropdownMenu::getButton()
{
    return m_pToolButton;
}

QIcon DDropdownMenu::createIcon()
{
    DPalette dpalette  = DApplicationHelper::instance()->palette(m_pToolButton);
    QColor textColor;

    QPixmap arrowPixmap;

    if(m_bPressed){
        textColor = dpalette.color(DPalette::Highlight);
        QString color = textColor.name(QColor::HexRgb);
        arrowPixmap = setSvgColor(color);
    }else {
        textColor = dpalette.color(DPalette::WindowText);
        arrowPixmap = m_arrowPixmap;
    }

    // 根据字体大小设置icon大小
    // height 30    width QFontMetrics fm(font()) fm.width(text)+40;
    int fontWidth = QFontMetrics(m_font).width(m_text)+20;
    int fontHeight = QFontMetrics(m_font).height();
    int iconW = 8;
    int iconH = 5;

    int totalWidth = fontWidth + iconW + 20;
    int totalHeigth = 28;
    m_pToolButton->setFixedSize(totalWidth,totalHeigth);
    m_pToolButton->setIconSize(QSize(totalWidth,totalHeigth));

    qreal rate = this->devicePixelRatioF();
    QPixmap icon(QSize(totalWidth,totalHeigth)*rate);
    icon.setDevicePixelRatio(rate);
    icon.fill(Qt::transparent);

    QPainter painter(&icon);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);

    painter.save();
    painter.setFont(m_font);
    painter.setPen(textColor);
    painter.drawText(QRectF(10,(totalHeigth-fontHeight)/2,fontWidth,fontHeight),m_text);
    painter.restore();

    //arrowPixmap=arrowPixmap.scaled(iconW,iconH,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    painter.drawPixmap(QRectF(fontWidth,(totalHeigth-iconH)/2,iconW,iconH),arrowPixmap,arrowPixmap.rect());

    painter.end();
    return icon;
}

void DDropdownMenu::OnFontChangedSlot(const QFont &font)
{
    m_font = font;
    int fontsize =DFontSizeManager::instance()->fontPixelSize(DFontSizeManager::T8);
    m_font.setPixelSize(fontsize);
    m_pToolButton->setIcon(createIcon());
}

bool DDropdownMenu::eventFilter(QObject *object, QEvent *event)
{
    if(object == m_pToolButton){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            //QString key = Utils::getKeyshortcut(keyEvent);
            if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Space)        //按下enter展开列表
            {
                //if(isRequest){
                    Q_EMIT requestContextMenu(false);
                //}
                return true;
            }
            return false;
        }

        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if(mouseEvent->button() == Qt::LeftButton){
                m_bPressed = true;
                //if (isRequest) {
                    //重新绘制icon 点击改变前景色
                    m_pToolButton->setIcon(createIcon());
                //}
                return true;
            }

            if(mouseEvent->button() == Qt::RightButton){
                return true;
            }
        }

        if(event->type() == QEvent::MouseButtonRelease){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if(mouseEvent->button() == Qt::LeftButton){
                m_bPressed = false;
                m_pToolButton->setIcon(createIcon());
                if (isEnabled()) {
                    Q_EMIT requestContextMenu(true);
                }
                m_pToolButton->clearFocus();
            }
            return true;
        }
    }
    return QFrame::eventFilter(object,event);
}

QPixmap DDropdownMenu::setSvgColor(QString color)
{
    // 设置图标颜色
    QString path = QString(":/images/arrow_dark.svg");

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    QDomDocument doc;
    doc.setContent(data);
    file.close();
    QDomElement elem = doc.documentElement();
    SetSVGBackColor(elem, "fill", color);

    // 装换图片
    qreal scaled = this->devicePixelRatioF();
    QSvgRenderer svg_render(doc.toByteArray());

    QPixmap pixmap(QSize(8,5)*scaled);
    pixmap.fill(Qt::transparent);
    pixmap.setDevicePixelRatio(scaled);

    QPainter painter(&pixmap);
    svg_render.render(&painter,QRect(0,0,8,5));

    return pixmap;
}

void DDropdownMenu::SetSVGBackColor(QDomElement &elem, QString strattr, QString strattrval)
{
    if (elem.tagName().compare("g") == 0 && elem.attribute("id").compare("color") == 0)
    {
        QString before_color = elem.attribute(strattr);
        elem.setAttribute(strattr, strattrval);
    }
    for (int i = 0; i < elem.childNodes().count(); i++)
    {
        if (!elem.childNodes().at(i).isElement()) continue;
        QDomElement element = elem.childNodes().at(i).toElement();
        SetSVGBackColor(element, strattr, strattrval);
    }
}
