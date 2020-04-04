#-------------------------------------------------
#
# Project created by QtCreator 2019-02-15T13:35:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = everything
TEMPLATE = app
RC_ICONS = logo.ico

SOURCES += main.cpp\
        mainwindow.cpp\
		core/everything.cpp\
		core/files.cpp\
		core/mstring.cpp\
		core/regexcpp.cpp

HEADERS  += mainwindow.h\
		core/everything.h\
		core/mstring.h \
		core/regexcpp.h \
		core/files.h

FORMS    += mainwindow.ui
