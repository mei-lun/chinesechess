#include "chinesechess.h"
#include "chessmoverule.h"
#include <QTimer>

chinesechess::chinesechess(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_chinesechess)
{
    ui->setupUi(this);
    AddWindowModule();  // 添加窗体控件
    InitEnv();
    DrawChessBoard();
    SetWindowStyle();   // 设置窗体属性
///////////////////////////////////////////////
    // timer.setInterval(2000);
    // QObject::connect(&timer, &QTimer::timeout,[&](){
    //     if(chessNetwork){
    //         chessNetwork->SendMsg();
    //         QString msg;
    //         chessNetwork->PullMsg(msg);
    //         if(msg != nullptr) qDebug()<<"top: "<<msg;
    //     }
    // });
    // timer.start();
}

chinesechess::~chinesechess()
{
    delete ui; 
}

QPixmap GetStdPixmap(QString mPath)
{
    QPixmap pixmap(mPath);
    QBitmap mask = pixmap.createMaskFromColor(QColor(0, 255, 0), Qt::MaskInColor);
    pixmap.setMask(mask);
    return pixmap;
}

void chinesechess::InitEnv()
{
    // 初始化棋盘
    chessBoard = new QLabel(this);
    chessBoard->setPixmap(QPixmap(GET_RES("BOARD")));
    chessBoard->setGeometry(0, menuHeight, BOARD_W, BOARD_H);

    // 初始化棋子
    chessPieces = new chessBoardLabel(this, &pieceMp);
    chessPieces->setGeometry(0, menuHeight, BOARD_W, BOARD_H);

    Piece* piece = new Piece();
    // piece->pixmap = GetStdPixmap(":/images/SELECTED.BMP");
    piece->pixmapPath = ":/images/SELECTED.BMP";
    piece->status = 0;
    piece->id = -1;
    pieceMp.insert(piece->id, piece);

    // 开局由谁先走就先push一个对方的空走法进去,避免后续判空
    mStep.push_back(new PieceMoveStep(QPoint(0, 0), QPoint(0, 0), (BEGIN_ROLE<<1) * 10));

    QSettings settings("./ChessNetwork.ini", QSettings::IniFormat);
    QString addr = settings.value("addr").toString();
    qint32 port = settings.value("port").toInt();
    if(addr == nullptr || !port){
        settings.setValue("addr", "127.0.0.1");
        settings.setValue("port", 58830);
    }
    addrport.addr = settings.value("addr").toString();
    addrport.port =  settings.value("port").toInt();
}

