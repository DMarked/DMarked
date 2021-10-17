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
    widgets/ddropdownmenu.h \
    widgets/centralwidget.h \
    widgets/preview-widget/document.h \
    widgets/preview-widget/previewpage.h \
    widgets/preview-widget/previewwidget.h

SOURCES = \
    main.cpp \
    mainwindow.cpp \
    widgets/ddropdownmenu.cpp \
    widgets/bottombar.cpp \
    widgets/centralwidget.cpp \
    widgets/preview-widget/document.cpp \
    widgets/preview-widget/previewpage.cpp \
    widgets/preview-widget/previewwidget.cpp \
    application.cpp

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

DISTFILES += \
    ../resources/images/arrow_dark.svg \
    ../resources/images/arrow_light.svg \
    ../resources/images/bar_close_hover_dark.svg \
    ../resources/images/bar_close_hover_light.svg \
    ../resources/images/bar_close_normal_dark.svg \
    ../resources/images/bar_close_normal_light.svg \
    ../resources/images/bar_close_press_dark.svg \
    ../resources/images/bar_close_press_light.svg \
    ../resources/images/bookmark.svg \
    ../resources/images/close_hover_dark.svg \
    ../resources/images/close_hover_light.svg \
    ../resources/images/close_normal_dark.svg \
    ../resources/images/close_normal_light.svg \
    ../resources/images/close_press_dark.svg \
    ../resources/images/close_press_light.svg \
    ../resources/images/d-d.svg \
    ../resources/images/d-l.svg \
    ../resources/images/deepin-editor.svg \
    ../resources/images/dropdown_arrow_dark.svg \
    ../resources/images/dropdown_arrow_light.svg \
    ../resources/images/like_hover_dark.svg \
    ../resources/images/like_hover_light.svg \
    ../resources/images/logo_24.svg \
    ../resources/images/logo_48.svg \
    ../resources/images/logo_96.svg \
    ../resources/images/ok.svg \
    ../resources/images/u-d.svg \
    ../resources/images/u-l.svg \
    ../resources/images/warning.svg
