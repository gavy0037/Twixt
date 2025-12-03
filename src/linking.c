#include<stdio.h>

#include"../include/linking.h"

link link_array[1000000] ;
int link_counter = 0 ;


int abs1(int a){
    return a > 0 ? a : -a ;
}


//check if the pegs are within the kinght move distance
int valid_distance(int r1, int c1, int r2, int c2){
    int a = abs1(r2-r1);
    int b = abs1(c2-c1);

    return (b == 2 && a== 1) || (a ==2 && b == 1);
}


//check if a point x is between other points a and b
int is_middle(int x , int a , int b){
    if(a < b) return (x>a && x<b);
    return (x > b && x < a);
}
/*
given four points check if they intersect or not
first link is within the small small alphabets
second link is within the capitals
*/
int check_intersection(int r1, int c1, int r2, int c2, int R1, int C1, int R2, int C2){
    int x1 = c1, y1 = r1, x2 = c2, y2 = r2 , x3 = C1, y3 = R1, x4 = C2, y4 = R2;
    
    //if any of the end point is shared the link cannot be made
    if((x1 == x3 && y1 == y3) || (x1 == x4 && y1 == y4) || (x2 == x3 && y2 == y3) || (x2 == x4 && y2 == y4)) return 0 ;

    int dx1 = x2 - x1, dy1 = y2 - y1;
    int dx2 = x4 - x3, dy2 = y4 - y3;

    //same line
    if(dx1*dy2 == dy1 * dx2) return 0 ;

    double t1 = ((x3 - x1)*dy2-(y3 - y1)*dx2)/(double)(dx1*dy2-dy1*dx2);
    double t2 = ((x3 - x1)*dy1-(y3 - y1)*dx1)/(double)(dx1*dy2-dy1*dx2);

    return (t1>0 && t1<1 && t2>0 && t2<1);
}

//check if a link alreay exist
int link_exists(int r1, int c1, int r2, int c2) {
    for(int i = 0; i < link_counter; i++) {
        if((link_array[i].r1 == r1 && link_array[i].c1 == c1 && link_array[i].r2 == r2 && link_array[i].c2 == c2) ||(link_array[i].r2 == r1 && link_array[i].c2 == c1 && link_array[i].r1 == r2 && link_array[i].c1 == c2)) {
            return 1;
        }
    }
    return 0;
}

//checks the team and the intersection with any of the other links
int can_link(int r1, int c1, int r2, int c2, cell_state board_state[24][24] , cell_color color[24][24] ,int player) {
    //first check if the link is within valid distance
    if(!valid_distance(r1, c1, r2, c2)) return 0;

    cell_color player_color = ((player == 1) ? red : blue);
    if(color[r2][c2] != player_color) return 0;//if the player is of other team then link cannot exist
    
    if(board_state[r2][c2] != marked) return 0;//if the cell is noteven marked then the link cannot be made
    
    //check for any intersection
    for(int i = 0; i < link_counter; i++) {
        if(link_array[i].player != player) {
            if(check_intersection(r1, c1, r2, c2, link_array[i].r1, link_array[i].c1, link_array[i].r2, link_array[i].c2)) {
                return 0;
            }
        }
    }
    
    if(link_exists(r1, c1, r2, c2)) return 0; //if link already exists
    return 1;
}


//first check if the link is already their if not then a new link is made and link_counter is increased
void link_two_points(int r1 , int r2 , int c1 , int c2 , int player){
    if(link_exists(r1, c1, r2, c2)) return;

    link_array[link_counter].r1 = r1 ;
    link_array[link_counter].c1 = c1 ;
    link_array[link_counter].c2 = c2 ;
    link_array[link_counter].r2 = r2 ;
    link_array[link_counter].player = player ;

    link_counter++;
}


/*explore all the 8 knight moves where the player can link
if the newrow and newcol are within board and marked with the same team then check if the link can be made if yes the link is made
*/
void auto_link(int r , int c ,int player , cell_state board_state[24][24] , cell_color color[24][24]){

    int row[8] = {-2,-1, 1 , 2 , 2,  1 , -1 , -2};
    int col[8] = {1 , 2, 2 , 1 ,-1, -2 , -2 , -1};

    for(int i  = 0 ; i < 8 ; i++){
        int newrow = r + row[i];
        int newcol = c + col[i];
        if(newrow < 0 || newcol <0 || newrow >23 || newcol > 23) continue ;//if the coordinates are not within board then continue

        if(board_state[newrow][newcol] == marked && color[newrow][newcol] == color[r][c]){
            if(can_link(r, c, newrow, newcol, board_state, color,player)){
                link_two_points(r, newrow, c, newcol, player);
            }
        }
    }
}