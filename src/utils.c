#include<stdio.h>
#include<string.h>
#include"../include/utils.h"
#include"../include/board.h"

int print_menu(){
    printf("1.Mark\n2.Quit\nEnter Choice Number : ");
    int c ;
    scanf("%d",&c);
    return c ;
}

void print_turn(int pl){
    if(pl == 1){
        printf("It's Red's turn\n");
    }else if(pl == 2){
        printf("It's Blue's turn\n");
    }
}

void marker(char ****board ,int pl , cell_state state[24][24]  , cell_color color[24][24]){
    printf("Enter Row and Coloumn you want to mark: ");
    int row , col ;
    scanf("%d %d",&row , &col);
    char m[2];
    if(pl == 1){
        strcpy(m , "X");
    }else if(pl == 2){
        strcpy( m , "O");
    }
    mark_board(board , row , col , m , state , color , pl);
}