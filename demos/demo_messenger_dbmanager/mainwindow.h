#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSqlTableModel>
#include <QTableView>
#include <QDateTime>
#include "databasemanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void setupTableModel(QTableView* _tableView,
                         QSqlTableModel* _tableModel,
                         const QString& _tableName,
                         const QStringList& _headers);

public:
    void onPushButtonInsertUserClicked();
    void onPushButtonInsertPMClicked();
    void onPushButtonInsertMessageClicked();

    void slotInsertNewUser();

private:
    Ui::MainWindow*  ui{nullptr};
    DatabaseManager* pDBManager{nullptr};
    QSqlTableModel*  pUserTableModel{nullptr};
    QSqlTableModel*  pPMChatTableModel{nullptr};
    QSqlTableModel*  pMessageTableModel{nullptr};
};

#endif // MAINWINDOW_H
