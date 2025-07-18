// File Name: Minesweeper.cpp
// Author: Zhang Anjun
// Date: 2025-07-18
// Version: 1.1
// © 2025 Zhang Anjun. All rights reserved.

#include "Minesweeper.h"

int main()
{
    static bool firstGame = true;
    if (firstGame) {
        std::cout << "Welcome to Minesweeper" << std::endl;
        firstGame = false;
        minesweeper();
    }
    int menuChoice = 1;
    while (menuChoice == 1) {
        std::cout << "[1] Play again" << std::endl
            << "[2] Exit" << std::endl
            << std::endl
            << "Please enter your choice: ";
        char ch;
        if (!(std::cin >> menuChoice)) {
            std::cin.clear();
            while (std::cin.get(ch) && ch != '\n') {}
            invalidOption();
            continue;
        }
        while (std::cin.get(ch) && ch != '\n') {}
        if (menuChoice == 1)
        {
            minesweeper();
        }
    }
    return 0;
}

void copyrightNotice() {
    std::cout << "Author: Zhang Anjun" << std::endl
        << "Version: 1.1" << std::endl
        << "(C) 2025 Zhang Anjun. All rights reserved."
        << std::endl << std::endl;
}

void minesweeper()
{
    char command;
    int row = 0, col = 0, mineCount = 10;
    const int maxRow = 8;
    const int maxCol = 8;
    Cell board[maxRow + 1][maxCol + 1] = {};
    Cell(*ptrBoard)[maxCol + 1] = board;
    int result = 0; // 0 == in progress, 1 == win, 2 == lose
    initBoard(board, maxRow, maxCol);
    while (result == 0) {
        displayBoard(board, result, row, col, maxRow, maxCol, mineCount);
        prompt(command, row, col, maxRow, maxCol);
        flipCell(board, command, row, col, maxRow, maxCol);
        result = getResult(board, maxRow, maxCol, mineCount);
    }
    displayBoard(board, result, row, col, maxRow, maxCol, mineCount);
    copyrightNotice();
    if (result == 1) {
        std::cout << "Congratulations, you won this game!" << std::endl;
    }
    else if (result == 2) {
        std::cout << "Sorry, you lost this game." << std::endl;
    }
}

void initBoard(Cell(*ptrBoard)[9], const int maxRow, const int maxCol) {
    genMine(ptrBoard, maxRow, maxCol);
    int row = 0, col = 0;
    while (row <= maxRow) {
        while (col <= maxCol) {
            ptrBoard[row][col].status = 0;
            int mineFlag[2] = {};
            countCell(ptrBoard, mineFlag, row, col, maxRow, maxCol);
            ptrBoard[row][col].mineNum = mineFlag[0];
            col = col + 1;
        }
        col = 0;
        row = row + 1;
    }
}

int random(const int lBound, const int uBound) {
    std::uniform_int_distribution<int> dist(lBound, uBound);
    return dist(randomEngine());
}

static std::mt19937& randomEngine() {
    static std::mt19937 eng{ std::random_device{}() };
    return eng;
}

void genMine(Cell(*ptrBoard)[9], const int maxRow, const int maxCol) {
    int i = 0, mine = 10, mineRow = 0, mineCol = 0;
    while (mine > 0) {
        mineRow = random(0, maxRow);
        mineCol = random(0, maxCol);
        if (!(ptrBoard[mineRow][mineCol].mine)) {
            ptrBoard[mineRow][mineCol].mine = true;
            mine = mine - 1;
        }
    }
}

void countCell(Cell(*ptrBoard)[9], int (*mineFlag), const int row, const int col, const int maxRow, const int maxCol) {
    int i = -1, j = -1;
    while (i <= 1) {
        while (j <= 1) {
            if (row + i >= 0 && col + j >= 0 && row + i <= maxRow && col + j <= maxCol && !(i == 0 && j == 0)) {
                if (ptrBoard[row + i][col + j].mine) {
                    mineFlag[0] = mineFlag[0] + 1;
                }
                if (ptrBoard[row + i][col + j].status == 1) {
                    mineFlag[1] = mineFlag[1] + 1;
                }
            }
            j = j + 1;
        }
        j = -1;
        i = i + 1;
    }
}

void prompt(char &command, int &row, int &col, const int maxRow, const int maxCol)
{
    bool valid = false;
    char ch;
    while (!valid) {
        std::cout << "Command: F - Flag a cell   R - Reveal a cell" << std::endl;
        std::cout << "Input format: Command Row Column" << std::endl;
        std::cout << "Please enter your move: ";
        if (!(std::cin >> command >> row >> col)) {
            std::cin.clear();
            while (std::cin.get(ch) && ch != '\n') {}
            invalidOption();
            continue;
        }
        while (std::cin.get(ch) && ch != '\n') {}
        if ((command == 'R' || command == 'F' || command == 'r' || command == 'f') && row <= maxRow+1 && row >= 1 && col <= maxCol+1 && col >= 1) {
            valid = true;
            if (command == 'r') {
                command = 'R';
            }
            else if (command == 'f') {
                command = 'F';
            }
            row = row - 1;
            col = col - 1;
        }
        else {
            invalidOption();
        }
    }
}

