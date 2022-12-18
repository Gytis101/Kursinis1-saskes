#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include "screen.h"
#include "utils.h"
#include <conio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>

// 0 - empty white space
// 1 - empty black space
// 2 - white checker
// 3 - black checker
// 4 - white queen
// 5 - black queen
int board[8][8];
// cursors coordinates
int cursor[2] = { 0, 0 };
// selected checker. -1;-1 if not selected
int checker_selected[2] = { -1, -1 };
// whose turn it is
bool white_turn = true;
// if a capture is a must
bool must_capture = false;
std::vector<std::pair<int, int>> legal_moves;
std::vector<std::pair<int, int>> legal_captures;
std::string error_message = "\n", info_message = "\n";

int white_checkers = 12, red_checkers = 12;

void setup_board() {
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (i % 2 == 0 && j % 2 == 0) board[i][j] = 0;
            else if (i % 2 != 0 && j % 2 != 0) board[i][j] = 0;
            else if (i % 2 == 0 && j % 2 != 0) board[i][j] = 1;
            else if (i % 2 != 0 && j % 2 == 0) board[i][j] = 1;
        }
    }
}

void setup_checkers() {
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (i < 3) {
                if (i % 2 == 0 && j % 2 != 0) board[i][j] = 3;
                else if (i % 2 != 0 && j % 2 == 0) board[i][j] = 3;
            }
            else if (i > 4) {
                if (i % 2 == 0 && j % 2 != 0) board[i][j] = 2;
                else if (i % 2 != 0 && j % 2 == 0) board[i][j] = 2;
            }
        }
    }
}

std::pair<int, int> get_checker_between(int x1, int y1, int x2, int y2) {
    return std::pair<int, int>((x1 + x2) / 2, (y1 + y2) / 2);
}

