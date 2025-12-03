#include<stdio.h>

#include"../include/linking.h"

int abs(int a){
    return a > 0 ? a : -a ;
}

int is_valid_link_distance(int r1, int c1, int r2, int c2){
    int a = abs(r2-r1);
    int b = abs(c2-c1);

    return (b == 2 && a== 1) || (a ==2 && b == 1);
}

int is_middle(int x , int a , int b){
    if(a < b) return (x>a && x<b);
    return (x > b && x < a);
}

int check_intersection(int r1, int c1, int r2, int c2, int R1, int C1, int R2, int C2){
    int x1 = c1, y1 = r1, x2 = c2, y2 = r2 , x3 = C1, y3 = R1, x4 = C2, y4 = R2;
    
    //end point sharing
    if((x1 == x3 && y1 == y3) || (x1 == x4 && y1 == y4) || (x2 == x3 && y2 == y3) || (x2 == x4 && y2 == y4)) return 0 ;

    int dx1 = x2 - x1, dy1 = y2 - y1;
    int dx2 = x4 - x3, dy2 = y4 - y3;

    //same line
    if(dx1*dy2 == dy1 * dx2) return 0 ;

    double t1 = ((x3 - x1)*dy2-(y3 - y1)*dx2)/(double)(dx1*dy2-dy1*dx2);
    double t2 = ((x3 - x1)*dy1-(y3 - y1)*dx1)/(double)(dx1*dy2-dy1*dx2);

    return (t1>0 && t1<1 && t2>0 && t2<1);
}

int link_exists(int r1, int c1, int r2, int c2) {
    for(int i = 0; i < link_counter; i++) {
        if((link_array[i].r1 == r1 && link_array[i].c1 == c1 && link_array[i].r2 == r2 && link_array[i].c2 == c2) ||(link_array[i].r2 == r1 && link_array[i].c2 == c1 && link_array[i].r1 == r2 && link_array[i].c1 == c2)) {
            return 1;
        }
    }
    return 0;
}

void remove_link(int r1 , int c1 , int r2 , int c2){
    for(int i = 0; i < link_counter; i++) {
        if((link_array[i].r1 == r1 && link_array[i].c1 == c1 && link_array[i].r2 == r2 && link_array[i].c2 == c2) ||(link_array[i].r2 == r1 && link_array[i].c2 == c1 && link_array[i].r1 == r2 && link_array[i].c1 == c2)) {
            //Shift other links
            for(int j = i; j < link_counter - 1; j++) {
                link_array[j] = link_array[j + 1];
            }
            link_counter--;
            return;
        }
    }
}

int can_link(int r1, int c1, int r2, int c2, cell_state board_state[24][24] , cell_color color[24][24] ,int player) {
    //check if two pegs can be linked
    if(!is_valid_link_distance(r1, c1, r2, c2)) return 0;

    cell_color player_color = ((player == 1) ? red : blue);
    if(color[r2][c2] != player_color) return 0;
    
    if(board_state[r2][c2] != marked) return 0;
    
    // Check for link intersection with opponent links
    for(int i = 0; i < link_counter; i++) {
        if(link_array[i].player != player) {
            if(check_intersection(r1, c1, r2, c2, link_array[i].r1, link_array[i].c1, link_array[i].r2, link_array[i].c2)) {
                return 0;
            }
        }
    }
    
    if(link_exists(r1, c1, r2, c2)) return 0; // link already exists
    return 1;
}


void link_two_points(int r1 , int r2 , int c1 , int c2 , int player){
    if(link_exists(r1, c1, r2, c2)) return;

    link_array[link_counter].r1 = r1 ;
    link_array[link_counter].c1 = c1 ;
    link_array[link_counter].c2 = c2 ;
    link_array[link_counter].r2 = r2 ;
    link_array[link_counter].player = player ;

    link_counter++;
}

void auto_link(int r , int c ,int player , cell_state board_state[24][24] , cell_color color[24][24]){
    int row[8] = {-2,-1, 1 , 2 , 2,  1 , -1 , -2};
    int col[8] = {1 , 2, 2 , 1 ,-1, -2 , -2 , -1};
    for(int i  = 0 ; i < 8 ; i++){
        int newrow = r + row[i];
        int newcol = c + col[i];
        if(newrow < 0 || newcol <0 || newrow >23 || newcol > 23) continue ;

    if(board_state[newrow][newcol] == marked && color[newrow][newcol] == color[r][c])
        if(can_link(r, c, newrow, newcol, board_state, color,player)){
            link_two_points(r, newrow, c, newcol, player);
        }
    }
}