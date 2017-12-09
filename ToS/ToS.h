#pragma once
#include <vector>
#include <algorithm>
#define WIDTH 6
#define HEIGHT 5
#define DIRECTIONS 4

using std::vector;
using Point = std::pair<int, int>;


enum { NONE, FIRE, WATER, WOOD, LIGHT, DARK, HEART };

struct COMBO {
	int type;
	int count = 0;
	int ith;
};

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
	char board[HEIGHT][WIDTH];
	bool visited[HEIGHT][WIDTH];
	vector<Point> connects;
public:
	void setBoard(char newBoard[HEIGHT][WIDTH]);
	char getStone(int h, int w);
	bool isValid(int h, int w);
	void dropDown();
	vector<COMBO> getCombos();
	vector<COMBO> getPresentCombos();
	int isCombo(int h, int w);
	void getConnects(int h, int w);
	int isQualified(int d, Point p);
	int markQuals();


	float heuristic(float value, float maxi);
	float maxiExpected();
	float evalue(vector<vector<char>> newBoard);
	int getITHComboCount(int ith, vector<COMBO> combos);

	void setStone(int h, int w, char c);
	vector<vector<char>> getBoard();
	void setBoard(vector<vector<char>> newBoard);

	vector<Point> findPath();
	std::pair<vector<Point>, float> findPathFixedSource(vector<vector<char>> &simulateBoard, Point source);


	

	static bool isFixedCombo;
	static int fixedComboCount;
	static bool isPriorityStone;
	static int priorityStoneType;
	static bool isAttackAll;
	static bool isEnlargeCalcTime;
	static int enlargeScale;
};