void find_legal_moves(int x, int y) {
    int checker;
    int leftSpot, rightSpot;
    int originalX = x, originalY = y;

    legal_moves.clear();
    legal_captures.clear();
    checker = board[y][x];

    if (checker == 2) {
        leftSpot = board[y - 1][x - 1];
        rightSpot = board[y - 1][x + 1];
        if (leftSpot == 1 && x - 1 >= 0) legal_moves.push_back(std::pair<int, int>(x - 1, y - 1));
        else if ((leftSpot == 3 || leftSpot == 5) && x - 2 >= 0 && y - 2 >= 0 && board[y - 2][x - 2] == 1) legal_captures.push_back(std::pair<int, int>(x - 2, y - 2));

        if (rightSpot == 1 && x + 1 <= 7) legal_moves.push_back(std::pair<int, int>(x + 1, y - 1));
        else if ((rightSpot == 3 || rightSpot == 5) && x + 2 >= 0 && y - 2 >= 0 && board[y - 2][x + 2] == 1) legal_captures.push_back(std::pair<int, int>(x + 2, y - 2));
    }
    else if (checker == 3) {
        leftSpot = board[y + 1][x - 1];
        rightSpot = board[y + 1][x + 1];
        if (leftSpot == 1 && x - 1 >= 0) legal_moves.push_back(std::pair<int, int>(x - 1, y + 1));
        else if ((leftSpot == 2 || leftSpot == 4) && x - 2 >= 0 && y + 2 <= 7 && board[y + 2][x - 2] == 1) legal_captures.push_back(std::pair<int, int>(x - 2, y + 2));

        if (rightSpot == 1 && x + 1 <= 7) legal_moves.push_back(std::pair<int, int>(x + 1, y + 1));
        else if ((rightSpot == 2 || rightSpot == 4) && x + 2 >= 0 && y + 2 <= 7 && board[y + 2][x + 2] == 1) legal_captures.push_back(std::pair<int, int>(x + 2, y + 2));
    }
    else if (checker == 4) {
        x = originalX; y = originalY;
        while (x > 0 && y > 0) {
            x--; y--;
            if (board[y][x] == 1) legal_moves.push_back(std::pair<int, int>(x, y));
            else if ((board[y][x] == 3 || board[y][x] == 5) && x - 1 >= 0 && y - 1 >= 0 && board[y - 1][x - 1] == 1)
            {
                legal_captures.push_back(std::pair<int, int>(x - 1, y - 1));
                break;
            }
            else if (board[y][x] == 2 || board[y][x] == 4) break;
        }
        x = originalX; y = originalY;

        while (x < 7 && y > 0) {
            x++; y--;
            if (board[y][x] == 1) legal_moves.push_back(std::pair<int, int>(x, y));
            else if ((board[y][x] == 3 || board[y][x] == 5) && x + 1 <= 7 && y - 1 >= 0 && board[y - 1][x + 1] == 1)
            {
                legal_captures.push_back(std::pair<int, int>(x + 1, y - 1));
                break;
            }
            else if (board[y][x] == 2 || board[y][x] == 4) break;
        }
        x = originalX; y = originalY;
        while (x > 0 && y < 7) {
            x--; y++;
            if (board[y][x] == 1) legal_moves.push_back(std::pair<int, int>(x, y));
            else if ((board[y][x] == 3 || board[y][x] == 5) && x - 1 >= 0 && y + 1 <= 7 && board[y + 1][x - 1] == 1)
            {
                legal_captures.push_back(std::pair<int, int>(x - 1, y + 1));
                break;
            }
            else if (board[y][x] == 2 || board[y][x] == 4) break;
        }
        x = originalX; y = originalY;
        while (x < 7 && y < 7) {
            x++; y++;
            if (board[y][x] == 1) legal_moves.push_back(std::pair<int, int>(x, y));
            else if ((board[y][x] == 3 || board[y][x] == 5) && x + 1 <= 7 && y + 1 <= 7 && board[y + 1][x + 1] == 1)
            {
                legal_captures.push_back(std::pair<int, int>(x + 1, y + 1));
                break;
            }
            else if (board[y][x] == 2 || board[y][x] == 4) break;
        }
    }
    else if (checker == 5) {
        x = originalX; y = originalY;
        while (x > 0 && y > 0) {
            x--; y--;
            if (board[y][x] == 1) legal_moves.push_back(std::pair<int, int>(x, y));
            else if ((board[y][x] == 2 || board[y][x] == 4) && x - 1 >= 0 && y - 1 >= 0 && board[y - 1][x - 1] == 1)
            {
                legal_captures.push_back(std::pair<int, int>(x - 1, y - 1));
                break;
            }
            else if (board[y][x] == 3 || board[y][x] == 5) break;
        }
        x = originalX; y = originalY;
        while (x < 7 && y > 0) {
            x++; y--;
            if (board[y][x] == 1) legal_moves.push_back(std::pair<int, int>(x, y));
            else if ((board[y][x] == 2 || board[y][x] == 4) && x + 1 <= 7 && y - 1 >= 0 && board[y - 1][x + 1] == 1)
            {
                legal_captures.push_back(std::pair<int, int>(x + 1, y - 1));
                break;
            }
            else if (board[y][x] == 3 || board[y][x] == 5) break;
        }
        x = originalX; y = originalY;
        while (x > 0 && y < 7) {
            x--; y++;
            if (board[y][x] == 1) legal_moves.push_back(std::pair<int, int>(x, y));
            else if ((board[y][x] == 2 || board[y][x] == 4) && x - 1 >= 0 && y + 1 <= 7 && board[y + 1][x - 1] == 1)
            {
                legal_captures.push_back(std::pair<int, int>(x - 1, y + 1));
                break;
            }
            else if (board[y][x] == 3 || board[y][x] == 5) break;
        }
        x = originalX; y = originalY;
        while (x < 7 && y < 7) {
            x++; y++;
            if (board[y][x] == 1) legal_moves.push_back(std::pair<int, int>(x, y));
            else if ((board[y][x] == 2 || board[y][x] == 4) && x + 1 <= 7 && y + 1 <= 7 && board[y + 1][x + 1] == 1)
            {
                legal_captures.push_back(std::pair<int, int>(x + 1, y + 1));
                break;
            }
            else if (board[y][x] == 3 || board[y][x] == 5) break;
        }
    }
}

bool is_legal_move(int x, int y) {
    for (std::pair<int, int> i : legal_moves) {
        if (i.first == x && i.second == y) return true;
    }
    for (std::pair<int, int> i : legal_captures) {
        if (i.first == x && i.second == y) return true;
    }
    return false;
}

