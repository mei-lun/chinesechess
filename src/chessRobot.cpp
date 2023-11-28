// 人机对战的人机算法引擎
// 先通过走法生成器,得到盘面上所有棋子走法
#include <QVector>
#include "chessmoverule.h"

PositionStruct::PositionStruct(QVector<QVector<qint32>> chessboard)
{
   chess_board_piece = chessboard;
}

class chessrobot: public MainChessClass
{
private:
public:
    chessrobot(){};
    ~chessrobot(){};
    // 当前盘面红棋的分数， 黑棋的分数
    qint32 redScore, blackScore;
    // 记录当前盘面所有的走法
    QVector<MoveNode> *allPiecePos;
    // 记录从当前盘面最好的走法,带深度的,这里有个问题就是,我的最好走法,对应对方最差的走法所以这里是只有一条链
    QList<MoveNode> bestMovePos;
    // 调用人机的时候的初始盘面,供还原使用
    qint32 back_chess_board[BOARD_ROW][BOARD_COL];
    // 如果是要备份数组则把初始数组传进来,如果是要还原数组则把备份的数组传进来
    void setChessBoardArray(qint32 **board_array);
    void searchMain();
    void makeMove();
    void undoMove();
};

void chessrobot::makeMove(qint32 bx, qint32 by, qint32 ex, qint32 ey, QVector<MoveNode> *moveStep)
{
    MoveNode md;
    md.beginPos(qMakePair(bx, by)), md.endPos(qMakePair(ex, ey));
    md.beginChessNum = chess_board[bx][by], md.endChessNum = chess_board[ex][ey];
    moveStep->push_back(md);
    chess_board[ex][ey] = chess_board[bx][by];
}

void chessrobot::undoMove(qint32 bx, qint32 by, qint32 ex, qint32 ey, QVector<MoveNode> *moveStep)
{
    MoveNode md = moveStep->back();
    // 还原初始位置和结束位置的棋子
    qint32 bc = md.beginChessNum, ec = md.endChessNum;
    chess_board[md.BX()][md.BY()] = bc;
    chess_board[md.EX()][md.EY()] = ec;
    moveStep->pop_back();
}

// 进入招法生成之前先备份初始数组,算出预设深度的最优解之后再还原当前盘面
void chessrobot::setChessBoardArray(qint32 **board_array)
{
    for(qint32 i = 0; i < BOARD_ROW; i++){
        for(qint32 j = 0; j < BOARD_COL; j++){
            chess_board[i][j] = board_array[i][j];
        }
    }
}

// 传入参数当前是该谁下棋的
void chessRobot::searchMain(qint32 curRole)
{
    // 移动这一步的时候需要考虑自身是否安全，否则不能移动
    auto tryMakeMove = [&](qint32 bx, qint32 by, qint32 ex, qint32 ey, QVector<MoveNode> *moveStep){
        bool checkKingSafe(qint32 chessNum, qint32 &tc, bool tg = true)
        qint32 tc = 0, bp = chess_board[bx][by], ep = chess_board[ex][ey];
        // 先把最终棋盘形成，然后判断，判断完成后，再还原，再真实走棋
        chess_board[ex][ey] = chess_board[bx][by];
        // 如果走这步棋自身是安全的
        bool flag = checkKingSafe(bp, tc, false);
        chess_board[bx][by] = bp, chess_board[ex][ey] = ep;
        if(flag) makeMove(bx, by, ex, ey, moveStep);
    };
    auto searchFull = [&](qint32 qvalAlpha, qint32 qvalBeta, qint32 nDepth, qint32 color, QVector<MoveNode> *moveStep)->qint32{
        // 当达到搜索深度的时候,返回盘面的价值
        if(nDepth == 0){
            return 0;
        }
        // 初始化最佳走法
        // 生成全部走法
        QVector<MoveNode>* nGenMoves = new QVector<MoveNode>;
        generateCurAllBoard(color， nGenMoves);
        for(auto it : *nGenMoves){
            tryMakeMove(it.BX(), it.BY(), it.EX(), it.EY());
            searchFull(-qvalBeta, -qvalAlpha, nDepth-1, color==1?2:1, moveStep);
            undoMove(it.BX(), it.BY(), it.EX(), it.EY());
        }
        return 0;
    };
    // 当前盘面的分数,如果分数达到了上限或者下限则是有杀棋
    qint32 score = 0;
    // 使用记忆化搜索减去同类型的分支
    // HistoryTable
    // 避免搜索时间过长
    qint32 t = clock();
    // 记录从当前盘面开始所有的步骤
    QVector<MoveNode> moveStep;
    // 定义最大的搜索深度
    for(qint32 i = 1; i <= 10; i++){
        // 需要有一个变量记录从当前盘面开始记录的步骤每次进入时重新计算
        qint32 qval = searchFull(-10000, 10000, i, curRole, moveStep);
        if(qval > 9900 || qval < -9900) break;
    }
}