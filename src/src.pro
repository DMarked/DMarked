TEMPLATE = app

QT += webenginewidgets webchannel

PKGCONFIG += dtkwidget
CONFIG -= qtquickcompiler
CONFIG += c++11 link_pkgconfig

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    widgets/bottombar.h \
    mainwindow.h \
    widgets/centralwidget.h \
    widgets/preview-widget/document.h \
    widgets/preview-widget/previewpage.h \
    widgets/preview-widget/previewwidget.h

SOURCES = \
    main.cpp \
    mainwindow.cpp \
    widgets/bottombar.cpp \
    widgets/centralwidget.cpp \
    widgets/preview-widget/document.cpp \
    widgets/preview-widget/previewpage.cpp \
    widgets/preview-widget/previewwidget.cpp

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
