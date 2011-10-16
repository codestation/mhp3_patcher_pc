# -------------------------------------------------
# Project created by QtCreator 2010-12-30T12:49:15
# -------------------------------------------------
QT += core \
    gui
TARGET = mhp3_patcher_pc
TEMPLATE = app
SOURCES += main.cpp \
    patcher.cpp \
    about.cpp
HEADERS += patcher.h \
    about.h
FORMS += patcher.ui \
    about.ui
RESOURCES += mh_res.qrc
win32:RC_FILE = windows_icon.rc
