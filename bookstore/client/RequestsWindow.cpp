#include "RequestsWindow.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QMessageBox>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

RequestsWindow::RequestsWindow(QWidget *parent)
    : QWidget(parent), socket(new QTcpSocket(this)) {
    setupUI();

    // Подключаем сокет
    connect(socket, &QTcpSocket::connected, this, &RequestsWindow::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &RequestsWindow::onDataReceived);
    connect(socket, &QTcpSocket::errorOccurred, this, &RequestsWindow::onSocketError);
}

RequestsWindow::~RequestsWindow() {
    socket->disconnectFromHost();
}

void RequestsWindow::setupUI()
{
    setWindowTitle("Заявки");

    QVBoxLayout *layout = new QVBoxLayout(this);

    requestsTable = new QTableWidget(this);
    requestsTable->setColumnCount(16);  // 13 столбцов для всех данных
    requestsTable->setHorizontalHeaderLabels({
        "ID заявки", "ID книги", "Название", "Автор",
        "Категория", "Издатель", "Город издателя", "Год публикации",
        "Количество страниц", "Цена", "Количество иллюстраций", "Статус",
        "Магазин", "Адрес магазина", "График работы", "Тип заявки"
    });
    requestsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Кнопки для одобрения, отклонения, удаления заявок и обновления данных
    approveButton = new QPushButton("Одобрить заявку", this);
    rejectButton = new QPushButton("Отклонить заявку", this);
    deleteButton = new QPushButton("Удалить заявку", this);
    refreshButton = new QPushButton("Обновить данные", this);  // Новая кнопка

    // Добавляем виджеты в layout
    layout->addWidget(new QLabel("Список заявок:"));
    layout->addWidget(requestsTable);
    layout->addWidget(approveButton);
    layout->addWidget(rejectButton);
    layout->addWidget(deleteButton);
    layout->addWidget(refreshButton);  // Добавляем кнопку обновления

    // Подключаем действия к кнопкам
    connect(approveButton, &QPushButton::clicked, this, &RequestsWindow::onApproveRequest);
    connect(rejectButton, &QPushButton::clicked, this, &RequestsWindow::onRejectRequest);
    connect(deleteButton, &QPushButton::clicked, this, &RequestsWindow::onDeleteRequest);
    connect(refreshButton, &QPushButton::clicked, this, &RequestsWindow::onRefreshRequest);  // Подключаем слот для обновления
}

void RequestsWindow::onConnected() {
    qDebug() << "Connected to server.";
    if (socket->isOpen()) {
    } else {
        qWarning() << "Socket is not open!";
    }
}

void RequestsWindow::onDataReceived() {
    QByteArray response = socket->readAll();

    // Парсим данные в формате JSON
    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (doc.isArray()) {
        QJsonArray requestsArray = doc.array();
        // Заполняем таблицу
        showRequests(requestsArray);
    } else {
        //QMessageBox::warning(this, "Ошибка", "Не удалось получить данные заявок.");
    }
}

void RequestsWindow::onSocketError(QAbstractSocket::SocketError socketError) {
    qCritical() << "Socket error: " << socketError;
    //QMessageBox::warning(this, "Ошибка подключения", "Не удалось подключиться к серверу.");
}

void RequestsWindow::onRefreshRequest() {
    socket->connectToHost("192.168.0.13", 1234);
    socket->write("GET_ALL_REQUESTS");
}

void RequestsWindow::onApproveRequest()
{
    int row = requestsTable->currentRow();
    if (row >= 0) {
        QString requestId = requestsTable->item(row, 0)->text();
        // Отправляем запрос на сервер для одобрения заявки
        QString approveRequestCommand = "APPROVE_REQUEST " + requestId;
        socket->write(approveRequestCommand.toUtf8());

        // Ожидаем ответа от сервера
        connect(socket, &QTcpSocket::readyRead, [this, requestId]() {
            QByteArray response = socket->readAll();
            if (response == "APPROVED") {
                QMessageBox::information(this, "Заявка одобрена", "Заявка с ID " + requestId + " одобрена.");
                onRefreshRequest();  // Обновляем таблицу
            } else {
               // QMessageBox::warning(this, "Ошибка", "Не удалось одобрить заявку с ID " + requestId);
            }
        });
    } else {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, выберите заявку для одобрения.");
    }
}

