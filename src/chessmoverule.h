#define UTIL_BOARD_W 15
#define UTIL_BOARD_H 16

// 棋盘的行列数
#define BOARD_ROW 10
#define BOARD_COL 9

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
// 棋盘格子的像素大小
#define GRRID_SIZE 57

// 通过棋子ID取得棋子颜色
#define GET_COLOR(PNUM) ((qint32)(PNUM / 10) == 1 ? 'R':'B')
#define GET_CHESSNAME(PNUM) (chessNum2Name[(qint32)(PNUM % 10)])

// 数组坐标转换成图坐标
#define A2GX(AP) (RL_MARGE + GRRID_SIZE * (AP) - PIECE_CENT)
#define A2GY(AP) (UD_MARGE + GRRID_SIZE * (AP) - PIECE_CENT)
// 图坐标转换成数组坐标
#define G2AX(X) ((qint32)((X - RL_MARGE + PIECE_CENT) / GRRID_SIZE))
#define G2AY(Y) ((qint32)((Y - UD_MARGE + PIECE_CENT) / GRRID_SIZE))

// 选择图标在MAP中的索引
#define SELECT_MAP_ID -1

// 资源文件通用前缀
#define RES_HEAD ":/images/"
// 资源文件通用后缀
#define RES_LAST ".BMP"
// 拼接资源文件
#define GET_RES(FNAME) (QString(RES_HEAD) + QString(FNAME) + QString(RES_LAST))

// 判断棋子的状态
#define PIECE_IS_LIVE(ST) (ST == 1)
// 棋盘坐标转换为棋子编号
#define G2APOS(GX, GY) ((GY) * 10 + (GX))

struct Piece {
    // 棋子的编号
    qint32 id = 0;
    // 棋子当前的坐标
    QPoint pos;
    // 棋子的像素图
    QPixmap pixmap;
    // 棋子的状态,0(初始状态),1(就绪状态),2(死亡状态)
    qint32 status = 0;
};
// 当前棋盘数组的方向 1表示标准的红上黑下, 0表示黑上红下
static qint32 BOARD_FORWARD = 1;
static qint32 chess_board[BOARD_ROW][BOARD_COL] = {
                                        {11, 12, 13, 14, 15, 14, 13, 12, 11},
                                        {0,   0,  0,  0,  0,  0,  0,  0,  0},
                                        {0,   16, 0,  0,  0,  0,  0, 16,  0},
                                        {17,  0,  17, 0,  17, 0,  17, 0, 17},
                                        {0,   0,  0,  0,  0,  0,  0,  0,  0},
                                        {0,   0,  0,  0,  0,  0,  0,  0,  0},
                                        {27,  0,  27, 0,  27, 0,  27, 0, 27},
                                        {0,   26, 0,  0,  0,  0,  0, 26,  0},
                                        {0,   0,  0,  0,  0,  0,  0,  0,  0},
                                        {21, 22, 23, 24, 25, 24, 23, 22, 21}};

// 判断棋子是否在棋盘中
static const char chessInBoard[UTIL_BOARD_H][UTIL_BOARD_W] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

// 判断棋子是否在九宫格中
static const char chessInGrade[UTIL_BOARD_H][UTIL_BOARD_W] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

QMap<qint32, char> chessNum2Name = {{1, 'R'}, {2, 'N'}, {3, 'B'}, {4, 'A'}, {5, 'K'}, {6, 'C'}, {7, 'P'}};
/* 一下所有招法对应的是在棋盘下方, 如果是棋盘上方则需要翻转 */
// 将帅的步长, 最多有四种走法上左下右
static qint32 chessKingRule[4][2] = {{0, -1}, {-1 , 0}, {0, 1}, {1, 0}};
// 士的步长, 最多四种走法,左上,左下,右上,右下
static qint32 chessSHIRule[4][2] = {{-1, -1}, {1 , -1}, {-1, 1}, {1, 1}};
// 马的步长, 最多八种走法, 左上进2, 左上进1, 左下退2, 左下退1, 右上进2,右上进1, 右下退2, 右下退1
static qint32 chessMaRule[8][2] = {{-2, -1}, {-1, -2}, {2, -1}, {1, -2}, 
                                    {-2, 1}, {-1, 2}, {2, 1}, {1, 2}};
