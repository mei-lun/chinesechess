// // 人机对战的人机算法引擎
// // 先通过走法生成器,得到盘面上所有棋子走法
// #include <QVector>
// // #include "chessmoverule.h"
// // 维护一个盘面节点
// class PositionStruct
// {
// public:
//     PositionStruct(){};
//     PositionStruct(QVector<QVector<qint32>>);
//     ~PositionStruct(){};
//     qint32 sdPlayer;                            // 轮到谁走, 0:红方, 1:黑方
//     QVector<QVector<qint32>> chess_board_piece; // 棋盘
//     qint32 valRed, valBlack;
//     qint32 nDistance;           
// };

// PositionStruct::PositionStruct(QVector<QVector<qint32>> chessboard)
// {
//    chess_board_piece = chessboard;
// }

// class chessRobot
// {
// private:
// public:
//     chessRobot();
//     ~chessRobot();
//     // 记录当前盘面所有的走法
//     QVector<MoveNode> *allPiecePos;
//     // 记录从当前盘面最好的走法,带深度的,这里有个问题就是,我的最好走法,对应对方最差的走法所以这里是只有一条链
//     QList<MoveNode> bestMovePos;
//     // 调用人机的时候的初始盘面,供还原使用
//     qint32 back_chess_board[BOARD_ROW][BOARD_COL];
//     // 如果是要备份数组则把初始数组传进来,如果是要还原数组则把备份的数组传进来
//     void setChessBoardArray(qint32 **first_array, qint32 **second_array);
//     void searchMain();
// };

// // 进入招法生成之前先备份初始数组,算出预设深度的最优解之后再还原当前盘面
// void chessRobot::setChessBoardArray(qint32 **first_array, qint32 **second_array)
// {
//     for(qint32 i = 0; i < BOARD_ROW; i++){
//         for(qint32 j = 0; j < BOARD_COL; j++){
//             first_array[i][j] = second_array[i][j];
//         }
//     }
// }

// // void chessRobot::searchMain()
// // {
// //     auto searchFull = [&](qint32 qvalAlpha, qint32 qvalBeta, int nDepth)->qint32{
// //         // 当达到搜索深度的时候,返回盘面的价值
// //         if(nDepth == 0){
// //             return 0;
// //         }
// //         // 初始化最佳走法
// //         // 生成全部走法
// //         auto nGenMoves = xxx;

// //         return 0;
// //     };
// //     // 当前盘面的分数,如果分数达到了上限或者下限则是有杀棋
// //     qint32 score = 0;
// //     // 使用记忆化搜索减去同类型的分支
// //     // HistoryTable
// //     // 避免搜索时间过长
// //     qint32 t = clock();
// //     // 定义最大的搜索深度
// //     for(qint32 i = 1; i <= 10; i++){
// //         qint32 qval = searchFull();
// //         if(qval > 9900 || qval < -9900) break;
// //     }
// // }

// chessRobot::chessRobot()
// {

// }

// chessRobot::~chessRobot()
// {
// }