void chinesechess::SelectPiece(qint32 x, qint32 y)
{
    qDebug()<<"tick left "<<x<<" "<<y;
    if(chess_board[x][y])
    {
        Piece* p = pieceMp[SELECT_MAP_ID];
        p->pos = QPoint(A2GY(y), A2GX(x));
        p->status = 1;
    }
}
/*
*@by @bx 这一步棋,棋盘上的目标位置
*@aby @abx 这一步棋棋盘的起始位置
*@forceMove 是否强制移动
*这两个坐标都是图上坐标,需要反过来转换成数组坐标
*/
bool chinesechess::TryMovePiece(qint32 abx, qint32 aby, qint32 bx, qint32 by, qint32 forceMove = false)
{
    // 当我们尝试移动的时候,先判断这个移动是不是合法的,如果是合法的才能尝试移动
    // 如果是强制移动则不需要检查这个
    if(!checkMove(abx, aby, bx, by) && !forceMove){
        updateTitleTips("非法移动");
        return false;
    }
    auto move2tar = [&](qint32 bx, qint32 by, qint32 ex, qint32 ey, bool fm, qint32 status = 1, qint32 pieceNum = 0){
        Piece* bp = pieceMp[G2APOS(bx, by)];
        bp->id = G2APOS(ex, ey);
        bp->status = status;
        bp->pos = QPoint(A2GX(ey), A2GY(ex));
        // 使用swap交换的时候产生的空位置引用,被认为是一个长度
        qSwap(pieceMp[G2APOS(bx, by)], pieceMp[G2APOS(ex, ey)]);
        if(!pieceMp[G2APOS(bx, by)]){
            // 这里不清除会造成内存泄漏
            pieceMp.remove(G2APOS(bx, by));
        }
        // 如果是吃棋
        if(status != 1){
            // 把起始位置让出来,死亡的棋子已经被映射到pieceMp中,棋盘不需要显示他
            chess_board[bx][by] = 0;
        }else{// 如果是不吃子的走棋
            // 不吃子的走棋分为两种
            // 1.普通走棋直接交换起始和终点位置
            // 2.如果是强制走棋,就需要把死亡位置的棋子再映射回来
            if(!fm){
                qSwap(chess_board[bx][by], chess_board[ex][ey]);
            }else{
                bx = (qint32)(bx/DIE_PIECE_NUM), by = (qint32)(by/DIE_PIECE_NUM);
                chess_board[bx][by] = pieceNum;
            }
        } 
    };
    // 目标位置有棋子
    if(chess_board[bx][by])
    {
        // 任何时候目标位置都不应该有同颜色棋子, 除非强制移动
        if((qint32)(chess_board[bx][by] / 10) == (qint32)(chess_board[abx][aby] / 10) && !forceMove){
            updateTitleTips("非法移动");
            return false;
        }
        move2tar(bx, by, bx * DIE_PIECE_NUM, by * DIE_PIECE_NUM, false, 2);
    }
    move2tar(abx, aby, bx, by, false);
    // 如果是强制移动需要把死亡的棋子再移回自己本来的位置
    if(forceMove && pieceMp.contains(G2APOS(abx * DIE_PIECE_NUM, aby * DIE_PIECE_NUM))){
        qint32 cid = pieceMp[G2APOS(abx * DIE_PIECE_NUM, aby * DIE_PIECE_NUM)]->cid;
        move2tar(abx * DIE_PIECE_NUM, aby * DIE_PIECE_NUM, abx, aby, true, 1, cid);
    }
    return true;
}

/*
移动棋子的时候, 如果目标位置没有棋子那么直接赋值
如果目标位置有棋子那么首先把目标位置的棋子置为死亡,然后再对目标位置赋值
*/
bool chinesechess::MovePiece(qint32 abx, qint32 aby, qint32 x, qint32 y, qint32 &tc)
{
    // 棋盘上的i,j对应棋盘的x, y
    qint32 bx = x, by = y;
    // 记录被什么棋子将军了
    qDebug()<<"begin self is safe? "<<checkKingSafe(chess_board[abx][aby], tc, false)<<" "<<tc;
    qDebug()<<"end self is safe? "<<checkKingSafe(chess_board[x][y], tc, false)<<" "<<tc;
    // 每次走棋的时候要检查自己是否被将军,如果自己被将军了必须得应将
    if(!checkKingSafe(chess_board[abx][aby], tc, false)){
        updateTitleTips("被将军了,必须应将");
        return false;
    }
    // 之前已经移动了
    // if(!TryMovePiece(abx, aby, bx, by)){
    //     qDebug()<<"移动失败";
    //     return false;
    // }
    // 检查是否将对方军
    if(!checkKingSafe(chess_board[bx][by], tc, true)) updateTitleTips("对方被将军了!!!");
    return true;
}

// 回退到上一步
void chinesechess::BackLastStep()
{
    auto lastStep = mStep.back();
    // 强制回退, 这里有两种情况
    // 1.目标点没有棋子,则只需要回退自己(我被将军,我没有解将)
    // 2.目标点有棋子,我吃了别人的棋子,需要把我走的那步退回,并把吃掉的棋子还原回本来的位置
    TryMovePiece(lastStep->endPos.x(), lastStep->endPos.y(), lastStep->beginPos.x(), lastStep->beginPos.y(), true);
    // 走棋回退
    mStep.pop_back();
    chessPieces->update();
}

