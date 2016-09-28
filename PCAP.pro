TEMPLATE = app

QT += qml quick
LIBS += -lpcap
CONFIG += c++11

SOURCES += main.cpp \
    pcap_s.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    pcap_s.h \
    mediaclass.h

