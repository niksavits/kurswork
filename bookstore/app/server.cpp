#include "server.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>  // Для ошибок базы данных
#include <QMessageBox>
#include <QDebug>
#include <QCryptographicHash>
#include <QApplication>
#include <QThread>  // Не забывайте подключить QThread
#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Server::Server(QObject *parent)
    : QObject(parent), server(new QTcpServer(this)) {
    connect(server, &QTcpServer::newConnection, this, &Server::handleClient);
}

void Server::startServer() {
    if (server->listen(QHostAddress::Any, 1234)) {
        qDebug() << "Server is listening on port 1234...";
    } else {
        qCritical() << "Server failed to start!";
    }
}

void Server::handleClient() {
    QTcpSocket *clientSocket = server->nextPendingConnection();

    // Создаем поток для клиента
    QThread *clientThread = new QThread();
    clientSocket->moveToThread(clientThread); // Перемещаем сокет в новый поток

    // Подключаем обработку данных клиента
    connect(clientSocket, &QTcpSocket::readyRead, [this, clientSocket]() {
        QByteArray request = clientSocket->readAll();
        QString requestString = QString::fromUtf8(request);
        qDebug() << "Receive: " << requestString;

        if (requestString.startsWith("GET_ALL_BOOKS")) {
        handleGetAllBooksRequest(clientSocket);
        }
        else if (requestString.startsWith("LOGIN")) {
            QStringList credentials = requestString.split(" ");
            if (credentials.size() == 3) {
                QString username = credentials[1];
                QString password = credentials[2];
                emit onAuthenticateUser(username, password, clientSocket); // Вызов аутентификации
            } else {
                QString response = "LOGIN failure\n";
                qDebug() << "Sent to client: " << response;
                clientSocket->write(response.toUtf8());
            }
        } else if (requestString.startsWith("REGISTER")) {
            QStringList credentials = requestString.split(" ");
            if (credentials.size() == 3) {
                QString username = credentials[1];
                QString password = credentials[2];
                handleRegisterRequest(username, password, clientSocket);  // Обрабатываем запрос на регистрацию
            } else {
                QString response = "REGISTER failure\n";
                qDebug() << "Sent to client: " << response;
                clientSocket->write(response.toUtf8());
            }
            } else if (requestString.startsWith("ADD_BOOK")) {
            QByteArray jsonData = request.mid(8).trimmed(); // Обрезаем "ADD_BOOK "
            qDebug() << "Данные книги после обрезки префикса:" << jsonData;
            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            if (doc.isObject()) {
                handleAddBookRequest(clientSocket, doc.object());
            } else {
                clientSocket->write("ADD_BOOK failure: Invalid data\n");
            }
        } else if (requestString.startsWith("EDIT_BOOK")) {
            QByteArray jsonData = request.mid(9); // Обрезаем "EDIT_BOOK "
            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            if (doc.isObject()) {
                handleEditBookRequest(clientSocket, doc.object());
            } else {
                clientSocket->write("EDIT_BOOK failure: Invalid data\n");
            }
        } else if (request.startsWith("APPROVE_REQUEST")) {
        QString requestId = request.split(' ')[1];
        approveRequestInDatabase(requestId);
        clientSocket->write("APPROVED");
        } else if (request.startsWith("REJECT_REQUEST")) {
        QString requestId = request.split(' ')[1];
        rejectRequestInDatabase(requestId);
        clientSocket->write("REJECTED");
        } else if (request.startsWith("DELETE_REQUEST")) {
        QString requestId = request.split(' ')[1];
        deleteRequestInDatabase(requestId);
        clientSocket->write("DELETED");
        } else if (request == "GET_ALL_REQUESTS") {
        sendAllRequests(clientSocket);
        } else if (requestString.startsWith("DELETE_BOOK")) {
        QByteArray jsonData = request.mid(12).trimmed();
            qDebug() << "Данные книги после обрезки префикса:" << jsonData;
            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            if (doc.isObject()) {
                handleDeleteBookRequest(clientSocket, doc.object());
            } else {
                clientSocket->write("ADD_BOOK failure: Invalid data\n");
            }
        } else if (requestString.startsWith("SEARCH")) {
            QStringList searchParams = requestString.split(" ");
            QString author;
                QString title;
                QString minPrice;             // Минимальная цена (по умолчанию пусто)
                QString maxPrice;             // Максимальная цена (по умолчанию пусто)
                QString section;              // Категория (по умолчанию пусто)
                QString publisher;
                QString minPages;
                QString maxPages;
                QString minYear;
                QString maxYear;
            if (searchParams.size() >= 3) {
                 author = searchParams[1];
                 title = searchParams[2];
                 minPrice = "";             // Минимальная цена (по умолчанию пусто)
                 maxPrice = "";             // Максимальная цена (по умолчанию пусто)
                 section = "";              // Категория (по умолчанию пусто)
                 publisher = "";            // Издатель (по умолчанию пусто)
                 minPages = "";
                 maxPages = "";
                 minYear = "";
                 maxYear = "";
            }

            // Проверяем наличие дополнительных фильтров
            if (searchParams.size() > 3) {
                int index = 3;
                while (index < searchParams.size()) {
                QString param = searchParams[index];
                
                if (param.startsWith("MIN_PRICE:")) {
                    minPrice = param.mid(10);  // Отделяем значение после MIN_PRICE:
                } else if (param.startsWith("MAX_PRICE:")) {
                    maxPrice = param.mid(10);  // Отделяем значение после MAX_PRICE:
                } else if (param.startsWith("SECTION:")) {
                    section = param.mid(8);    // Отделяем значение после SECTION:
                } else if (param.startsWith("PUBLISHER:")) {
                    publisher = param.mid(10); // Отделяем значение после PUBLISHER:
                } else if (param.startsWith("MIN_PAGE:")) {
                    minPages = param.mid(9);
                } else if (param.startsWith("MAX_PAGE:")) {
                    maxPages = param.mid(9);
                } else if (param.startsWith("YEAR_TO:")) {
                    maxYear = param.mid(8);  // Максимальный год
                } else if (param.startsWith("YEAR_FROM:")) {
                    minYear = param.mid(10);  // Минимальный год
                } 
                index++;
            }
            }
            handleSearchRequest(clientSocket, author, title, minPrice, maxPrice, section, publisher, minPages, maxPages, minYear, maxYear);
            } else {
            QString response = "Unknown command\n";
            clientSocket->write(response.toUtf8());
        }
    });

    connect(clientSocket, &QTcpSocket::disconnected, clientThread, &QThread::quit);
    connect(clientThread, &QThread::finished, clientSocket, &QObject::deleteLater);
    connect(clientThread, &QThread::finished, clientThread, &QObject::deleteLater);

    clientThread->start(); // Запускаем поток
} 

