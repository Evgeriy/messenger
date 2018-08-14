#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    pDBManager = new DatabaseManager(this,  "../../../resources/");
    pDBManager->open();

//    pDBManager->insertPMChat("Evgeriy", "Third");
//    pDBManager->insertMessage("Evgeriy", "Second", "Hi!!!");

//    setupUserTableModel();
//    setupPMCharTableModel();
//    setupMessageTableModel();

    pUserTableModel = new QSqlTableModel(this);
    pPMChatTableModel = new QSqlTableModel(this);
    pMessageTableModel = new QSqlTableModel(this);

    setupTableModel(ui->tableViewUser,
                    pUserTableModel,
                    USER_TABLE_NAME,
                    QStringList() << USER_TABLE_ID
                    << USER_TABLE_NIK
                    << USER_TABLE_FIRST_NAME
                    << USER_TABLE_SECOND_NAME
                    << USER_TABLE_PASSWORD);

    setupTableModel(ui->tableViewMessage,
                    pMessageTableModel,
                    MESSAGE_TABLE_NAME,
                    QStringList() << MESSAGE_TABLE_ID
                    << MESSAGE_TABLE_PM_CHAT_ID
                    << MESSAGE_TABLE_AUTHOR_ID
                    << MESSAGE_TABLE_CONTENT
                    << MESSAGE_TABLE_DATE);

    setupTableModel(ui->tableViewPMChat,
                    pPMChatTableModel,
                    PRIVATE_MESSAGE_CHAT_TABLE_NAME,
                    QStringList() << PRIVATE_MESSAGE_CHAT_TABLE_ID
                    << PRIVATE_MESSAGE_CHAT_TABLE_FIRST_USER_ID
                    << PRIVATE_MESSAGE_CHAT_TABLE_SECOND_USER_ID);


    connect(ui->pushButtonInsertUser, &QPushButton::clicked, this, &MainWindow::slotInsertNewUser);
    connect(ui->pushButtonInsertPM, &QPushButton::clicked, this, &MainWindow::onPushButtonInsertPMClicked);
    connect(ui->pushButtonInsertMessage, &QPushButton::clicked, this, &MainWindow::onPushButtonInsertMessageClicked);
}

MainWindow::~MainWindow() {
    if (pUserTableModel != nullptr) {
        delete pUserTableModel;
        pUserTableModel = nullptr;
    }
    if (pPMChatTableModel != nullptr) {
        delete pPMChatTableModel;
        pPMChatTableModel = nullptr;
    }
    if (pMessageTableModel != nullptr) {
        delete pMessageTableModel;
        pMessageTableModel = nullptr;
    }
    if (pDBManager != nullptr) {
        delete pDBManager;
        pDBManager = nullptr;
    }
    if (ui != nullptr) {
        delete ui;
        ui = nullptr;
    }
}

void MainWindow::setupTableModel(QTableView *_tableView, QSqlTableModel *_tableModel, const QString &_tableName, const QStringList &_headers) {
    _tableModel->setTable(_tableName);

    for (int section = 0, headerIndex = 0; section < _tableModel->columnCount(); ++section, ++headerIndex) {
        _tableModel->setHeaderData(section, Qt::Horizontal, _headers[headerIndex]);
    }

    _tableView->setModel(_tableModel);
    _tableModel->select();
}

void MainWindow::onPushButtonInsertUserClicked() {
    QString nik        = ui->lineEditNik->text();
    QString firstName  = ui->lineEditFirstName->text();
    QString secondName = ui->lineEditSecondName->text();
    QString password   = ui->lineEditPassword->text();

    ui->lineEditNik->clear();
    ui->lineEditFirstName->clear();
    ui->lineEditSecondName->clear();
    ui->lineEditPassword->clear();

    pDBManager->insertUser(nik, firstName, secondName, password);
    pUserTableModel->select();
}

void MainWindow::onPushButtonInsertPMClicked() {
    QString firstNik  = ui->lineEditFirstNikPM->text();
    QString secondNik = ui->lineEditSecondNikPM->text();

    ui->lineEditFirstNikPM->clear();
    ui->lineEditSecondNikPM->clear();

    pDBManager->insertPMChat(firstNik, secondNik);
    pPMChatTableModel->select();
}

void MainWindow::onPushButtonInsertMessageClicked() {
    QString firstNik  = ui->lineEditFirstNikMessage->text();
    QString secondNik = ui->lineEditSecondNikMessage->text();
    QString message   = ui->textEditMessage->toPlainText();

    ui->lineEditFirstNikMessage->clear();
    ui->lineEditSecondNikMessage->clear();
    ui->textEditMessage->clear();

    pDBManager->insertMessage(firstNik, secondNik, message, QDateTime::currentDateTime().toString());
    pMessageTableModel->select();
}

void MainWindow::slotInsertNewUser() {
    QString nik        = ui->lineEditNik->text();
    QString firstName  = ui->lineEditFirstName->text();
    QString secondName = ui->lineEditSecondName->text();
    QString password   = ui->lineEditPassword->text();

    ui->lineEditNik->clear();
    ui->lineEditFirstName->clear();
    ui->lineEditSecondName->clear();
    ui->lineEditPassword->clear();

    pDBManager->insertUser(nik,
                           firstName,
                           secondName,
                           password);

    pUserTableModel->select();
}
