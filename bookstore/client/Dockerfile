FROM ubuntu:22.04
# Установим временную зону, если она была указана в .env
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow
# Установка зависимостей
RUN apt-get update && apt-get install -y \
tzdata \
qt5-qmake \
qtbase5-dev \
qt5-qmake-bin \
libqt5core5a \
libqt5gui5 \
libqt5widgets5 \
build-essential \
libc6 \
libxcb-xinerama0 \
&& rm -rf /var/lib/apt/lists/*

# Устанавливаем рабочую директорию
WORKDIR /client

# Копируем все файлы проекта в контейнер
COPY . /client
# Установим нужные пакеты для работы с временной зоной
RUN apt-get update && apt-get install -y tzdata
ENV UID=100001
ENV GID=100001

# Создаем пользователя
RUN groupadd -g $GID tester && \
    useradd -m -u $UID -g $GID tester

# Устанавливаем владельца для файлов
RUN chown -R tester:tester /client

# Переключаемся на пользователя tester
USER tester

# Строим приложение
RUN qmake && make

# Экспонируем порты
EXPOSE 1234

# Запуск приложения
CMD ["./BookstoreClient"]
