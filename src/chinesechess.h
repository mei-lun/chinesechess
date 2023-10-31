#pragma once
#include "ui_chinesechess.h"
#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>

// 棋盘资源的像素大小
#define BOARD_H 576 
#define BOARD_W 520
// 491 547
// 棋盘的两边的边距
#define RL_MARGE 29
#define UD_MARGE 29

// 棋子的宽度(正方形)
#define PIECE_SIZE 56
// 棋子的中心点,相对于左上角的点
#define PIECE_CENT 28
/*
433 490
433 433
376 433
*/
#define GRRID_SIZE 57

#define BOARD_ROW 10
#define BOARD_COL 9

// 通过棋子ID取得棋子颜色
#define GET_COLOR(PNUM) ((qint32)(PNUM / 10) == 1 ? 'R':'B')
#define GET_CHESSNAME(PNUM) (chessNum2Name[(qint32)(PNUM % 10)])

// 数组坐标转换成图坐标
#define A2GX(AP) (RL_MARGE + GRRID_SIZE * (AP) - PIECE_CENT)
#define A2GY(AP) (UD_MARGE + GRRID_SIZE * (AP) - PIECE_CENT)
// 图坐标转换成数组坐标
#define G2AX(X) ((qint32)((X - RL_MARGE + PIECE_CENT) / GRRID_SIZE))
#define G2AY(Y) ((qint32)((Y - UD_MARGE + PIECE_CENT) / GRRID_SIZE))

// 选择图标在MAP中的索引
#define SELECT_MAP_ID -1

// 资源文件通用前缀
#define RES_HEAD ":/images/"
// 资源文件通用后缀
#define RES_LAST ".BMP"
// 拼接资源文件
#define GET_RES(FNAME) (QString(RES_HEAD) + QString(FNAME) + QString(RES_LAST))

// 判断棋子的状态
#define PIECE_IS_LIVE(ST) (ST == 1)
// 棋盘坐标转换为棋子编号
#define G2APOS(GX, GY) ((GY) * 10 + (GX))
// #define A2GPOS(AX, AY) (())
struct Piece {
    // 棋子的编号
    qint32 id = 0;
    // 棋子当前的坐标
    QPoint pos;
    // 棋子的像素图
    QPixmap pixmap;
    // 棋子的状态,0(初始状态),1(就绪状态),2(死亡状态)
    qint32 status = 0;
};

class chinesechess : public QMainWindow {
    Q_OBJECT
    
public:
    chinesechess(QWidget* parent = nullptr);
    ~chinesechess();
    void InitEnv();
    void WithDraw();
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
    qint32 chess_board[BOARD_ROW][BOARD_COL] = {{11, 12, 13, 14, 15, 14, 13, 12, 11},
                                            {0,   0,  0,  0,  0,  0,  0,  0,  0},
                                            {0,   16, 0,  0,  0,  0,  0, 16,  0},
                                            {17,  0,  17, 0,  17, 0,  17, 0, 17},
                                            {0,   0,  0,  0,  0,  0,  0,  0,  0},
                                            {0,   0,  0,  0,  0,  0,  0,  0,  0},
                                            {27,  0,  27, 0,  27, 0,  27, 0, 27},
                                            {0,   26, 0,  0,  0,  0,  0, 26,  0},
                                            {0,   0,  0,  0,  0,  0,  0,  0,  0},
                                            {21, 22, 23, 24, 25, 24, 23, 22, 21}};
    QMap<qint32, char> chessNum2Name = {{1, 'R'}, {2, 'N'}, {3, 'B'}, {4, 'A'}, {5, 'K'}, {6, 'C'}, {7, 'P'}};
    void SelectPiece(qint32 x, qint32 y);
    void MovePiece(qint32 x, qint32 y);
    void SetWindowStyle();
    void DrawChessBoard();
    void LevelReverse(qint32 &X, qint32 &Y);
    void PlumbReverse(qint32 &X, qint32 &Y);

private:
    Ui_chinesechess* ui;
    QMap<qint32, Piece> pieceMp;
};