#include "client.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>

Client::Client(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)) {}

void Client::connectToServer()
{
    socket->connectToHost("192.168.0.13", 1234);

    if (socket->waitForConnected()) {
        qDebug() << "Connected to server!";
    } else {
        qDebug() << "Connection failed!";
    }
}

void Client::getBooks()
{
    socket->write("GET books");
    socket->waitForBytesWritten();

    connect(socket, &QTcpSocket::readyRead, this, &Client::readData);
}

void Client::readData()
{
    QByteArray data = socket->readAll();
    QStringList books = QString(data).split("\n");

    emit booksReceived(books);
}