bool can_move() {
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (white_turn) {
                if (board[i][j] == 2 || board[i][j] == 4) {
                    find_legal_moves(j, i);
                    if (legal_captures.size() + legal_moves.size() > 0) {
                        return true;
                    }
                }
            }
            else {
                if (board[i][j] == 3 || board[i][j] == 5) {
                    find_legal_moves(j, i);
                    if (legal_captures.size() + legal_moves.size() > 0) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool is_must_capture() {
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (white_turn) {
                if (board[i][j] == 2 || board[i][j] == 4) {
                    find_legal_moves(j, i);
                    if (legal_captures.size() > 0) {
                        return true;
                    }
                }
            }
            else {
                if (board[i][j] == 3 || board[i][j] == 5) {
                    find_legal_moves(j, i);
                    if (legal_captures.size() > 0) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
// 1 up     2 right     3 down      4 left
void move(int direction) {
    if (direction == 1) cursor[1]--;
    else if (direction == 2) cursor[0]++;
    else if (direction == 3) cursor[1]++;
    else if (direction == 4) cursor[0]--;

    cursor[0] = clamp(cursor[0], 0, 7);
    cursor[1] = clamp(cursor[1], 0, 7);
}

void action() {
    int selected_spot;
    if (checker_selected[0] == -1) {
        selected_spot = board[cursor[1]][cursor[0]];
        if (selected_spot == 0 || selected_spot == 1) {
            error_message = "Can't select an empty spot.";
            return;
        }
        else if ((selected_spot == 2 || selected_spot == 4) && !white_turn) {
            error_message = "Can't select white's pieces while it is red's move.";
            return;
        }
        else if ((selected_spot == 3 || selected_spot == 5) && white_turn)
        {
            error_message = "Can't select red's pieces while it is white's move.";
            return;
        }

        checker_selected[0] = cursor[0];
        checker_selected[1] = cursor[1];
        find_legal_moves(cursor[0], cursor[1]);
        if ((legal_moves.size() + legal_captures.size() == 0) || (must_capture && legal_captures.size() == 0)) {
            error_message = "Can't select a piece which has no legal moves.";
            checker_selected[0] = -1;
            checker_selected[1] = -1;
            return;
        }
        else
        {
            checker_selected[0] = cursor[0];
            checker_selected[1] = cursor[1];
            find_legal_moves(cursor[0], cursor[1]);
        }
    }
    else {
        find_legal_moves(checker_selected[0], checker_selected[1]);
        if (is_legal_move(cursor[0], cursor[1])) {
            int checker = board[checker_selected[1]][checker_selected[0]];
            // promote if needed
            if ((cursor[1] == 0 || cursor[1] == 7) && checker < 4) {
                checker += 2;
            }

            board[cursor[1]][cursor[0]] = checker;
            board[checker_selected[1]][checker_selected[0]] = 1;

            if (abs(checker_selected[1] - cursor[1]) > 1) {
                int x, y;

                if (checker_selected[0] < cursor[0]) x = cursor[0] - 1;
                else x = cursor[0] + 1;

                if (checker_selected[1] < cursor[1]) y = cursor[1] - 1;
                else y = cursor[1] + 1;

                int lastChecker = board[y][x];
                board[y][x] = 1;

                // checks if a piece was captured
                if (lastChecker != 1) {
                    if (white_turn) {
                        red_checkers--;
                    }
                    else {
                        white_checkers--;
                    }
                    // see if multi capture possible
                    find_legal_moves(cursor[0], cursor[1]);
                    if (legal_captures.size() > 0) {
                        checker_selected[0] = cursor[0];
                        checker_selected[1] = cursor[1];
                        return;
                    }
                }
            }


            white_turn = !white_turn;
            checker_selected[0] = -1;
            checker_selected[1] = -1;
        }
    }
}

int main() {
    char key_hit;
    bool game_over = false;

    setup_board();
    setup_checkers();

    initialize();

    print_board(board, cursor);

    while (!game_over) {
        if (_kbhit()) {


            key_hit = _getch();
            error_message = info_message = "";
            if (key_hit == 'w') {
                move(1);
            }
            else if (key_hit == 'd') {
                move(2);
            }
            else if (key_hit == 's') {
                move(3);
            }
            else if (key_hit == 'a') {
                move(4);
            }
            else if (key_hit == ' ') {
                action();
            }

            if (red_checkers == 0 || (!white_turn && !can_move())) {
                printWhite("WHITE WON!");
                game_over = true;
                break;
            }
            else if (white_checkers == 0 || (white_turn && !can_move())) {
                printWhite("RED WON!");
                game_over = true;
                break;
            }

            must_capture = is_must_capture();


            clear_screen();
            print_board(board, cursor);
            if (must_capture) {
                printRed("YOU MUST CAPTURE!!!");
            }
            printWhite("White Checkers: " + std::to_string(white_checkers) + "\nRed Checkers: " + std::to_string(red_checkers));
            printWhite("");
            if (white_turn) {
                printWhite("White's turn.");
            }
            else {
                printWhite("Red's turn.");
            }

            printRed(error_message);
        }
    }
    std::cin >> key_hit;
    return 0;
}