void Server::handleRegisterRequest(const QString &username, const QString &password, QTcpSocket *clientSocket) {
    QSqlQuery query;
    query.prepare("SELECT username FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qCritical() << "Database query failed:" << query.lastError();
        return;
    }

    QString response;
    if (query.next()) {
        response = "REGISTER failure";  // Если пользователь уже существует
    } else {
        QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
        QString hashStr = QString::fromUtf8(hashedPassword.toHex());
        query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
        query.bindValue(":username", username);
        query.bindValue(":password", hashStr);
        qDebug() << hashStr;
        qDebug() << query.lastQuery();

        if (!query.exec()) {
            response = "REGISTER failure";
            qCritical() << "Failed to insert user:" << query.lastError();
        } else {
            response = "REGISTER success";
        }
    }

    qDebug() << "Sent to client: " << response;
    clientSocket->write(response.toUtf8());
}
void Server::handleAddBookRequest(QTcpSocket *clientSocket, const QJsonObject &bookData) {
    QString book_id = bookData["book_id"].toString();
    QString title = bookData["title"].toString();
    QString author = bookData["author"].toString();
    QString category = bookData["category"].toString();
    QString publisher = bookData["publisher"].toString();
    QString publisherCity = bookData["publisher_city"].toString();
    QString publicationYear = bookData["publication_year"].toString();
    QString pagesCount = bookData["pages_count"].toString();
    QString price = bookData["price"].toString();
    QString illustrationsCount = bookData["illustrations_count"].toString();
    QString requestType = bookData["request_type"].toString(); // Тип заявки (например, "Добавить")
    QDateTime createdAt = QDateTime::currentDateTime(); // Время создания заявки
    QString storeName = bookData["store_name"].toString();         // Название магазина
    QString storeAddress = bookData["store_location"].toString();   // Адрес магазина
    QString storeSchedule = bookData["working_hours"].toString();

    QSqlQuery query;
    query.prepare("INSERT INTO requests (book_id, request_type, title, author, category, publisher, publisher_city, publication_year, pages_count, price, illustrations_count, status, created_at, store_name, store_location, working_hours) "
                  "VALUES (:book_id, :request_type, :title, :author, :category, :publisher, :publisher_city, :publication_year, :pages_count, :price, :illustrations_count, :status, :created_at, :store_name, :store_location, :working_hours)");

    query.bindValue(":book_id", book_id);
    query.bindValue(":title", title);
    query.bindValue(":author", author);
    query.bindValue(":category", category);
    query.bindValue(":publisher", publisher);
    query.bindValue(":publisher_city", publisherCity);
    query.bindValue(":publication_year", publicationYear);
    query.bindValue(":pages_count", pagesCount);
    query.bindValue(":price", price);
    query.bindValue(":illustrations_count", illustrationsCount);
    query.bindValue(":status", "Ожидает"); // Статус заявки (по умолчанию "Ожидает")
    query.bindValue(":request_type", requestType); // Тип заявки (например, "Добавить")
    query.bindValue(":created_at", createdAt.toString("yyyy-MM-dd HH:mm:ss")); // Время создания
    query.bindValue(":store_name", storeName);   // Название магазина
    query.bindValue(":store_location", storeAddress); // Адрес магазина
    query.bindValue(":working_hours", storeSchedule);

    if (!query.exec()) {
        qCritical() << "Failed to add book request:" << query.lastError();
        clientSocket->write("ADD_BOOK failure\n");
        return;
    }

    clientSocket->write("ADD_BOOK success\n");
}
void Server::sendAllRequests(QTcpSocket *clientSocket)
{
    QJsonArray requestsArray = fetchRequestsFromDatabase();  // Получаем данные из базы данных

    if (requestsArray.isEmpty()) {
        qWarning() << "No requests found in the database.";
        return;
    }

    QJsonDocument doc(requestsArray);
    QByteArray responseData = doc.toJson();

    // Отправляем данные клиенту
    clientSocket->write(responseData);
    clientSocket->flush();
    qDebug() << QString(responseData);  // Преобразуем байты в строку для отображения
    qDebug() << "Sent all requests to client.";
}
QJsonArray Server::fetchRequestsFromDatabase()
{
    QJsonArray requestsArray;
    QSqlQuery query;

    query.prepare("SELECT * FROM requests");
    qDebug() << query.lastQuery();
    if (!query.exec()) {
        qCritical() << "SQL query failed: " << query.lastError().text();
        return requestsArray;
    }

    while (query.next()) {
        QJsonObject request;
        request["request_id"] = query.value("request_id").toString();
        request["book_id"] = query.value("book_id").toString();
        request["title"] = query.value("title").toString();
        request["author"] = query.value("author").toString();
        request["category"] = query.value("category").toString();
        request["publisher"] = query.value("publisher").toString();
        request["publisher_city"] = query.value("publisher_city").toString();
        request["publication_year"] = query.value("publication_year").toString();
        request["pages_count"] = query.value("pages_count").toString();
        request["price"] = query.value("price").toString();
        request["illustrations_count"] = query.value("illustrations_count").toString();
        request["status"] = query.value("status").toString();
        request["store_name"] = query.value("store_name").toString();          // Имя магазина
        request["store_location"] = query.value("store_location").toString();  // Адрес магазина
        request["working_hours"] = query.value("working_hours").toString();    // График работы магазина
        request["request_type"] = query.value("request_type").toString();

        requestsArray.append(request);
    }

    return requestsArray;
}
void Server::handleEditBookRequest(QTcpSocket *clientSocket, const QJsonObject &bookData) {
    QString book_id = bookData["book_id"].toString();
    QString title = bookData["title"].toString();
    QString author = bookData["author"].toString();
    QString category = bookData["category"].toString();
    QString publisher = bookData["publisher"].toString();
    QString publisherCity = bookData["publisher_city"].toString();
    QString publicationYear = bookData["publication_year"].toString();
    QString pagesCount = bookData["pages_count"].toString();
    QString price = bookData["price"].toString();
    QString illustrationsCount = bookData["illustrations_count"].toString();
    QString requestType = bookData["request_type"].toString(); // Тип заявки (например, "Добавить")
    QDateTime createdAt = QDateTime::currentDateTime(); // Время создания заявки

    QSqlQuery query;
    query.prepare("INSERT INTO requests (book_id, request_type, title, author, category, publisher, publisher_city, publication_year, pages_count, price, illustrations_count, status, created_at) "
                  "VALUES (:book_id, :request_type, :title, :author, :category, :publisher, :publisher_city, :publication_year, :pages_count, :price, :illustrations_count, :status, :created_at)");

    query.bindValue(":book_id", book_id);
    query.bindValue(":title", title);
    query.bindValue(":author", author);
    query.bindValue(":category", category);
    query.bindValue(":publisher", publisher);
    query.bindValue(":publisher_city", publisherCity);
    query.bindValue(":publication_year", publicationYear);
    query.bindValue(":pages_count", pagesCount);
    query.bindValue(":price", price);
    query.bindValue(":illustrations_count", illustrationsCount);
    query.bindValue(":status", "Ожидает"); // Статус заявки (по умолчанию "Ожидает")
    query.bindValue(":request_type", requestType); // Тип заявки (например, "Добавить")
    query.bindValue(":created_at", createdAt.toString("yyyy-MM-dd HH:mm:ss")); // Время создания

    if (!query.exec()) {
        qCritical() << "Failed to update book:" << query.lastError();
        clientSocket->write("EDIT_BOOK failure\n");
        return;
    }

    clientSocket->write("EDIT_BOOK success\n");
}
void Server::handleDeleteBookRequest(QTcpSocket *clientSocket, const QJsonObject &bookData) {
    QString book_id = bookData["book_id"].toString();
    QString title = bookData["title"].toString();
    QString author = bookData["author"].toString();
    QString category = bookData["category"].toString();
    QString publisher = bookData["publisher"].toString();
    QString publisherCity = bookData["publisher_city"].toString();
    QString publicationYear = bookData["publication_year"].toString();
    QString pagesCount = bookData["pages_count"].toString();
    QString price = bookData["price"].toString();
    QString illustrationsCount = bookData["illustrations_count"].toString();
    QString requestType = bookData["request_type"].toString(); // Тип заявки (например, "Добавить")
    QDateTime createdAt = QDateTime::currentDateTime(); // Время создания заявки

    QSqlQuery query;
    query.prepare("INSERT INTO requests (book_id, request_type, title, author, category, publisher, publisher_city, publication_year, pages_count, price, illustrations_count, status, created_at) "
                  "VALUES (:book_id, :request_type, :title, :author, :category, :publisher, :publisher_city, :publication_year, :pages_count, :price, :illustrations_count, :status, :created_at)");

    query.bindValue(":book_id", book_id);
    query.bindValue(":title", title);
    query.bindValue(":author", author);
    query.bindValue(":category", category);
    query.bindValue(":publisher", publisher);
    query.bindValue(":publisher_city", publisherCity);
    query.bindValue(":publication_year", publicationYear);
    query.bindValue(":pages_count", pagesCount);
    query.bindValue(":price", price);
    query.bindValue(":illustrations_count", illustrationsCount);
    query.bindValue(":status", "Ожидает"); // Статус заявки (по умолчанию "Ожидает")
    query.bindValue(":request_type", requestType); // Тип заявки (например, "Добавить")
    query.bindValue(":created_at", createdAt.toString("yyyy-MM-dd HH:mm:ss")); // Время создания

    if (!query.exec()) {
        qCritical() << "Failed to delete book:" << query.lastError();
        clientSocket->write("DELETE_BOOK failure\n");
        return;
    }

    clientSocket->write("DELETE_BOOK success\n");
}
void Server::onAuthenticateUser(const QString &username, const QString &password, QTcpSocket *clientSocket) {
    QSqlQuery query;
    query.prepare("SELECT password, is_admin FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qCritical() << "Database query failed:" << query.lastError();
        return;
    }

    QString response;
    if (query.next()) {
        QByteArray savedHash = query.value(0).toByteArray();
        qDebug() << savedHash << endl;
        QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
        qDebug() << hashedPassword.toHex() << endl;
        QString role = query.value(1).toString();
        if (savedHash == hashedPassword.toHex()) {
            response = (role == "true") ? "LOGIN success: admin" : "LOGIN success: user";
    } else {
        response = "Incorrect password";
    }
    } else {
        response = "User not found.";
    }

    qDebug() << "Sent to client: " << response;
    clientSocket->write(response.toUtf8());
}

