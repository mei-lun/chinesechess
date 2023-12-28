// 人机对战的人机算法引擎
// 先通过走法生成器,得到盘面上所有棋子走法
#include <QVector>
#include <QPoint>
#include <qalgorithms.h>
#include "chessmoverule.h"
// struct HistoryTable{
//     MoveNode node;
//     qint32 val = 0;
//     HistoryTable(MoveNode n, qint32 v){
//         node = n, val = v;
//     }
// };
static QMap<MoveNode, qint32> bestMovePos;

static MoveNode mvResult;
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
    // 调用人机的时候的初始盘面,供还原使用
    qint32 back_chess_board[BOARD_ROW][BOARD_COL];
    // 走棋的步数
    qint32 ndistance = 0;
    qint32 makeMove(qint32 bx, qint32 by, qint32 ex, qint32 ey)
    {
        qint32 diechess = chess_board[ex][ey];
        chess_board[ex][ey] = chess_board[bx][by];
        chess_board[bx][by] = 0;
        if(GET_CHESSNUM(chess_board[ex][ey]) == 5){  
            GET_COLOR(chess_board[ex][ey]) == 'R'? RedKing->pos = QPoint(A2GX(ey), A2GY(ex)):BlackKing->pos = QPoint(A2GX(ey), A2GY(ex));
        }
        ndistance++;
        return diechess;
    }
    void undoMove(qint32 bx, qint32 by, qint32 ex, qint32 ey, qint32 cnum)
    {
        chess_board[bx][by] = chess_board[ex][ey];
        chess_board[ex][ey] = cnum;
        if(GET_CHESSNUM(chess_board[bx][by]) == 5){
            GET_COLOR(chess_board[bx][by]) == 'R'? RedKing->pos = QPoint(A2GX(by), A2GY(bx)):BlackKing->pos = QPoint(A2GX(by), A2GY(bx));
        }
        ndistance--;
    }
    // 进入招法生成之前先备份初始数组,算出预设深度的最优解之后再还原当前盘面
    void setChessBoardArray(qint32 board_array[][9])
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

    qint32 searchFull(qint32 qvalAlpha, qint32 qvalBeta, qint32 nDepth, qint32 color)
    {
        auto evaluate = [&]()->qint32{
            qint32 redVal = 0, blackVal = 0, val = 0;
            boardPieceVal(redVal, blackVal);
            qint32 RBVal = finalBoardVal(redVal, blackVal);
            val = (color == 1)? (RBVal + 3):(-RBVal);
            return val;
        };
        // 当达到搜索深度的时候,返回盘面的价值
        if(nDepth == 0){
            return evaluate();
        }
        // 初始化最佳走法
        qint32 vl = 0;
        // 初始化一个最佳走法
        MoveNode mvBest(0, 0, 0, 0, -10000);
        // 生成全部走法
        QVector<MoveNode> nGenMoves;
        generateCurAllBoard(color, &nGenMoves);
        // 按历史表排序，如果这步棋的深度越大，越说明这步棋的价值越高
        std::sort(nGenMoves.begin(), nGenMoves.end(), [&](MoveNode a, MoveNode b)->bool{
            return bestMovePos[b] < bestMovePos[a];
        });
        for(auto it : nGenMoves){
            qint32 diechess = makeMove(it.BX(), it.BY(), it.EX(), it.EY());
            qint32 tc = 0;
            bool flag = checkKingSafe(chess_board[it.EX()][it.EY()], tc, false);
            // 如果自己走这步棋被将军了，则撤销走法走下一步棋
            if(!flag) {
                undoMove(it.BX(), it.BY(), it.EX(), it.EY(), diechess);
                continue;
            }
            vl = -searchFull(-qvalBeta, -qvalAlpha, nDepth-1, (color==1?2:1));
            undoMove(it.BX(), it.BY(), it.EX(), it.EY(), diechess);
            if(vl > mvBest.score){
                mvBest.score = vl;
                // Max层收下界，Min层收上界
                if(vl >= qvalBeta){
                    mvBest.score = vl;
                    mvBest.beginPos = qMakePair(it.BX(), it.BY());
                    mvBest.endPos = qMakePair(it.EX(), it.EY());
                    // 截断
                    break;
                }
                if(vl > qvalAlpha){
                    mvBest.beginPos = qMakePair(it.BX(), it.BY());
                    mvBest.endPos = qMakePair(it.EX(), it.EY());
                    // 缩小边界
                    qvalAlpha = vl;
                }
            }
        }
        // 如果达到将死的分值，则返回步数和最高分的差值作为走棋的价值
        if(mvBest.score == -10000) {
            return ndistance - mvBest.score;
        }else{
            bestMovePos[mvBest] += nDepth * nDepth;
            mvResult = mvBest;
        }
        return mvBest.score;
    }
    // 传入参数当前是该谁下棋的
    MoveNode searchMain(qint32 curRole)
    {
        // 当前盘面的分数,如果分数达到了上限或者下限则是有杀棋
        qint32 score = 0;
        // 使用记忆化搜索减去同类型的分支
        bestMovePos.clear();
        // 初始化计步器
        ndistance = 0;
        // 避免搜索时间过长
        qint32 t = clock();
        // 定义最大的搜索深度
        for(qint32 i = 1; i <= 4; i++){
            // 需要有一个变量记录从当前盘面开始记录的步骤每次进入时重新计算
            qint32 qval = 0;
            qval = searchFull(-10000, 10000, i, curRole);
            if(qval > 9900 || qval < -9900) break;
            // 如果超过5秒就不再继续往下搜了
            // if(clock() - t > 1000 * 5) break;
            qDebug()<<"qval: "<<qval;
        }
        // 玩家当前搜索出来的最佳走法
        qDebug()<<(curRole==1?'R':'B')<< " best move::begin: "<< mvResult.beginPos.first<<" "<<mvResult.beginPos.second<<" end: "<< mvResult.endPos.first<<" "<<mvResult.endPos.second;
        return mvResult;
    }
};