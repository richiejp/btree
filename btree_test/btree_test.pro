#-------------------------------------------------
#
# Project created by QtCreator 2015-02-01T21:31:46
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_btree
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_btree.cpp ../btree.cpp
HEADERS += ../btree.h
DEFINES += SRCDIR=\\\"$$PWD/\\\"
