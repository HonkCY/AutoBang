#include "ToS.h"
#include <iostream>
#include <cstring>
#include <omp.h>

int movingVector[DIRECTIONS][2] = { 
    { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 },
    { -1,-1 },{ -1,1 },{ 1,-1 },{ 1,1 }
};

// 評估用的權重
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
	//備份原始盤面
	memcpy(boardBackup, this->board, sizeof(this->board));
	// partial用來記錄當前盤面Combo
	vector<COMBO> result, partial;
	int ith = 1;
	do {
		partial = this->getPresentCombos(); // 取得目前盤面Combo
		for (int i = 0; i < partial.size(); ++i) // 紀錄這些Combo是第幾消
			partial[i].ith = ith;
		result.insert(result.end(), partial.begin(), partial.end());
		this->dropDown(); // 模擬落珠
		++ith; // 下一消
	} while (partial.size()); // 如果盤面已經完全沒Combo就停止
	// 回復原始盤面
	memcpy(this->board, boardBackup, sizeof(this->board));
	return result;
}

vector<COMBO> ToS::getPresentCombos() {
	vector<COMBO> result;
	for (int h = 0; h < HEIGHT; ++h) {
		for (int w = 0; w < WIDTH; ++w) {
			// 逐個座標看是否可以連通成Combo
			// 因為會把連通的Combo改成NONE
			// 所以不會有重複計算的問題
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
	this->getConnects(h, w); // connect資訊會被存在 this->connects
	return this->markQuals();
}

// 用dfs來標記連通又同種類的符石
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

// 判斷座標P(h,w)直或橫可以連通幾個
// 這個函數稍微改一下規則 就可以適應下面這種三消規則之類的
//口口
//口
//有些技能是像上面那樣連通也能消，就要從這改(如果要改那d就不能只代表直或橫)
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

// 把Combo中連通的標記成NONE並傳回連通數量
int ToS::markQuals() {
	int result = 0;
	for (int i = 0; i < this->connects.size(); ++i) {
		memset(this->visited, false, sizeof(this->visited));
		// 連通>=3就算在Combo裡面，這裡3可以變數化，像改成2就能改成二消隊
		if (this->isQualified(0, this->connects[i]) >= 3 || this->isQualified(1, this->connects[i]) >= 3) {
			++result;
		}
		else {
			// 雖然連通且同屬性但不屬於這個Combo
			// 就刪掉
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
	// 簡單的預估達到最大分數的步數
	return (maxi - value) * wHeuristic;
}

float ToS::maxiExpected() {
	// 簡單的預估最大分數
	int count[6] = { 0 };
	float result = 0.0;
	// 計算所有屬性存在盤面上的符石數
	for (int h = 0; h < HEIGHT; ++h) {
		for (int w = 0; w < WIDTH; ++w) {
			count[this->board[h][w] - 1]++;
		}
	}

	// 簡單評估最大分數
	for (int i = 0; i < 6; ++i) {
		if (ToS::isPriorityStone && ToS::priorityStoneType == i) {
			//如果是優先屬性
			if(ToS::isAttackAll) // 全體優先就假設每五顆一combo
				result += wAllAttack * count[i] / 5;
			else
				result += wPriority*(count[i] / 3);
		}
		else // 簡單的假設每3顆都能形成 1 combo
			result += count[i] / 3;
	}
	if (isFixedCombo) // 指定首消的狀況，達成那個首消數就是最佳
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
	if (isFixedCombo && fixedComboCount == firstComboCount) // 達成指定首消，設定分數無限大
		return 0x7fffffff;
	else if (isFixedCombo) // 若有指定首消那評分就只是Combo數 超過就0分
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
	// 指定四個角落當起點
	sources.push_back(Point(0, 0));
	sources.push_back(Point(4, 0));
	sources.push_back(Point(4, 5));
	sources.push_back(Point(0, 5));
	int idx = 0;
	float valMin = 0x7fffffff;
	vector<vector<vector<char>>> simulateBoard(SourceCount);
	vector<std::pair<vector<Point>, float>> results(SourceCount);
	// 平行巡路
#pragma omp parallel for
	for (int i = 0; i < SourceCount; ++i) {
		simulateBoard[i].resize(HEIGHT);
		for (int j = 0; j < HEIGHT; ++j) {
			simulateBoard[i][j].resize(WIDTH);
		}
		simulateBoard[i] = this->getBoard();
		results[i] = this->findPathFixedSource(simulateBoard[i], sources[i]);
	}

	// 找出最好的路
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
	int queueLimit = isEnlargeCalcTime ? 5000*enlargeScale : 5000; // queue size 的最大值
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
	vQueue.push_back(tmp); // 把起點push進去模擬的priority queue
	while (top <= vQueue.size()) {
		NODE thisNode, fatherNode;
		// pop出值最小的Node (對我沒打錯是值最小的，不懂可以看A*評分定義)(值最小不是指vQueue[i].evalue最小)是指fval
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
		// 檢查是不是比當前最好的盤面還好，如果還要好，那就換
		if (valMin <= finalVal) {
			finalVal = valMin;
			finalIdx = top;
			finalBoard = thisNode.board;
			// achived goal
			if (finalVal >= 0x7fffffff)
				break;
		}
		// 這段很像是忘記刪掉的，因為在評分都有對限制Combo做過處理了
		// 上面achived goal就會break掉
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
			// 開始走
			int newh = thisNode.p.first + movingVector[(i + rnd) % this->directionCount][0], neww = thisNode.p.second + movingVector[(i + rnd) % this->directionCount][1];
			NODE newNode; //下一個座標
			newNode.p = Point(newh, neww);
			if (!this->isValid(newh, neww) || newNode.p == fatherNode.p) // not go back immediately 不馬上往回走，所以不能是fatherNode
				continue;
			// swap 
			std::swap(simulateBoard[newh][neww], simulateBoard[thisNode.p.first][thisNode.p.second]);
			// 紀錄Node資訊並push到queue
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
	// path backtrace 用backtrace方法取得路徑
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

