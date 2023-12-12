// 人机对战的人机算法引擎
// 先通过走法生成器,得到盘面上所有棋子走法
#include <QVector>
#include <QPoint>
#include "chessmoverule.h"
class chessRobot: public MainChessClass
{
private:
public:
    chessRobot(){};
    ~chessRobot(){
        delete RedKing;
        delete BlackKing;
    };
    // 当前盘面红棋的分数， 黑棋的分数
    qint32 redScore, blackScore;// 当前回合该谁走棋，0红，1黑
    // 记录当前盘面所有的走法
    QVector<MoveNode> *allPiecePos;
    // 记录从当前盘面最好的走法,带深度的,这里有个问题就是,我的最好走法,对应对方最差的走法所以这里是只有一条链
    QList<MoveNode> bestMovePos;
    // 调用人机的时候的初始盘面,供还原使用
    qint32 back_chess_board[BOARD_ROW][BOARD_COL];
    // 如果是要备份数组则把初始数组传进来,如果是要还原数组则把备份的数组传进来
    // void setChessBoardArray(qint32 **board_array);
    // void searchMain(qint32 curRole);
    // void tryMakeMove(qint32 bx, qint32 by, qint32 ex, qint32 ey, QVector<MoveNode> *moveStep);
    // void searchFull(qint32 qvalAlpha, qint32 qvalBeta, qint32 nDepth, qint32 color, QVector<MoveNode> *moveStep);
    // void makeMove(qint32 bx, qint32 by, qint32 ex, qint32 ey, QVector<MoveNode> *moveStep);
    // void undoMove(qint32 bx, qint32 by, qint32 ex, qint32 ey, QVector<MoveNode> *moveStep);
    void makeMove(qint32 bx, qint32 by, qint32 ex, qint32 ey, QVector<MoveNode> *moveStep)
    {
        MoveNode md;
        md.beginPos = qMakePair(bx, by), md.endPos = qMakePair(ex, ey);
        md.beginChessNum = chess_board[bx][by], md.endChessNum = chess_board[ex][ey];
        moveStep->push_back(md);
        chess_board[ex][ey] = chess_board[bx][by];
        if(GET_CHESSNUM(chess_board[ex][ey]) == 5){  
            GET_COLOR(chess_board[ex][ey]) == 'R'? RedKing->pos = QPoint(A2GX(ey), A2GY(ex)):BlackKing->pos = QPoint(A2GX(ey), A2GY(ex));
        }
    }
    void undoMove(qint32 bx, qint32 by, qint32 ex, qint32 ey, QVector<MoveNode> *moveStep)
    {
        MoveNode md = moveStep->back();
        // 还原初始位置和结束位置的棋子
        qint32 bc = md.beginChessNum, ec = md.endChessNum;
        chess_board[md.BX()][md.BY()] = bc;
        chess_board[md.EX()][md.EY()] = ec;
        moveStep->pop_back();
        if(GET_CHESSNUM(bc) == 5){
            GET_COLOR(bc) == 'R'? RedKing->pos = QPoint(A2GX(md.BY()), A2GY(md.BX())):BlackKing->pos = QPoint(A2GX(md.BY()), A2GY(md.BX()));
        }
    }
    // 进入招法生成之前先备份初始数组,算出预设深度的最优解之后再还原当前盘面
    void setChessBoardArray(qint32 board_array[10][9])
    {
        auto func = [&](qint32 i, qint32 j)->Piece*{
            Piece* p = new Piece();
            p->pos = QPoint(A2GX(j), A2GY(i));
            p->status = 1;
            p->id = i * 10 + j;
            return p;
        };
        for(qint32 i = 0; i < BOARD_ROW; i++){
            for(qint32 j = 0; j < BOARD_COL; j++){
                chess_board[i][j] = board_array[i][j];
                // 如果这个棋子是将或者帅则更新将帅位置
                if(GET_CHESSNUM(board_array[i][j]) == 5){
                    GET_COLOR(board_array[i][j]) == 'R'? RedKing = func(i, j): BlackKing = func(i, j);
                }
            }
        }
    }
    void searchFull(qint32 qvalAlpha, qint32 qvalBeta, qint32 nDepth, qint32 color, QVector<MoveNode> *moveStep)
    {
        // 当达到搜索深度的时候,返回盘面的价值
        if(nDepth == 0){
            return;
        }
        // 初始化最佳走法
        qint32 vl = 0, vlBest = 0, mvBest = 0;
        // 将死的分值
        vlBest = -10000;
        // 生成全部走法
        QVector<MoveNode>* nGenMoves = new QVector<MoveNode>;
        generateCurAllBoard(color, nGenMoves);
        for(auto it : *nGenMoves){
            tryMakeMove(it.BX(), it.BY(), it.EX(), it.EY(), moveStep);
            searchFull(-qvalBeta, -qvalAlpha, nDepth-1, (color==1?2:1), moveStep);
            qint32 redVal = 0, blackVal = 0;
            boardPieceVal(redVal, blackVal);
            qint32 RBVal = finalBoardVal(redVal, blackVal);
            vl = (color == 1)? (RBVal + 3):(-RBVal);
            undoMove(it.BX(), it.BY(), it.EX(), it.EY(), moveStep);
            if(vl > vlBest){
                vlBest = vl;
                if(vl >= qvalBeta){
                    // 截断
                    break;
                }
                if(vl > qvalAlpha){
                    // 缩小边界
                    qvalAlpha = vl;
                }
            }
        }
    }
    void tryMakeMove(qint32 bx, qint32 by, qint32 ex, qint32 ey, QVector<MoveNode> *moveStep)
    {
        qint32 tc = 0, bp = chess_board[bx][by], ep = chess_board[ex][ey];
        // 先把最终棋盘形成，然后判断，判断完成后，再还原，再真实走棋
        chess_board[ex][ey] = chess_board[bx][by];
        // 如果走这步棋自身是安全的
        bool flag = checkKingSafe(bp, tc, false);
        chess_board[bx][by] = bp, chess_board[ex][ey] = ep;
        if(flag) makeMove(bx, by, ex, ey, moveStep);   
    }
    // 传入参数当前是该谁下棋的
    void searchMain(qint32 curRole)
    {
        // 当前盘面的分数,如果分数达到了上限或者下限则是有杀棋
        qint32 score = 0;
        // 使用记忆化搜索减去同类型的分支
        // HistoryTable
        // 避免搜索时间过长
        // qint32 t = clock();
        // 记录从当前盘面开始所有的步骤
        QVector<MoveNode> moveStep;
        // 定义最大的搜索深度
        for(qint32 i = 1; i <= 1; i++){
            // 需要有一个变量记录从当前盘面开始记录的步骤每次进入时重新计算
            qint32 qval = 0;
            searchFull(-10000, 10000, i, curRole, &moveStep);
            if(qval > 9900 || qval < -9900) break;
        }
    }
};