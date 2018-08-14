#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    pClient = new TCPClient(this);
    pClient->connectToServer("192.168.0.117", 4000);

    pUserListModel = new QStringListModel(this);
    ui->listViewUserList->setModel(pUserListModel);

    pDialogModel = new QStringListModel(this);
    ui->listViewUserDialog->setModel(pDialogModel);

    connect(ui->pushButtonLogIn, &QPushButton::clicked, this, &MainWindow::onPushButtonLogIn);
    connect(ui->pushButtonSend, &QPushButton::clicked, this, &MainWindow::onPushButtonSend);
    connect(ui->listViewUserList, &QListView::clicked, this, &MainWindow::onUserList);

    connect(this, &MainWindow::sendActiveUser, pClient, &TCPClient::onSendUserChatsToGUI);
    connect(pClient, &TCPClient::sendUserProfileToGUI, this, &MainWindow::updateUserProfile);
    connect(pClient, &TCPClient::sendUserListToGUI, this, &MainWindow::updateUserListModel);
    connect(pClient, &TCPClient::sendUserChatsToGUI, this, &MainWindow::updateUserChatModel);
}

MainWindow::~MainWindow() {
    if (pClient != nullptr) {
        delete pClient;
        pClient = nullptr;
    }

    if (ui != nullptr) {
        delete ui;
        ui = nullptr;
    }
}

void MainWindow::onPushButtonLogIn() {
    pClient->doAuthorization(ui->lineEditNik->text(), ui->lineEditPassword->text());
    pClient->loadUserList();
}

void MainWindow::onPushButtonSend() {
    QString msg = ui->textEditMessage->toPlainText();
    pClient->sendPM(mSelectedUser, msg);
    ui->textEditMessage->clear();
}

void MainWindow::onUserList(QModelIndex index) {
    qDebug() << index.data().toString();
    emit sendActiveUser(index.data().toString());
}

void MainWindow::updateUserProfile(const QString &_firstName, const QString &_secondName) {
    ui->labelUserPersonalData->setText(_firstName + " " + _secondName);
}

void MainWindow::updateUserListModel(const QList<QString> &_list) {
    pUserListModel->setStringList(_list);
}

void MainWindow::updateUserChatModel(const QString &_from, const QList<QString> &_list) {
    mSelectedUser = _from;
    pDialogModel->setStringList(_list);
}
