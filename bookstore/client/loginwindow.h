#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void onLoginButtonClicked();
    void onReadyRead();
    void onRegisterButtonClicked();
    void onSocketError(QAbstractSocket::SocketError socketError);
    void onSocketDisconnected();  // Добавить слот для отключения

private:
    Ui::LoginWindow *ui;
    QTcpSocket *socket;
    QString userRole;
    void openMainWindow();
    void setRole(const QString& role);
};

#endif // LOGINWINDOW_H