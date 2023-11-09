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
    // 这里有两个回调函数,一个是tcp服务器的,另外一个是tcp套接字的,这里没有区分agent,否则应该每个不同的用户分配不同的回调函数,
    // 用同一个套接字也要区分回是哪个用户的
    m_tcpServer = new TcpServer([&](TcpSocket *socket){
        onClientConnect(socket);
    }, cbfunc);
    //监听
    m_tcpServer->listen(QHostAddress(addrport.addr), addrport.port);
    servType = 1;
    return true;
}

void ChessNetwork::onClientConnect(TcpSocket *socket)
{
    m_tcpSocket = socket;
    cbfunc(EVENT_TYPE::NEW_CLIENT_CONNECT);
}

bool ChessNetwork::InitClient()
{
    m_tcpSocket = new TcpSocket(cbfunc);
    m_tcpSocket->connectToHost(addrport.addr, addrport.port);
    MsgThread *thread = new MsgThread(m_tcpSocket);
    thread->start();
    servType = 2;
    return true;
}

bool ChessNetwork::SendMsg(QByteArray &msg)
{
    if(m_tcpSocket == nullptr) return false;
    m_tcpSocket->SendMsg(msg);
    return true;
}

bool ChessNetwork::PushMsg(QByteArray *msg)
{
    return true;
}

bool ChessNetwork::PullMsg(QByteArray &msg)
{
    if(m_tcpSocket == nullptr) return false;
    m_tcpSocket->GetMsg(msg);
    qDebug()<<"pull msg socket desc: "<<m_tcpSocket->socketDescriptor();
    return true;
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"incomingConnection...";
    // 传入新连接的套接字的回调函数是最上层来的
    TcpSocket *socket = new TcpSocket(cbfunc, socketDescriptor);   
    func(socket);
    MsgThread *thread = new MsgThread(socket);
    thread->start();
    connect(thread, &MsgThread::finished, thread, &MsgThread::deleteLater);
}

TcpServer::TcpServer(std::function<void(TcpSocket*)> f1, std::function<void(qint32)> f2){
    func = f1;
    cbfunc = f2;
}