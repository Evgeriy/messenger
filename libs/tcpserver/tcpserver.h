#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "tcpserver_global.h"
#include "databasemanager.h"

#include <QTime>
#include <QDebug>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>
#include <QHostAddress>
#include <QMessageBox>

class TCPServer : public QObject {
    Q_OBJECT
public:
    explicit TCPServer(QObject *_parent = nullptr, const quint16 &_port = 4000);
    ~TCPServer();

signals:
    void sendMessageToDB(QString _userFrom,
                         QString _userTo,
                         QString _message,
                         QString _dateTime);

public slots:
    void onNewConnection();
    void onReadyRead();

public:
    void sendAuthorizationStatusToClient(QTcpSocket* _to,
                                         const bool& _status,
                                         const QString& _firstName,
                                         const QString& _secondName);
    void sendUserListToClient(QTcpSocket* _to, const QList<QString>& _userList);
    void sendMessageListToClient(QTcpSocket* _to,
                                 const QString& _from,
                                 const  QList<QString>& _messageList);
    void sendPMToClient(QTcpSocket* _to, const QString& _from, const QString& _message);

public:
    void receiveToDoAuthorization(QDataStream& _stream, QTcpSocket* _socket);
    void receiveToLoadUserList(QDataStream& _stream);
    void receiveToLoadMessageList(QDataStream& _stream);
    void receiveToSendPM(QDataStream& _stream);

private:
    QTcpServer*                pTcpServer;
    DatabaseManager*           pDBManager;
    QMap<QString, QTcpSocket*> mUserMap;
    quint16                    mNextBlockSize;
};

#endif // TCPSERVER_H
