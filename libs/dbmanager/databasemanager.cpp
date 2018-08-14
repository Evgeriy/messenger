#include "databasemanager.h"

#include <QDate>
#include <QDebug>
#include <QFile>
#include <QSql>
#include <QSqlError>
#include <QSqlQuery>

///
/// \brief DatabaseManager::DatabaseManager - constructor
/// \param _parent - parent QObject object
/// \param _pathToDatabase - path to file *.db
///
DatabaseManager::DatabaseManager(QObject *_parent, const QString &_pathToDatabase) :
    QObject(_parent),
    mPathToDatabase(_pathToDatabase) {

}

///
/// \brief DatabaseManager::~DatabaseManager - destructor
///
DatabaseManager::~DatabaseManager() {

}

///
/// \brief DatabaseManager::initialize - initialize QSqlDatabase object
/// \return
///
bool DatabaseManager::initialize() {
    mDatabase = QSqlDatabase::addDatabase(DB_TYPE);
    mDatabase.setHostName(DB_HOSTNAME);
    mDatabase.setDatabaseName(mPathToDatabase + DB_NAME);

    return mDatabase.open();
}

///
/// \brief DatabaseManager::open - open database
/// \return
///
bool DatabaseManager::open() {
    bool retValue = false;

    if (QFile(mPathToDatabase + DB_NAME).exists()) {
        retValue = initialize();
    } else {
        retValue = initialize() && create();
    }

    return retValue;
}

///
/// \brief DatabaseManager::create - create new database
/// \return
///
bool DatabaseManager::create() {
    return createUserTable() && createPrivateMessageChatTable() && createMessageTable();
}

///
/// \brief DatabaseManager::createUserTable - create user table
/// \return
///
bool DatabaseManager::createUserTable() {
    bool retValue = false;

    QSqlQuery query;
    QString queryString = "CREATE TABLE IF NOT EXISTS " USER_TABLE_NAME
                          " (" USER_TABLE_ID " INTEGER PRIMARY KEY AUTOINCREMENT, "
                               USER_TABLE_NIK " VARCHAR(255) NOT NULL, "
                               USER_TABLE_FIRST_NAME " VARCHAR(255) NOT NULL, "
                               USER_TABLE_SECOND_NAME " VARCHAR(255) NOT NULL, "
                               USER_TABLE_PASSWORD " VARCHAR(255) NOT NULL)";

    if (!query.exec(queryString)) {
        qDebug() << "DataBase: error of create" <<
                    USER_TABLE_NAME <<
                    query.lastError().text();
        retValue = false;
    } else {
        retValue = true;
    }

    return retValue;
}

///
/// \brief DatabaseManager::createPrivateMessageChatTable - create PM Chat table
/// \return
///
bool DatabaseManager::createPrivateMessageChatTable() {
    bool retValue = false;
    QSqlQuery query;
    QString queryString = "CREATE TABLE IF NOT EXISTS " PRIVATE_MESSAGE_CHAT_TABLE_NAME
                          " (" PRIVATE_MESSAGE_CHAT_TABLE_ID " INTEGER PRIMARY KEY AUTOINCREMENT, "
                               PRIVATE_MESSAGE_CHAT_TABLE_FIRST_USER_ID " INTEGER NOT NULL, "
                               PRIVATE_MESSAGE_CHAT_TABLE_SECOND_USER_ID " INTEGER NOT NULL, "
                          "FOREIGN KEY (" PRIVATE_MESSAGE_CHAT_TABLE_FIRST_USER_ID  ") REFERENCES "
                                          USER_TABLE_NAME " (" USER_TABLE_ID ") ,"
                          "FOREIGN KEY (" PRIVATE_MESSAGE_CHAT_TABLE_SECOND_USER_ID ") REFERENCES "
                                          USER_TABLE_NAME " (" USER_TABLE_ID ") )";

    if (!query.exec(queryString)) {
        qDebug() << "DataBase: error of create" <<
                    PRIVATE_MESSAGE_CHAT_TABLE_NAME <<
                    query.lastError().text();
        retValue = false;
    } else {
        retValue = true;
    }

    return retValue;
}