void RequestsWindow::onRejectRequest()
{
    int row = requestsTable->currentRow();
    if (row >= 0) {
        QString requestId = requestsTable->item(row, 0)->text();
        // Отправляем запрос на сервер для отклонения заявки
        QString rejectRequestCommand = "REJECT_REQUEST " + requestId;
        socket->write(rejectRequestCommand.toUtf8());

        // Ожидаем ответа от сервера
        connect(socket, &QTcpSocket::readyRead, [this, requestId]() {
            QByteArray response = socket->readAll();
            if (response == "REJECTED") {
                QMessageBox::information(this, "Заявка отклонена", "Заявка с ID " + requestId + " отклонена.");
                onRefreshRequest();  // Обновляем таблицу
            } else {
                //QMessageBox::warning(this, "Ошибка", "Не удалось отклонить заявку с ID " + requestId);
            }
        });
    } else {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, выберите заявку для отклонения.");
    }
}

void RequestsWindow::onDeleteRequest()
{
    int row = requestsTable->currentRow();
    if (row >= 0) {
        QString requestId = requestsTable->item(row, 0)->text();
        // Отправляем запрос на сервер для удаления заявки
        QString deleteRequestCommand = "DELETE_REQUEST " + requestId;
        socket->write(deleteRequestCommand.toUtf8());

        // Ожидаем ответа от сервера
        connect(socket, &QTcpSocket::readyRead, [this, requestId]() {
            QByteArray response = socket->readAll();
            if (response == "DELETED") {
                QMessageBox::information(this, "Заявка удалена", "Заявка с ID " + requestId + " удалена.");
                onRefreshRequest();  // Обновляем таблицу
            } else {
                //QMessageBox::warning(this, "Ошибка", "Не удалось удалить заявку с ID " + requestId);
            }
        });
    } else {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, выберите заявку для удаления.");
    }
}

void RequestsWindow::showRequests(const QJsonArray &requestsArray) {
    requestsTable->setRowCount(requestsArray.size());

    // Проходим по всем заявкам и заполняем таблицу
    for (int i = 0; i < requestsArray.size(); ++i) {
        QJsonObject request = requestsArray[i].toObject();

        requestsTable->setItem(i, 0, new QTableWidgetItem(request["request_id"].toString()));       // ID заявки
        requestsTable->setItem(i, 1, new QTableWidgetItem(request["book_id"].toString()));          // ID книги
        requestsTable->setItem(i, 2, new QTableWidgetItem(request["title"].toString()));            // Название книги
        requestsTable->setItem(i, 3, new QTableWidgetItem(request["author"].toString()));           // Автор
        requestsTable->setItem(i, 4, new QTableWidgetItem(request["category"].toString()));         // Категория
        requestsTable->setItem(i, 5, new QTableWidgetItem(request["publisher"].toString()));        // Издатель
        requestsTable->setItem(i, 6, new QTableWidgetItem(request["publisher_city"].toString()));   // Город издателя
        requestsTable->setItem(i, 7, new QTableWidgetItem(request["publication_year"].toString())); // Год публикации
        requestsTable->setItem(i, 8, new QTableWidgetItem(request["pages_count"].toString()));      // Количество страниц
        requestsTable->setItem(i, 9, new QTableWidgetItem(request["price"].toString()));           // Цена
        requestsTable->setItem(i, 10, new QTableWidgetItem(request["illustrations_count"].toString())); // Количество иллюстраций
        requestsTable->setItem(i, 11, new QTableWidgetItem(request["status"].toString()));         // Статус
        requestsTable->setItem(i, 12, new QTableWidgetItem(request["store_name"].toString()));        // Магазин
        requestsTable->setItem(i, 13, new QTableWidgetItem(request["store_location"].toString()));    // Адрес магазина
        requestsTable->setItem(i, 14, new QTableWidgetItem(request["working_hours"].toString()));     // График работы
        requestsTable->setItem(i, 15, new QTableWidgetItem(request["request_type"].toString()));
    }
}