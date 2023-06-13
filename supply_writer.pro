QT       += core gui sql widgets network multimedia

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_CXXFLAGS += -Wno-unused-parameter

#可执行文件存放目录
DESTDIR = $$PWD/../output/

DEFINES += VERSION=\\\"4.0\\\"

#以下为相应的版本信息，分为立思辰版本，南瑞蓝鸟版本，版本中必须取其一
#立思辰
#DEFINES += LANXUM
#南瑞，蓝鸟项目
DEFINES += NARI

if(contains(DEFINES, LANXUM)) {
    TARGET = supply_writer1_lanxum
    RC_ICONS = images/cgprint.ico
}

if(contains(DEFINES, NARI)) {
    TARGET = supply_writer1_nari
    RC_ICONS = images/nari.ico
}

SOURCES += \
    readback.cpp \
    sqlchipinfo.cpp \
    statemonitor.cpp \
    supplywriter.cpp

HEADERS += \
    readback.h \
    sqlchipinfo.h \
    supplywriter.h \
    statemonitor.h \
    common.h

FORMS += \
    readback.ui \
    sqlchipinfo.ui \
    supplywriter.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += src.qrc