void displayBoard(Cell(&board)[9][9], int result, const int row, const int col, const int maxRow, const int maxCol, int mineCount) {
    int i = 0, j = 0, flippedCell = 0, mineRemain = mineCount;
    std::cout << std::endl << "   | ";
    while (j <= maxCol) {
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        std::cout << j + 1;
        resetColor();
        std::cout << " | ";
        j = j + 1;
    }
    std::cout << std::endl;
    j = 0;
    while (i <= maxRow) {
        displayLine(maxRow, maxCol);
        std::cout << " ";
        setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        std::cout << i + 1;
        resetColor();
        std::cout << " ";
        while (j <= maxCol) {
            if (board[i][j].status == 0 && result == 0) {
                std::cout << "| # ";
            }
            else if (board[i][j].status == 1) {
                std::cout << "| ";
                setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
                std::cout << "F";
                resetColor();
                std::cout << " ";
                mineRemain = mineRemain - 1;
            }
            else if (board[i][j].mine && (board[i][j].status == 2 || result != 0)) {
                std::cout << "| ";
                setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
                std::cout << "*";
                resetColor();
                std::cout << " ";
            }
            else if (!(board[i][j].mine) && (board[i][j].status == 2 || result != 0)) {
                std::cout << "| ";
                if (board[i][j].mineNum != 0) {
                    setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    std::cout << board[i][j].mineNum;
                    resetColor();
                }
                else {
                    std::cout << " ";
                }
                std::cout << " ";
            }
            j = j + 1;
        }
        std::cout << "|" << std::endl;
        j = 0;
        i = i + 1;
    }
    displayLine(maxRow, maxCol);
    std::cout << std::endl;
    if (result == 0) {
        std::cout << "Mines remaining: " << mineRemain << std::endl << std::endl;
    }
}

void displayLine(const int maxRow, const int maxCol) {
    int j = 0;
    std::cout << "---";
    while (j <= maxCol) {
        std::cout << "+---";
        j = j + 1;
    }
    std::cout << "+" << std::endl;
}

void setColor(WORD color) {
    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void resetColor() {
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void flipCell(Cell(&board)[9][9], const char command, const int row, const int col, const int maxRow, const int maxCol) {
    if (command == 'F') {
        if (board[row][col].status == 0) {
            board[row][col].status = 1;
        }
        else if (board[row][col].status == 1) {
            board[row][col].status = 0;
        }
        else {
            invalidOption();
        }
    }
    else if (command == 'R') {
        if (board[row][col].status == 0 && !(board[row][col].mine)) {
            board[row][col].status = 2;
            chord(board, row, col, maxRow, maxCol);
        }
        else if (board[row][col].status == 0 && board[row][col].mine) {
            board[row][col].status = 2;
        }
        else if (board[row][col].status == 1) {
            board[row][col].status = 0;
        }
        else if (board[row][col].status == 2 && (!board[row][col].mine)) {
            chord(board, row, col, maxRow, maxCol);
        }
        else {
            invalidOption();
        }
    }
}

void chord(Cell(&board)[9][9], const int row, const int col, const int maxRow, const int maxCol) {
    int mineFlag[2] = {};
    countCell(board, mineFlag, row, col, maxRow, maxCol);
    if (mineFlag[1] == board[row][col].mineNum) {
        int i = -1, j = -1;
        while (i <= 1) {
            while (j <= 1) {
                if (row + i >= 0 &&
                    col + j >= 0 &&
                    row + i <= maxRow &&
                    col + j <= maxCol &&
                    !(i == 0 && j == 0) &&
                    board[row + i][col + j].status == 0) {
                    flipCell(board, 'R', row + i, col + j, maxRow, maxCol);
                }
                j = j + 1;
            }
            j = -1;
            i = i + 1;
        }
    }
}

int getResult(Cell(&board)[9][9], const int maxRow, const int maxCol, int mineCount) {
    int i = 0, j = 0, flippedCell = 0, result = 0;
    while (i <= maxRow) {
        while (j <= maxCol) {
            if (board[i][j].mine && board[i][j].status == 2) {
                result = 2;
            }
            else if (!(board[i][j].mine) && board[i][j].status == 2) {
                flippedCell = flippedCell + 1;
            }
            j = j + 1;
        }
        j = 0;
        i = i + 1;
    }
    if (result == 0 && flippedCell == (maxRow + 1) * (maxCol + 1) - mineCount) {
        result = 1;
    }
    return result;
}

void invalidOption() {
    std::cout << "Invalid option. Please try again." << std::endl << std::endl;
}
