#include "tcpserver.h"

///
/// \brief TCPServer::TCPServer
/// \param _parent
/// \param _port
///
TCPServer::TCPServer(QObject *_parent, const quint16 &_port) :
    QObject(_parent),
    pTcpServer(nullptr),
    pDBManager(nullptr),
    mNextBlockSize(0) {

    pTcpServer = new QTcpServer(this);
    if (!pTcpServer->listen(QHostAddress::Any, _port)) {
        QString title = "Server Critical Error";
        QString text  = "Unable to start the server:" + pTcpServer->errorString();
        QMessageBox::critical(nullptr, title, text);
        pTcpServer->close();
        return;
    }

    pDBManager = new DatabaseManager(this,  "../../../resources/");
    pDBManager->open();

    connect(pTcpServer, &QTcpServer::newConnection, this, &TCPServer::onNewConnection);
    connect(this, &TCPServer::sendMessageToDB, pDBManager, &DatabaseManager::insertMessage);
}

///
/// \brief TCPServer::~TCPServer
///
TCPServer::~TCPServer() {

}

///
/// \brief TCPServer::onNewConnection
///
void TCPServer::onNewConnection() {
    QTcpSocket* clientSocket = pTcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    connect(clientSocket, &QTcpSocket::readyRead, this, &TCPServer::onReadyRead);

    qDebug() << clientSocket->peerAddress().toString();
}

///
/// \brief TCPServer::onReadyRead
///
void TCPServer::onReadyRead() {
    QTcpSocket* clientSocket = static_cast<QTcpSocket*>(sender());
    QDataStream inputDataStream(clientSocket);
    inputDataStream.setVersion(QDataStream::Qt_5_11);

    while (true) {
        if (!mNextBlockSize) {
            if (clientSocket->bytesAvailable() < sizeof(qint16)) {
                break;
            }
            inputDataStream >> mNextBlockSize;
        }

        if (clientSocket->bytesAvailable() < mNextBlockSize) {
            break;
        }

        mNextBlockSize = 0;

        int clientCommand;
        inputDataStream >> clientCommand;

        QString from;
        QString password;
        QString to;
        QString msg;
        QString firstName;
        QString secondName;
        bool isAuth;
        QList<QString> userList;
        QList<QString> messageList;
        QString        message;

        switch (clientCommand) {
        case COMMAND_DO_AUTHORIZATION:
            inputDataStream >> from;
            inputDataStream >> password;

            isAuth = pDBManager->checkAuthorization(from, password, firstName, secondName);
            mUserMap[from] = clientSocket;
            sendAuthorizationStatusToClient(mUserMap[from], isAuth, firstName, secondName);

            break;

        case COMMAND_LOAD_USER_LIST:
            inputDataStream >> from;

            pDBManager->getUserList(from, userList);
            sendUserListToClient(mUserMap[from], userList);

            break;

        case COMMAND_LOAD_MESSAGE_LIST:
            inputDataStream >> from;
            inputDataStream >> to;

            pDBManager->getMessageList(from, to, messageList);
            sendMessageListToClient(mUserMap[from], to, messageList);

            break;

        case COMMAND_SEND_PM:
            inputDataStream >> from;
            inputDataStream >> to;
            inputDataStream >> msg;

            QString dateTime = QDateTime::currentDateTime().toString();
            QString sended = "[" + dateTime + " " + from + "]\t\t" + msg;

            if (mUserMap.contains(to)) {
                sendPMToClient(mUserMap[to], from, sended);
            }

            if (mUserMap.contains(from)) {
                sendPMToClient(mUserMap[from], to, sended);
            }

            emit sendMessageToDB(from, to, msg, dateTime);

            break;
        }
    }
}

///
/// \brief TCPServer::sendAuthorizationStatusToClient
/// \param _to
/// \param _status
/// \param _firstName
/// \param _secondName
///
void TCPServer::sendAuthorizationStatusToClient(QTcpSocket *_to, const bool &_status, const QString &_firstName, const QString &_secondName) {
    QByteArray arrBlock;
    QDataStream outputDataStream(&arrBlock, QIODevice::WriteOnly);
    outputDataStream.setVersion(QDataStream::Qt_5_11);

    outputDataStream << quint16(0) << static_cast<int>(COMMAND_SEND_AUTHORIZATION_STATUS) << _status << _firstName << _secondName;

    outputDataStream.device()->seek(0);
    outputDataStream << quint16(static_cast<quint16>(arrBlock.size()) - sizeof(quint16));

    _to->write(arrBlock);
}

///
/// \brief TCPServer::sendUserListToClient
/// \param _to
/// \param _userList
///
void TCPServer::sendUserListToClient(QTcpSocket *_to, const QList<QString> &_userList) {
    QByteArray arrBlock;
    QDataStream outputDataStream(&arrBlock, QIODevice::WriteOnly);
    outputDataStream.setVersion(QDataStream::Qt_5_11);

    outputDataStream << quint16(0) << static_cast<int>(COMMAND_SEND_USER_LIST) << _userList.size();
    for (int i = 0; i < static_cast<int>(_userList.size()); ++i) {
        outputDataStream << _userList[i];
    }

    outputDataStream.device()->seek(0);
    outputDataStream << quint16(static_cast<quint16>(arrBlock.size()) - sizeof(quint16));

    _to->write(arrBlock);
}

///
/// \brief TCPServer::sendMessageListToClient
/// \param _to
/// \param _from
/// \param _messageList
///
void TCPServer::sendMessageListToClient(QTcpSocket *_to, const QString &_from, const QList<QString> &_messageList) {
    QByteArray arrBlock;
    QDataStream outputDataStream(&arrBlock, QIODevice::WriteOnly);
    outputDataStream.setVersion(QDataStream::Qt_5_11);

    outputDataStream << quint16(0) << static_cast<int>(COMMAND_SEND_MESSAGE_LIST) << _from << _messageList.size();
    for (int i = 0; i < static_cast<int>(_messageList.size()); ++i) {
        outputDataStream << _messageList[i];
    }

    outputDataStream.device()->seek(0);
    outputDataStream << quint16(static_cast<quint16>(arrBlock.size()) - sizeof(quint16));

    _to->write(arrBlock);
}

///
/// \brief TCPServer::sendPMToClient
/// \param _to
/// \param _from
/// \param _message
///
void TCPServer::sendPMToClient(QTcpSocket *_to, const QString &_from, const QString &_message) {
    QByteArray arrBlock;
    QDataStream outputDataStream(&arrBlock, QIODevice::WriteOnly);
    outputDataStream.setVersion(QDataStream::Qt_5_11);

    outputDataStream << quint16(0) << static_cast<int>(COMMAND_SEND_MESSAGE) << _from << _message;

    outputDataStream.device()->seek(0);
    outputDataStream << quint16(static_cast<quint16>(arrBlock.size()) - sizeof(quint16));

    _to->write(arrBlock);
}

