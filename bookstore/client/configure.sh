#!/bin/bash

# Проверяем наличие qmake
command -v qmake >/dev/null 2>&1 || { echo "Ошибка: qmake не установлен. Установите Qt."; exit 1; }

# Проверяем наличие make
command -v make >/dev/null 2>&1 || { echo "Ошибка: make не установлен. Установите make."; exit 1; }

# Переменные для путей
INSTALL_PREFIX="/home/skywalker/Рабочий стол"  # Путь установки программы
BUILD_DIR="build"           # Папка для сборки

# Создаем директорию для сборки, если она не существует
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

# Переходим в директорию сборки
cd "$BUILD_DIR"

# Запускаем qmake с указанием пути к исходникам
echo "Запуск qmake..."
qmake /home/skywalker/bookstore/client/client.pro

# Если qmake не выполнится, выводим ошибку
if [ $? -ne 0 ]; then
    echo "Ошибка при выполнении qmake!"
    exit 1
fi

# Запускаем make для компиляции
echo "Запуск make для сборки..."
make

# Если make не выполнится, выводим ошибку
if [ $? -ne 0 ]; then
    echo "Ошибка при сборке программы с помощью make!"
    exit 1
fi

# Установка программы
echo "Установка программы в $INSTALL_PREFIX..."
sudo make install

# Завершаем скрипт
echo "Сборка и установка завершены успешно."
