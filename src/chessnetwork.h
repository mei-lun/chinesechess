#include <QTcpSocket>
#include <QThread>
#include <QTcpServer>
#include <QObject>
#include <QtNetwork>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket(){};
    TcpSocket(qintptr socket){
        this->setSocketDescriptor(socket);
        connect(this, &TcpSocket::disconnected, [=]{
            this->disconnectFromHost();
            this->close();
            qDebug()<<"disconnected";
        });
    };
    QQueue<QByteArray> recvMsg;
    void ReceiveMsg(){
        recvMsg.push_back(this->readAll());
        // qDebug()<<"form server: "<<recvMsg.back().data();
    }
    void GetMsg(QByteArray &msg){
        if(recvMsg.empty()){
            msg = nullptr;
            return;
        }
        msg = recvMsg.front();
        recvMsg.pop_front();
    }
    bool SendMsg(QByteArray msg){
       this->write(msg);
       // 等待数据全部写完
       this->waitForBytesWritten();
       return true; 
    }
};

class MsgThread : public QThread
{
    Q_OBJECT
public:
    MsgThread(qintptr skptr):ptr(skptr){
        socket = new TcpSocket(ptr);
    }
    MsgThread(TcpSocket* tcpSocket):socket(tcpSocket){}
private:
    qintptr ptr;
    TcpSocket *socket;
    virtual void run(){
        qDebug()<<"run....";
        connect(socket, &TcpSocket::readyRead, socket, &TcpSocket::ReceiveMsg);
        connect(socket, &TcpSocket::disconnected, this, &MsgThread::deal_quit, Qt::DirectConnection);
    }
signals:
public slots:
    void deal_quit(){
        this->quit();
    }
};

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    std::function<void(TcpSocket*)> func;
    TcpServer(std::function<void(TcpSocket*)> f):func(f){};
    virtual void incomingConnection(qintptr socketDescriptor);
};

struct AddrPort
{
    QString addr;
    qint32 port;
};

class ChessNetwork{
public:
    ChessNetwork(AddrPort ap, std::function<void(qint32)> cb):addrport(ap), cbfunc(cb){};
    TcpServer *m_tcpServer = nullptr;
    TcpSocket *m_tcpSocket = nullptr;
    // 回调到上一层的函数
    std::function<void(qint32)> cbfunc;
    // 当前创建的网络服务类型,0是初始状态,1是服务器,2是客户端
    qint32 servType = 0;
    // 初始化服务器的IP和端口号
    void InitSocket(QString ip, qint32 port);
    // 如果是服务器就初始化这个
    bool InitServer();
    // 如果是客户端就初始化这个
    bool InitClient();
    // 发送数据
    bool SendMsg();
    // 把消息压入要发送的队列
    bool PushMsg(QString *msg);
    // 从模块的消息队列取一条消息
    bool PullMsg(QString &msg);
    void onClientConnect(TcpSocket *socket);
    // 要发送的,已经序列化的数据
    QQueue<QString> msgQueue;
    // 当前程序为服务器的时候这里是监听的本地IP和端口号,当前程序为客户端的时候这里设置的是连接的服务器的IP
    AddrPort addrport;
};