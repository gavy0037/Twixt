#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include"../include/board.h"
#include"../include/linking.h"

char*** create_board(cell_state state[24][24] ,cell_color color[24][24]){
    char*** board = malloc(sizeof(char**)*24);
    for(int i = 0 ; i < 24 ; i++){
        board[i] = malloc(sizeof(char*)*24);
        for(int j = 0 ; j  < 24 ;  j++){
            board[i][j] = malloc(sizeof(char)*32);
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
    printf("    ");
    printf("%s",YELLOW);
    for(int i = 0 ; i<24 ; i++ ) printf("%-4d" , i+1);
    printf("\n\n");
    printf("%s",COLOR_RESET);

    for(int i = 0 ; i < 24 ; i++){
        printf("%s",YELLOW);
        printf("%-4d",i+1);
        printf("%s",COLOR_RESET);
        for(int j = 0 ; j < 24 ; j++){
            int linked = 0 ;
            char to_write[10] ; 
            for(int k = 0; k < link_counter; k++) {
                if((link_array[k].r1 == i && link_array[k].c1 == j) ||(link_array[k].r2 == i && link_array[k].c2 == j)) {
                    linked = 1;
                    if(link_array[k].player == 1) {
                        strcpy(to_write, "*X*");
                    } else {
                        strcpy(to_write, "*O*");
                    }
                    break;
                }
            }
            if(linked) {
                if(colors[i][j] == red) printf("%s", RED);
                else if(colors[i][j] == blue) printf("%s", BLUE);
                printf("%-4s", to_write);
                printf("%s", COLOR_RESET);
            } else {
                if(colors[i][j] == red) printf("%s", RED);
                else if(colors[i][j] == blue) printf("%s", BLUE);
                printf("%-4s", board[i][j]);
                printf("%s", COLOR_RESET);
            }
        }
        printf("\n\n");
    }
}

int mark_board(char ***board , int row , int col , char *s , cell_state state[24][24] , cell_color colors[24][24] ,int pl){
    if(state[row-1][col-1] != unmarked){
        return 0 ;
    }
    strcpy(board[row-1][col-1],s) ;
    state[row-1][col-1] = marked ;
    if(pl == 1){
        colors[row-1][col-1] = red ;
    }else if(pl == 2){
        colors[row-1][col-1] = blue; 
    }
    return 1 ;
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