// 这个函数只接收翻转之后变成默认的坐标, 返回值表示是否需要发送这一次鼠标事件的数据
bool chinesechess::SelectMoveEvent(qint32 px, qint32 py)
{     
    qint32 x = G2AX(px); qint32 y = G2AY(py);
    qSwap(x, y);
    qDebug()<< "moveP: x"<< x <<" y:"<< y;
    qint32 curRole = 0;
    auto curRound = [&](qint32 cx, qint32 cy)->bool{
        // 当玩家走棋的时候判断现在是不是他的回合,如果不是他的回合则不能操作
        curRole = GETPIECECOLOR(cx, cy); // 当前走棋的角色是谁
        if(!mStep.isEmpty()){
            // 当前走棋的人必须要和顶部记录的角色不同否则不是他的回合
            auto lastStep = mStep.back();
            // 判断现在该哪个颜色的走棋,如果要区分角色,那么就要判断当前网络角色是什么颜色的
            qint32 lastRole = (qint32)(lastStep->chessNum/10);
            // 当前走棋的人不可一次走两步,同一个角色也不能同时走两步
            if(curRole == lastRole || (curNetRole > 0 && curNetRole == lastRole)){
                updateTitleTips("不是你的回合!!!");
                return false;
            }
        }
        return true;
    };
    if(!PIECE_IS_LIVE(pieceMp[SELECT_MAP_ID]->status)){
        // 判断当前回合该谁走棋,不是自己的回合不能选
        if(!curRound(x, y)) return false;
        SelectPiece(x, y);
    }else{
        Piece* sp = pieceMp[SELECT_MAP_ID];
        qint32 abx = G2AX(sp->pos.x()), aby = G2AY(sp->pos.y());
        // 图到数组是相反的
        qSwap(abx, aby);
        // 如果不是自己的回合则不能走棋
        if(!curRound(abx, aby)) return false;
        // 先尝试走,如果不行则返回,如果走棋失败再回退
        if(!TryMovePiece(abx, aby, x, y)){
            updateTitleTips("走棋失败,请重新下棋");
            return false;
        }
        // 如果可以走则加入到步骤记录器
        mStep.push_back(new PieceMoveStep(QPoint(abx, aby), QPoint(x, y), chess_board[x][y]));
        // 如果移动失败了,则需要判断是谁被将军了,如果自己移动自己被将军则需要回退,如果自己移动,对方将军则正常进行走棋,让对方解棋
        qint32 tc = 0;
        if(!MovePiece(abx, aby, x, y, tc)){
            // 我走棋我被对方将军了,则回退重新走棋
            if(curRole != (qint32)(tc/10)){
                // 取到栈顶的步骤,进行强制回退操作
                updateTitleTips("步骤非法, 请重走");
                BackLastStep();
            }
        }
        // 消除选择框
        sp->status = 0;
        return true;
    }
    setWindowTitle(titleTips);
    return false;
}

void chinesechess::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {   
        qint32 px = event->x(), py = event->y() - menuHeight;
        // 如果棋盘是反向的则需要翻转坐标
        if(!BOARD_FORWARD){
            plumbReverse(px, py);
        }
        if(SelectMoveEvent(px, py)){
            // 如果移动完成, 则发送数据
            sendPackData();
        }
        updateTitleTips("当前是我的回合");
        chessPieces->update(); // 更新画布
        event->accept();
    }else{   
        // 取消选择框
        pieceMp[SELECT_MAP_ID]->status = 0;
        chessPieces->update();
        event->ignore();
    }
}

void chinesechess::updateTitleTips(QString info)
{
    auto lastStep = mStep.back();
    QString role = ((qint32)(lastStep->chessNum/10) == 1)?"黑方":"红方";
    titleTips = "当前第:" + QString::number(mStep.size()) + "步 " +  role + ":" + info;
}

