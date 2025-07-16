// File Name: Minesweeper.h
// Author: Zhang Anjun
// Date: 2025-07-16
// Version: 1.0
// © 2025 Zhang Anjun. All rights reserved.

#pragma once
#include <iostream>
#include <random>
#include <windows.h>

struct Cell {
    bool mine;
    int status;     // 0 unchecked; 1 flagged; 2 flipped
    int mineNum;
};

void minesweeper();

void initBoard(Cell(*ptrBoard)[9], const int maxRow, const int maxCol);

int random(const int lBound, const int uBound);

static std::mt19937& randomEngine();

void genMine(Cell(*ptrBoard)[9], const int maxRow, const int maxCol);

void countCell(Cell(*ptrBoard)[9], int(*mineFlag), const int row, const int col, const int maxRow, const int maxCol);

void prompt(char& command, int& row, int& col, const int maxRow, const int maxCol);

void displayBoard(Cell(&board)[9][9], int result, const int row, const int col, const int maxRow, const int maxCol, int mineCount);

void displayLine(const int maxRow, const int maxCol);

void setColor(WORD color);

void resetColor();

void flipCell(Cell(&board)[9][9], const char command, const int row, const int col, const int maxRow, const int maxCol);

void chord(Cell(&board)[9][9], const int row, const int col, const int maxRow, const int maxCol);

int getResult(Cell(&board)[9][9], const int maxRow, const int maxCol, int mineCount);

void invalidOption();