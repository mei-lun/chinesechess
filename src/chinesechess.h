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
#define A2GX(i) (RL_MARGE + GRRID_SIZE * (j) - PIECE_CENT)
#define A2GY(i) (UD_MARGE + GRRID_SIZE * (i) - PIECE_CENT)
// 图坐标转换成数组坐标
#define G2AX(X) ((qint32)((X - RL_MARGE + PIECE_CENT) / GRRID_SIZE))
#define G2AY(Y) ((qint32)((Y - UD_MARGE + PIECE_CENT) / GRRID_SIZE))

// 资源文件通用前缀
#define RES_HEAD ":/images/"
// 资源文件通用后缀
#define RES_LAST ".BMP"
// 拼接资源文件
#define GET_RES(FNAME) (QString(RES_HEAD) + QString(FNAME) + QString(RES_LAST))

struct Piece {
    // 棋子的编号, 和类型id
    qint32 id = 0, cid = 0;
    // 棋子当前的坐标
    QPoint pos;
    // 棋子的像素图
    QPixmap pixmap;
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
    void SetWindowStyle();
    void DrawChessBoard();
    void LevelReverse(qint32 &X, qint32 &Y);
    void PlumbReverse(qint32 &X, qint32 &Y);

private:
    Ui_chinesechess* ui;
    bool isDrawing;
    QVector<Piece> pieceVt;
};