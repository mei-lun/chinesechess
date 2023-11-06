#pragma once
#include "ui_chinesechess.h"
#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QLabel>
#include <QStack>

struct Piece;
struct PieceMoveStep{
    QPoint beginPos, endPos;
    qint32 chessNum;
    PieceMoveStep(QPoint b, QPoint e, qint32 n):beginPos(b), endPos(e), chessNum(n){}
};
class chessBoardLabel : public QLabel
{
public:
    chessBoardLabel(QWidget *parent = nullptr, QMap<qint32, Piece*> *pm = nullptr) : QLabel(parent), pieceMp(pm) {}
    void WithDraw();
    QMap<qint32, Piece*> *pieceMp;

protected:
    void paintEvent(QPaintEvent *event) override;
};

class chinesechess : public QMainWindow {
    Q_OBJECT
    
public:
    chinesechess(QWidget* parent = nullptr);
    ~chinesechess();
    void InitEnv();
    void mousePressEvent(QMouseEvent *event) override;
    void SelectPiece(qint32 x, qint32 y);
    bool MovePiece(qint32 abx, qint32 aby, qint32 x, qint32 y, qint32 &tc);
    bool TryMovePiece(qint32 abx, qint32 aby, qint32 bx, qint32 by, qint32 forceMove);
    void SetWindowStyle();
    void DrawChessBoard();
    void BackLastStep();
    void updateTitleTips(QString info);
    QLabel *chessBoard;
    chessBoardLabel *chessPieces;
    QVector<PieceMoveStep*> mStep;
    QString titleTips;

private:
    Ui_chinesechess* ui;
    QMap<qint32, Piece*> pieceMp;
    void AddWindowModule();
    QMenu *chessMenu;
    QActionGroup *alignmentGroup;
    QAction *invertBoard;
    QAction *backStep;
private slots:
    void menuActionSetClicked();
};