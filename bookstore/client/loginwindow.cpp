#include "loginwindow.h"
#include "ui_ui_loginwindow.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QRegExpValidator>

void writeLog(const QString &message)
{
    QFile logFile("application_log.txt");
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&logFile);
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " - " << message << "\n";
        logFile.close();
    } else {
        qCritical() << "Не удалось открыть файл для записи логов!";
    }
}

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterButtonClicked);
    connect(socket, &QTcpSocket::errorOccurred, this, &LoginWindow::onSocketError);
    connect(socket, &QTcpSocket::readyRead, this, &LoginWindow::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &LoginWindow::onSocketDisconnected);  // Обработчик для отключения
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::onLoginButtonClicked()
{
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();
    if (username.length() > 15) {
        QMessageBox::warning(this, "Ошибка", "Логин не может быть длиннее 15 символов.");
        return;
    }

    QRegExp usernameRegex("^[a-zA-Zа-яА-ЯёЁ]+$");
    if (!usernameRegex.exactMatch(username)) {
        QMessageBox::warning(this, "Ошибка", "Логин может содержать только буквы.");
        return;
    }

    writeLog("Попытка подключения к серверу...");

    socket->connectToHost("192.168.0.13", 1234);

    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к серверу.");
        writeLog("Ошибка подключения к серверу: " + socket->errorString());
        return;
    }

    writeLog("Подключение успешно.");

    QString data;
    data.append("LOGIN " + username + " " + password);  // Используем \n для завершения строки
    socket->write(data.toUtf8());
    // Ждем, пока данные будут отправлены
    if (!socket->waitForBytesWritten(3000)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось отправить данные на сервер.");
        writeLog("Ошибка отправки данных: " + socket->errorString());
        return;
    }

    writeLog("Отправлен запрос на вход: " + data + socket->state());
}
void LoginWindow::onRegisterButtonClicked()
{
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля.");
        return;
    }

    if (username.length() > 15) {
        QMessageBox::warning(this, "Ошибка", "Логин не может быть длиннее 15 символов.");
        return;
    }

    QRegExp usernameRegex("^[a-zA-Zа-яА-ЯёЁ]+$");
    if (!usernameRegex.exactMatch(username)) {
        QMessageBox::warning(this, "Ошибка", "Логин может содержать только буквы.");
        return;
    }

    writeLog("Попытка регистрации нового пользователя...");

    socket->connectToHost("192.168.0.13", 1234);

    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к серверу.");
        writeLog("Ошибка подключения к серверу: " + socket->errorString());
        return;
    }

    writeLog("Подключение успешно.");

    // Отправка запроса на регистрацию
    QString data;
    data.append("REGISTER " + username + " " + password);  // Используем \n для завершения строки
    socket->write(data.toUtf8());
    
    if (!socket->waitForBytesWritten(3000)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось отправить данные на сервер.");
        writeLog("Ошибка отправки данных: " + socket->errorString());
        return;
    }

    writeLog("Отправлен запрос на регистрацию: " + data + socket->state());
}
void LoginWindow::openMainWindow()
{
    MainWindow *mainWindow = new MainWindow();
    mainWindow->setRole(userRole);  // Передаем роль в главное окно
    mainWindow->show();
}
void LoginWindow::setRole(const QString& role) {
    userRole = role;
}

void LoginWindow::onReadyRead()
{
    QByteArray response = socket->readAll();
    writeLog("Ответ от сервера: " + QString(response));

    if (response.startsWith("LOGIN success")) {
        // Извлекаем роль из ответа
        if (response.contains("admin")) {
            userRole = "admin";  // Устанавливаем роль администратора
        } else {
            userRole = "user";   // Роль обычного пользователя
        }

        writeLog("Авторизация успешна. Роль: " + userRole);
        
        openMainWindow();  // Открываем главное окно, передаем роль
        this->close();     // Закрываем окно авторизации
    } 
    else if (response == "LOGIN failure") {
        writeLog("Ошибка входа: неверное имя пользователя или пароль.");
        QMessageBox::warning(this, "Ошибка входа", "Неверное имя пользователя или пароль.");
    } 
    else if (response == "REGISTER success") {
        writeLog("Регистрация успешна.");
        QMessageBox::information(this, "Успех", "Вы успешно зарегистрированы.");
    } 
    else if (response == "REGISTER failure") {
        writeLog("Ошибка регистрации.");
        QMessageBox::warning(this, "Ошибка регистрации", "Не удалось зарегистрировать пользователя.");
    }
    else {
        response == "User not found.";
        writeLog("Пользователя не существует или введён неверный пароль!");
        QMessageBox::warning(this, "Ошибка", "Пользователя не существует или введён неверный пароль!");
    }

    socket->disconnectFromHost();
}

void LoginWindow::onSocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);

    QString errorMessage = "Произошла ошибка при подключении к серверу: " + socket->errorString();
    writeLog(errorMessage);

    QMessageBox::critical(this, "Ошибка", errorMessage);
    socket->disconnectFromHost();
}

void LoginWindow::onSocketDisconnected()
{
    qDebug() << "Отключено от сервера.";  // Можно здесь добавить дополнительные действия, если необходимо
}