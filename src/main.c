#include<stdio.h>
#include<string.h>

#include"../include/board.h"
#include"../include/utils.h"

int main(){
    cell_color color[24][24];
    cell_state state[24][24];

    char ***board = create_board(state , color);
    int pl = 1 ;//1 is for red and 2 is for blue
    while(1){
        print_board(board , state , color);
        print_turn(pl);
        int choice = print_menu();
        if(choice == 1) marker(board , pl , state, color);
        else if(choice == 2){
            printf("Good_Bye....\n");
            printf("Exiting...\n");
            break ;
        }
        if(pl == 1) pl = 2 ;
        else pl = 1;
    }

    free_board(&board);
}