///
/// \brief DatabaseManager::createMessageTable - create Message table
/// \return
///
bool DatabaseManager::createMessageTable() {
    bool retValue = false;

    QSqlQuery query;
    QString queryString = "CREATE TABLE IF NOT EXISTS " MESSAGE_TABLE_NAME
                          " ( " MESSAGE_TABLE_ID " INTEGER PRIMARY KEY AUTOINCREMENT, "
                                MESSAGE_TABLE_PM_CHAT_ID " INTEGER NOT NULL, "
                                MESSAGE_TABLE_AUTHOR_ID " INTEGER NOT NULL, "
                                MESSAGE_TABLE_CONTENT " TEXT, "
                                MESSAGE_TABLE_DATE " TEXT, "
                          "FOREIGN KEY (" MESSAGE_TABLE_PM_CHAT_ID ") REFERENCES "
                          PRIVATE_MESSAGE_CHAT_TABLE_NAME " (" PRIVATE_MESSAGE_CHAT_TABLE_ID "), "
                          "FOREIGN KEY (" MESSAGE_TABLE_AUTHOR_ID  ") REFERENCES "
                          USER_TABLE_NAME " (" USER_TABLE_ID ") )";

    if (!query.exec(queryString)) {
        qDebug() << "DataBase: error of create " <<
                    MESSAGE_TABLE_NAME <<
                    query.lastError().text();
        retValue = false;
    } else {
        retValue = true;
    }

    return retValue;
}

///
/// \brief DatabaseManager::checkAuthorization
/// \param _nik        - user nik
/// \param _password   - user password
/// \param _firstName  - returned first name if found that user
/// \param _secondName - returned first name if found that user
/// \return
///
bool DatabaseManager::checkAuthorization(const QString& _nik,
                                         const QString& _password,
                                         QString&       _firstName,
                                         QString&       _secondName) {
    bool retValue = false;

    QSqlQuery query;
    QString queryString = "SELECT " USER_TABLE_FIRST_NAME  ", "
                                    USER_TABLE_SECOND_NAME
                          " FROM "   USER_TABLE_NAME
                          " WHERE "  USER_TABLE_NIK        " = '" + _nik +      "'" + " AND "
                                    USER_TABLE_PASSWORD    " = '" + _password + "'";

    query.exec(queryString);
    if (query.next()) {
        _firstName  = query.value(0).toString();
        _secondName = query.value(1).toString();
        retValue    = true;
    }

    return retValue;
}

///
/// \brief DatabaseManager::getUserList get list of all user (friends)
/// \param _nik      - user nik
/// \param _userList - returned user list
///
void DatabaseManager::getUserList(const QString& _nik, QList<QString>& _userList) {
    QSqlQuery query;
    QString queryString = "SELECT "  USER_TABLE_NIK
                          " FROM "   USER_TABLE_NAME
                          " WHERE "  USER_TABLE_NIK  " != '" + _nik + "'";

    query.exec(queryString);
    while (query.next()) {
        _userList << query.value(0).toString();
    }
}

///
/// \brief DatabaseManager::getMessageList get list of all messages between two users
/// \param _firstUser   - first user nik
/// \param _secondUser  - second user nik
/// \param _messageList - returned message list
///
void DatabaseManager::getMessageList(const QString& _firstUser,
                                     const QString& _secondUser,
                                     QList<QString>& _messageList) {

    int pmChatId = getPMChatIdByNiks(_firstUser, _secondUser); // get pm chat id between users
    int firstUserId = getUserIdByNik(_firstUser);              // get user id by first nik

    QSqlQuery query;
    QString queryString = "SELECT " MESSAGE_TABLE_AUTHOR_ID ", "
                                    MESSAGE_TABLE_CONTENT   ", "
                                    MESSAGE_TABLE_DATE
                          " FROM "  MESSAGE_TABLE_NAME
                          " WHERE " MESSAGE_TABLE_PM_CHAT_ID " = " + QString::number(pmChatId);


    query.exec(queryString);
    while (query.next()) {
        int     authorId = query.value(0).toInt();
        QString author   = (authorId == firstUserId) ? _firstUser : _secondUser;
        QString content  = query.value(1).toString();
        QString date     = query.value(2).toString();

        _messageList << QString("[" + date + " " + author + "]\t\t" + content);
    }
}

///
/// \brief DatabaseManager::insertUser - insert new user to user table
/// \param _userNik
/// \param _userFirstName
/// \param _userSecondName
/// \param _userPassword
/// \return
///
bool DatabaseManager::insertUser(const QString& _userNik,
                                 const QString& _userFirstName,
                                 const QString& _userSecondName,
                                 const QString& _userPassword) {
    bool retValue = false;

    QSqlQuery query;
    QString queryString = "INSERT INTO " USER_TABLE_NAME
                                    " (" USER_TABLE_NIK         ", "
                                         USER_TABLE_FIRST_NAME  ", "
                                         USER_TABLE_SECOND_NAME ", "
                                         USER_TABLE_PASSWORD    ") "
                           "VALUES (:Nik, :FirstName, :SecondName, :Password)";

    query.prepare(queryString);
    query.bindValue(":Nik",        _userNik);
    query.bindValue(":FirstName",  _userFirstName);
    query.bindValue(":SecondName", _userSecondName);
    query.bindValue(":Password",   _userPassword);

    if (!query.exec()) {
        qDebug() << "DataBase: error of inserting into" <<
                    USER_TABLE_NAME <<
                    query.lastError().text();
        retValue = false;
    } else {
        retValue = true;
    }

    return retValue;
}

