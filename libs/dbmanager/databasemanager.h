#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QDateTime>
#include <sys/types.h>
#include <qlogging.h>

#include "dbmanager_global.h"

class DatabaseManager : public QObject {
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *_parent = nullptr, const QString &_pathToDatabase = "");
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
    void getUserList(const QString&  _nik,
                     QList<QString>& _userList);
    void getMessageList(const QString&  _firstUser,
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

private:
    QSqlDatabase mDatabase;
    QString      mPathToDatabase;
};

#endif // DATABASEMANAGER_H
