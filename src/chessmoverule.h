#define UTIL_BOARD_W 15
#define UTIL_BOARD_H 16

// 棋盘的行列数
#define BOARD_ROW 10
#define BOARD_COL 9

#define HALF_ROW 5

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
// 棋局的先手一方的颜色,固定红色先走
#define BEGIN_ROLE 1

// 通过棋子ID取得棋子颜色
#define GET_COLOR(PNUM) ((qint32)(PNUM / 10) == 1 ? 'R':'B')
#define GET_CHESSNAME(PNUM) (chessNumName[(qint32)(PNUM % 10)])

// 数组坐标转换成图坐标
#define A2GX(AP) (RL_MARGE + GRRID_SIZE * (AP) - PIECE_CENT)
#define A2GY(AP) (UD_MARGE + GRRID_SIZE * (AP) - PIECE_CENT)
// 图坐标转换成数组坐标
#define G2AX(X) ((qint32)((X - RL_MARGE + PIECE_CENT) / GRRID_SIZE))
#define G2AY(Y) ((qint32)((Y - UD_MARGE + PIECE_CENT) / GRRID_SIZE))

// 根据棋子的ID获取棋子的颜色
#define GETPIECECOLOR(X, Y) ((qint32)(chess_board[X][Y]/10))

// 选择图标在MAP中的索引
#define SELECT_MAP_ID -1

// 死亡棋子的坐标倍数
#define DIE_PIECE_NUM (-10)

// 资源文件通用前缀
#define RES_HEAD ":/images/"
// 资源文件通用后缀
#define RES_LAST ".BMP"
// 拼接资源文件
#define GET_RES(FNAME) (QString(RES_HEAD) + QString(FNAME) + QString(RES_LAST))

// 判断棋子的状态
#define PIECE_IS_LIVE(ST) (ST == 1)
// 棋盘坐标转换为棋子编号
#define G2APOS(GX, GY) ((GX) * 10 + (GY))

// 所有的棋子基础编号
#define CHE 1
#define MA 2
#define XAING 3
#define SHI 4
#define KING 5
#define PAO 6
#define BING 7

qint32 menuHeight = 0;

bool checkMove(qint32, qint32, qint32, qint32, qint32);
struct Piece {
    // 棋子的编号
    qint32 id = 0, cid;
    // 棋子当前的坐标
    QPoint pos;
    // 棋子的像素图
    QPixmap pixmap;
    // 棋子的状态,0(初始状态),1(就绪状态),2(死亡状态)
    qint32 status = 0;
};
// 双方的将/帅, 用来判断将军
static Piece* RedKing = nullptr;
static Piece* BlackKing = nullptr;
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

QMap<qint32, char> chessNumName = {{1, 'R'}, {2, 'N'}, {3, 'B'}, {4, 'A'}, {5, 'K'}, {6, 'C'}, {7, 'P'}};
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

// 棋盘坐标的水平翻转, 像素的X, Y和数组的X, Y是相反的
void levelReverse(qint32 &px, qint32 &py)
{
    // 先将像素坐标标准化,转换为数组坐标,再通过标准的数组坐标, 翻转, 再转换为标准像素坐标
    qint32 ax = G2AX(py), ay = G2AY(px);
    // 翻转 这里加一的原因是数组下标从0开始算,棋盘行列是从1开始算
    ax = ax, ay = BOARD_COL - (ay + 1);
    // 还原
    px = A2GX(ay), py = A2GY(ax);
}

// 棋盘坐标的垂直翻转
void plumbReverse(qint32 &px, qint32 &py)
{
    qint32 ax = G2AX(py), ay = G2AY(px);
    ax = BOARD_ROW - (ax + 1), ay = ay;
    px = A2GX(ay), py = A2GY(ax);
}

