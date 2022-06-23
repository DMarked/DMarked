#-------------------------------------------------
#
# Project created by QtCreator 2021-10-16T11:40:15
#
#-------------------------------------------------

TEMPLATE = subdirs

isEmpty(PREFIX):PREFIX=/usr
isEmpty(LIBS_PREFIX):LIBS_PREFIX=/usr

SUBDIRS = \
    3rdparty \
    dmarked

dmarked.depends = 3rdparty


