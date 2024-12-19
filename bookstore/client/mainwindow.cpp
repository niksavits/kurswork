#include "mainwindow.h"
#include "RequestsWindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QAction>
#include <QMenuBar>
#include <QComboBox>
#include <QRegExpValidator>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    socket(new QTcpSocket(this)),
    tableWidget(new QTableWidget(this)),
    authorEdit(new QLineEdit(this)),
    titleEdit(new QLineEdit(this)),
    sectionEdit(new QLineEdit(this)),           // Новое поле для раздела
    publisherEdit(new QLineEdit(this)),         // Новое поле для издательства
    minPriceEdit(new QLineEdit(this)),          // Поле для минимальной цены
    maxPriceEdit(new QLineEdit(this)),
    yearFromEdit(new QLineEdit(this)),
    yearToEdit(new QLineEdit(this)),
    roleLabel(new QLabel(this)),
    requestButton(new QPushButton("Оставить заявку", this)),
    toolBar(new QToolBar(this))
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(roleLabel); 
    configureForRole();

    // Поля для ввода поиска
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Автор:"));
    searchLayout->addWidget(authorEdit);
    searchLayout->addWidget(new QLabel("Название:"));
    searchLayout->addWidget(titleEdit);

    QPushButton *searchButton = new QPushButton("Поиск", this);
    searchLayout->addWidget(searchButton);

    layout->addLayout(searchLayout);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->addWidget(new QLabel("Раздел:"));
    filterLayout->addWidget(sectionEdit = new QLineEdit(this));  // Новое поле для раздела
    filterLayout->addWidget(new QLabel("Издательство:"));
    filterLayout->addWidget(publisherEdit = new QLineEdit(this));  // Новое поле для издательства
    filterLayout->addWidget(new QLabel("Мин. цена:"));
    filterLayout->addWidget(minPriceEdit = new QLineEdit(this));  // Поле для минимальной цены
    filterLayout->addWidget(new QLabel("Макс. цена:"));
    filterLayout->addWidget(maxPriceEdit = new QLineEdit(this));  // Поле для максимальной цены
    yearEdit = new QLineEdit(this);  // Год издания
    pagesEdit = new QLineEdit(this);  // Количество страниц 
    illustrationsEdit = new QLineEdit(this);  // Количество иллюстраций
    yearFromEdit = new QLineEdit(this);  // Поле для "От" года
    yearToEdit = new QLineEdit(this);    // Поле для "До" года

    QHBoxLayout *yearLayout = new QHBoxLayout();
    yearLayout->addWidget(new QLabel("Год издания от:"));
    yearLayout->addWidget(yearFromEdit);
    yearLayout->addWidget(new QLabel("до:"));
    yearLayout->addWidget(yearToEdit);
    layout->addLayout(yearLayout);
    minPagesEdit = new QLineEdit(this);  // Поле для минимального числа страниц
    maxPagesEdit = new QLineEdit(this);  

