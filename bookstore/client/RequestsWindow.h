#ifndef REQUESTSWINDOW_H
#define REQUESTSWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>

class RequestsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RequestsWindow(QWidget *parent = nullptr);
    ~RequestsWindow();

private slots:
    void onConnected();
    void onDataReceived();
    void onSocketError(QAbstractSocket::SocketError socketError);
    void onApproveRequest();
    void onRejectRequest();
    void onDeleteRequest();
    void onRefreshRequest();  // Новый слот для обновления данных

private:
    void setupUI();
    void showRequests(const QJsonArray &requestsArray);

    QTcpSocket *socket;                // Сокет для подключения к серверу
    QTableWidget *requestsTable;        // Таблица для отображения заявок
    QPushButton *approveButton;         // Кнопка для одобрения заявки
    QPushButton *rejectButton;          // Кнопка для отклонения заявки
    QPushButton *deleteButton;          // Кнопка для удаления заявки
    QPushButton *refreshButton;         // Кнопка для обновления данных

};

#endif // REQUESTSWINDOW_H