void Server::handleSearchRequest(QTcpSocket *clientSocket, const QString &author, const QString &title,
                                  const QString &minPrice, const QString &maxPrice,
                                  const QString &section, const QString &publisher, const QString &minPages, const QString &maxPages, const QString& minYear, const QString& maxYear) {
    qDebug() << "Searching with parameters - Author:" << "%" + author + "%" << ", Title:" << "%" + title + "%" << ", price:" << minPrice.toDouble() << ", maxprice:" << maxPrice.toDouble() << "minPages: " << minPages.toInt() << " maxPages: " << maxPages.toInt() << "minYear: " << minYear.toInt() << " maxYear: " << maxYear.toInt();
    QString queryStr = "SELECT books.book_id, books.title, books.author, books.category, books.publisher, books.publisher_city, books.publication_year, books.pages_count, books.price, books.illustrations_count, stores.store_id, stores.store_name AS store_name, stores.store_location AS store_location, stores.working_hours AS working_hours FROM books LEFT JOIN stores ON books.store_id = stores.store_id WHERE 1=1";

    QSqlQuery query;
    if (!author.isEmpty()) {
        queryStr += " AND author ILIKE :author";
    }
    if (!title.isEmpty()) {
        queryStr += " AND title ILIKE :title";
    }
    if (!minPrice.isEmpty()) {
        queryStr += " AND price >= :minPrice";
    }
    if (!maxPrice.isEmpty()) {
        queryStr += " AND price <= :maxPrice";
    }
    if (!section.isEmpty()) {
        queryStr += " AND category ILIKE :section";
    }
    if (!publisher.isEmpty()) {
        queryStr += " AND publisher ILIKE :publisher";
    }
    if (!minYear.isEmpty()) {
        queryStr += " AND publication_year >= :minYear";
    }
    if (!maxYear.isEmpty()) {
        queryStr += " AND publication_year <= :maxYear";
    }
    if (!minPages.isEmpty()) {
        queryStr += " AND pages_count >= :minPages";
    }
    if (!maxPages.isEmpty()) {
        queryStr += " AND pages_count <= :maxPages";
    }

    query.prepare(queryStr);

    // Привязываем параметры
    if (!author.isEmpty()) query.bindValue(":author", "%" + author + "%");
    if (!title.isEmpty()) query.bindValue(":title", "%" + title + "%");
    if (!minPrice.isEmpty()) query.bindValue(":minPrice", minPrice.toDouble());
    if (!maxPrice.isEmpty()) query.bindValue(":maxPrice", maxPrice.toDouble());
    if (!section.isEmpty()) query.bindValue(":section", "%" + section + "%");
    if (!publisher.isEmpty()) query.bindValue(":publisher", "%" + publisher + "%");
    if (!minYear.isEmpty()) query.bindValue(":minYear", minYear.toInt());
    if (!maxYear.isEmpty()) query.bindValue(":maxYear", maxYear.toInt());
    if (!minPages.isEmpty()) query.bindValue(":minPages", minPages.toInt());
    if (!maxPages.isEmpty()) query.bindValue(":maxPages", maxPages.toInt());

    qDebug() << "Executing query:" << query.lastQuery();  // Логируем запрос

    if (!query.exec()) {
        qCritical() << "Database query failed:" << query.lastError();
        return;
    }

    QJsonArray booksArray;
    // Проверка, возвращаются ли данные
    if (!query.first()) {
        qDebug() << "No records found in the database.";
    }
    if (query.first()) {
        do {
        QString id = query.value(0).toString();
        QString bookTitle = query.value(1).toString();
        QString bookAuthor = query.value(2).toString();
        QString category = query.value(3).toString();
        QString publicate = query.value(4).toString();
        QString publicate_city = query.value(5).toString();
        QString publication_year = query.value(6).toString();
        QString page_count = query.value(7).toString();
        QString price = query.value(8).toString();
        QString illustration = query.value(9).toString();
        QString storeId = query.value(10).toString();
        QString storeName = query.value(11).toString();
        QString storeLocation = query.value(12).toString();
        QString workingHours = query.value(13).toString();

        qDebug() << "Retrieved book data:" << id << bookTitle << bookAuthor << category << publicate << publicate_city << publication_year << page_count << price << illustration;  // Логируем полученные данные

        QJsonObject book;
        book["id"] = id;
        book["title"] = bookTitle;
        book["author"] = bookAuthor;
        book["category"] = category;
        book["publisher"] = publicate;
        book["publisher_city"] = publicate_city;
        book["publication_year"] = publication_year;
        book["pages_count"] = page_count;
        book["price"] = price;
        book["illustrations_count"] = illustration;
        QJsonObject store;
        store["store_id"] = storeId;
        store["store_name"] = storeName;
        store["store_location"] = storeLocation;
        store["working_hours"] = workingHours;
        book["store"] = store;
        booksArray.append(book);
        } while (query.next());
    }

    if (booksArray.isEmpty()) {
        qDebug() << "No books found matching the search criteria.";
    }

    QJsonDocument doc(booksArray);
    QByteArray responseData = doc.toJson();
    qDebug() << "Sent to client: " << responseData;

    clientSocket->write(responseData);
}

