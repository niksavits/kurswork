#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlQuery>
#include <QSqlError>
#include <QThread>

class Server : public QObject {
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void startServer();
signals:
    void clientAuthenticated(const QString &message);

public slots:
    void handleClient();
    void onAuthenticateUser(const QString &username, const QString &password, QTcpSocket *clientSocket);
    void handleSearchRequest(QTcpSocket *clientSocket, const QString &author, const QString &title, const QString &minPrice, const QString &maxPrice, const QString &section, const QString &publisher, const QString &maxPages, const QString &minPages, const QString& minYear, const QString& maxYear);    
    void handleGetAllBooksRequest(QTcpSocket *clientSocket);
    void handleRegisterRequest(const QString &username, const QString &password, QTcpSocket *clientSocket);
    void handleAddBookRequest(QTcpSocket *clientSocket, const QJsonObject &bookData);
    void handleEditBookRequest(QTcpSocket *clientSocket, const QJsonObject &bookData);
    void handleDeleteBookRequest(QTcpSocket *clientSocket, const QJsonObject &bookData);
    void approveRequestInDatabase(const QString &requestId);
    void rejectRequestInDatabase(const QString &requestId);
    void deleteRequestInDatabase(const QString &requestId);

private:
    QTcpServer *server;
    void sendAllRequests(QTcpSocket *clientSocket);  // Добавляем объявление метода
    QJsonArray fetchRequestsFromDatabase();  // Добавляем объявление метода
};

#endif // SERVER_H