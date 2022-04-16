TEMPLATE = app
TARGET = btctl

QT = core bluetooth widgets
requires(qtConfig(listwidget))
android: QT += androidextras
CONFIG += c++11
CONFIG += c++14
CONFIG += c++17

QT += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT       += concurrent
#QT += core gui charts greaterThan(QT_MAJOR_VERSION, 4): QT += widgets SOURCES += \ source.cpp
QT       += core gui charts
QT += opengl
QT += svg
DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES = \
    chat.cpp \
    graphicsline.cpp \
    main.cpp \
    mainwindow4.cpp \
    mainwindow5.cpp \
    plot.cpp \
    remoteselector.cpp \
    chatserver.cpp \
    chatclient.cpp \
    samplingthread.cpp \
    signaldata.cpp

HEADERS = \
    Global_Var.h \
    chat.h \
    filter.h \
    graphicsline.h \
    mainwindow4.h \
    mainwindow5.h \
    plot.h \
    remoteselector.h \
    chatserver.h \
    chatclient.h \
    samplingthread.h \
    signaldata.h

FORMS = \
    chat.ui \
    filter.ui \
    remoteselector.ui \
    remoteselector2.ui

UI_DIR = $$PWD
QT       += concurrent
#QT += core gui charts greaterThan(QT_MAJOR_VERSION, 4): QT += widgets SOURCES += \ source.cpp

#QT       += core gui charts



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
INCLUDEPATH += https://github.com/shamino1978/include.git
INCLUDEPATH += D:\Microsoft Visual Studio 2019b\VC\Tools\MSVC\14.25.28610\include
#INCLUDEPATH += C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\um\x64
INCLUDEPATH += D:\Python27\include
INCLUDEPATH += D:\Python27\Lib\site-packages\numpy\core\include

INCLUDEPATH += D:\Python27\include\ D:\Python27\libs\

#INCLUDEPATH += D:\Qtfiles\Latestgood20
#INCLUDEPATH += D:\Qtfiles\Latestgood29
#INCLUDEPATH += D:\Qtfiles\Latestgood35

#LIBS += D:\Qtfiles\Latestgood35\SetupApi.lib
#LIBS += D:\Qtfiles\Latestgood35\qwt.lib
#LIBS += D:\Qtfiles\Latestgood35\qwtd.lib

#LIBS += D:\Qtfiles\Latestgood30\cpu_xgb.lib
#LIBS += D:\Qtfiles\Latestgood30\wevtapi.lib
#LIBS += D:\Qtfiles\Latestgood20\python27.lib

#CONFIG   += ordered
#CONFIG += plugin
#CONFIG += skip_target_version_ext
#CONFIG += unversioned_soname
#CONFIG += unversioned_libname

#LIBS += -L$$PWD/./ -lqwt
#LIBS += -L$$PWD/./ -lqwtd

#LIBS += D:\Qtfiles\btappandroid2\libqwt.so
#LIBS += -L"D:\Qtfiles\btappandroid2" -libqwt.so


#QMAKE_CXXFLAGS += D:\Qtfiles\Latestgood20\python27.lib

#DEFINES += QWT_DLL

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

#win32: LIBS += -L$$PWD/./ -lpython27


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lqwt
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lqwtd

#INCLUDEPATH += $$PWD/.

#DEPENDPATH += $$PWD/.

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./libqwt.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./libqwtd.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./qwt.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./qwtd.lib

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lqwt
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lqwtd

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./libqwt.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./libqwtd.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./qwt.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./qwtd.lib

#include( qwtconfig.pri )

#qwtspec.files  = qwtconfig.pri qwtfunctions.pri qwt.prf
#qwtspec.path  = $${QWT_INSTALL_FEATURES}


#DISTFILES += D:\\handcpp\\HuffmanUtil.obj



#INSTALLS += qwtspec
#CONFIG   += ordered

#contains(QWT_CONFIG, QwtWidgets)

#contains(QWT_CONFIG, QwtDll) {
#        DEFINES    += QT_DLL QWT_DLL
#}

#win32 {
#    contains(QWT_CONFIG, QwtDll) {
#        DEFINES    += QT_DLL QWT_DLL
#    }
#}



DISTFILES += \
    android/AndroidManifest.xml \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/build.gradle \
    android/gradle.properties \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew \
    android/gradlew.bat \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/res/values/libs.xml \

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

ANDROID_TARGET_SDK_VERSION = 30

