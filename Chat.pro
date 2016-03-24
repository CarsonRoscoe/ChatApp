TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    appcontroller.cpp \
    clientqtwrappers.cpp \
    message.cpp

RESOURCES += qml.qrc

RC_ICONS = images/misc/logo.ico

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    appcontroller.h \
    clientCode.h \
    shared.h \
    clientqtwrappers.h \
    message.h
