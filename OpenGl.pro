TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/main.cpp \
    src/SceneOpenGL.cpp \
    src/Shader.cpp \
    src/polygon.cpp

HEADERS += \
    include/SceneOpenGL.h \
    include/Shader.h \
    include/polygon.h

LIBS += -lSDL2 -lSDL2_image -lGL

INCLUDEPATH += ./include
