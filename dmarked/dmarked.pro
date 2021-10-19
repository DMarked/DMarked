TEMPLATE = app

QT += webenginewidgets webchannel xml svg

PKGCONFIG += dtkwidget
CONFIG -= qtquickcompiler
CONFIG += c++11 link_pkgconfig

DEFINES += QT_DEPRECATED_WARNINGS

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
    utils/mdexport.h \

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
    utils/mdexport.cpp

RESOURCES = \
    ../resources/markdowneditor.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/webenginewidgets/markdowneditor
INSTALLS += target

# qmarkdowntextedit
LIBS += -lQMarkdownTextedit -L$$OUT_PWD
INCLUDEPATH += $$PWD/../3rdparty/qmarkdowntextedit
DEPENDPATH += $$PWD/../3rdparty/qmarkdowntextedit

# peg-markdown-highlight
#LIBS += -L$$OUT_PWD/../3rdparty/peg-markdown-highlight/ -lpmh
#INCLUDEPATH += $$PWD/../3rdparty/peg-markdown-highlight
#DEPENDPATH += $$PWD/../3rdparty/peg-markdown-highlight
