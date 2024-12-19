QT       += core gui widgets network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BookstoreApp
TEMPLATE = app

# Исходные файлы
SOURCES += \
    client/main.cpp \
    client/client.cpp \
    server/server.cpp \
    server/bookstore.cpp\
    client/mainwindow.cpp\
    client/loginwindow.cpp

# Заголовочные файлы
HEADERS += \
    client/client.h \
    server/server.h \
    server/bookstore.h\
    client/mainwindow.h\
    client/loginwindow.h


# Файл интерфейса
FORMS += \
    client/ui_mainwindow.ui\
    client/ui_loginwindow.ui

# Подключение libpqxx для работы с PostgreSQL
INCLUDEPATH += /usr/include/libpqxx
LIBS += -lpqxx -lpq
