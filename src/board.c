#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include"../include/board.h"

char*** create_board(cell_state state[24][24] ,cell_color color[24][24]){
    char*** board = malloc(sizeof(char**)*24);
    for(int i = 0 ; i < 24 ; i++){
        board[i] = malloc(sizeof(char*)*24);
        for(int j = 0 ; j  < 24 ;  j++){
            board[i][j] = malloc(sizeof(char)*4);
        }
    }
    
    for(int i = 0 ; i < 24 ; i++){
        for(int j = 0 ; j < 24 ; j++){
            state[i][j] = unmarked ;
            color[i][j] = white ;
            strcpy(board[i][j] ,".");
        }
    }
    return board ;
}

void print_board(char ***board , cell_state state[24][24] , cell_color colors[24][24]){
    printf("   ");
    for(int i = 0 ; i<24 ; i++ ) printf("%-4d" , i+1);
    printf("\n\n");
    for(int i = 0 ; i < 24 ; i++){
        printf("%-4d",i+1);
        for(int j = 0 ; j < 24 ; j++){
            if(colors[i][j] == red) printf("%s",RED);
            else if(colors[i][j] == blue) printf("%s",BLUE);
            printf("%-4s",board[i][j]);

            printf("%s" , COLOR_RESET);
        }
        printf("\n\n");
    }
}

void mark_board(char ****board , int row , int col , char *s , cell_state state[24][24] , cell_color colors[24][24] ,int pl){
    strcpy(*board[row-1][col-1],s) ;
    state[row-1][col] = marked ;
    if(pl == 1){
        colors[row-1][col-1] = red ;
    }else if(pl == 2){
        colors[row-1][col-1] = blue; 
    }
}

void free_board(char ****board){
    char ***b = *board ;

    for(int i = 0 ; i < 24 ; i++){
        for(int j = 0 ; j < 24 ; j++){
            free(b[i][j]);
        }
        free(b[i]);
    }
    free(b);
}
