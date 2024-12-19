QT       += core gui widgets network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BookstoreClient
TEMPLATE = app

# Исходные файлы
SOURCES += \
    main.cpp \
    client.cpp \
    mainwindow.cpp\
    loginwindow.cpp\
    RequestsWindow.cpp

# Заголовочные файлы
HEADERS += \
    client.h \
    mainwindow.h\
    loginwindow.h\
    RequestsWindow.h

# Файл интерфейса
FORMS += \
    ui_mainwindow.ui\
    ui_loginwindow.ui

INCLUDEPATH += /usr/include/libpqxx
LIBS += -lpqxx -lpq