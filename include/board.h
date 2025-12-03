#ifndef BOARD_H
#define BOARD_H

#define RED   "\x1b[31m"
#define BLUE  "\x1b[34m"
#define COLOR_RESET "\x1b[0m"
#define YELLOW "\x1b[93m"


typedef enum state{
  unmarked = 0 ,
  marked,
  linked1,
  linked2
  //linked 1 means it will contain only one * and liked 2 means i will contain 2 stars
  //so that i can easily see if it is the peg of same player or opposite player
} cell_state ;

typedef enum color{
  white,
  red,
  blue
} cell_color ;

//for the above two enums i will make a 2d array of them in main file itself

char*** create_board(cell_state state[24][24] ,cell_color color[24][24]) ;//also create a link 2d array

void print_board(char ***board , cell_state state[24][24] ,cell_color color[24][24] );
int mark_board(char ***board , int row , int col , char *s , cell_state states[24][24],cell_color colors[24][24] ,int pl);//will pass the s according to the move made by specific player
void free_board(char ****board);

#endif