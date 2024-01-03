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
#include "chessmoverule.h"
#include "chessrobot.h"

class PieceNode;
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
    void update(const PieceMoveStep &p){
        beginPos = p.beginPos, endPos = p.endPos, chessNum = p.chessNum;
    }
};
class chessBoardLabel : public QLabel
{
public:
    chessBoardLabel(QWidget *parent = nullptr, QMap<qint32, PieceNode> *pm = nullptr) : QLabel(parent), pieceMp(pm) {}
    void WithDraw();
    QMap<qint32, PieceNode> *pieceMp;

protected:
    void paintEvent(QPaintEvent *event) override;
};

class PieceNode {
public:
    // 记录坐标当前放置的棋子
    Piece *curPiece = nullptr;
    // 记录在这个坐标死亡的棋子
    QQueue<Piece*> deadPiece;
    PieceNode(){};
    // 拷贝构造
    PieceNode(const PieceNode &p):curPiece(p.curPiece), deadPiece(p.deadPiece){};
    PieceNode(Piece *p):curPiece(p){};
    void pushDeadPiece(Piece *p);
    void popDeadPiece();
    void updateDeadPiece(qint32 i, Piece p);
};

class chinesechess : public QMainWindow, MainChessClass {
    Q_OBJECT
    
public:
    chinesechess(QWidget* parent = nullptr);
    ~chinesechess();
    void InitEnv();
    void mousePressEvent(QMouseEvent *event) override;
    void SelectPiece(qint32 x, qint32 y);
    bool SelectMoveEvent(qint32 px, qint32 py);
    bool CheckMovePiece(qint32 abx, qint32 aby, qint32 x, qint32 y, qint32 &tc);
    bool TryMovePiece(qint32 abx, qint32 aby, qint32 bx, qint32 by, qint32 forceMove);
    void SetWindowStyle();
    void DrawChessBoard();
    void BackLastStep();
    void InitServer();
    void InitClient();
    void updateTitleTips(QString info);
    void onNetworkEvent(qint32 etype);
    void sendPackData();
    void receivePackData();
    void packSendData(QByteArray &data);
    void unPackSendData(QByteArray &data);
    void updatePieceMap(qint32 k, PieceNode p);
    void updatePieceMapStep(qint32 i, PieceMoveStep p);
    bool isNetworkMode();
    void runRebotThread(qint32 roleColor);
    QLabel *chessBoard;
    chessBoardLabel *chessPieces;
    QVector<PieceMoveStep*> mStep;
    QString titleTips;
    ChessNetwork *chessNetwork = nullptr;
    QQueue<QString> msgQue;
    AddrPort addrport;
    QTimer timer;
    // 大于0的时候玩家当前的角色是执红棋还是黑棋(红就是1, 黑就是2),小于0的时候就是单机模式,不分红黑
    qint32 curNetRole = -1;
    bool redRobotStatus = false;
    bool blackRobotStatus = false;
    SearchInfo searchinfo;

private:
    Ui_chinesechess* ui;
    QMap<qint32, PieceNode> pieceMp;
    void AddWindowModule();
    QMenu *chessMenu;
    QMenu *robotMenu;
    QActionGroup *alignmentGroup;
    QAction *invertBoard;
    QAction *backStep;
    QAction *initServer;
    QAction *initClient;
    QAction *redBot;
    QAction *blackBot;

private slots:
    void menuActionSetClicked();
    void menuActionRedBot();
    void menuActionBlackBot();
};