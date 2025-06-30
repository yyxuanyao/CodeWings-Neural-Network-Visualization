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
    layerblockitem.cpp \
    layeritem.cpp \
    main.cpp \
    mainwindow.cpp \
    matrial.cpp \
    movablelayergroup.cpp \
    networkvisualizer.cpp \
    neuronitem.cpp \
    programfragmentprocessor.cpp \
    propertypanel.cpp \
    resourcepage.cpp

HEADERS += \
    backend.h \
    codegenerator.h \
    codegeneratorwindow.h \
    colorthememanager.h \
    colorthememanager.h \
    connectionitem.h \
    json_utils.h \
    layerblockitem.h \
    layeritem.h \
    mainwindow.h \
    matrial.h \
    movablelayergroup.h \
    networkvisualizer.h \
    neuronitem.h \
    programfragmentprocessor.h \
    propertypanel.h \
    resourcepage.h

FORMS += \
    mainwindow.ui \
    codegeneratorwindow.ui \
    matrial.ui

RESOURCES += resource.qrc

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES +=*/ \
    Icon/background.jpg \
    Icon/code-blue.png \
    Icon/code-green.png \
    Icon/code-grey.png \
    Icon/code-pink.png \
    Icon/code-purple.png \
    Icon/code-white.png \
    Icon/code-yellow.png \
    Icon/code.png \
    Icon/color-blue.png \
    Icon/color-green.png \
    Icon/color-grey.png \
    Icon/color-pink.png \
    Icon/color-purple.png \
    Icon/color-white.png \
    Icon/color-yellow.png \
    Icon/history-blue.png \
    Icon/history-green.png \
    Icon/history-grey.png \
    Icon/history-pink.png \
    Icon/history-purple.png \
    Icon/history-white.png \
    Icon/history-yellow.png \
    Icon/history.png \
    Icon/image-blue.png \
    Icon/image-green.png \
    Icon/image-grey.png \
    Icon/image-pink.png \
    Icon/image-purple.png \
    Icon/image-white.png \
    Icon/image-yellow.png \
    Icon/image.png \
    Icon/mode-blue.png \
    Icon/mode-green.png \
    Icon/mode-grey.png \
    Icon/mode-pink.png \
    Icon/mode-purple.png \
    Icon/mode-white.png \
    Icon/mode-yellow.png \
    Icon/mode.png \
    Icon/new-blue.png \
    Icon/new-green.png \
    Icon/new-grey.png \
    Icon/new-pink.png \
    Icon/new-purple.png \
    Icon/new-white.png \
    Icon/new-yellow.png \
    Icon/new.png \
    Icon/previous-blue.png \
    Icon/previous-green.png \
    Icon/previous-grey.png \
    Icon/previous-pink.png \
    Icon/previous-purple.png \
    Icon/previous-white.png \
    Icon/previous-yellow.png \
    Icon/previous.png \
    Icon/resource-blue.png \
    Icon/resource-green.png \
    Icon/resource-grey.png \
    Icon/resource-pink.png \
    Icon/resource-purple.png \
    Icon/resource-white.png \
    Icon/resource-yellow.png \
    Icon/save-blue.png \
    Icon/save-green.png \
    Icon/save-grey.png \
    Icon/save-pink.png \
    Icon/save-purple.png \
    Icon/save-white.png \
    Icon/save-yellow.png \
    Icon/save.png \
    Icon/theme-blue.png \
    Icon/theme-green.png \
    Icon/theme-grey.png \
    Icon/theme-pink.png \
    Icon/theme-purple.png \
    Icon/theme-white.png \
    Icon/theme-yellow.png \
    Icon/turnback-blue.png \
    Icon/turnback-green.png \
    Icon/turnback-grey.png \
    Icon/turnback-pink.png \
    Icon/turnback-purple.png \
    Icon/turnback-white.png \
    Icon/turnback-yellow.png \
    Icon/turnback.png \
    Icon/user-blue.png \
    Icon/user-green.png \
    Icon/user-grey.png \
    Icon/user-pink.png \
    Icon/user-purple.png \
    Icon/user-white.png \
    Icon/user-yellow.png \
    Icon/user.png \
    Icon/wing.png \
    images/neural_network_1.png \
    images/neural_network_2.png \
    images/neural_network_3.png \
    images/neural_network_4.png \
    images/neural_network_5.png
