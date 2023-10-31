#include "chinesechess.h"

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

void chinesechess::InitEnv()
{
    isDrawing = true;
}

void chinesechess::LevelReverse(int &X, int &Y)
{

}

void chinesechess::PlumbReverse(int &X, int &Y)
{

}

void chinesechess::paintEvent(QPaintEvent *e)
{
    qDebug()<<isDrawing<<" "<<pieceVt.size();
    // if(!isDrawing) return;
    WithDraw();
    // isDrawing = false;
}

void chinesechess::WithDraw()
{
    QPainter painter(this);
    for(int i = 0; i < pieceVt.size(); i++)
    {
        QPixmap pixmap = pieceVt[i].pixmap;
        QBitmap mask = pixmap.createMaskFromColor(QColor(0, 255, 0), Qt::MaskInColor);
        pixmap.setMask(mask);
        painter.drawPixmap(pieceVt[i].pos, pixmap);   
    }
}

void chinesechess::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDrawing = true;
        update(); // 更新画布
        qDebug()<<"tick left";
        event->accept();
    }
    else
    {   
        qDebug()<<"other event";
        // update(); // 更新画布
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
                Piece p;
                p.pixmap = QPixmap(GET_RES(QString(GET_COLOR(chessNum)) + QString(GET_CHESSNAME(chessNum))));
                p.pos = QPoint(A2GX(j), A2GY(i));
                pieceVt.push_back(p);
            }
        }
    }
}