// 马腿的位置,本质上只有四种马腿,上下左右
static qint32 chessMaBanRule[8][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, -1}, 
                                    {-1, 0}, {0, 1}, {1, 0}, {0, 1}};
// 象的步长, 最多四种走法, 左上, 左下, 右上, 右下
static qint32 chessXIANGRule[4][2] = {{-2, -2}, {2 , -2}, {-2, 2}, {2, 2}};
// 象眼的位置,根据象的走法查找象眼
static qint32 chessXIANGBANRule[4][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
// 兵的步长, 最多三种走法还要判断是否过河和颜色 进1, 平左, 平右
static qint32 chessBINGRule[3][2] = {{-1, 0}, {0 , -1}, {0, 1}};

// 判断当前点和目标点中间是否有棋子(给炮和车用), 如果不存在则返回false
bool checkHavePiece(qint32 bx, qint32 by, qint32 ex, qint32 ey)
{
    // 如果是横向移动
    if(bx == ex){
        for(qint32 i = qMin(by, ey) + 1; i < qMax(by, ey); i++)
        {
            if(chess_board[bx][i]) return true;
        }
    }else if(by == ey){ // 如果是纵向移动
        for(qint32 i = qMin(bx, ex) + 1; i < qMax(bx, ex); i++)
        {
            if(chess_board[i][by]) return true;
        }
    }
    else{
        // 如果行列没有一个相同,那么走法也是非法的
        return true;
    }
    return false;
}

// 判断棋子是否在棋盘内
bool checkChessInBoard(qint32 x, qint32 y)
{
    x += 3, y += 3;
    if(chessInBoard[x][y]) return true;
    return false;
}

// 判断棋子是否在九宫格内
bool checkChessInGrade(qint32 x, qint32 y)
{
    x += 3, y += 3;
    if(chessInGrade[x][y]) return true;
    return false;
}

// 判断棋子是否在自己这边(是否过河)
bool checkChessInHome(qint32 bx, qint32 by)
{
    // 首先用ID算出原本的位置
    // 如果不等于则是算下界
    if(((qint32)(chess_board[bx][by] / 10) - 1) != BOARD_FORWARD){
        return bx < (qint32)(BOARD_ROW / 2);
    }else{ // 如果等于则是算上界
        return bx >= (qint32)(BOARD_ROW / 2);
    }
}

// 检查车的走法是否合法
bool checkCHEMove(qint32 bx, qint32 by, qint32 ex, qint32 ey)
{
    // 先检查两个点之间是否有其他的棋子
    // 如果当前点和目标点中间有其他棋子或者不在棋盘内则非法
    if(checkHavePiece(bx, by, ex, ey)) return false;
    return true;
}

// 检查马的走法是否合法
bool checkMAMove(qint32 bx, qint32 by, qint32 ex, qint32 ey)
{
    // 用起点遍历走法列表,如果存在于列表中并且没有马腿则合法,否则非法
    for(qint32 i = 0; i < 8; i++){
        qint32* tr = chessMaRule[i];
        qint32* tb = chessMaBanRule[i];
        if((tr[0] + bx == ex) && (tr[1] + by == ey) && !chess_board[bx + tb[0]][by + tb[1]]) return true;
    }
    return false;
}

// 检查象的走法是否合法
bool checkXIANGMove(qint32 bx, qint32 by, qint32 ex, qint32 ey)
{
    // 用起点遍历走法列表,如果存在于列表中并且没有压象心则合法,否则非法
    for(qint32 i = 0; i < 4; i++){
        qint32* tr = chessXIANGRule[i];
        qint32* tb = chessXIANGBANRule[i];
        if((tr[0] + bx == ex) && (tr[1] + by == ey) && !chess_board[bx + tb[0]][by + tb[1]]) return true;
    }
    return false;
}

// 检查士的走法是否合法
bool checkSHIMove(qint32 bx, qint32 by, qint32 ex, qint32 ey)
{
    for(qint32 i = 0; i < 4; i++){
        qint32* tr = chessSHIRule[i];
        if((tr[0] + bx == ex) && (tr[1] + by == ey)) return true;
    }
    return false;
}

// 检查将的走法是否合法
bool checkKingMove(qint32 bx, qint32 by, qint32 ex, qint32 ey)
{
    for(qint32 i = 0; i < 4; i++){
        qint32* tr = chessKingRule[i];
        // 走法首先要符合招法生成器的然后还要在本颜色方的九宫格内
        if((tr[0] + bx == ex) && (tr[1] + by == ey) && checkChessInGrade(ex, ey)) return true;
    }
    return false;
}

// 检查炮的走法是否合法
bool checkPAOMove(qint32 bx, qint32 by, qint32 ex, qint32 ey)
{
    // 如果不是直线移动则是非法的
    if(bx != ex && by != ey) return false;
    // 先检查两个点之间是否有其他的棋子
    // 如果没有棋子并且目标点也没有棋子则是合法的
    if(!checkHavePiece(bx, by, ex, ey)){
       return !chess_board[ex][ey];
    }else{// 如果有就要判断是否有目标不同颜色的棋子
        // 自己的颜色
        qint32 sc = chess_board[bx][by] / 10;
        qint32 tc = chess_board[ex][ey] / 10;
        return tc && sc != tc;
    }
}

// 检查兵的走法是否合法
bool checkBINGMove(qint32 bx, qint32 by, qint32 ex, qint32 ey)
{
    // 检查棋子是否没过河,如果没过河则只能X轴移动
    if(checkChessInHome(bx, by) && by != ey) return false;

    // 如果起点棋子的颜色和棋盘朝向相同则直接用走法数组走棋, 否则要用走法数组乘-1
    qint32 fwp = ((qint32)(chess_board[bx][by] / 10) - 1) == BOARD_FORWARD ? 1 : -1;
    for(qint32 i = 0; i < 3; i++){
        qint32* tr = chessBINGRule[i];
        if((tr[0] * fwp + bx == ex) && (tr[1] * fwp + by == ey)) return true;
    }
    return false;
}
// 如果要通过ai生成一个玩法的所有招法应该有一个招法生成器,生成所有合法的招法
// 这个是检测招法是否合法
bool checkMove(qint32 bx, qint32 by, qint32 ex, qint32 ey)
{
    // 先取得起始位置的棋子类型,然后通过类型调用不同的方法
    qint32 pNum = chess_board[bx][by];
    bool isIllegal = false;
    switch (chessNum2Name[pNum % 10])
    {
    case 'R':// 车
        isIllegal = checkCHEMove(bx, by, ex, ey);
        break;
    case 'N':// 马
        isIllegal = checkMAMove(bx, by, ex, ey);
        break;
    case 'B':// 象
        isIllegal = checkXIANGMove(bx, by, ex, ey);
        break;
    case 'A':// 士
        isIllegal = checkSHIMove(bx, by, ex, ey);
        break;
    case 'K':// 将/帅
        isIllegal = checkKingMove(bx, by, ex, ey);
        break;
    case 'C':// 炮
        isIllegal = checkPAOMove(bx, by, ex, ey);
        break;
    case 'P':// 卒/兵
        isIllegal = checkBINGMove(bx, by, ex, ey);
        break;
    default:
        qDebug()<<"error: Unknow error";
        break;
    }
    return isIllegal;
}