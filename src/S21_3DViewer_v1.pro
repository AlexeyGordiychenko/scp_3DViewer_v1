QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    s21_affine.c \
    s21_gifcreator.cpp \
    s21_glwidget.cpp \
    s21_parser.c \
    thirdparty/gifanim/gifanim.cpp

HEADERS += \
    mainwindow.h \
    s21_gifcreator.h \
    s21_glwidget.h \
    s21_viewer.h \
    thirdparty/gifanim/gifanim.h

FORMS += \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    thirdparty/gifanim/LICENSE \
    thirdparty/gifanim/README.md
