#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    pUi(new Ui::MainWindow) {
    pUi->setupUi(this);

    pDatabase = new Database(this, "../../../resources/");
    pDatabase->connectToDatabase();

    pUserListModel = new UserListModel(this);
    pUserListModel->setupModel("User",
                               QStringList() << trUtf8("UserId")
                                             << trUtf8("UserFirstName")
                                             << trUtf8("UserSecondName")
                                             << trUtf8("UserPassword"));

    pUi->tableView->setModel(pUserListModel);
    pUserListModel->select();

    connect(pUi->pushButtonInsert, &QPushButton::clicked, this, &MainWindow::slotInsert);
}

MainWindow::~MainWindow() {
    delete pUi;
}

void MainWindow::slotInsert() {
    QString firstName = pUi->lineEditFirstName->text();
    QString secondName = pUi->lineEditSecondName->text();
    QString password = pUi->lineEditPassword->text();

    pDatabase->insertUser(firstName, secondName, password);
    pUserListModel->select();
}
