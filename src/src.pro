TEMPLATE = app

QT += webenginewidgets webchannel

PKGCONFIG += dtkwidget
CONFIG -= qtquickcompiler
CONFIG += c++11 link_pkgconfig

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    mainwindow.h \
    previewpage.h \
    document.h
    #highlighter.h

SOURCES = \
    main.cpp \
    mainwindow.cpp \
    previewpage.cpp \
    document.cpp
    #highlighter.cpp

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
