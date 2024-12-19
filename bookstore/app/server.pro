# server.pro
QT += core gui widgets network sql # Зависимости от Qt (для работы с сокетами и базой данных)

TARGET = BookstoreServer  # Название для серверной части
TEMPLATE = app            # Приложение (не библиотека)

# Источники и заголовочные файлы для серверной части
SOURCES += \
    server.cpp \

HEADERS += \
    server.h \

# Добавляем зависимости для PostgreSQL
INCLUDEPATH += /usr/include/libpqxx  # Путь к библиотеке libpqxx для PostgreSQL
LIBS += -lpqxx -lpq  # Либы для работы с PostgreSQL

# Указываем дополнительные библиотеки для подключения к базе данных PostgreSQL