#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QDateTime>

#include "dbmanager_global.h"

class DatabaseManager : public QObject {
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *_parent = nullptr);
    ~DatabaseManager();

public:
    bool initialize();
    bool open();

private:
    bool create();
    bool createUserTable();
    bool createPrivateMessageChatTable();
    bool createMessageTable();

public:
    bool checkAuthorization(const QString& _nik,
                            const QString& _password,
                                  QString& _firstName,
                                  QString& _secondName);
    bool getUserList(const QString&  _nik,
                     QList<QString>& _userList);
    bool getMessageList(const QString&  _firstUser,
                        const QString&  _secondUser,
                        QList<QString>& _messageList);

public:
    bool insertUser(const QString& _userNik,
                    const QString& _userFirstName,
                    const QString& _userSecondName,
                    const QString& _userPassword);
    bool insertPMChat(const QString& _firstUserNik,
                      const QString& _secondUserNik);
    bool insertMessage(const QString& _firstUserNik,
                       const QString& _secondUserNik,
                       const QString& _content,
                       const QString& _date);

private:
    int getUserIdByNik(const QString &_nik);
    int getPMChatIdByNiks(const QString &_firstNik, const QString &_secondNik);
    bool isQueryExecuted(QSqlQuery &_query, const QString &_tableName, const QString &_desc = "");

private:
    QSqlDatabase mDatabase;
};

#endif // DATABASEMANAGER_H
