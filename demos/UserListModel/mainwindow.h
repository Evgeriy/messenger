#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include "userlistmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void slotInsert();

private:
    Ui::MainWindow* pUi{nullptr};
    Database*       pDatabase{nullptr};
    UserListModel*  pUserListModel{nullptr};
};

#endif // MAINWINDOW_H
