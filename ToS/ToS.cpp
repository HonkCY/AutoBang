#include "ToS.h"
#include <iostream>
#include <cstring>
#include <omp.h>

int movingVector[DIRECTIONS][2] = { 
    { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 },
    { -1,-1 },{ -1,1 },{ 1,-1 },{ 1,1 }
};

// �����Ϊ��v��
float wPriority = 2, wAllAttack = 4,wHeuristic = 71, wStep = 3;

bool ToS::isFixedCombo;
int ToS::fixedComboCount;
bool ToS::isPriorityStone;
int ToS::priorityStoneType;
bool ToS::isAttackAll;
bool ToS::isEnlargeCalcTime;
int ToS::enlargeScale;

void ToS::setSrcBoard(char newBoard[HEIGHT][WIDTH]) {
    for (int h = 0; h < HEIGHT; ++h) {
        for (int w = 0; w < WIDTH; ++w) {
            this->srcBoard[h][w] = newBoard[h][w];
        }
    }
    initBoard();
}

void ToS::setSrcBoard(vector<vector<char>> newBoard) {
    for (int h = 0; h < HEIGHT; ++h) {
        for (int w = 0; w < WIDTH; ++w) {
            this->srcBoard[h][w] = newBoard[h][w];
        }
    }
    initBoard();
}
void ToS::setSrcStone(int h, int w, char c) {
    this->srcBoard[h][w] = c;
}
void ToS::initBoard() {
    this->setBoard(this->srcBoard);
}
void ToS::setDirectionCount(int c) {
    this->directionCount = c;
}
void ToS::setBoard(char newBoard[HEIGHT][WIDTH]) {
	for (int h = 0; h < HEIGHT; ++h) {
		for (int w = 0; w < WIDTH; ++w) {
			this->board[h][w] = newBoard[h][w];
		}
	}
}

void ToS::dropDown() {
	for (int k = 0; k < HEIGHT; ++k) {
		for (int j = 0; j < WIDTH; ++j) {
			for (int i = HEIGHT - 1; i > 0; --i) {
				if (this->board[i][j] == NONE) {
					this->board[i][j] = this->board[i - 1][j];
					this->board[i - 1][j] = NONE;
				}
			}
		}
	}
}

bool ToS::isValid(int h, int w) {
	return h >= 0 && h < HEIGHT && w >= 0 && w < WIDTH;
}

vector<COMBO> ToS::getCombos() {
	char boardBackup[HEIGHT][WIDTH];
	//�ƥ���l�L��
	memcpy(boardBackup, this->board, sizeof(this->board));
	// partial�ΨӰO����e�L��Combo
	vector<COMBO> result, partial;
	int ith = 1;
	do {
		partial = this->getPresentCombos(); // ���o�ثe�L��Combo
		for (int i = 0; i < partial.size(); ++i) // �����o��Combo�O�ĴX��
			partial[i].ith = ith;
		result.insert(result.end(), partial.begin(), partial.end());
		this->dropDown(); // �������]
		++ith; // �U�@��
	} while (partial.size()); // �p�G�L���w�g�����SCombo�N����
	// �^�_��l�L��
	memcpy(this->board, boardBackup, sizeof(this->board));
	return result;
}

vector<COMBO> ToS::getPresentCombos() {
	vector<COMBO> result;
	for (int h = 0; h < HEIGHT; ++h) {
		for (int w = 0; w < WIDTH; ++w) {
			// �v�Ӯy�ЬݬO�_�i�H�s�q��Combo
			// �]���|��s�q��Combo�令NONE
			// �ҥH���|�����ƭp�⪺���D
			if (this->board[h][w] == NONE)
				continue;
			COMBO combo;
			combo.type = this->board[h][w];
			combo.count = this->isCombo(h, w);
			if (combo.count)
				result.push_back(combo);
		}
	}
	return result;
}

int ToS::isCombo(int h, int w) {
	memset(this->visited, false, sizeof(this->visited));
	this->connects.clear();
	this->getConnects(h, w); // connect��T�|�Q�s�b this->connects
	return this->markQuals();
}

// ��dfs�ӼаO�s�q�S�P�������ť�
void ToS::getConnects(int h, int w) {
	this->visited[h][w] = true;
	this->connects.push_back(Point(h, w));
	for (int i = 0; i < 4; ++i) {
		int newh = h + movingVector[i][0], neww = w + movingVector[i][1];
		if (!this->isValid(newh, neww) || this->board[newh][neww] != this->board[h][w] || this->visited[newh][neww])
			continue;
		this->getConnects(newh, neww);
	}
}

