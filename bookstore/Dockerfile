# Используем официальный образ на базе Alpine Linux с Qt5
FROM ubuntu:20.04
# Установим временную зону, если она была указана в .env
ENV TZ=Europe/Moscow
# Установка зависимостей
RUN apt-get update && apt-get install -y \
tzdata \
    && ln -fs /usr/share/zoneinfo/$TZ /etc/localtime \
    && dpkg-reconfigure -f noninteractive tzdata \
    && rm -rf /var/lib/apt/lists \
    build-essential \
    qt5-qmake qtbase5-dev qtchooser \
    libsqlite3-dev \
    libssl-dev \
    libpq-dev \
    curl \
    git \
    && rm -rf /var/lib/apt/lists/*

# Устанавливаем рабочую директорию
WORKDIR /app

# Копируем все файлы проекта в контейнер
COPY . /app
# Установим нужные пакеты для работы с временной зоной
RUN apt-get update && apt-get install -y tzdata
ENV UID=100001
ENV GID=100001

# Создаем пользователя
RUN groupadd -g $GID tester && \
    useradd -m -u $UID -g $GID tester

# Устанавливаем владельца для файлов
RUN chown -R tester:tester /app

# Переключаемся на пользователя tester
USER tester

# Строим приложение
RUN qmake server.pro && make

# Экспонируем порты
EXPOSE 1234

# Запуск приложения
CMD ["./BookstoreServer"]
