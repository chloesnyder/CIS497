QT += core widgets

TARGET = 277
TEMPLATE = app
CONFIG += c++11
CONFIG += warn_on
CONFIG += debug

INCLUDEPATH += include

include(src/src.pri)

FORMS += forms/mainwindow.ui

RESOURCES += glsl.qrc

# Enable console on Windows so that we can actually see debug prints.
win32 {
    CONFIG += console
}

*-clang*|*-g++* {
    message("Enabling additional warnings")
    CONFIG -= warn_on
    QMAKE_CXXFLAGS += -Wall -Wextra -pedantic -Winit-self
    QMAKE_CXXFLAGS += -Wno-strict-aliasing
    QMAKE_CXXFLAGS += -Wno-unneeded-internal-declaration
    QMAKE_CXXFLAGS += -fno-omit-frame-pointer
}
linux-clang*|linux-g++*|macx-clang*|macx-g++* {
    message("Enabling stack protector")
    QMAKE_CXXFLAGS += -fstack-protector-all
}

# FOR LINUX & MAC USERS INTERESTED IN ADDITIONAL BUILD TOOLS
# ----------------------------------------------------------
# This conditional exists to enable Address Sanitizer (ASAN) during
# the automated build. ASAN is a compiled-in tool which checks for
# memory errors (like Valgrind). You may use these tools yourself;
# `asan-build.sh` builds the project with ASAN. But be aware: ASAN may
# trigger a lot of false-positive leak warnings for the Qt libraries.
# (Use `asan-run.sh` to run the program with leak checking disabled.)
address_sanitizer {
    message("Enabling Address Sanitizer")
    QMAKE_CXXFLAGS += -fsanitize=address
    QMAKE_LFLAGS += -fsanitize=address
}


unix: LIBS += -L$$PWD/include/dcmtk/lib/ \
-loflog \
-ldcmimgle \
-ldcmdata \
-lofstd

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/liboflog.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -ldcmimgle

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libdcmimgle.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -ldcmdata

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libdcmdata.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -lcharls

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libcharls.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -ldcmdsig

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libdcmdsig.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -ldcmimage

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libdcmimage.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -ldcmjpeg

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libdcmjpeg.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -ldcmjpls

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libdcmjpls.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -ldcmjpls

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libdcmjpls.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -ldcmnet

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libdcmnet.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -ldcmpstat

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libdcmpstat.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -ldcmqrdb

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libdcmqrdb.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -ldcmsr

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libdcmsr.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -ldcmtls

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libdcmtls.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -ldcmwlm

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libdcmwlm.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -lijg8

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libijg8.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -lijg12

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libijg12.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -lijg16

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libijg16.a

unix: LIBS += -L$$PWD/include/dcmtk/lib/ -llibi2d

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/liblibi2d.a
unix: LIBS += -L$$PWD/include/dcmtk/lib/ -lofstd

INCLUDEPATH += $$PWD/include/dcmtk/include
DEPENDPATH += $$PWD/include/dcmtk/include

unix: PRE_TARGETDEPS += $$PWD/include/dcmtk/lib/libofstd.a