QHBoxLayout *pagesLayout = new QHBoxLayout();
pagesLayout->addWidget(new QLabel("Страницы от:"));
pagesLayout->addWidget(minPagesEdit);
pagesLayout->addWidget(new QLabel("до:"));
pagesLayout->addWidget(maxPagesEdit);
layout->addLayout(pagesLayout);
QHBoxLayout *storeLayout = new QHBoxLayout();
layout->addLayout(storeLayout);

    QHBoxLayout *illustrationsLayout = new QHBoxLayout();
    layout->addLayout(illustrationsLayout);
    layout->addLayout(filterLayout);
    layout->addWidget(requestButton); 
    connect(requestButton, &QPushButton::clicked, this, &MainWindow::onRequestButtonClicked);
    
    tableWidget->setColumnCount(12);  // ID, Название, Автор, Раздел, Издательство, Год, Страницы, Цена, Иллюстрации
    tableWidget->setHorizontalHeaderLabels({"ID", "Название", "Автор", "Раздел", "Издательство", "Год", "Страницы", "Цена", "Иллюстрации", "Магазин", "Адрес магазина", "График работы"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSortingEnabled(true);
    layout->addWidget(tableWidget);
    socket->connectToHost("192.168.0.13", 1234);
    authorEdit->setMaxLength(100);
    titleEdit->setMaxLength(200);
    sectionEdit->setMaxLength(50);
    publisherEdit->setMaxLength(100);
    minPriceEdit->setMaxLength(10);
    maxPriceEdit->setMaxLength(10);
    yearFromEdit->setMaxLength(4);
    yearToEdit->setMaxLength(4);
    minPagesEdit->setMaxLength(5);
    maxPagesEdit->setMaxLength(5);
    
    QRegExpValidator *priceValidator = new QRegExpValidator(QRegExp("[0-9]+(\\.[0-9]{1,2})?"), this);
    QRegExpValidator *authorValidator = new QRegExpValidator(QRegExp("^[a-zA-Zа-яА-ЯёЁ\\s]+$"), this);
    QRegExpValidator *titleValidator = new QRegExpValidator(QRegExp("^[a-zA-Zа-яА-ЯёЁ0-9\\s\\-\\.,!?]+$"), this);
    QRegExpValidator *publisherValidator = new QRegExpValidator(QRegExp("^[a-zA-Zа-яА-ЯёЁ\\s]+$"), this);
    publisherEdit->setValidator(publisherValidator);
    titleEdit->setValidator(titleValidator);
    authorEdit->setValidator(authorValidator);
    minPriceEdit->setValidator(priceValidator);
    maxPriceEdit->setValidator(priceValidator);
    yearFromEdit->setValidator(new QRegExpValidator(QRegExp("\\d{4}"), this));
    yearToEdit->setValidator(new QRegExpValidator(QRegExp("\\d{4}"), this));

    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::onSocketError);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onSocketDisconnected);
    resize(800, 600);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    createToolBar();
    fetchAllBooks();
}
void MainWindow::showFilterMenu() {
    QMenu *filterMenu = new QMenu(this);

    // Создаем действия для фильтров с галочками
    QAction *filterByAuthor = new QAction("По автору", this);
    filterByAuthor->setCheckable(true);

    QAction *filterByTitle = new QAction("По названию", this);
    filterByTitle->setCheckable(true);

    QAction *filterByYear = new QAction("По году издания", this);
    filterByYear->setCheckable(true);

    QAction *filterByPages = new QAction("По количеству страниц", this);
    filterByPages->setCheckable(true);

    // Добавляем действия в меню
    filterMenu->addAction(filterByAuthor);
    filterMenu->addAction(filterByTitle);
    filterMenu->addAction(filterByYear);
    filterMenu->addAction(filterByPages);

    // Показываем меню
    filterMenu->exec(QCursor::pos());
}
void MainWindow::createToolBar() {
    toolBar = new QToolBar(this);
    QAction *filterAction = new QAction("Фильтры", this);
    connect(filterAction, &QAction::triggered, this, &MainWindow::showFilterMenu);
    toolBar->addAction(filterAction);
    addToolBar(toolBar);
}
void MainWindow::onRequestButtonClicked() {
    QDialog *requestDialog = new QDialog(this);
    requestDialog->setWindowTitle("Оставить заявку");

    QVBoxLayout *dialogLayout = new QVBoxLayout(requestDialog);
    dialogLayout->addWidget(new QLabel("Тип заявки:"));

    QComboBox *requestTypeComboBox = new QComboBox(requestDialog);
    requestTypeComboBox->addItem("ADD_BOOK");
    requestTypeComboBox->addItem("EDIT_BOOK");
    requestTypeComboBox->addItem("DELETE_BOOK");
    dialogLayout->addWidget(requestTypeComboBox);

    QLineEdit *idEdit = new QLineEdit(requestDialog);
    idEdit->setPlaceholderText("ID");
    dialogLayout->addWidget(idEdit);

    QLineEdit *titleEdit = new QLineEdit(requestDialog);
    titleEdit->setPlaceholderText("Название книги");
    dialogLayout->addWidget(titleEdit);

    QLineEdit *authorEdit = new QLineEdit(requestDialog);
    authorEdit->setPlaceholderText("Автор");
    dialogLayout->addWidget(authorEdit);

    QLineEdit *sectionEdit = new QLineEdit(requestDialog);
    sectionEdit->setPlaceholderText("Раздел");
    dialogLayout->addWidget(sectionEdit);

    QLineEdit *publisherEdit = new QLineEdit(requestDialog);
    publisherEdit->setPlaceholderText("Издательство");
    dialogLayout->addWidget(publisherEdit);

    QLineEdit *yearEdit = new QLineEdit(requestDialog);
    yearEdit->setPlaceholderText("Год издания");
    dialogLayout->addWidget(yearEdit);

    QLineEdit *pagesEdit = new QLineEdit(requestDialog);
    pagesEdit->setPlaceholderText("Количество страниц");
    dialogLayout->addWidget(pagesEdit);

    QLineEdit *priceEdit = new QLineEdit(requestDialog);
    priceEdit->setPlaceholderText("Цена");
    dialogLayout->addWidget(priceEdit);

    QLineEdit *illustrationsEdit = new QLineEdit(requestDialog);
    illustrationsEdit->setPlaceholderText("Количество иллюстраций");
    dialogLayout->addWidget(illustrationsEdit);

    QLineEdit *storeNameEdit = new QLineEdit(requestDialog);
    storeNameEdit->setPlaceholderText("Магазин");
    dialogLayout->addWidget(storeNameEdit);

    QLineEdit *storeAddressEdit = new QLineEdit(requestDialog);
    storeAddressEdit->setPlaceholderText("Адрес магазина");
    dialogLayout->addWidget(storeAddressEdit);

    QLineEdit *storeScheduleEdit = new QLineEdit(requestDialog);
    storeScheduleEdit->setPlaceholderText("График работы");
    dialogLayout->addWidget(storeScheduleEdit);

    authorEdit->setMaxLength(100);
    titleEdit->setMaxLength(200);
    sectionEdit->setMaxLength(50);
    publisherEdit->setMaxLength(100);
    pagesEdit->setMaxLength(10);
    priceEdit->setMaxLength(10);
    illustrationsEdit->setMaxLength(6);
    storeNameEdit->setMaxLength(100);
    storeScheduleEdit->setMaxLength(50);
    yearEdit->setMaxLength(4);
    idEdit->setMaxLength(8);
    sectionEdit->setMaxLength(35);
    
    QRegExpValidator *priceValidator = new QRegExpValidator(QRegExp("[0-9]+(\\.[0-9]{1,2})?"), this);
    QRegExpValidator *authorValidator = new QRegExpValidator(QRegExp("^[a-zA-Zа-яА-ЯёЁ\\s]+$"), this);
    QRegExpValidator *titleValidator = new QRegExpValidator(QRegExp("^[a-zA-Zа-яА-ЯёЁ0-9\\s\\-\\.,!?]+$"), this);
    QRegExpValidator *storeNameValidator = new QRegExpValidator(QRegExp("^[a-zA-Zа-яА-ЯёЁ0-9\\s\\-\\.,!?]+$"), this);
    QRegExpValidator *storeScheduleValidator = new QRegExpValidator(QRegExp("^[a-zA-Zа-яА-ЯёЁ0-9\\s\\-\\.,!?]+$"), this);
    QRegExpValidator *publisherValidator = new QRegExpValidator(QRegExp("^[a-zA-Zа-яА-ЯёЁ\\s]+$"), this);
    QRegExpValidator *pagesValidator = new QRegExpValidator(QRegExp("[0-9]+(\\.[0-9]{1,2})?"), this);
    QRegExpValidator *yearValidator = new QRegExpValidator(QRegExp("[0-9]+(\\.[0-9]{1,2})?"), this);
    QRegExpValidator *illustrationsValidator = new QRegExpValidator(QRegExp("[0-9]+(\\.[0-9]{1,2})?"), this);
    QRegExpValidator *idValidator = new QRegExpValidator(QRegExp("[0-9]+(\\.[0-9]{1,2})?"), this);
    QRegExpValidator *sectionValidator = new QRegExpValidator(QRegExp("^[a-zA-Zа-яА-ЯёЁ\\s]+$"), this);

    publisherEdit->setValidator(publisherValidator);
    titleEdit->setValidator(titleValidator);
    authorEdit->setValidator(authorValidator);
    pagesEdit->setValidator(priceValidator);
    yearEdit->setValidator(priceValidator);
    illustrationsEdit->setValidator(illustrationsValidator);
    idEdit->setValidator(idValidator);
    priceEdit->setValidator(priceValidator);
    storeNameEdit->setValidator(storeNameValidator);
    storeScheduleEdit->setValidator(storeScheduleValidator);
    sectionEdit->setValidator(sectionValidator);

    QPushButton *submitButton = new QPushButton("Отправить заявку", requestDialog);
    connect(submitButton, &QPushButton::clicked, [this, idEdit, titleEdit, authorEdit, sectionEdit, publisherEdit, yearEdit, pagesEdit, priceEdit, illustrationsEdit, storeNameEdit, storeAddressEdit, storeScheduleEdit, requestTypeComboBox, requestDialog]() {
        QString requestType = requestTypeComboBox->currentText();  // "ADD_BOOK", "EDIT_BOOK", "DELETE_BOOK"
        QJsonObject requestObject;
        requestObject["book_id"] = idEdit->text();
        requestObject["title"] = titleEdit->text();
        requestObject["author"] = authorEdit->text();
        requestObject["category"] = sectionEdit->text();
        requestObject["publisher"] = publisherEdit->text();
        requestObject["publisher_city"] = publisherEdit->text();
        requestObject["publication_year"] = yearEdit->text();
        requestObject["pages_count"] = pagesEdit->text();
        requestObject["price"] = priceEdit->text();
        requestObject["illustrations_count"] = illustrationsEdit->text();
        requestObject["request_type"] = requestType;
        requestObject["store_name"] = storeNameEdit->text();
        requestObject["store_location"] = storeAddressEdit->text();
        requestObject["working_hours"] = storeScheduleEdit->text();
        QJsonDocument jsonDoc(requestObject);
        QByteArray requestData = requestType.toUtf8() + " " + jsonDoc.toJson();

        // Отправка данных на сервер
        socket->write(requestData);
        socket->flush();

        // Закрываем окно после отправки заявки
        requestDialog->accept();
    });
    dialogLayout->addWidget(submitButton);

    requestDialog->exec();  // Открыть диалог
}
void MainWindow::onViewRequestsButtonClicked() {
    // Реализация просмотра заявок
    QMessageBox::information(this, "Просмотр заявок", "Заявки для просмотра.");
}
void MainWindow::onEditDataButtonClicked() {
    // Реализация редактирования данных
    QMessageBox::information(this, "Редактировать данные", "Редактирование данных.");
}

