TEMPLATE = app

QT += webenginewidgets webchannel xml svg

PKGCONFIG += dtkwidget
CONFIG -= qtquickcompiler
CONFIG += c++17 link_pkgconfig

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
    utils/utils.h \
    widgets/editor-widget/fakevimproxy.h

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
    utils/settingsdialog.cpp \
    widgets/editor-widget/fakevimproxy.cpp

RESOURCES = \
    ../resources/markdowneditor.qrc \
    translations.qrc

# install
isEmpty(BINDIR):BINDIR=$${PREFIX}/bin
isEmpty(APPDIR):APPDIR=$${PREFIX}/share/applications
isEmpty(DSRDIR):DSRDIR=$${PREFIX}/share/dmarked
isEmpty(ICONDIR):ICONDIR=$${PREFIX}/share/icons

target.path = $${BINDIR}

desktop.path = $${APPDIR}
desktop.files = $$PWD/../dmarked.desktop

icon.path = $${ICONDIR}
icon.files = $$PWD/../resources/images/dmarked.svg

system("lrelease translations/*.ts")
qm_file.files = translations/*.qm
qm_file.path = $${DSRDIR}/translations/

INSTALLS += target qm_file icon desktop

# qmarkdowntextedit
LIBS += -L$$OUT_PWD/../3rdparty/qmarkdowntextedit -lQMarkdownTextedit
INCLUDEPATH += $$PWD/../3rdparty/qmarkdowntextedit
DEPENDPATH += $$PWD/../3rdparty/qmarkdowntextedit

# FakeVim
LIBS += -L$$OUT_PWD/../3rdparty/FakeVim/fakevim -lFakeVim
INCLUDEPATH += $$PWD/../3rdparty/FakeVim/fakevim
DEPENDPATH += $$PWD/../3rdparty/FakeVim/fakevim

FORMS += \
    widgets/topdfdlg.ui



