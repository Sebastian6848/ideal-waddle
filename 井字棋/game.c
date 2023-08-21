#define _CRT_SECURE_NO_WARNINGS 1
#include "game.h"

//棋盘初始化
void Initboard(char board[ROW][COL], int row, int col)
{
	int i = 0, j = 0;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			board[i][j] = ' ';//每个格子都置空
		}
	}
}

//打印棋盘
void Displayboard(char board[ROW][COL], int row, int col)
{
	int i = 0, j = 0;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			printf(" %c ", board[i][j]);
			if (j < col - 1)
				printf("|");
		}
		printf("\n");
		if (i < row - 1)
		{
			for (j = 0; j < col; j++)
			{
				printf("---");
				if (j < col - 1)
					printf("|");
			}
		}
		printf("\n");
	}
}

//玩家走棋
void Playermove(char board[ROW][COL], int row, int col)
{
	int x = 0, y = 0;
	printf("请玩家走棋\n");
	while (1)
	{
		printf("请输入下棋位置:");
		scanf("%d %d", &x, &y);
		if (x >= 1 && x <= row && y >= 1 && y <= col)
		{
			if (board[x - 1][y - 1] == ' ')
			{
				board[x - 1][y - 1] = '*';
				break;
			}
			else
				printf("所输入位置已被占用，请重新输入:\n");
		}
		else
			printf("请输入有效位置!\n");
	}
}

//电脑走棋
void Computermove(char board[ROW][COL], int row, int col)
{
	printf("电脑开始走棋\n");
	int x = 0, y = 0;
	while (1)
	{
		x = rand() % row;
		y = rand() % col;
		if (board[x][y] == ' ')
		{
			board[x][y] = '#';
			break;
		}
	}
}

//判断棋盘是否已满
static IsFull(char board[ROW][COL], int row, int col)
{
	int i = 0, j = 0;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			if (board[i][j] == ' ')
				return 0;
		}
	}
	return 1;
}

//判断胜负
char IsWin(char board[ROW][COL], int row, int col)
{
	int i = 0;
	for (i = 0; i < row; i++)
	{
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] == board[i][0] && board[i][2] != ' ')
		{
			return board[i][0];
		}
	}
	for (i = 0; i < col; i++)
	{
		if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] == board[2][i] && board[0][i] != ' ')
		{
			return board[i][0];
		}
	}
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] == board[2][2] && board[0][0] != ' ')
	{
		return board[0][0];
	}
	if (board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[0][2] == board[2][0] && board[1][1] != ' ')
	{
		return board[1][1];
	}
	if (IsFull(board, ROW, COL))
	{
		return 'Q';
	}
	return 'C';
}
