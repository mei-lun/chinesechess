// #include "chessrobot.h"
// void chessRobot::makeMove(qint32 bx, qint32 by, qint32 ex, qint32 ey, QVector<MoveNode> *moveStep)
// {
//     MoveNode md;
//     md.beginPos = qMakePair(bx, by), md.endPos = qMakePair(ex, ey);
//     md.beginChessNum = chess_board[bx][by], md.endChessNum = chess_board[ex][ey];
//     moveStep->push_back(md);
//     chess_board[ex][ey] = chess_board[bx][by];
// }

// void chessRobot::undoMove(qint32 bx, qint32 by, qint32 ex, qint32 ey, QVector<MoveNode> *moveStep)
// {
//     MoveNode md = moveStep->back();
//     // 还原初始位置和结束位置的棋子
//     qint32 bc = md.beginChessNum, ec = md.endChessNum;
//     chess_board[md.BX()][md.BY()] = bc;
//     chess_board[md.EX()][md.EY()] = ec;
//     moveStep->pop_back();
// }

// // 进入招法生成之前先备份初始数组,算出预设深度的最优解之后再还原当前盘面
// void chessRobot::setChessBoardArray(qint32 **board_array)
// {
//     for(qint32 i = 0; i < BOARD_ROW; i++){
//         for(qint32 j = 0; j < BOARD_COL; j++){
//             chess_board[i][j] = board_array[i][j];
//         }
//     }
// }

// void chessRobot::searchFull(qint32 qvalAlpha, qint32 qvalBeta, qint32 nDepth, qint32 color, QVector<MoveNode> *moveStep)
// {
//     // 当达到搜索深度的时候,返回盘面的价值
//     if(nDepth == 0){
//         return;
//     }
//     // 初始化最佳走法
//     qint32 vl = 0, vlBest = 0, mvBest = 0;
//     // 将死的分值
//     vlBest = -10000;
//     // 生成全部走法
//     QVector<MoveNode>* nGenMoves = new QVector<MoveNode>;
//     generateCurAllBoard(color, nGenMoves);
//     for(auto it : *nGenMoves){
//         tryMakeMove(it.BX(), it.BY(), it.EX(), it.EY(), moveStep);
//         searchFull(-qvalBeta, -qvalAlpha, nDepth-1, (color==1?2:1), moveStep);
//         qint32 redVal = 0, blackVal = 0;
//         boardPieceVal(redVal, blackVal);
//         qint32 RBVal = finalBoardVal(redVal, blackVal);
//         vl = (color == 1)? (RBVal + 3):(-RBVal);
//         undoMove(it.BX(), it.BY(), it.EX(), it.EY(), moveStep);
//         if(vl > vlBest){
//             vlBest = vl;
//             if(vl >= qvalBeta){
//                 // 截断
//                 break;
//             }
//             if(vl > qvalAlpha){
//                 // 缩小边界
//                 qvalAlpha = vl;
//             }
//         }
//     }
// }

// void chessRobot::tryMakeMove(qint32 bx, qint32 by, qint32 ex, qint32 ey, QVector<MoveNode> *moveStep)
// {
//     qint32 tc = 0, bp = chess_board[bx][by], ep = chess_board[ex][ey];
//     // 先把最终棋盘形成，然后判断，判断完成后，再还原，再真实走棋
//     chess_board[ex][ey] = chess_board[bx][by];
//     // 如果走这步棋自身是安全的
//     bool flag = checkKingSafe(bp, tc, false);
//     chess_board[bx][by] = bp, chess_board[ex][ey] = ep;
//     if(flag) makeMove(bx, by, ex, ey, moveStep);   
// }

// // 传入参数当前是该谁下棋的
// void chessRobot::searchMain(qint32 curRole)
// {
//     // 当前盘面的分数,如果分数达到了上限或者下限则是有杀棋
//     qint32 score = 0;
//     // 使用记忆化搜索减去同类型的分支
//     // HistoryTable
//     // 避免搜索时间过长
//     // qint32 t = clock();
//     // 记录从当前盘面开始所有的步骤
//     QVector<MoveNode> moveStep;
//     // 定义最大的搜索深度
//     for(qint32 i = 1; i <= 1; i++){
//         // 需要有一个变量记录从当前盘面开始记录的步骤每次进入时重新计算
//         qint32 qval = 0;
//         searchFull(-10000, 10000, i, curRole, &moveStep);
//         if(qval > 9900 || qval < -9900) break;
//     }
// }