void chinesechess::AddWindowModule()
{
    // 创建主窗口控件
    chessMenu = menuBar()->addMenu(tr("&设置"));
    menuHeight = menuBar()->height();
    invertBoard = new QAction(tr("&翻转"), this);
    connect(invertBoard, &QAction::triggered, this, &chinesechess::menuActionSetClicked);
    chessMenu->addAction(invertBoard);
    backStep = new QAction(tr("&回撤"), this);
    connect(backStep, &QAction::triggered, this, &chinesechess::BackLastStep);
    chessMenu->addAction(backStep);
    initServer = new QAction(tr("&打开服务器模式"), this);
    connect(initServer, &QAction::triggered, this, &chinesechess::InitServer);
    chessMenu->addAction(initServer);
    initClient = new QAction(tr("&打开客户端模式"), this);
    connect(initClient, &QAction::triggered, this, &chinesechess::InitClient);
    chessMenu->addAction(initClient);
}

void chinesechess::InitServer()
{
    chessNetwork = new ChessNetwork(addrport, [&](qint32 etype){
        onNetworkEvent(etype);
    });
    chessNetwork->InitServer();
    // 联网下棋只是一个功能不需要搞太复杂,服务器默认红棋,客户端默认黑棋
    curNetRole = 1;
}

// 打包需要同步的数据,我们需要同步的数据有,chess_board棋盘数组, 
// pieceMp(这个列表需要同步最后一次走棋和被选择的棋子), 
// mStep所有的走棋步骤
// 先把要打包的数据序列化成字节流
void chinesechess::packSendData(QByteArray &data)
{
    QDataStream stream(&data,QIODevice::WriteOnly);
    // chess_board
    for(int i = 0; i < BOARD_ROW; i++){
        for(int j = 0; j < BOARD_COL; j++){
            stream<<chess_board[i][j];
        }
    }
    // pieceMp
    stream<<qint32(pieceMp.size());
    for(const auto &pair : pieceMp.toStdMap()){
        stream<<qint32(pair.first)<<Piece(*pair.second);
    }
    // mStep
    stream<<qint32(mStep.size());
    for(auto it : mStep){
        stream<<PieceMoveStep(*it);
    }
}

void chinesechess::unPackSendData(QByteArray &data)
{
    QDataStream stream(&data, QIODevice::ReadOnly);
    // chess_board
    for(qint32 i = 0; i < BOARD_ROW; i++){
        for(qint32 j = 0; j < BOARD_COL; j++){
            stream>>chess_board[i][j];
        }
    }
    // pieceMp
    qint32 bpMpSize = 0;
    stream>>bpMpSize;
    for(qint32 i = 0; i < bpMpSize; i++){
        qint32 k; Piece p;
        stream>> k >> p;
        updatePieceMap(k, p);
    }
    // mStep
    qint32 bpMsSize = 0;
    stream>>bpMsSize;
    for(qint32 i = 0; i < bpMsSize; i++){
        PieceMoveStep p;
        stream>> p;
        updatePieceMapStep(i, p);
    }
    // 取到移动步骤的最后一步,把起始位置的棋子在画图表中删除
    if(bpMsSize > 1){
        PieceMoveStep *p = mStep.back();
        pieceMp.remove(G2APOS(p->beginPos.x(), p->beginPos.y()));
    }
}

// 所有通过网络交换的数据以服务器传来的为准,否则可能本地篡改, 更新数据的数量级就百级不过千
void chinesechess::updatePieceMapStep(qint32 i, PieceMoveStep p)
{
    if(i < mStep.size()){
        mStep[i]->update(p);
    }else{
        mStep.push_back(new PieceMoveStep(p));
    }
}

void chinesechess::updatePieceMap(qint32 k, Piece p)
{
    // 如果要更新的节点不在绘图列表中就要创建一个
    if(!pieceMp.contains(k)){
        pieceMp[k] = new Piece(p);
    }else{
        // 如果在绘图列表中就要更新值,这里不只是为了传输棋局,也应该是为了后续摆棋
        pieceMp[k]->update(p);
    }
}