void Server::approveRequestInDatabase(const QString &requestId) {
    QSqlQuery query;

    QSqlDatabase::database().transaction();

    // Получаем информацию о заявке
    query.prepare("SELECT * FROM requests WHERE request_id = :request_id");
    query.bindValue(":request_id", requestId);

    if (!query.exec()) {
        qCritical() << "Ошибка при получении данных заявки: " << query.lastError().text();
        QSqlDatabase::database().rollback();
        return;
    }

    if (!query.next()) {
        qCritical() << "Заявка с ID " << requestId << " не найдена.";
        QSqlDatabase::database().rollback();
        return;
    }

    // Извлекаем данные заявки
    int bookId = query.value("book_id").toInt();
    QString title = query.value("title").toString();
    QString author = query.value("author").toString();
    QString category = query.value("category").toString();
    QString publisher = query.value("publisher").toString();
    QString publisherCity = query.value("publisher_city").toString();
    int publicationYear = query.value("publication_year").toInt();
    int pagesCount = query.value("pages_count").toInt();
    double price = query.value("price").toDouble();
    int illustrationsCount = query.value("illustrations_count").toInt();

    // Получаем данные о магазине из заявки
    QString storeName = query.value("store_name").toString();
    QString storeLocation = query.value("store_location").toString();
    QString workingHours = query.value("working_hours").toString();
    QString requestType = query.value("request_type").toString(); // Получаем тип заявки

    // Проверка на тип заявки
    if (requestType == "ADD_BOOK") {
        // Если заявка на добавление книги
        qDebug() << "Тип заявки: Добавить книгу";

        // Проверяем, существует ли магазин в таблице stores
        query.prepare("SELECT store_id FROM stores WHERE store_name = :store_name AND store_location = :store_location");
        query.bindValue(":store_name", storeName);
        query.bindValue(":store_location", storeLocation);
        if (!query.exec()) {
            qCritical() << "Ошибка при проверке магазина: " << query.lastError().text();
            QSqlDatabase::database().rollback();
            return;
        }

        int storeId;
        if (!query.next()) {
            // Если магазин не найден, добавляем его в таблицу stores
            query.prepare("INSERT INTO stores (store_name, store_location, working_hours) "
                          "VALUES (:store_name, :store_location, :working_hours)");
            query.bindValue(":store_name", storeName);
            query.bindValue(":store_location", storeLocation);
            query.bindValue(":working_hours", workingHours);

            if (!query.exec()) {
                qCritical() << "Ошибка при добавлении магазина: " << query.lastError().text();
                QSqlDatabase::database().rollback();
                return;
            }

            // Получаем ID только что добавленного магазина
            storeId = query.lastInsertId().toInt();
            qDebug() << "Магазин добавлен в базу данных с ID " << storeId;
        } else {
            // Если магазин существует, получаем его store_id
            storeId = query.value("store_id").toInt();
            qDebug() << "Магазин найден в базе данных с ID " << storeId;
        }

        // Теперь добавляем книгу в таблицу books
        query.prepare("INSERT INTO books (book_id, title, author, category, publisher, publisher_city, "
                      "publication_year, pages_count, price, illustrations_count, store_id) "
                      "VALUES (:book_id, :title, :author, :category, :publisher, :publisher_city, "
                      ":publication_year, :pages_count, :price, :illustrations_count, :store_id)");
                      // Привязываем параметры для книги
        query.bindValue(":book_id", bookId);
        query.bindValue(":title", title);
        query.bindValue(":author", author);
        query.bindValue(":category", category);
        query.bindValue(":publisher", publisher);
        query.bindValue(":publisher_city", publisherCity);
        query.bindValue(":publication_year", publicationYear);
        query.bindValue(":pages_count", pagesCount);
        query.bindValue(":price", price);
        query.bindValue(":illustrations_count", illustrationsCount);
        query.bindValue(":store_id", storeId);

        if (!query.exec()) {
            qCritical() << "Ошибка при добавлении книги в таблицу books: " << query.lastError().text();
            QSqlDatabase::database().rollback();
            return;
        }

        // Обновляем статус заявки в таблице requests
        query.prepare("UPDATE requests SET status = 'approved' WHERE request_id = :request_id");
        query.bindValue(":request_id", requestId);

        if (!query.exec()) {
            qCritical() << "Ошибка при обновлении статуса заявки: " << query.lastError().text();
            QSqlDatabase::database().rollback();
            return;
        }

        qDebug() << "Заявка с ID " << requestId << " успешно одобрена, книга добавлена в таблицу books и информация о магазине в таблицу stores.";

    } else if (requestType == "DELETE_BOOK") {
        // Если заявка на удаление книги
        qDebug() << "Тип заявки: Удалить книгу";

        // Удаляем книгу из таблицы books
        query.prepare("DELETE FROM books WHERE book_id = :book_id");
        query.bindValue(":book_id", bookId);

        if (!query.exec()) {
            qCritical() << "Ошибка при удалении книги из таблицы books: " << query.lastError().text();
            QSqlDatabase::database().rollback();
            return;
        }

        // Обновляем статус заявки в таблице requests
        query.prepare("UPDATE requests SET status = 'deleted' WHERE request_id = :request_id");
        query.bindValue(":request_id", requestId);

        if (!query.exec()) {
            qCritical() << "Ошибка при обновлении статуса заявки: " << query.lastError().text();
            QSqlDatabase::database().rollback();
            return;
        }

        qDebug() << "Заявка с ID " << requestId << " успешно отклонена, книга удалена из таблицы books.";

    } else {
        // Если неизвестный тип заявки
        qCritical() << "Неизвестный тип заявки: " << requestType;
        QSqlDatabase::database().rollback();
        return;
    }

    // Завершаем транзакцию
    QSqlDatabase::database().commit();
}

