#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTcpSocket>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString userRole;
    void setRole(const QString& role);

private slots:
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError socketError);
    void onSocketDisconnected();
    void onSearchClicked();
    void fetchAllBooks();
    void onFilterChanged();
    void onRequestButtonClicked();
    void onViewRequestsButtonClicked();  // Обработка нажатия кнопки просмотра заявок
    void onEditDataButtonClicked();     // Обработка нажатия кнопки редактирования данных
    void onApproveRequestButtonClicked();  // Одобрение заявки
    void onRejectRequestButtonClicked();   // Отклонение заявки
    void onDeleteBookButtonClicked();  // Удаление книги

private:
    void setupMenu();
    void setupToolBar();
    void createToolBar();
    void showFilterMenu();

    QTcpSocket *socket;        // Сокет для связи с сервером
    QTableWidget *tableWidget;  // Таблица для отображения книг
    QLineEdit *authorEdit;
    QLineEdit *titleEdit;
    QLineEdit *sectionEdit;     // Новое поле для раздела
    QLineEdit *publisherEdit;   // Новое поле для издательства
    QLineEdit *minPriceEdit;    // Поле для минимальной цены
    QLineEdit *maxPriceEdit;    // Поле для максимальной цены
    QToolBar *toolBar;

    QString authorFilter;
    QString titleFilter;
    QString yearFilter;
    QString pagesFilter;
    QString sectionFilter;     // Новый фильтр для раздела
    QString publisherFilter;   // Новый фильтр для издательства
    QString minPriceFilter;    // Новый фильтр для минимальной цены
    QString maxPriceFilter;    // Новый фильтр для максимальной цены
    QLabel *roleLabel;
    QLineEdit *yearEdit;    // Поле для ввода года издания
    QLineEdit *priceEdit;  // Поле для ввода цены
    QLineEdit *illustrationsEdit;
    QLineEdit *pagesEdit;
    QLineEdit *minPagesEdit;
    QLineEdit *maxPagesEdit;
    QLineEdit *yearFromEdit;
    QLineEdit *yearToEdit;
    QLineEdit *requestTypeEdit;  // Поле для выбора типа заявки (добавление, удаление, изменение)
    QPushButton *requestButton;  // Кнопка для создания заявки
    QLineEdit *storeNameEdit;
    QLineEdit *storeAddressEdit;
    QLineEdit *storeScheduleEdit;
    void configureForRole();
};

#endif // MAINWINDOW_H
