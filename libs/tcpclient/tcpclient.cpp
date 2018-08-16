#include "tcpclient.h"

///
/// \brief TCPClient::TCPClient
/// \param _parent
///
TCPClient::TCPClient(QObject *_parent) :
    QObject(_parent),
    pSocket(nullptr),
    mNextBlockSize(0),
    mIsAuthorized(false) {

}

///
/// \brief TCPClient::~TCPClient
///
TCPClient::~TCPClient() {
    disconnect(pSocket, &QTcpSocket::connected, this, &TCPClient::onConnect);
    disconnect(pSocket, &QTcpSocket::disconnected, this, &TCPClient::onDisconnect);
    disconnect(pSocket, &QTcpSocket::readyRead, this, &TCPClient::onReadyRead);

    if (pSocket != nullptr) {
        delete pSocket;
        pSocket = nullptr;
    }
}

///
/// \brief TCPClient::connectToServer
/// \param _hostAddress
/// \param _port
///
void TCPClient::connectToServer(const QHostAddress &_hostAddress, const quint16 &_port) {
    pSocket = new QTcpSocket(this);
    pSocket->connectToHost(_hostAddress, _port);

    connect(pSocket, &QTcpSocket::connected, this, &TCPClient::onConnect);
    connect(pSocket, &QTcpSocket::disconnected, this, &TCPClient::onDisconnect);
    connect(pSocket, &QTcpSocket::readyRead, this, &TCPClient::onReadyRead);
}

///
/// \brief TCPClient::connectToServer
/// \param _hostAddressString
/// \param _port
///
void TCPClient::connectToServer(const QString &_hostAddressString, const quint16 &_port) {
    pSocket = new QTcpSocket(this);
    pSocket->connectToHost(_hostAddressString, _port);

    connect(pSocket, &QTcpSocket::connected, this, &TCPClient::onConnect);
    connect(pSocket, &QTcpSocket::disconnected, this, &TCPClient::onDisconnect);
    connect(pSocket, &QTcpSocket::readyRead, this, &TCPClient::onReadyRead);
    //connect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
}

///
/// \brief TCPClient::doRegistration
/// \param _nik
/// \param _firstName
/// \param _secondName
/// \param _password
///
void TCPClient::doRegistration(const QString &_nik, const QString &_firstName, const QString &_secondName, const QString &_password) {
    mNik = _nik;
    mFirstName = _firstName;
    mSecondName = _secondName;
    mPassword = _password;

    //TODO ...
}

///
/// \brief TCPClient::doAuthorization
/// \param _nik
/// \param _password
///
void TCPClient::doAuthorization(const QString &_nik, const QString &_password) {
    mNik = _nik;
    mPassword = _password;

    QByteArray arrBlock;
    QDataStream outputDataStream(&arrBlock, QIODevice::WriteOnly);
    outputDataStream.setVersion(QDataStream::Qt_5_11);

    outputDataStream << quint16(0) <<
                        static_cast<int>(COMMAND_DO_AUTHORIZATION) <<
                        mNik <<
                        mPassword;

    outputDataStream.device()->seek(0);
    outputDataStream << quint16(static_cast<quint16>(arrBlock.size()) - sizeof(quint16));

    pSocket->write(arrBlock);
}

///
/// \brief TCPClient::loadUserList
///
void TCPClient::loadUserList() {
    QByteArray arrBlock;
    QDataStream outputDataStream(&arrBlock, QIODevice::WriteOnly);
    outputDataStream.setVersion(QDataStream::Qt_5_11);

    outputDataStream << quint16(0) <<
                        static_cast<int>(COMMAND_LOAD_USER_LIST) <<
                        mNik;

    outputDataStream.device()->seek(0);
    outputDataStream << quint16(static_cast<quint16>(arrBlock.size()) - sizeof(quint16));

    pSocket->write(arrBlock);
}

///
/// \brief TCPClient::loadMessageList
/// \param _user
///
void TCPClient::loadMessageList(const QString &_user) {
    QByteArray arrBlock;
    QDataStream outputDataStream(&arrBlock, QIODevice::WriteOnly);
    outputDataStream.setVersion(QDataStream::Qt_5_11);

    outputDataStream << quint16(0) <<
                        static_cast<int>(COMMAND_LOAD_MESSAGE_LIST) <<
                        mNik <<
                        _user;

    outputDataStream.device()->seek(0);
    outputDataStream << quint16(static_cast<quint16>(arrBlock.size()) - sizeof(quint16));

    pSocket->write(arrBlock);
}

