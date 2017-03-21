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


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/Cellar/boost/1.63.0/lib/release/ -lboost_filesystem-mt -lboost_system
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/Cellar/boost/1.63.0/lib/debug/ -lboost_filesystem-mt -lboost_system
else:unix: LIBS += -L$$PWD/../../../../../../usr/local/Cellar/boost/1.63.0/lib/ -lboost_filesystem-mt -lboost_system

INCLUDEPATH += $$PWD/../../../../../../usr/local/Cellar/boost/1.63.0/include
DEPENDPATH += $$PWD/../../../../../../usr/local/Cellar/boost/1.63.0/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/Cellar/vtk/7.1.0_5/lib/release/ -lvtkalglib-7.1.1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/Cellar/vtk/7.1.0_5/lib/debug/ -lvtkalglib-7.1.1
else:unix: LIBS += -L$$PWD/../../../../../../usr/local/Cellar/vtk/7.1.0_5/lib/ -lvtkalglib-7.1.1

INCLUDEPATH += $$PWD/../../../../../../usr/local/Cellar/vtk/7.1.0_5/include
DEPENDPATH += $$PWD/../../../../../../usr/local/Cellar/vtk/7.1.0_5/include

HEADERS +=

SOURCES +=
