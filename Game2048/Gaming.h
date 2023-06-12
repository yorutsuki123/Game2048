// Gaming.h
//

#pragma once
#ifndef GAMING
#define GAMING

#define TABLE_SIZE 4
#define DIR_U 1
#define DIR_D 2
#define DIR_R 3
#define DIR_L 4
namespace gaming
{
	// 取得第i橫列、第j直行的值，若超出範圍則回傳-1
	int getGameTable(int i, int j);

	// 取得目前得分
	int getGameScore();

	// 初始化隨機種子、宮格、分數
	// 隨機產生2個新數字
	void initGame();

	// 朝一個方向移動、合併數字
	// 如果合併數字成功，則分數加上合併後的新數值
	// 如果有移動或合併成功則隨機產生1個新數字
	void gameAction(int direction);

	// 出現2048則win
	bool isWin();

	// 全部填滿且無法合併則lose
	bool isLose();
}
#endif
