

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = canopen_ctrl_app
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \

HEADERS += \
        widget.h

FORMS += \
        widget.ui
#Ros-related configure:
INCLUDEPATH += -I /opt/ros/noetic/include
INCLUDEPATH += -I /usr/include/boost

DEPENDPATH +=  /opt/ros/noetic/include

LIBS += -L /opt/ros/noetic/lib -lpthread
LIBS += -L /opt/ros/noetic/lib -lcpp_common

LIBS += /usr/lib/x86_64-linux-gnu/libboost_thread.a
LIBS += /usr/lib/x86_64-linux-gnu/libboost_system.a

LIBS += /opt/ros/noetic/lib/libroscpp.so
LIBS += /opt/ros/noetic/lib/libroscpp_serialization.so
LIBS += /opt/ros/noetic/lib/libroslib.so
LIBS += /opt/ros/noetic/lib/libactionlib.so
LIBS += /opt/ros/noetic/lib/libtf.so
LIBS += /opt/ros/noetic/lib/libtf2.so
LIBS += /opt/ros/noetic/lib/libtf2_ros.so
LIBS += /opt/ros/noetic/lib/librostime.so
LIBS += /opt/ros/noetic/lib/librosconsole.so
LIBS += /opt/ros/noetic/lib/librosconsole_log4cxx.so
LIBS += /opt/ros/noetic/lib/librosconsole_backend_interface.so
LIBS += /opt/ros/noetic/lib/libxmlrpcpp.so

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
