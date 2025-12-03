#include<stdio.h>

#include"../include/linking.h"

void link_two_points(int r1 , int r2 , int c1 , int c2 , int player){
    for(int i = 0 ; i < link_counter ; i++){
        if((link_array[i].r1 == r1 && link_array[i].c1 == c1 && link_array[i].r2 == r2 && link_array[i].c2 == c2 ) || (link_array[i].r2 == r1 && link_array[i].c2 == c1 && link_array[i].r1 == r2 && link_array[i].c1 == c2 )){
            return ;//link exist
        }
    }
    link_array[link_counter].r1 = r1 ;
    link_array[link_counter].c1 = c1 ;
    link_array[link_counter].c2 = c2 ;
    link_array[link_counter].r2 = r2 ;
    link_array[link_counter].player = player ;

    link_counter++;
}

void auto_link(int r , int c ,int player , cell_state board_state[24][24] , which_team team[24][24]){
    int row[8] = {-2,-1, 1 , 2 , 2,  1 , -1 , -2};
    int col[8] = {1 , 2, 2 , 1 ,-1, -2 , -2 , -1};
    for(int i  = 0 ; i < 8 ; i++){
        int newrow = r + row[i];
        int newcol = c + col[i];
        if(newrow < 0 || newcol <0 || newrow >23 || newcol > 23) continue ;

    if(board_state[newrow][newcol] == marked && team[newrow][newcol] == player)
       link_two_points(r, newrow, c, newcol, player);
    }
}

int is_middle(int x , int a , int b){
    if(a < b) return (x>a && x<b);
    return (x > b && x < a);
}

int check_intersection(int r1,int c1,int r2,int c2,int  R1,int C1,int R2,int C2){
    int x1 = c1 , y1 = r1 , x2 = c2 , y2 = r2 , x3 = C1 , y3 = R1 , x4 = C2 , y4 = R2 ;

    //diagonals
    int dx1 = x2-x1 , dy1 = y2-y1 , dx2 = x4-x3 , dy2 = y4-y3 ;
    
    if((dx1 != 0 && dy1 != 0) && dx2 != 0 && dy2 != 0){
        int s1 = (dy1 == dx1 ? 1 : -1);
        int s2 = (dy2 == dx2 ? 1 : -1);
        

    }
}