void MainWindow::onApproveRequestButtonClicked() {
    // Реализация одобрения заявки
    QMessageBox::information(this, "Одобрение заявки", "Заявка была одобрена.");
}

void MainWindow::onRejectRequestButtonClicked() {
    // Реализация отклонения заявки
    QMessageBox::information(this, "Отклонение заявки", "Заявка была отклонена.");
}

void MainWindow::onDeleteBookButtonClicked() {
    // Реализация удаления книги
    QMessageBox::information(this, "Удаление книги", "Книга была удалена.");
}
void MainWindow::configureForRole() {
    if (userRole == "admin") {
        roleLabel->setText("Роль: Администратор");
        roleLabel->setStyleSheet("font-weight: bold; color: red;");
        RequestsWindow *requestsWindow = new RequestsWindow();
        requestsWindow->show();
    } else if (userRole == "user") {
        roleLabel->setText("Роль: Пользователь");
        roleLabel->setStyleSheet("font-weight: bold; color: green;");
    }
}
void MainWindow::onFilterChanged() {
    qDebug() << "Filters applied: Author - " << authorFilter << ", Title - " << titleFilter;

    // Запускаем новый поиск с обновленными фильтрами
    onSearchClicked();
} 

MainWindow::~MainWindow()
{
    delete socket;  // Закрытие подключения к базе данных
}
void MainWindow::setRole(const QString& role) {
    userRole = role;
    configureForRole();
}

