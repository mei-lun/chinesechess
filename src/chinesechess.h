#pragma once
#include "ui_chinesechess.h"
#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>

struct Piece;

class chinesechess : public QMainWindow {
    Q_OBJECT
    
public:
    chinesechess(QWidget* parent = nullptr);
    ~chinesechess();
    void InitEnv();
    void WithDraw();
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
    void SelectPiece(qint32 x, qint32 y);
    void MovePiece(qint32 x, qint32 y);
    void SetWindowStyle();
    void DrawChessBoard();
    void LevelReverse(qint32 &X, qint32 &Y);
    void PlumbReverse(qint32 &X, qint32 &Y);

private:
    Ui_chinesechess* ui;
    QHash<qint32, Piece*> pieceMp;
};