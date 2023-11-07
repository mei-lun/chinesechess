#include "chessnetwork.h"

void ChessNetwork::InitSocket(QString ip, qint32 port)
{
    addrport.addr = ip;
    addrport.port = port;
}

bool ChessNetwork::InitServer()
{
    //本地主机名
    QString hostName = QHostInfo::localHostName();
    //本机IP地址
    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    m_tcpServer = new TcpServer([&](TcpSocket *socket){
        onClientConnect(socket);
    });
    //监听
    m_tcpServer->listen(QHostAddress(addrport.addr), addrport.port);
    servType = 1;
    return true;
}

void ChessNetwork::onClientConnect(TcpSocket *socket)
{
    m_tcpSocket = socket;
    cbfunc(1);
}

bool ChessNetwork::InitClient()
{
    m_tcpSocket = new TcpSocket();
    m_tcpSocket->connectToHost(addrport.addr, addrport.port);
    MsgThread *thread = new MsgThread(m_tcpSocket);
    thread->start();
    servType = 2;
    return true;
}

bool ChessNetwork::SendMsg()
{
    if(m_tcpSocket == nullptr) return false;
    m_tcpSocket->SendMsg(("hello server: " + QString::number(servType)).toUtf8());
    return true;
}

bool ChessNetwork::PushMsg(QString *msg)
{
    return true;
}

bool ChessNetwork::PullMsg(QString &msg)
{
    if(m_tcpSocket == nullptr) return false;
    QByteArray qbmsg;
    m_tcpSocket->GetMsg(qbmsg);
    msg = QString(qbmsg);
    return true;
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"incomingConnection...";
    TcpSocket *socket = new TcpSocket(socketDescriptor);   
    func(socket);
    MsgThread *thread = new MsgThread(socket); 
    thread->start();
    connect(thread, &MsgThread::finished, thread, &MsgThread::deleteLater);
}