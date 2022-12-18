#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include "utils.h"
#include <string>
#include <vector>

const wchar_t* top_left = L"\u2554";
const wchar_t* top_right = L"\u2557";
const wchar_t* bot_left = L"\u255A";
const wchar_t* bot_right = L"\u255D";
const wchar_t* horizontal = L"\u2550";
const wchar_t* vertical = L"\u2551";
const wchar_t* cross = L"\u256C";

const wchar_t* checker_white = L"o";
const wchar_t* checker_white_queen = L"O";
const wchar_t* checker_red = L"\u25A0";
const wchar_t* checker_red_queen = L"\u2588";
const wchar_t* white_space = L"\u2588";

int height = 8 * 2 + 1, width = 8 * 2 + 1;

int colour = 0;
int boardX, boardY;

HANDLE  hConsole;

void clear_screen(char fill = ' ') {
    COORD tl = { 0,0 };
    CONSOLE_SCREEN_BUFFER_INFO s;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(console, fill, cells, tl, &written);
    FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(console, tl);
}

void initialize() {

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void print_board(int board[][8], int cursor[]) {
    int currentBoardSpot;
    _setmode(_fileno(stdout), _O_U16TEXT);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i % 2 != 0 && j % 2 != 0) {
                boardX = (j - 1) / 2;
                boardY = (i - 1) / 2;

                currentBoardSpot = board[boardY][boardX];

                if (cursor[0] == boardX && cursor[1] == boardY) colour = 160;
                else colour = 0;
                SetConsoleTextAttribute(hConsole, 7 + colour);


                if (currentBoardSpot == 0)
                {
                    if (colour == 160) {
                        SetConsoleTextAttribute(hConsole, colour);
                    }
                    else {
                        SetConsoleTextAttribute(hConsole, 240);
                    }
                    wprintf(L" ");
                }
                else if (currentBoardSpot == 2) wprintf(checker_white);
                else if (currentBoardSpot == 3) {
                    SetConsoleTextAttribute(hConsole, 12 + colour);
                    wprintf(checker_red);
                }
                else if (currentBoardSpot == 4) wprintf(checker_white_queen);
                else if (currentBoardSpot == 5) {
                    SetConsoleTextAttribute(hConsole, 12 + colour);
                    wprintf(checker_red_queen);
                }
                else wprintf(L" ");

            }
            else {
                SetConsoleTextAttribute(hConsole, 8);
                if (i == 0) {
                    if (j == 0) wprintf(top_left);
                    else if (j == width - 1) wprintf(top_right);
                    else wprintf(horizontal);
                }
                else if (i == height - 1) {
                    if (j == 0) wprintf(bot_left);
                    else if (j == width - 1) wprintf(bot_right);
                    else wprintf(horizontal);
                }
                else if (j == 0 || j == width - 1) wprintf(vertical);
                else if (j % 2 == 0 && i % 2 == 0) wprintf(cross);
                else if (j % 2 == 0) wprintf(vertical);
                else if (i % 2 == 0) wprintf(horizontal);
                else if (i < 3) wprintf(checker_red);
                else if (i > height - 3) wprintf(checker_white);
            }
        }
        wprintf(L"\n");
    }
}

void printRed(std::string message) {
    _setmode(_fileno(stdout), _O_TEXT);
    SetConsoleTextAttribute(hConsole, 12);
    const char* c = message.c_str();
    printf("%s\n", c);
}

void printWhite(std::string message) {
    _setmode(_fileno(stdout), _O_TEXT);
    SetConsoleTextAttribute(hConsole, 7);
    const char* c = message.c_str();
    printf("%s\n", c);
}