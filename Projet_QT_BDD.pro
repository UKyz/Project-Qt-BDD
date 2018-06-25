#-------------------------------------------------
#
# Project created by QtCreator 2017-12-11T15:50:15
#
#-------------------------------------------------

#QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TARGET = Projet_QT_BDD
#TEMPLATE = app
TRANSLATIONS = /Users/Victor/Projet_QT_BDD/test_en_EN.ts


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


QT       += sql # On rajoute SQL
QT       -= gui # On enlève la GUI
TARGET = test # La cible de compilation, le résultat
CONFIG   += console # On rajoute la console
TEMPLATE = app # On veut une application, oui
SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

macx {
    QMAKE_MAC_SDK = macosx10.13.2
}
