#include <QTcpSocket>
#include <QThread>
#include <QTcpServer>
#include <QObject>
#include <QtNetwork>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    std::function<void(qint32)> func;
    TcpSocket(std::function<void(qint32)> f):func(f){};
    TcpSocket(std::function<void(qint32)> f, qintptr socket):func(f){
        this->setSocketDescriptor(socket);
        connect(this, &TcpSocket::disconnected, [=]{
            this->disconnectFromHost();
            this->close();
            qDebug()<<"disconnected";
        });
    };
    QQueue<QByteArray> recvMsg;
    void ReceiveMsg(){
        auto t = this->readAll();
        recvMsg.push_back(t);
        // 有消息过来就要通知上层来读
        func(2);
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
        //this->waitForBytesWritten();
        return true; 
    }
};

class MsgThread : public QThread
{
    Q_OBJECT
public:
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
// 这个是TCP服务器开启监听,如果有新的客户端连进来就会创建一个套接字提供给上层
class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    std::function<void(TcpSocket*)> func;
    std::function<void(qint32)> cbfunc;
    // TcpServer(std::function<void(TcpSocket*)> f1, std::function<void(qint32)> f2):func(f1),cbfunc(f2){};
    TcpServer(std::function<void(TcpSocket*)> f1, std::function<void(qint32)> f2);
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
    bool SendMsg(QByteArray &msg);
    // 把消息压入要发送的队列
    bool PushMsg(QByteArray *msg);
    // 从模块的消息队列取一条消息
    bool PullMsg(QByteArray &msg);
    void onClientConnect(TcpSocket *socket);
    // 要发送的,已经序列化的数据
    QQueue<QString> msgQueue;
    // 当前程序为服务器的时候这里是监听的本地IP和端口号,当前程序为客户端的时候这里设置的是连接的服务器的IP
    AddrPort addrport;
};