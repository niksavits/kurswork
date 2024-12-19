#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QStringList>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);

    // Функции для подключения к серверу и получения данных
    void connectToServer();
    void getBooks();

signals:
    void booksReceived(const QStringList &books);  // Сигнал для передачи полученных книг

private slots:
    void readData();  // Слот для обработки данных, полученных от сервера

private:
    QTcpSocket *socket;  // Сокет для подключения к серверу
};

#endif // CLIENT_H