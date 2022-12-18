#pragma once
#include<vector>
#include <string>

void clear_screen(char fill = ' ');

void initialize();

void print_board(int board[][8], int cursor[]);

void printRed(std::string message);

void printWhite(std::string message);