// 判断当前点和目标点中间是否有棋子(给炮和车用), 如果不存在则返回false
bool checkHavePiece(qint32 bx, qint32 by, qint32 ex, qint32 ey, qint32 &cc)
{
    // 如果是横向移动
    if(bx == ex){
        for(qint32 i = qMin(by, ey) + 1; i < qMax(by, ey); i++)
        {
            if(chess_board[bx][i]) cc++;
        }
    }else if(by == ey){ // 如果是纵向移动
        for(qint32 i = qMin(bx, ex) + 1; i < qMax(bx, ex); i++)
        {
            if(chess_board[i][by]) cc++;
        }
    }
    else{
        // 如果行列没有一个相同,那么走法也是非法的
        return true;
    }
    return cc > 0;
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

// 判断棋子是否在自己这边(是否过河),BP(起点开始算), EP(终点开始算)
void checkChessInHome(qint32 bx, qint32 by, qint32 ex, qint32 ey, bool &BP, bool &EP)
{
    // 首先用ID算出原本的位置
    // 如果不等于则是算下界
    if(((qint32)(chess_board[bx][by] / 10) - 1) != BOARD_FORWARD){
        BP = bx < HALF_ROW;
        EP = ex < HALF_ROW;
    }else{ // 如果等于则是算上界
        BP = bx >= HALF_ROW;
        EP = ex >= HALF_ROW;
    }
}

// 检查车的走法是否合法
bool checkCHEMove(qint32 bx, qint32 by, qint32 ex, qint32 ey)
{
    // 先检查两个点之间是否有其他的棋子
    // 如果当前点和目标点中间有其他棋子或者不在棋盘内则非法
    qint32 cc = 0;
    if(checkHavePiece(bx, by, ex, ey, cc)) return false;
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
    // 象的目标点不可过河
    bool BP = false, EP = false;
    checkChessInHome(bx, by, ex, ey, BP, EP);
    if(!EP) return false;
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
        if((tr[0] + bx == ex) && (tr[1] + by == ey) && checkChessInGrade(tr[0] + bx, tr[1] + by)) return true;
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
    qint32 cc = 0;
    if(!checkHavePiece(bx, by, ex, ey, cc)){
       return !chess_board[ex][ey];
    }else{// 如果有就要判断是否有目标不同颜色的棋子并且只间隔一个棋子
        // 自己的颜色
        qint32 sc = chess_board[bx][by] / 10;
        qint32 tc = chess_board[ex][ey] / 10;
        return cc <= 1 && tc && sc != tc;
    }
}

// 检查兵的走法是否合法
bool checkBINGMove(qint32 bx, qint32 by, qint32 ex, qint32 ey)
{
    bool BP = false, EP = false;
    checkChessInHome(bx, by, ex, ey, BP, EP);
    // 检查棋子是否没过河,如果没过河则只能X轴移动
    if(!BP && by != ey) return false;

    // 如果起点棋子的颜色和棋盘朝向相同则直接用走法数组走棋, 否则要用走法数组乘-1
    // 这里不要用变量的棋盘朝向,棋盘翻转并不会导致棋子的坐标位置发生变化
    // 所以这里的方向固定是红上黑下
    const qint32 DEFAULT_FORWARD = 1;
    qint32 fwp = ((qint32)(chess_board[bx][by] / 10) - 1) == DEFAULT_FORWARD ? 1 : -1;
    for(qint32 i = 0; i < 3; i++){
        qint32* tr = chessBINGRule[i];
        if((tr[0] * fwp + bx == ex) && (tr[1] * fwp + by == ey)) return true;
    }
    return false;
}

// 通过一次遍历所有数组找出所有在某一个点的所有走法
void generateAllMoves(qint32 ax, qint32 ay, QHash<int, QVector<QPair<int, int>>> &allMoves)
{
    for(auto it = chessNumName.begin(); it != chessNumName.end(); it++){
        QVector<QPair<int, int>>* vt = &allMoves[it.key()];
        for(int i = 0; i < BOARD_ROW; i++){
            for(int j = 0; j < BOARD_COL; j++){
                // 检查走法是否合法, 并且目标点没有友军
                /* 这里反向检测用起始点到终点的判断,会让炮漏算走法(中间没有子的情况,因为炮在中间没有子的时候不能移动到目标有子的位置)
                    但是不影响,因为在判断将军的时候中间没有子的炮是必不能将军的*/
                if(GETPIECECOLOR(ax, ay) != GETPIECECOLOR(i, j) && checkMove(i, j, ax, ay, it.key())){
                    vt->push_back(QPair<int, int>(i, j));
                }
            }
        }
    }
}

// 判断是否被将军, 传入盘面最后一次走棋的棋子
bool checkKingSafe(qint32 chessNum, int &tc, bool tg = true)
{
    // 根据最后一次走棋的棋子判断相反颜色的被将军
    Piece* king = (qint32)(chessNum / 10) == 1 ? (tg?BlackKing:RedKing) : (tg?RedKing:BlackKing);
    // 先取到将/帅的坐标
    qint32 ax = G2AX(king->pos.x()), ay = G2AY(king->pos.y());
    // 图坐标和数组坐标需要互换
    qSwap(ax, ay);
    QHash<int, QVector<QPair<int, int>>> allMoves;
    generateAllMoves(ax, ay, allMoves);
    qint32 c = GETPIECECOLOR(ax, ay);
    // 1.假设将是车/将, 判断能走的位置里面有没有对方的车或者将
    for(auto it : allMoves[CHE]){
        auto t = chess_board[it.first][it.second];
        if(((t%10 == CHE) || (t%10 == KING)) && (qint32)(t/10) != c) {
            tc = t;
            return false;
        }
    }
    auto Judge = [&](qint32 c){
        for(auto it : allMoves[c]){
            auto t = chess_board[it.first][it.second];
            if(t%10 == c){
                tc = t;
                return false;
            }
        }
        return true;
    };
    // 2.假设将是炮
    // 3.假设将是马
    // 4.假设将是兵
    for(auto i : {PAO, MA, BING}){
        if(!Judge(i)) return false;
    }
    // 自己移动棋子的时候也要考虑是否被将,如果被将则不能走
    return true;
}

// 如果要通过ai生成一个玩法的所有招法应该有一个招法生成器,生成所有合法的招法
// 这个是检测招法是否合法
bool checkMove(qint32 bx, qint32 by, qint32 ex, qint32 ey, qint32 cNum = 0)
{
    // 先取得起始位置的棋子类型,然后通过类型调用不同的方法
    qint32 pNum = chess_board[bx][by], checkNum = pNum % 10;
    if(cNum > 0) checkNum = cNum;
    bool isIllegal = false;
    switch (chessNumName[checkNum])
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
        qDebug()<<"error: Unknow error:"<<chessNumName[checkNum];
        break;
    }
    return isIllegal;
}