void chinesechess::sendPackData()
{
    QByteArray data;
    packSendData(data);
    qDebug()<< "sendPackData";
    chessNetwork->SendMsg(data);
}

void chinesechess::receivePackData()
{
    // 1.先从套接字拉取消息
    QByteArray msg;
    chessNetwork->PullMsg(msg);
    // 解包数据
    unPackSendData(msg);
    // if(msg != nullptr) qDebug()<<"top: "<<msg;
}

void chinesechess::onNetworkEvent(qint32 etype)
{
    // 回调类型1是从网络模块来的,通知上层有新的客户端连进来,需要给客户端同步棋盘数据
    if(etype == EVENT_TYPE::NEW_CLIENT_CONNECT){
        sendPackData();
    }else if(etype == EVENT_TYPE::NETWORK_RECEIVE_NEW_MESSAGE){ // 从最下层的socket回调上来的,表示套接字有新的消息过来
        receivePackData();
        // 接受完数据之后,需要刷新棋子画布
        updateTitleTips("当前是我的回合");
        chessPieces->update();
    }else{
        qDebug()<<"unkown etype: "<< etype;
    }
}

void chinesechess::InitClient()
{
    chessNetwork = new ChessNetwork(addrport, [&](qint32 etype){
        onNetworkEvent(etype);
    });
    chessNetwork->InitClient();
    curNetRole = 2;
}

// 翻转棋盘显示
void chinesechess::menuActionSetClicked()
{
    BOARD_FORWARD = (BOARD_FORWARD + 1) % 2;
    chessPieces->update();
}

void chinesechess::SetWindowStyle()
{
    // 设置窗体的宽度和高度
    this->setFixedSize(BOARD_W, BOARD_H + menuHeight);

    // 设置图标
    QIcon icon = QIcon(":/images/XQWIZARD.ICO");
    this->setWindowIcon(icon);

    // 画棋盘
    // QPalette pal = this->palette();
    // pal.setBrush(QPalette::Window, QBrush(QImage(GET_RES("BOARD"))));
    // this->setPalette(pal);
    // QPainter painter(this);
    // painter.drawPixmap(0, menuHeight, BOARD_W, BOARD_H, QPixmap(GET_RES("BOARD")));  
}

void chinesechess::DrawChessBoard()
{
    for(qint32 i = 0; i < BOARD_ROW; i++){
        for(qint32 j = 0;  j < BOARD_COL; j++){
            if(chess_board[i][j]) {
                // 通过棋子的编号得到棋子资源的路径
                qint32 chessNum = chess_board[i][j];
                Piece* p = new Piece();
                // p->pixmap = GetStdPixmap(GET_RES(QString(GET_COLOR(chessNum)) + QString(GET_CHESSNAME(chessNum))));
                p->pixmapPath = GET_RES(QString(GET_COLOR(chessNum)) + QString(GET_CHESSNAME(chessNum)));
                p->pos = QPoint(A2GX(j), A2GY(i));
                p->status = 1;
                p->id = i * 10 + j;
                p->cid = chessNum;
                pieceMp.insert(p->id, p);
            }
        }
    }
    // 初始化双方的将帅
    RedKing = pieceMp[4], BlackKing = pieceMp[94];
}

//////////////////////////////////////////////////////////////////////////////////
void chessBoardLabel::paintEvent(QPaintEvent *e)
{
    WithDraw();
}

void chessBoardLabel::WithDraw()
{
    QPainter painter(this);

    for(auto it : *pieceMp)
    {
        // qDebug()<< "auto it map: " << it->id;
        if(PIECE_IS_LIVE(it->status))
        {
            qint32 ax = it->pos.x(), ay = it->pos.y();
            if(!BOARD_FORWARD){
                plumbReverse(ax, ay);
            }
            painter.drawPixmap(QPoint(ax, ay), GetStdPixmap(it->pixmapPath));
        }
    }
}