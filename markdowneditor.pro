TEMPLATE = app

QT += webenginewidgets webchannel


HEADERS += \
    mainwindow.h \
    previewpage.h \
    document.h

SOURCES = \
    main.cpp \
    mainwindow.cpp \
    previewpage.cpp \
    document.cpp

RESOURCES = \
    resources/markdowneditor.qrc

CONFIG -= qtquickcompiler

FORMS += \
    mainwindow.ui

DISTFILES += \
    resources/3rdparty/MARKDOWN-LICENSE.txt \
    resources/3rdparty/MARKED-LICENSE.txt

# install
target.path = $$[QT_INSTALL_EXAMPLES]/webenginewidgets/markdowneditor
INSTALLS += target
