TEMPLATE = app

QT += qml quick

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH = $$OUT_PWD/../Orm

message($$OUT_PWD)

# Default rules for deployment.
include(deployment.pri)
