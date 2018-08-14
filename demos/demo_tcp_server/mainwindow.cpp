#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    pServer = new TCPServer(this);
}

MainWindow::~MainWindow() {
    if (pServer != nullptr) {
        delete pServer;
        pServer = nullptr;
    }

    if (ui != nullptr) {
        delete ui;
        ui   = nullptr;
    }
}
