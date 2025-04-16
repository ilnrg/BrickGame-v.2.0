QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

LIBS += -lncurses

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ../../brick_game/common/common_back.c \
    ../../brick_game/tetris/tetris_backend.c \
    ../../brick_game/snake/snake_controller.cc \
    ../../brick_game/snake/snake_model.cc \
    game_widget.cpp \
    snake_widget.cpp \
    tetris_widget.cpp

HEADERS += \
    mainwindow.h \
    ../../brick_game/common/common_back.h \
    ../../brick_game/common/common_specification.h \
    ../../brick_game/tetris/tetris_backend.h \
    ../../brick_game/snake/snake_controller.h \
    ../../brick_game/snake/snake_model.h \
    game_widget.h \
    snake_widget.h \
    tetris_widget.h

FORMS += \
    mainwindow.ui \
    snake_widget.ui \
    tetris_widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
