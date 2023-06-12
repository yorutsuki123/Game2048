#include <cstdlib>
#include <ctime>
#include <cstring>
#include <algorithm>
#include "Gaming.h"

int gameTable[TABLE_SIZE][TABLE_SIZE];
int gameScore;

void randomGenBlock()
{
	int null_lst[16][2] = { 0 };
	int len = 0;
	for (int i = 0; i < TABLE_SIZE; i++)
		for (int j = 0; j < TABLE_SIZE; j++)
			if (!gameTable[i][j])
			{
				null_lst[len][0] = i;
				null_lst[len][1] = j;
				len++;
			}
	if (!len) return;
	int target = rand() % len;
	int val = rand() % 2 * 2 + 2;
	gameTable[null_lst[target][0]][null_lst[target][1]] = val;
}


int gaming::getGameTable(int x, int y)
{
	if (0 <= x && x < TABLE_SIZE && 0 <= y && y < TABLE_SIZE)
		return gameTable[x][y];
	return -1;
}

int gaming::getGameScore()
{
	return gameScore;
}

void gaming::initGame()
{
	srand(time(NULL));
	for (int i = 0; i < TABLE_SIZE; i++)
		for (int j = 0; j < TABLE_SIZE; j++)
			gameTable[i][j] = 0;
	gameScore = 0;
	randomGenBlock();
	randomGenBlock();
}

void moveLeft(int tb[])
{
	int tmp[TABLE_SIZE] = { 0 };
	int tmplen = 0;
	for (int i = 0; i < TABLE_SIZE; i++)
		if (tb[i])
			tmp[tmplen++] = tb[i];
	for (int i = 0; i < TABLE_SIZE; i++)
		tb[i] = tmp[i];
}

void transTable()
{
	for (int i = 0; i < TABLE_SIZE; i++)
		for (int j = i; j < TABLE_SIZE; j++)
			std::swap(gameTable[i][j], gameTable[j][i]);
}

void flipTable()
{
	for (int i = 0; i < TABLE_SIZE; i++)
		for (int j = 0; j < TABLE_SIZE / 2; j++)
			std::swap(gameTable[i][j], gameTable[i][TABLE_SIZE - 1 - j]);
}

void handleDir(int direction, bool to)
{
	switch (direction)
	{
	case DIR_U:
		transTable();
		break;
	case DIR_R:
		flipTable();
		break;
	case DIR_D:
		if (to) transTable();
		flipTable();
		if (!to) transTable();
		break;
	default:
		break;
	}
}

void gaming::gameAction(int direction)
{
	int tmpTable[TABLE_SIZE][TABLE_SIZE];
	memcpy(tmpTable, gameTable, sizeof(int) * TABLE_SIZE * TABLE_SIZE);
	handleDir(direction, true);
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		// move
		moveLeft(gameTable[i]);
		// combine
		for (int j = 0; j < TABLE_SIZE - 1; j++)
		{
			if (gameTable[i][j] == gameTable[i][j + 1])
			{
				gameTable[i][j] *= 2;
				gameTable[i][j + 1] = 0;
				gameScore += gameTable[i][j];
			}
		}
		// move
		moveLeft(gameTable[i]);
	}
	handleDir(direction, false);
	if (memcmp(tmpTable, gameTable, sizeof(int) * TABLE_SIZE * TABLE_SIZE))
		randomGenBlock();
}

bool gaming::isWin()
{
	for (int i = 0; i < TABLE_SIZE; i++)
		for (int j = 0; j < TABLE_SIZE; j++)
			if (gameTable[i][j] == 2048)
				return true;
	return false;
}

bool gaming::isLose()
{
	for (int i = 0; i < TABLE_SIZE; i++)
		for (int j = 0; j < TABLE_SIZE; j++)
			if (gameTable[i][j] == 0 ||
				gameTable[i][j] == getGameTable(i - 1, j) ||
				gameTable[i][j] == getGameTable(i + 1, j) ||
				gameTable[i][j] == getGameTable(i, j - 1) ||
				gameTable[i][j] == getGameTable(i, j + 1))
				return false;
	return true;
}