TEMPLATE = app

QT += webenginewidgets webchannel xml svg

PKGCONFIG += dtkwidget
CONFIG -= qtquickcompiler
CONFIG += c++11 link_pkgconfig

DEFINES += QT_DEPRECATED_WARNINGS

TRANSLATIONS = \
    translations/dmarked_zh_CN.ts

HEADERS += \
    widgets/bottombar.h \
    mainwindow.h \
    application.h \
    widgets/dropdown-menu/ddropdownmenu.h \
    widgets/centralwidget.h \
    widgets/preview-widget/document.h \
    widgets/preview-widget/previewpage.h \
    widgets/preview-widget/previewwidget.h \
    widgets/dropdown-menu/mdtheme.h \
    widgets/topdfdlg.h \
    utils/mappagesize.h \
    settings.h \
    widgets/editor-widget/editorwidget.h \
    utils/utils.h

SOURCES = \
    main.cpp \
    mainwindow.cpp \
    widgets/dropdown-menu/ddropdownmenu.cpp \
    widgets/bottombar.cpp \
    widgets/centralwidget.cpp \
    widgets/preview-widget/document.cpp \
    widgets/preview-widget/previewpage.cpp \
    widgets/preview-widget/previewwidget.cpp \
    application.cpp \
    widgets/dropdown-menu/mdtheme.cpp \
    widgets/topdfdlg.cpp \
    utils/mappagesize.cpp \
    settings.cpp \
    widgets/editor-widget/editorwidget.cpp \
    utils/utils.cpp \
    utils/settingsdialog.cpp

RESOURCES = \
    ../resources/markdowneditor.qrc \
    translations.qrc

# install
isEmpty(BINDIR):BINDIR=/usr/bin
isEmpty(APPDIR):APPDIR=/usr/share/applications
isEmpty(DSRDIR):DSRDIR=/usr/share/dmarked

target.path = $$INSTROOT$$BINDIR
desktop.path = $$INSTROOT$$APPDIR
desktop.files = $$PWD/../dmarked.desktop

ICONDIR=/usr/share/icons
icon.path = $$INSTROOT$$ICONDIR
icon.files = $$PWD/../resources/images/dmarked.svg

system("lrelease translations/*.ts")
qm_file.files = translations/*.qm
qm_file.path = $${DSRDIR}/translations/

INSTALLS += target qm_file icon desktop

# qmarkdowntextedit
LIBS += -lQMarkdownTextedit -L$$OUT_PWD
INCLUDEPATH += $$PWD/../3rdparty/qmarkdowntextedit
DEPENDPATH += $$PWD/../3rdparty/qmarkdowntextedit

FORMS += \
    widgets/topdfdlg.ui



