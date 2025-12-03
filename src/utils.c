#include<stdio.h>
#include<string.h>
#include"../include/utils.h"
#include"../include/board.h"
#include"../include/linking.h"

int print_menu(){
    printf("1.Mark\n2.Quit\nEnter Choice Number : ");
    int c ;
    scanf("%d",&c);
    while(c != 1 && c != 2){
        printf("Please enter 1 or 2 as choice : ");
        scanf("%d",&c);
    }
    return c ;
}

void print_turn(int pl){
    if(pl == 1){
        printf("It's (X)  turn\n");
    }else if(pl == 2){
        printf("It's (O)  turn\n");
    }
}



/*

i will check winning by going through every row

first i will mark all the row 1 pegs that are marked
then i will mark all the pegs that are linked with row 1 pegs as reachable
then i will mark all the pegs linked to row 2

this way i will go row by row and then check at the last row if it is reacable or not

if reachable then the player wins

the same is done for the other player just the coloums and rows are exchanged
*/
int check_win(cell_state board_state[24][24],cell_color color[24][24] ,int player){
    cell_color ply = ((player == 1) ? red : blue);

    int reachable[24][24];
    for(int i =0 ;i < 24 ; i++){
        for(int j = 0 ;j < 24 ; j++) reachable[i][j] = 0 ;
    }

    if(ply == red){
        //we go from top to bottom
        for(int i = 0 ; i < 24 ; i++){
            if(board_state[0][i] == marked && color[0][i] == ply){
                reachable[0][i] = 1 ;
            }
        }

        //we go row by row
        for(int r = 0 ; r < 24 ; r++){
            for(int j = 0 ; j < 24 ; j++){
                if(reachable[r][j]){//if this cell is reachable
                    //mark all the linked cells from the current cell
                    for(int i = 0 ; i < link_counter ; i++){
                        if(link_array[i].player == player){
                            int a = -1, b = -1;
                            if(link_array[i].r1 == r && link_array[i].c1 == j){
                                a = link_array[i].r2 ;
                                b = link_array[i].c2 ;
                            }else if(link_array[i].r2 == r && link_array[i].c2 == j) {
                                a = link_array[i].r1;
                                b = link_array[i].c1;
                            }

                            //mark all the pegs linked to the current that are not yet reachable
                            if(a != -1 && !reachable[a][b]) reachable[a][b] = 1 ;
                        }
                    }
                }
            }
        }

        //now see if any cell in the last row is reachable
        for(int j = 0 ; j < 24 ; j++){
            if(reachable[23][j]) return 1 ;
        }
    }else{
        //mark all the column 0 as reachable
        for(int i = 0 ; i < 24 ; i++){
            if(board_state[i][0] == marked && color[i][0] == ply){
                reachable[i][0] = 1 ;
            }
        }

        //go through all the columns
        for(int c = 0 ; c < 24 ; c++){
            for(int i = 0 ; i < 24 ; i++){
                if(reachable[i][c]){
                    for(int k = 0 ; k < link_counter ; k++){
                        if(link_array[k].player == player){
                            int a = -1  , b = -1 ;
                            if(link_array[k].r1 == i && link_array[k].c1 == c){
                                a = link_array[k].r2;
                                b = link_array[k].c2;
                            }else if(link_array[k].r2 == i && link_array[k].c2 == c) {
                                a = link_array[k].r1;
                                b = link_array[k].c1;
                            }

                            if(a != -1 && !reachable[a][b]) reachable[a][b] = 1; 
                        }
                    }
                }
            }
        }

        for(int i = 0 ;i < 24 ; i++){
            if(reachable[i][23]) return 1; 
        }
    }

    return 0 ;
}