///
/// \brief TCPClient::sendPM
/// \param _to
/// \param _msg
///
void TCPClient::sendPM(const QString &_to, const QString &_msg) {
    QByteArray arrBlock;
    QDataStream outputDataStream(&arrBlock, QIODevice::WriteOnly);
    outputDataStream.setVersion(QDataStream::Qt_5_11);

    outputDataStream << quint16(0) <<
                        static_cast<int>(COMMAND_SEND_PM) <<
                        mNik <<
                        _to <<
                        _msg;

    outputDataStream.device()->seek(0);
    outputDataStream << quint16(static_cast<quint16>(arrBlock.size()) - sizeof(quint16));

    pSocket->write(arrBlock);
}

///
/// \brief TCPClient::onSendUserChatsToGUI
/// \param _user
///
void TCPClient::onSendUserChatsToGUI(const QString& _user) {
    loadMessageList(_user);
    emit sendUserChatsToGUI(_user, mChats[_user]);
}

///
/// \brief TCPClient::onConnect
///
void TCPClient::onConnect() {
    qDebug() << "Server IP:" << pSocket->localAddress().toString() <<
                "Client IP:" << pSocket->peerAddress().toString();
}

///
/// \brief TCPClient::onDisconnect
///
void TCPClient::onDisconnect() {

}

///
/// \brief TCPClient::onReadyRead
///
void TCPClient::onReadyRead() {
    QDataStream inputDataStream(pSocket);
    inputDataStream.setVersion(QDataStream::Qt_5_11);

    while (true) {
        if (!mNextBlockSize) {
            if (pSocket->bytesAvailable() < sizeof(qint16)) {
                break;
            }
            inputDataStream >> mNextBlockSize;
        }

        if (pSocket->bytesAvailable() < mNextBlockSize) {
            break;
        }

        mNextBlockSize = 0;

        int serverCommand;
        inputDataStream >> serverCommand;

        switch (serverCommand) {
        case static_cast<int>(COMMAND_SEND_AUTHORIZATION_STATUS):
            receiveAuthorizationStatus(inputDataStream);
            break;

        case static_cast<int>(COMMAND_SEND_USER_LIST):
            receiveUserList(inputDataStream);
            break;

        case static_cast<int>(COMMAND_SEND_MESSAGE_LIST):
            receiveMessageList(inputDataStream);
            break;

        case static_cast<int>(COMMAND_SEND_MESSAGE):
            receiveMessage(inputDataStream);
            break;
        }
    }
}

///
/// \brief TCPClient::receiveRegistrationStatus
/// \param _stream input data stream
///
void TCPClient::receiveRegistrationStatus(QDataStream &_stream) {
    // TODO
}

///
/// \brief TCPClient::receiveAuthorizationStatus
/// \param _stream input data stream
///
void TCPClient::receiveAuthorizationStatus(QDataStream &_stream) {
    _stream >> mIsAuthorized;

    if (mIsAuthorized) {
        _stream >> mFirstName;
        _stream >> mSecondName;
    }

    emit sendUserProfileToGUI(mFirstName, mSecondName);
}

///
/// \brief TCPClient::receiveUserList
/// \param _stream input data stream
///
void TCPClient::receiveUserList(QDataStream &_stream) {
    int listSize;
    QString currentUser = "";

    _stream >> listSize;

    mFriends.clear();
    for (size_t i = 0; i < listSize; ++i) {
        _stream >> currentUser;
        mFriends << currentUser;
    }

    emit sendUserListToGUI(mFriends);
}

///
/// \brief TCPClient::receiveMessageList
/// \param _stream input data stream
///
void TCPClient::receiveMessageList(QDataStream &_stream) {
    QString currentUser = "";
    QString currentMessage = "";
    int listSize = 0;

    _stream >> currentUser;
    _stream >> listSize;

    mChats[currentUser].clear();
    for (size_t i = 0; i < listSize; ++i) {
        _stream >> currentMessage;
        mChats[currentUser] << currentMessage;
    }

    emit sendUserChatsToGUI(currentUser, mChats[currentUser]);
}

///
/// \brief TCPClient::receiveMessage
/// \param _stream input data stream
///
void TCPClient::receiveMessage(QDataStream &_stream) {
    QString currentUser = "";
    QString currentMessage = "";

    _stream >> currentUser;
    _stream >> currentMessage;

    mChats[currentUser] << currentMessage;

    emit sendUserChatsToGUI(currentUser, mChats[currentUser]);
}

///
/// \brief TCPClient::onError
/// \param _error error
///
void TCPClient::onError(QAbstractSocket::SocketError _error) const {
    switch (_error) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "Error: The host was not found";
        break;
    case QAbstractSocket::ConnectionRefusedError:
         qDebug() << "Error: The connection was refused by the peer.";
        break;
    default:
         qDebug() << "Error:  The following error occurred:" << pSocket->errorString();
    }
}
