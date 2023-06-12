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
	// ���o��i��C�B��j���檺�ȡA�Y�W�X�d��h�^��-1
	int getGameTable(int i, int j);

	// ���o�ثe�o��
	int getGameScore();

	// ��l���H���ؤl�B�c��B����
	// �H������2�ӷs�Ʀr
	void initGame();

	// �¤@�Ӥ�V���ʡB�X�ּƦr
	// �p�G�X�ּƦr���\�A�h���ƥ[�W�X�᪺֫�s�ƭ�
	// �p�G�����ʩΦX�֦��\�h�H������1�ӷs�Ʀr
	void gameAction(int direction);

	// �X�{2048�hwin
	bool isWin();

	// �����񺡥B�L�k�X�֫hlose
	bool isLose();
}
#endif
