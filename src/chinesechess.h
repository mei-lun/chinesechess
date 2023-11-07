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
#include "chessnetwork.h"

struct Piece;
struct PieceMoveStep{
    QPoint beginPos, endPos;
    qint32 chessNum;
    PieceMoveStep(){};
    PieceMoveStep(const PieceMoveStep &p):beginPos(p.beginPos), endPos(p.endPos), chessNum(p.chessNum){};
    PieceMoveStep(QPoint b, QPoint e, qint32 n):beginPos(b), endPos(e), chessNum(n){}
    friend QDataStream& operator<<(QDataStream& stream, const PieceMoveStep& p){
        stream<<p.beginPos<<p.endPos<<p.chessNum;
        return stream;
    }
    friend QDataStream& operator>>(QDataStream& stream, PieceMoveStep& p){
        stream>>p.beginPos>>p.endPos>>p.chessNum;
        return stream;
    }
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
    bool SelectMoveEvent(qint32 px, qint32 py);
    bool MovePiece(qint32 abx, qint32 aby, qint32 x, qint32 y, qint32 &tc);
    bool TryMovePiece(qint32 abx, qint32 aby, qint32 bx, qint32 by, qint32 forceMove);
    void SetWindowStyle();
    void DrawChessBoard();
    void BackLastStep();
    void InitServer();
    void InitClient();
    void updateTitleTips(QString info);
    void onNetworkEvent(qint32 etype);
    void packSendData();
    void unPackSendData();
    QLabel *chessBoard;
    chessBoardLabel *chessPieces;
    QVector<PieceMoveStep*> mStep;
    QString titleTips;
    ChessNetwork *chessNetwork = nullptr;
    QQueue<QString> msgQue;
    AddrPort addrport;
    QTimer timer;

private:
    Ui_chinesechess* ui;
    QMap<qint32, Piece*> pieceMp;
    void AddWindowModule();
    QMenu *chessMenu;
    QActionGroup *alignmentGroup;
    QAction *invertBoard;
    QAction *backStep;
    QAction *initServer;
    QAction *initClient;

private slots:
    void menuActionSetClicked();
};