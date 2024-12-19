-- Создание базы данных (если она еще не существует)
CREATE DATABASE bookstore;

-- Использование базы данных
\c bookstore;

-- Создание таблицы пользователей
CREATE TABLE users (
    username VARCHAR(255) PRIMARY KEY,
    password VARCHAR(255) NOT NULL
);

-- Вставка тестовых данных (логин/пароль)
INSERT INTO users (username, password) VALUES ('postgres', 'password');
INSERT INTO users (username, password) VALUES ('user1', 'mypassword');