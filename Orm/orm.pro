TEMPLATE = lib
TARGET = Orm
QT += qml quick sql
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = QtQuick.Orm

# Input
SOURCES += \
    orm_plugin.cpp \
    qorm.cpp \
    qormmetaattribute.cpp \
    qormmetatable.cpp \
    qormobject.cpp \
    #qormloader.cpp \
    #qormsaver.cpp
    private/qormqueryparser.cpp \
    qormloader.cpp

HEADERS += \
    orm_plugin.h \
    qorm.h \
    qormmetaattribute.h \
    qormmetatable.h \
    qormobject.h \
    #qormloader.h \
    #qormsaver.h
    private/qormqueryparser.h \
    qormloader.h

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