// �P�_�y��P(h,w)���ξ�i�H�s�q�X��
// �o�Ө�Ƶy�L��@�U�W�h �N�i�H�A���U���o�ؤT���W�h������
//�f�f
//�f
//���ǧޯ�O���W�����˳s�q�]����A�N�n�q�o��(�p�G�n�墨d�N����u�N���ξ�)
int ToS::isQualified(int d, Point p) {
	this->visited[p.first][p.second] = true;
	int result = 1;
	for (int i = d * 2; i < d * 2 + 2; ++i) {
		int newh = p.first + movingVector[i][0], neww = p.second + movingVector[i][1];
		if (!this->isValid(newh, neww) || this->visited[newh][neww] || this->board[newh][neww] != this->board[p.first][p.second])
			continue;
		result += this->isQualified(d, Point(newh, neww));
	}
	return result;
}

// ��Combo���s�q���аO��NONE�öǦ^�s�q�ƶq
int ToS::markQuals() {
	int result = 0;
	for (int i = 0; i < this->connects.size(); ++i) {
		memset(this->visited, false, sizeof(this->visited));
		// �s�q>=3�N��bCombo�̭��A�o��3�i�H�ܼƤơA���令2�N��令�G����
		if (this->isQualified(0, this->connects[i]) >= 3 || this->isQualified(1, this->connects[i]) >= 3) {
			++result;
		}
		else {
			// ���M�s�q�B�P�ݩʦ����ݩ�o��Combo
			// �N�R��
			this->connects.erase(this->connects.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < this->connects.size(); ++i) {
		this->board[this->connects[i].first][this->connects[i].second] = NONE;
	}
	this->connects.clear();

	return result;
}

float ToS::heuristic(float value, float maxi) {
	// ²�檺�w���F��̤j���ƪ��B��
	return (maxi - value) * wHeuristic;
}

float ToS::maxiExpected() {
	// ²�檺�w���̤j����
	int count[6] = { 0 };
	float result = 0.0;
	// �p��Ҧ��ݩʦs�b�L���W���ťۼ�
	for (int h = 0; h < HEIGHT; ++h) {
		for (int w = 0; w < WIDTH; ++w) {
			count[this->board[h][w] - 1]++;
		}
	}

	// ²������̤j����
	for (int i = 0; i < 6; ++i) {
		if (ToS::isPriorityStone && ToS::priorityStoneType == i) {
			//�p�G�O�u���ݩ�
			if(ToS::isAttackAll) // �����u���N���]�C�����@combo
				result += wAllAttack * count[i] / 5;
			else
				result += wPriority*(count[i] / 3);
		}
		else // ²�檺���]�C3������Φ� 1 combo
			result += count[i] / 3;
	}
	if (isFixedCombo) // ���w���������p�A�F�����ӭ����ƴN�O�̨�
		return fixedComboCount;
	return result;
}

float ToS::evalue(vector<vector<char>> newBoard) {
	ToS tmpToS;
	tmpToS.setBoard(newBoard);
	vector<COMBO> combos = tmpToS.getCombos();
	int firstComboCount = 0;
	float result = 0.0;
	for (int i = 0; i < combos.size(); ++i) {
		if (combos[i].ith == 1)
			++firstComboCount;
		if (isPriorityStone && priorityStoneType == combos[i].type - 1) {
			if (ToS::isAttackAll && combos[i].count >= 5)
				result += wAllAttack;
			else
				result += wPriority;
		}
		else
			result += 1;
	}
	if (isFixedCombo && fixedComboCount == firstComboCount) // �F�����w�����A�]�w���ƵL���j
		return 0x7fffffff;
	else if (isFixedCombo) // �Y�����w�����������N�u�OCombo�� �W�L�N0��
		return firstComboCount > fixedComboCount?0: firstComboCount;
	return result;
}

vector<vector<char>> ToS::getBoard() {
	vector<vector<char>> copyBoard(HEIGHT);
	for (int h = 0; h < HEIGHT; ++h) {
		copyBoard[h].resize(WIDTH);
		for (int w = 0; w < WIDTH; ++w) {
			copyBoard[h][w] = this->board[h][w];
		}
	}
	return copyBoard;
}
void ToS::setBoard(vector<vector<char>> newBoard) {
	for (int h = 0; h < HEIGHT; ++h) {
		for (int w = 0; w < WIDTH; ++w) {
			this->board[h][w] = newBoard[h][w];
		}
	}
}

void ToS::setStone(int h, int w, char c) {
	this->board[h][w] = c;
}

vector<Point> ToS::findPath() {
	const int SourceCount = 4;
	vector<Point> sources;
	vector<Point> path(SourceCount);
	// ���w�|�Ө�����_�I
	sources.push_back(Point(0, 0));
	sources.push_back(Point(4, 0));
	sources.push_back(Point(4, 5));
	sources.push_back(Point(0, 5));
	int idx = 0;
	float valMin = 0x7fffffff;
	vector<vector<vector<char>>> simulateBoard(SourceCount);
	vector<std::pair<vector<Point>, float>> results(SourceCount);
	// ���樵��
#pragma omp parallel for
	for (int i = 0; i < SourceCount; ++i) {
		simulateBoard[i].resize(HEIGHT);
		for (int j = 0; j < HEIGHT; ++j) {
			simulateBoard[i][j].resize(WIDTH);
		}
		simulateBoard[i] = this->getBoard();
		results[i] = this->findPathFixedSource(simulateBoard[i], sources[i]);
	}

	// ��X�̦n����
	for (int i = 0; i < SourceCount; ++i) {
		if (results[i].second <= valMin) {
			valMin = results[i].second;
			idx = i;
			path = results[i].first;
		}
	}
	this->setBoard(simulateBoard[idx]);
	return path;

}



std::pair<vector<Point>, float> ToS::findPathFixedSource(vector<vector<char>> &simulateBoard, Point source) {
	std::pair<vector<Point>, float> result;
	vector<NODE> vQueue;
	int queueLimit = isEnlargeCalcTime ? 5000*enlargeScale : 5000; // queue size ���̤j��
	int top = 0, finalIdx = 0, expGoal = this->maxiExpected(); // expGoal should not be that easy
	float finalVal = 0x7fffffff;
	vector<vector<char>> finalBoard = simulateBoard;
	NODE tmp;
	tmp.p = source;
	tmp.idx = 0;
	tmp.board = finalBoard;
	tmp.poped = false;
	tmp.evalue = 0;
	tmp.depth = 0;
	vQueue.push_back(tmp); // ��_�Ipush�i�h������priority queue
	while (top <= vQueue.size()) {
		NODE thisNode, fatherNode;
		// pop�X�ȳ̤p��Node (��ڨS�����O�ȳ̤p���A�����i�H��A*�����w�q)(�ȳ̤p���O��vQueue[i].evalue�̤p)�O��fval
		float valMin = 0x7fffffff;
		for (int i = 0; i < vQueue.size(); ++i) {
			if (vQueue[i].poped)
				continue;
			float fval;
			// f = h + g*wg 
			fval = heuristic(vQueue[i].evalue,expGoal) + vQueue[i].depth * wStep;
			//
			if (fval <= valMin) {
				valMin = fval;
				top = i;
			}
		}
		thisNode = vQueue[top];
		vQueue[top].poped = true;
		fatherNode = vQueue[thisNode.idx];

		//

		// swap board
		simulateBoard = thisNode.board;
		// �ˬd�O���O���e�̦n���L���٦n�A�p�G�٭n�n�A���N��
		if (valMin <= finalVal) {
			finalVal = valMin;
			finalIdx = top;
			finalBoard = thisNode.board;
			// achived goal
			if (finalVal >= 0x7fffffff)
				break;
		}
		// �o�q�ܹ��O�ѰO�R�����A�]���b���������ﭭ��Combo���L�B�z�F
		// �W��achived goal�N�|break��
		if (isFixedCombo) {
			vector<COMBO> combos;
			ToS tmpToS;
			tmpToS.setBoard(finalBoard);
			combos = tmpToS.getCombos();
			if (this->getITHComboCount(1, combos) == fixedComboCount)
				break;
		}
		//
		if (vQueue.size() > queueLimit)// restrict vQueue size as end condition
			break;
        int rnd = rand();
		for (int i = 0; i < this->directionCount; ++i) {
			// �}�l��
			int newh = thisNode.p.first + movingVector[(i + rnd) % this->directionCount][0], neww = thisNode.p.second + movingVector[(i + rnd) % this->directionCount][1];
			NODE newNode; //�U�@�Ӯy��
			newNode.p = Point(newh, neww);
			if (!this->isValid(newh, neww) || newNode.p == fatherNode.p) // not go back immediately �����W���^���A�ҥH����OfatherNode
				continue;
			// swap 
			std::swap(simulateBoard[newh][neww], simulateBoard[thisNode.p.first][thisNode.p.second]);
			// ����Node��T��push��queue
			newNode.idx = top;
			newNode.board = simulateBoard;
			newNode.evalue = this->evalue(simulateBoard);
			newNode.poped = false;
			newNode.depth = thisNode.depth + 1;
			vQueue.push_back(newNode);
			//swap back
			std::swap(simulateBoard[thisNode.p.first][thisNode.p.second], simulateBoard[newh][neww]);
			//
		}
		// swap board
		//
		//++top;
	}
	// path backtrace ��backtrace��k���o���|
	int idx = finalIdx;
	while (idx != 0) {
		result.first.push_back(vQueue[idx].p);
		idx = vQueue[idx].idx;
	}
	result.first.push_back(source);
	std::reverse(result.first.begin(), result.first.end());
	simulateBoard = finalBoard;

	result.second = finalVal;
	return result;
}

char ToS::getStone(int h, int w) {
	return this->board[h][w];
}

int ToS::getITHComboCount(int ith, vector<COMBO> combos) {
	int result = 0;
	for (int i = 0; i < combos.size(); ++i)
		if (combos[i].ith == ith)++result;
	return result;
}

