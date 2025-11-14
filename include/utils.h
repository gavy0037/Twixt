#ifndef UTILS_H
#define UTILS_H

#include"../include/board.h"

int print_menu();
void print_turn(int pl);

void marker(char ***board ,int pl , cell_state state[24][24]  , cell_color color[24][24]);

#endif