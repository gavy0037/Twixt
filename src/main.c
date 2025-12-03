#include<stdio.h>
#include<string.h>

#include"../include/board.h"
#include"../include/utils.h"
#include"../include/linking.h"

int main(){
    cell_color color[24][24];
    cell_state state[24][24];

    char ***board = create_board(state , color);
    int pl = 1 ;//1 is for red and 2 is for blue
    
    printf("** IMPORTANT INSTRUCTION **\nRed connects top and bottom , Blue connects left and right\n");

    while(1){
        print_board(board , state , color);
        print_turn(pl);
        int choice = print_menu();
        if(choice == 1){
            int row, col;
            int marked = 0;
            do{
                printf("Enter Row and Column you want to mark: ");
                int a = scanf("%d %d", &row, &col);
                getchar();
                while(row > 24 || col > 24 || row < 1 || col < 1 || a != 2) {
                    printf("Please enter valid row and column : ");
                    a = scanf("%d %d", &row, &col);
                }
                if(pl == 1) {
                    marked = mark_board(board, row, col, "X", state, color, pl);
                } else if(pl == 2) {
                    marked = mark_board(board, row, col, "O", state, color, pl);
                }
                if(!marked) {
                    printf("Cell already marked\nSelect another cell\n");
                }
            } while(!marked);
            auto_link(row - 1, col - 1, pl, state, color);
            if(check_win(state, color, pl)) {
                print_board(board, state, color);
                if(pl == 1) {
                    printf("%sRED WINS!%s\n", RED, COLOR_RESET);
                } else {
                    printf("%sBLUE WINS!%s\n", BLUE, COLOR_RESET);
                }
                break;
            }
            pl = (pl == 1) ? 2 : 1;
        }
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