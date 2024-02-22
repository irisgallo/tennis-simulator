QT += core widgets opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
win32 {
    LIBS += -lopengl32
    LIBS += -lglu32
}
CONFIG += warn_on
CONFIG += debug

INCLUDEPATH += include

include(src/src.pri)

FORMS += \
    forms/mainwindow.ui \
    forms/physicscontrols.ui

RESOURCES += glsl.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS +=

SOURCES +=
