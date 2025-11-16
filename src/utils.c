#include<stdio.h>
#include<string.h>
#include"../include/utils.h"
#include"../include/board.h"

int print_menu(){
    printf("1.Mark\n2.Quit\nEnter Choice Number : ");
    int c ;
    scanf("%d",&c);
    getchar();
    while(c != 1 && c != 2){
        printf("Please enter 1 or 2 as choice : ");
        scanf("%d",&c);
    }
    return c ;
}

void print_turn(int pl){
    if(pl == 1){
        printf("It's Red's  (X)  turn\n");
    }else if(pl == 2){
        printf("It's Blue's (O)  turn\n");
    }
}

void marker(char ***board ,int pl , cell_state state[24][24]  , cell_color color[24][24]){
    int row , col ;
    int marked = 0 ;
    do{
        printf("Enter Row and Column you want to mark: ");
        int a = scanf("%d %d",&row , &col);
        getchar();
        while(row>24 || col > 24 || row<1 || col<1 ||a != 2){
            printf("Please enter valid row and column\n");
            a = scanf("%d %d",&row , &col);
            getchar();
        }
        if(pl == 1){
            marked = mark_board(board , row , col , "X" , state , color , pl);
        }else if(pl == 2){
            marked = mark_board(board , row , col , "O" , state , color , pl);
        }
        if(!marked){
            printf("Cell already marked\nSelect another cell\n");
        }
    }while(!marked);
}