#pragma once
#include <vector>
#include <algorithm>
#define WIDTH 6
#define HEIGHT 5

#define DIRECTIONS 8


using std::vector;
using Point = std::pair<int, int>;

// �C�|�ťۺ���NONE�N��L�ťۡA�i�b�o�W�[���Ư]�q���]��
enum { NONE, FIRE, WATER, WOOD, LIGHT, DARK, HEART };

// �ΨӰO���ӧOCombo
// count �OCombo���ťۼƶq
// ith �N��ĴX��
struct COMBO {
	int type;
	int count = 0;
	int ith;
};

// �]���|�Ϊ�Node 
// �]���O��vector����priority queue 
// �ҥH�[�F�@�� poped �ӰO���O�_�Qpop�L
// idx�O����father�bqueue�̭���index
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

	// �O���s�q���ťۡA�b�I�sgetConnect�ɧ�s
	vector<Point> connects;
    int directionCount = 4;
public:
	void setBoard(char newBoard[HEIGHT][WIDTH]);
	char getStone(int h, int w);
	bool isValid(int h, int w); // �P�_�i���i�H���A�p�G�n�B�z���ਫ���ťۡA�i�H�b�o�̳B�z

	void dropDown(); //�����ť۸��U
	vector<COMBO> getCombos(); // ���o�L���Ҧ�Combo�]�A�|�]
	vector<COMBO> getPresentCombos(); // ���o�{�b�L��"�@��"��Combo
	int isCombo(int h, int w); // �P�_�y��(h,w)�O���O�s�q�ۤ@��Combo�A���O�N�^��0�A�O�N�^��Combo�s�q��
	void getConnects(int h, int w); // ��M�y��(h,w)�s�q�S�ۦP�ݩʪ��O���b this->connects ���ӬO�i�H���οW�ߦ��@��member�Pı�|�v�T����B��
	int isQualified(int d, Point p); // d ��ܤ�V���ξ�A�^�Ǯy��P(h,w)�bd��V�s�q���ƶq�A�o�ӦW�r���o���n
	int markQuals(); // ��Combo�s�q���y�маO��NONE(��)�æ^��Combo�s�q��


	float heuristic(float value, float maxi); // �j���w����MaxiExpected���Z��
	float maxiExpected(); // �j����X�L���̲פ��Ƴ̰����i��
	float evalue(vector<vector<char>> newBoard); // �����{�b�L��������
	//�W�����T�Ө�ƹ�A*�ܭ��n�A�٦��ܤj���u�ƪŶ�

	// �p��Combo vector����ith����combo�ƶq
	int getITHComboCount(int ith, vector<COMBO> combos);

	void setStone(int h, int w, char c);
	vector<vector<char>> getBoard();
	void setBoard(vector<vector<char>> newBoard);
    // �]�w��l�L��
    void setSrcBoard(char newBoard[HEIGHT][WIDTH]);
    void setSrcBoard(vector<vector<char>> newBoard);
    void setSrcStone(int h, int w, char c);
    void initBoard();
    void setDirectionCount(int c);
	// �M����]���g
	vector<Point> findPath();
	// �M��T�w�_�I����]���|
	std::pair<vector<Point>, float> findPathFixedSource(vector<vector<char>> &simulateBoard, Point source);


	
	// �\�౱���ܼ�
	static bool isFixedCombo;
	static int fixedComboCount;
	static bool isPriorityStone;
	static int priorityStoneType;
	static bool isAttackAll;
	static bool isEnlargeCalcTime;
	static int enlargeScale;
};