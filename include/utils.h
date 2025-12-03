#ifndef UTILS_H

#define UTILS_H

#include"../include/board.h"

int print_menu();
void print_turn(int pl);
int check_win(cell_state board_state[24][24],cell_color color[24][24] ,int player);

#endif