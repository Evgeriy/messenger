#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "tcpclient_global.h"

#include <QTime>
#include <QDebug>
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>

class TCPCLIENTSHARED_EXPORT TCPClient : public QObject {
    Q_OBJECT
public:
    explicit TCPClient(QObject *_parent = nullptr);
    ~TCPClient();

public:
    void connectToServer(const QHostAddress &_hostAddress, const quint16 &_port);
    void connectToServer(const QString &_hostAddressString, const quint16 &_port);

public:
    void doRegistration(const QString &_nik,
                        const QString &_firstName,
                        const QString &_secondName,
                        const QString &_password);
    void doAuthorization(const QString &_nik, const QString &_password);

public:
    void loadUserList();
    void loadMessageList(const QString &_user);

public:
    void findUser(const QString &_user);
    void sendPM(const QString &_to, const QString &_msg);

public:
    void sendToServer(const QString& _msg);

signals:
    void sendUserProfileToGUI(QString, QString);
    void sendUserListToGUI(QList<QString>);
    void sendUserChatsToGUI(QString, QList<QString>);
    void sendMessageToGUI(QString, QString);

public slots:
    void onSendUserChatsToGUI(const QString &_user);

private:
    void receiveRegistrationStatus(QDataStream *stream);
    void receiveAuthorizationStatus(QDataStream *stream);
    void receiveUserList(QDataStream *stream);
    void receiveMessageList(QDataStream *stream);
    void receiveMessage(QDataStream *stream);

private slots:
    void onConnect();
    void onDisconnect();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError _error) const;

private:
    QString mNik;
    QString mFirstName;
    QString mSecondName;
    QString mPassword;

    QList<QString>                mFriends;
    QMap<QString, QList<QString>> mChats;

    QTcpSocket* pSocket;
    quint16     mNextBlockSize;
    bool        mIsAuthorized;
};

#endif // TCPCLIENT_H