void Server::rejectRequestInDatabase(const QString &requestId) {
    QSqlQuery query;
    QSqlDatabase::database().transaction();

    query.prepare("SELECT * FROM requests WHERE request_id = :request_id");
    query.bindValue(":request_id", requestId);

    if (!query.exec()) {
        //qCritical() << "Ошибка при получении данных заявки: " << query.lastError().text();
        //QSqlDatabase::database().rollback();
        return;
    }

    if (!query.next()) {
        qCritical() << "Заявка с ID " << requestId << " не найдена.";
        QSqlDatabase::database().rollback();
        return;
    }

    if (!query.exec()) {
        qCritical() << "Ошибка при добавлении книги в таблицу books: " << query.lastError().text();
        QSqlDatabase::database().rollback();
        return;
    }

    query.prepare("UPDATE requests SET status = 'reject' WHERE request_id = :request_id");
    query.bindValue(":request_id", requestId);

    if (!query.exec()) {
        qCritical() << "Ошибка при обновлении статуса заявки: " << query.lastError().text();
        QSqlDatabase::database().rollback();
        return;
    }

    QSqlDatabase::database().commit();

    qDebug() << "Заявка с ID " << requestId << " успешно отклонена.";
}
void Server::deleteRequestInDatabase(const QString &requestId) {
    QSqlQuery query;
    QSqlDatabase::database().transaction();

    query.prepare("SELECT * FROM requests WHERE request_id = :request_id");
    query.bindValue(":request_id", requestId);

    if (!query.exec()) {
        //qCritical() << "Ошибка при получении данных заявки: " << query.lastError().text();
        //QSqlDatabase::database().rollback();
        return;
    }

    if (!query.exec()) {
        qCritical() << "Ошибка при добавлении книги в таблицу books: " << query.lastError().text();
        QSqlDatabase::database().rollback();
        return;
    }

    query.prepare("DELETE FROM requests WHERE request_id = :request_id");
    query.bindValue(":request_id", requestId);

    if (!query.exec()) {
        qCritical() << "Ошибка при обновлении статуса заявки: " << query.lastError().text();
        QSqlDatabase::database().rollback();
        return;
    }

    QSqlDatabase::database().commit();

    qDebug() << "Заявка с ID " << requestId << " успешно удалена.";
}