///
/// \brief DatabaseManager::insertPMChat - insert new pm chat row between two users
/// \param _firstUserNik
/// \param _secondUserNik
/// \return
///
bool DatabaseManager::insertPMChat(const QString& _firstUserNik, const QString& _secondUserNik) {
    bool retValue = false;

    int firstUserId  = getUserIdByNik(_firstUserNik);
    int secondUserId = getUserIdByNik(_secondUserNik);

    // if user found
    if (firstUserId > 0 && secondUserId > 0) {
        // if pm chat between these users was not create before
        int pmChatId = getPMChatIdByNiks(_firstUserNik, _secondUserNik);
        if (pmChatId == -1) {
            QSqlQuery query;
            QString queryString = "INSERT INTO " PRIVATE_MESSAGE_CHAT_TABLE_NAME
                                            " (" PRIVATE_MESSAGE_CHAT_TABLE_FIRST_USER_ID   ", "
                                                 PRIVATE_MESSAGE_CHAT_TABLE_SECOND_USER_ID  ") "
                                  "VALUES (:FirstUserId, :SecondUserId)";

            query.prepare(queryString);
            query.bindValue(":FirstUserId",  firstUserId);
            query.bindValue(":SecondUserId", secondUserId);
            query.exec();
            retValue = true;
        }
    }

    return retValue;
}

///
/// \brief DatabaseManager::insertMessage - insert new message
/// \param _firstUserNik
/// \param _secondUserNik
/// \param _content
/// \param _date
/// \return
///
bool DatabaseManager::insertMessage(const QString& _firstUserNik,
                                    const QString& _secondUserNik,
                                    const QString& _content,
                                    const QString& _date) {
    bool retValue = false;

    int authorId = getUserIdByNik(_firstUserNik);
    int pmChatId = getPMChatIdByNiks(_firstUserNik, _secondUserNik);

    if (authorId > 0 && pmChatId > 0) {
        QSqlQuery query;
        QString queryString = "INSERT INTO " MESSAGE_TABLE_NAME
                                        " (" MESSAGE_TABLE_PM_CHAT_ID ", "
                                             MESSAGE_TABLE_AUTHOR_ID  ", "
                                             MESSAGE_TABLE_CONTENT    ", "
                                             MESSAGE_TABLE_DATE       ") "
                              "VALUES (:ChatId, :AuthorId, :Content, :Date)";
        query.prepare(queryString);
        query.bindValue(":ChatId",   pmChatId);
        query.bindValue(":AuthorId", authorId);
        query.bindValue(":Content",  _content);
        query.bindValue(":Date",     _date);

        if (!query.exec()) {
            qDebug() << "DataBase: error of inserting into" <<
                        MESSAGE_TABLE_NAME <<
                        query.lastError().text();
            retValue = false;
        } else {
            retValue = true;
        }
    }

    return retValue;
}

///
/// \brief DatabaseManager::getUserIdByNik
/// \param _nik
/// \return
///
int DatabaseManager::getUserIdByNik(const QString& _nik) {
    int retValue = -1;

    QSqlQuery query;
    QString queryString = "SELECT " USER_TABLE_ID
                          " FROM "  USER_TABLE_NAME
                          " WHERE " USER_TABLE_NIK " = '" + _nik + "'";

    query.exec(queryString);
    if (query.next()) {
        retValue = query.value(0).toInt();
    }

    return retValue;
}

///
/// \brief DatabaseManager::getPMChatIdByNiks
/// \param _firstNik
/// \param _secondNik
/// \return
///
int DatabaseManager::getPMChatIdByNiks(const QString& _firstNik, const QString& _secondNik) {
    int retValue = -1;

    int firstUserId  = getUserIdByNik(_firstNik);
    int secondUserId = getUserIdByNik(_secondNik);

    QSqlQuery query;
    QString queryString =
    "SELECT "  PRIVATE_MESSAGE_CHAT_TABLE_ID " FROM " PRIVATE_MESSAGE_CHAT_TABLE_NAME
    " WHERE (" PRIVATE_MESSAGE_CHAT_TABLE_FIRST_USER_ID  " = " + QString::number(firstUserId)  +
        " OR " PRIVATE_MESSAGE_CHAT_TABLE_FIRST_USER_ID  " = " + QString::number(secondUserId) +
     ") AND (" PRIVATE_MESSAGE_CHAT_TABLE_SECOND_USER_ID " = " + QString::number(firstUserId)  +
        " OR " PRIVATE_MESSAGE_CHAT_TABLE_SECOND_USER_ID " = " + QString::number(secondUserId) + ")";

    query.exec(queryString);
    if (query.next()) {
        retValue = query.value(0).toInt();
    }

    return retValue;
}
