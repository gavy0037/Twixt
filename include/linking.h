#ifndef LINKING_H
#define LINKING_H

#include<stdio.h>
#include"board.h"

typedef struct link{
    int r1 , r2 , c1 , c2 ;
    int player ;
}link ;

link link_array[100000];
int link_counter = 0 ;



void link_two_points(int r1 , int r2 , int c1 , int c2 , int player);
void auto_link(int r , int c ,int player , cell_state board_state[24][24] , which_team team[24][24]);
int check_intersection(int r1,int c1,int r2,int c2,int  R1,int C1,int R2,int C2);


#endif