void Server::handleGetAllBooksRequest(QTcpSocket *clientSocket) {
    QSqlQuery query;
    query.prepare("SELECT id, title, author FROM books;");

    // Логируем запрос
    qDebug() << query.lastQuery();

    if (!query.exec()) {
        qCritical() << "Database query failed:" << query.lastError();
        return;
    }

    // Собираем результат в JSON
    QJsonArray booksArray;
    if (query.first()) {
        do {
        QString id = query.value(0).toString();
        QString title = query.value(1).toString();
        QString author = query.value(2).toString();

        // Логируем полученные данные
        qDebug() << "Retrieved book data:" << id << title << author;

        QJsonObject book;
        book["id"] = id;
        book["title"] = title;
        book["author"] = author;
        booksArray.append(book);
    } while (query.next());
    }

    // Отправляем результат обратно клиенту
    QJsonDocument doc(booksArray);
    QByteArray responseData = doc.toJson();

    qDebug() << "Sent to client: " << responseData;
    clientSocket->write(responseData);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("postgres");
    db.setDatabaseName("bookstore");
    db.setUserName("postgres");
    db.setPassword("root");

    if (!db.open()) {
        qCritical() << "Failed to connect to the database:" << db.lastError();
        return -1;
    }

    qDebug() << "Database connected successfully.";

    Server server;
    server.startServer();

    return app.exec();
}