QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

CONFIG += link_pkgconfig
CONFIG += disable-desktop

#INCLUDEPATH += "/usr/include/"
#INCLUDEPATH += "/usr/lib/include/"
##INCLUDEPATH += "/home/kikorik/QtFolder/Qt5.15.8_armV8/sysroot/usr/include/"
##INCLUDEPATH += "/home/kikorik/QtFolder/Qt5.15.8_armV8/sysroot/usr/lib/include/"


##LIBS += -lgpiod
#LIBS += -L"/usr/lib/aarch64-linux-gnu/" -lgpiod
##LIBS += -L"/home/kikorik/QtFolder/Qt5.15.8_armV8/sysroot/usr/lib/aarch64-linux-gnu/" -lgpiod

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QT_QPA_ENABLE_TERMINAL_KEYBOARD = 0

SOURCES += \
    LoggingCategories.cpp \
    linkstm.cpp \
    main.cpp \
    mainwindow.cpp \
    uartconnect.cpp

HEADERS += \
    LoggingCategories.h \
    linkstm.h \
    mainwindow.h \
    uartconnect.h

FORMS += \
    mainwindow.ui

target.path = /usr/share/qtpr
INSTALLS += target