void MainWindow::onSearchClicked() {
    QString author = authorEdit->text();
    QString title = titleEdit->text();
    QString section = sectionEdit->text();  // Новое поле для раздела
    QString publisher = publisherEdit->text();  // Новое поле для издательства
    QString minPrice = minPriceEdit->text();  // Поле для минимальной цены
    QString maxPrice = maxPriceEdit->text();  // Поле для максимальной цены
    QString year = yearEdit->text();  // Год издания
    QString pages = pagesEdit->text();  // Количество страниц
    QString illustrations = illustrationsEdit->text();  // Количество иллюстраций
    QString yearFrom = yearFromEdit->text();
    QString yearTo = yearToEdit->text();
    QString minPages = minPagesEdit->text();  // Минимальное количество страниц
    QString maxPages = maxPagesEdit->text();  // Максимальное количество страниц

    QString searchRequest = "SEARCH " + author + " " + title;

    // Добавляем дополнительные фильтры
    if (!authorFilter.isEmpty()) {
        searchRequest += " AUTHOR:" + authorFilter;
    }
    if (!titleFilter.isEmpty()) {
        searchRequest += " TITLE:" + titleFilter;
    }
    if (!section.isEmpty()) {
        searchRequest += " SECTION:" + section;
    }
    if (!publisher.isEmpty()) {
        searchRequest += " PUBLISHER:" + publisher;
    }
    if (!minPrice.isEmpty()) {
        searchRequest += " MIN_PRICE:" + minPrice;
    }
    if (!maxPrice.isEmpty()) {
        searchRequest += " MAX_PRICE:" + maxPrice;
    }
    if (!yearFrom.isEmpty()) {
        searchRequest += " YEAR_FROM:" + yearFrom;
    }
    if (!yearTo.isEmpty()) {
        searchRequest += " YEAR_TO:" + yearTo;
    }
    if (!minPages.isEmpty()) {
        searchRequest += " MIN_PAGE:" + minPages;
    }
    if (!maxPages.isEmpty()) {
        searchRequest += " MAX_PAGE:" + maxPages;
    }
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(searchRequest.toUtf8());
    } else if (socket->state() == QAbstractSocket::ConnectingState) {
        qDebug() << "Сокет подключается, подождите...";
    } else if (socket->state() == QAbstractSocket::UnconnectedState) {
        socket->connectToHost("192.168.0.13", 1234);
    }
}
void MainWindow::fetchAllBooks() {
    if (socket->state() == QAbstractSocket::ConnectedState) {
    QByteArray request = "GET_ALL_BOOKS";  // Запрос для получения всех книг
    socket->write(request);  // Отправка запроса серверу
    socket->flush();  // Убедитесь, что данные отправлены
    } else if (socket->state() == QAbstractSocket::ConnectingState) {
        // Если сокет еще подключается, подождем
        qDebug() << "Сокет подключается, подождите...";
    } else if (socket->state() == QAbstractSocket::UnconnectedState) {
        // Если сокет не подключен, пытаемся подключиться
    socket->connectToHost("192.168.0.13", 1234);
    }
}
void MainWindow::onReadyRead() {
    QByteArray data = socket->readAll();
    qDebug() << "Получены данные от сервера:" << data;

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isArray()) {
        QJsonArray booksArray = doc.array();
        tableWidget->setRowCount(booksArray.size());

        for (int i = 0; i < booksArray.size(); ++i) {
            QJsonObject bookObject = booksArray[i].toObject();
        
            QString id = bookObject["id"].toString();
            QString title = bookObject["title"].toString();
            QString author = bookObject["author"].toString();
            QString section = bookObject["category"].toString();
            QString publisher = bookObject["publisher"].toString();
            QString publisher_city = bookObject["publisher_city"].toString();
            QString year = bookObject["publication_year"].toString();
            QString pages = bookObject["pages_count"].toString();
            QString price = bookObject["price"].toString();
            QString illustrations = bookObject["illustrations_count"].toString();
            QJsonObject storeObject = bookObject["store"].toObject();
            QString storeId = storeObject["store_name"].toString();
            QString storeName = storeObject["store_location"].toString();
            QString storeLocation = storeObject["working_hours"].toString();

            tableWidget->setItem(i, 0, new QTableWidgetItem(id));
            tableWidget->setItem(i, 1, new QTableWidgetItem(title));
            tableWidget->setItem(i, 2, new QTableWidgetItem(author));
            tableWidget->setItem(i, 3, new QTableWidgetItem(section));
            tableWidget->setItem(i, 4, new QTableWidgetItem(publisher));
            tableWidget->setItem(i, 5, new QTableWidgetItem(year));
            tableWidget->setItem(i, 6, new QTableWidgetItem(pages));
            tableWidget->setItem(i, 7, new QTableWidgetItem(price));
            tableWidget->setItem(i, 8, new QTableWidgetItem(illustrations));
            tableWidget->setItem(i, 9, new QTableWidgetItem(storeId));
            tableWidget->setItem(i, 10, new QTableWidgetItem(storeName));
            tableWidget->setItem(i, 11, new QTableWidgetItem(storeLocation));
        }
    }
}

// Обработчик ошибок сокета
void MainWindow::onSocketError(QAbstractSocket::SocketError socketError)
{
    QMessageBox::critical(this, "Ошибка сети", socket->errorString());
}

// Обработчик отключения сокета
void MainWindow::onSocketDisconnected()
{
    socket->close();
}