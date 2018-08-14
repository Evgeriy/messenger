#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>
#include "tcpclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendActiveUser(QString);

public slots:
    void onPushButtonLogIn();
    void onPushButtonSend();
    void onUserList(QModelIndex index);

public:
    void updateUserProfile(const QString& _firstName, const QString& _secondName);
    void updateUserListModel(const QList<QString>& _list);
    void updateUserChatModel(const QString& _from, const QList<QString>& _list);

private:
    Ui::MainWindow* ui{nullptr};
    TCPClient*      pClient{nullptr};
    QString         mSelectedUser;

    QStringListModel *pUserListModel{nullptr};
    QStringListModel *pDialogModel{nullptr};
};

#endif // MAINWINDOW_H
