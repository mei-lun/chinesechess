#include "chinesechess.h"
#include "chessmoverule.h"

chinesechess::chinesechess(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_chinesechess)
{
    ui->setupUi(this);
    InitEnv();
    DrawChessBoard();
    SetWindowStyle();   // 设置窗体属性 
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
    Piece* piece = new Piece();
    piece->pixmap = GetStdPixmap(":/images/SELECTED.BMP");
    piece->status = 0;
    piece->id = -1;
    pieceMp.insert(piece->id, piece);
}

void chinesechess::paintEvent(QPaintEvent *e)
{
    qDebug()<<" "<<pieceMp.count();
    WithDraw();
}

void chinesechess::WithDraw()
{
    QPainter painter(this);

    for(auto it : pieceMp)
    {
        // qDebug()<< "auto it map: " << it->id;
        if(PIECE_IS_LIVE(it->status))
        {
            painter.drawPixmap(it->pos, it->pixmap);   
        }
    }
}

void chinesechess::SelectPiece(qint32 gx, qint32 gy)
{
    qint32 x = G2AX(gy); qint32 y = G2AY(gx);
    qDebug()<<"tick left "<<x<<" "<<y;
    if(chess_board[x][y])
    {
        Piece* p = pieceMp[SELECT_MAP_ID];
        p->pos = QPoint(A2GY(y), A2GX(x));
        p->status = 1;
    }
}

/*
移动棋子的时候, 如果目标位置没有棋子那么直接赋值
如果目标位置有棋子那么首先把目标位置的棋子置为死亡,然后再对目标位置赋值
*/
void chinesechess::MovePiece(qint32 gx, qint32 gy)
{
    qint32 x = G2AX(gy); qint32 y = G2AY(gx);
    Piece* sp = pieceMp[SELECT_MAP_ID];
    auto move2tar = [&](int bx, int by, int ex, int ey, int status = 1){
        Piece* bp = pieceMp[G2APOS(bx, by)];
        bp->id = G2APOS(ex, ey);
        bp->status = status;
        bp->pos = QPoint(A2GY(ex), A2GX(ey));
        // 使用swap交换的时候产生的空位置引用,被认为是一个长度
        qSwap(pieceMp[G2APOS(bx, by)], pieceMp[G2APOS(ex, ey)]);
        if(!pieceMp[G2APOS(bx, by)]){
            // 这里不清除会造成内存泄漏
            pieceMp.remove(G2APOS(bx, by));
        }
        if(status != 1){ex = qAbs(ex), ey = qAbs(ey);}
        chess_board[ey][ex] = chess_board[by][bx], chess_board[by][bx] = 0;
    };
    // 棋盘上的i,j对应棋盘的y, x
    qint32 ax = x, ay = y;
    qint32 bx = ay, by = ax;
    qint32 abx = G2AX(sp->pos.x()), aby = G2AY(sp->pos.y());
    // 如果移动的目标位置会被将军,则也不能移动
    qint32 tc = 0;
    qDebug()<<"self is safe? "<<checkKingSafe(aby, abx, tc, false)<<" "<<tc;
    if(!checkKingSafe(aby, abx, tc, false) || !checkMove(aby, abx, by, bx)){
        qDebug()<<"非法移动";
        return;
    }
    // 目标位置有棋子
    if(chess_board[x][y])
    {
        // 任何时候目标位置都不应该有同颜色棋子
        if((qint32)(chess_board[x][y] / 10) == (qint32)(chess_board[aby][abx] / 10)){
            qDebug()<<"error: move illegal";
            return;
        }
        move2tar(bx, by, -bx, -by, 2);
    }
    move2tar(abx, aby, bx, by);
    // 检查是否将对方军
    if(!checkKingSafe(by, bx, tc, true)) qDebug()<<"对方被将军了!!!"<<" "<<tc;
    // 消除选择框
    sp->status = 0;
}

void chinesechess::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {   
        if(!PIECE_IS_LIVE(pieceMp[SELECT_MAP_ID]->status))
        {
            SelectPiece(event->x(), event->y());
        }else
        {
            qDebug()<< "moveP: x"<< event->x() <<" y:"<< event->y();
            MovePiece(event->x(), event->y());
        }
        update(); // 更新画布
        event->accept();
    }
    else
    {   
        qDebug()<<"cancel select";
        // 取消选择框
        pieceMp[SELECT_MAP_ID]->status = 0;
        update();
        event->ignore();
    }
}

void chinesechess::SetWindowStyle()
{
    // 设置窗体的宽度和高度
    this->setFixedSize(BOARD_W, BOARD_H);

    // 设置图标
    QIcon icon = QIcon(":/images/XQWIZARD.ICO");
    this->setWindowIcon(icon);

    // 设置背景
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(QImage(GET_RES("BOARD"))));
    this->setPalette(pal);

    // 画棋盘上的棋子
    WithDraw();
}

void chinesechess::DrawChessBoard()
{
    for(qint32 i = 0; i < BOARD_ROW; i++){
        for(qint32 j = 0;  j < BOARD_COL; j++){
            if(chess_board[i][j]) {
                // 通过棋子的编号得到棋子资源的路径
                qint32 chessNum = chess_board[i][j];
                Piece* p = new Piece();
                p->pixmap = GetStdPixmap(GET_RES(QString(GET_COLOR(chessNum)) + QString(GET_CHESSNAME(chessNum))));
                p->pos = QPoint(A2GX(j), A2GY(i));
                p->status = 1;
                p->id = i * 10 + j;
                pieceMp.insert(p->id, p);
            }
        }
    }
    // 初始化双方的将帅
    RedKing = pieceMp[4], BlackKing = pieceMp[94];
}