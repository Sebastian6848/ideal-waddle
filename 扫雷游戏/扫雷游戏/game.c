#define _CRT_SECURE_NO_WARNINGS 1
#include "game.h"

void InitBoard(char board[ROWS][COLS], int rows, int cols, char set)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			board[i][j] = set;
		}
	}
}

void DisplayBoard(char board[ROWS][COLS], int row, int col)
{
	int i = 0, j = 0;
	printf("----------扫雷----------\n");
	for (i = 0; i <= row; i++)
	{
		printf("%d ", i);
		for (j = 1; j <= col; j++)
		{
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
	printf("----------扫雷----------\n");
}

void SetMine(char board[ROWS][COLS], int row, int col)
{
	int count = EASY_COUNT;
	while (count)
	{
		int x = rand() % row + 1;
		int y = rand() % col + 1;
		if (board[x][y] == '0')
		{
			board[x][y] = '1';
		}
		count--;
	}
}

int GetMineCount(char mine[ROWS][COLS], int x, int y)
{
	return mine[x + 1][y] + mine[x - 1][y] + mine[x][y + 1] + mine[x][y - 1]
		+ mine[x + 1][y + 1] + mine[x - 1][y + 1] + mine[x - 1][y - 1] + mine[x + 1][y - 1] - 8 * '0';
}

void FindMine(char mine[ROWS][COLS], char show[ROWS][COLS], int row, int col)
{
	int x = 0;
	int y = 0;
	int win = 0;
	while (win < row * col - EASY_COUNT)
	{
		printf("请输入排查坐标>");
		scanf("%d %d", &x, &y);
		if (x >= 1 && x <= row && y >= 1 && y <= col)
		{
			if (show[x][y] != '*')
			{
				printf("输入坐标重复，请重新输入\n");
			}
			else if (mine[x][y] == '1')
			{
				printf("很遗憾，你被炸死了\n");
				DisplayBoard(mine, ROW, COL);
				break;
			}
			else
			{
				//不是雷，就统计x,y坐标周围有几个雷
				int c = GetMineCount(mine, x, y);
				show[x][y] = c + '0';
				DisplayBoard(show, ROW, COL);
				win++;
			}
		}
		else
		{
			printf("输入坐标错误，请重新输入\n");
		}
	}
	if (win == row * col - EASY_COUNT)
	{
		printf("排雷成功\n");
		DisplayBoard(mine, ROW, COL);
	}
}
