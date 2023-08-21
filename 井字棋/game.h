#pragma once

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//定义棋盘大小
#define ROW 3
#define COL 3

//初始化棋盘
void Initboard(char board[ROW][COL], int row, int col);

//打印棋盘
void Displayboard(char board[ROW][COL], int row, int col);

//玩家走棋
void Playermove(char board[ROW][COL], int row, int col);

//电脑走棋
void Computermove(char board[ROW][COL], int row, int col);

//判断胜负
char IsWin(char board[ROW][COL], int row, int col);