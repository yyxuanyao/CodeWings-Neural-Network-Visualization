QT       += core gui
QT += widgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    backend.cpp \
    codegenerator.cpp \
    codegeneratorwindow.cpp \
    colorthememanager.cpp \
    connectionitem.cpp \
    json_utils.cpp \
    layeritem.cpp \
    main.cpp \
    mainwindow.cpp \
    matrial.cpp\
    movablelayergroup.cpp \
    networkvisualizer.cpp \
    neuronitem.cpp \
    programfragmentprocessor.cpp \
    propertypanel.cpp

HEADERS += \
    backend.h \
    codegenerator.h \
    codegeneratorwindow.h \
    colorthememanager.h \
    connectionitem.h \
    json_utils.h \
    layeritem.h \
    mainwindow.h \
    matrial.h\
    movablelayergroup.h \
    networkvisualizer.h \
    neuronitem.h \
    programfragmentprocessor.h \
    propertypanel.h

FORMS += \
    mainwindow.ui \
    codegeneratorwindow.ui \
    matrial.ui

#TRANSLATIONS += \
    #CodeWings_zh_CN.ts
CONFIG += lrelease
#CONFIG += embed_translations

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

#RESOURCES += \
    #resource.qrc

DISTFILES += \
    .gitignore \
    Neural_Network_Visualization.pro.user \
    Neural_Network_Visualization.pro.user.344c8fb

RESOURCES += \
    resource.qrc \
    resource.qrc
