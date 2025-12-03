#ifndef LINKING_H
#define LINKING_H

#include<stdio.h>
#include"board.h"

typedef struct link{
    int r1 , r2 , c1 , c2 ;
    int player ;
}link ;

extern link link_array[100000];
extern int link_counter;

void link_two_points(int r1, int r2, int c1, int c2, int player);
void auto_link(int r, int c, int player, cell_state board_state[24][24], cell_color color[24][24]);
int check_intersection(int r1, int c1, int r2, int c2, int R1, int C1, int R2, int C2);
int valid_distance(int r1, int c1, int r2, int c2);
int can_link(int r1, int c1, int r2, int c2, cell_state board_state[24][24], cell_color color[24][24], int player);
int link_exists(int r1, int c1, int r2, int c2);

#endif