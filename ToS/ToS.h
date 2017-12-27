#pragma once
#include <vector>
#include <algorithm>
#define WIDTH 6
#define HEIGHT 5

#define DIRECTIONS 8


using std::vector;
using Point = std::pair<int, int>;

// 列舉符石種類NONE代表無符石，可在這增加風化珠電擊珠等
enum { NONE, FIRE, WATER, WOOD, LIGHT, DARK, HEART };

// 用來記錄個別Combo
// count 是Combo的符石數量
// ith 代表第幾消
struct COMBO {
	int type;
	int count = 0;
	int ith;
};

// 跑路徑用的Node 
// 因為是用vector模擬priority queue 
// 所以加了一個 poped 來記錄是否被pop過
// idx是紀錄father在queue裡面的index
struct NODE {
	Point p;
	int idx;
	vector<vector<char>> board;
	float evalue;
	bool poped;
	int depth;
};

struct sCOLOR {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class ToS {
private:
    char srcBoard[HEIGHT][WIDTH];
	char board[HEIGHT][WIDTH];
	bool visited[HEIGHT][WIDTH];

	// 記錄連通的符石，在呼叫getConnect時更新
	vector<Point> connects;
    int directionCount = 4;
public:
	void setBoard(char newBoard[HEIGHT][WIDTH]);
	char getStone(int h, int w);
	bool isValid(int h, int w); // 判斷可不可以走，如果要處理不能走的符石，可以在這裡處理

	void dropDown(); //模擬符石落下
	vector<COMBO> getCombos(); // 取得盤面所有Combo包括疊珠
	vector<COMBO> getPresentCombos(); // 取得現在盤面"一消"的Combo
	int isCombo(int h, int w); // 判斷座標(h,w)是不是連通著一個Combo，不是就回傳0，是就回傳Combo連通數
	void getConnects(int h, int w); // 把和座標(h,w)連通又相同屬性的記錄在 this->connects 應該是可以不用獨立成一個member感覺會影響平行運算
	int isQualified(int d, Point p); // d 表示方向直或橫，回傳座標P(h,w)在d方向連通的數量，這個名字取得不好
	int markQuals(); // 把Combo連通的座標標記成NONE(空)並回傳Combo連通數


	float heuristic(float value, float maxi); // 大概預測離MaxiExpected的距離
	float maxiExpected(); // 大概算出盤面最終分數最高的可能
	float evalue(vector<vector<char>> newBoard); // 評估現在盤面的分數
	//上面那三個函數對A*很重要，還有很大的優化空間

	// 計算Combo vector中第ith消的combo數量
	int getITHComboCount(int ith, vector<COMBO> combos);

	void setStone(int h, int w, char c);
	vector<vector<char>> getBoard();
	void setBoard(vector<vector<char>> newBoard);
    // 設定初始盤面
    void setSrcBoard(char newBoard[HEIGHT][WIDTH]);
    void setSrcBoard(vector<vector<char>> newBoard);
    void setSrcStone(int h, int w, char c);
    void initBoard();
    void setDirectionCount(int c);
	// 尋找轉珠路經
	vector<Point> findPath();
	// 尋找固定起點的轉珠路徑
	std::pair<vector<Point>, float> findPathFixedSource(vector<vector<char>> &simulateBoard, Point source);


	
	// 功能控制變數
	static bool isFixedCombo;
	static int fixedComboCount;
	static bool isPriorityStone;
	static int priorityStoneType;
	static bool isAttackAll;
	static bool isEnlargeCalcTime;
	static int enlargeScale;
};