/* Twixt - Colorful Terminal Version with Boundaries and Link Visualization
Cross-platform (Windows & Linux) with ANSI color support

Simple playable Twixt-like game for two players in the terminal.
- Board size adjustable via BOARD_N (default 11)
- Players: X (RED - connect Top-Bottom) and O (BLUE - connect Left-Right)
- Pegs placed on integer grid points. Links are allowed between two pegs separated by a knight move
  (dx,dy) = (1,2) or (2,1) in any direction. Links cannot cross any existing link.
- After each placement, all legal non-crossing links between that new peg and that player's existing pegs
  are automatically added.
- Win is when a player has a connected path of their links between their two target sides.

Compile (Linux/Mac): gcc -O2 -o twixt twixt.c -lm
Run: ./twixt

Compile (Windows): gcc -O2 -o twixt twixt.c
Run: twixt.exe

Controls:
  p r c    -- place a peg at row r col c (1-based)
  show     -- print board
  links    -- list all links
  help     -- show help
  quit     -- exit
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#define BOARD_N 11
#define MAX_LINKS 2000

/* ANSI Color Codes */
#define RED     "\x1b[91m"
#define BLUE    "\x1b[94m"
#define GREEN   "\x1b[92m"
#define YELLOW  "\x1b[93m"
#define CYAN    "\x1b[96m"
#define MAGENTA "\x1b[95m"
#define WHITE   "\x1b[97m"
#define BLACK   "\x1b[30m"
#define RESET   "\x1b[0m"
#define BOLD    "\x1b[1m"

enum { EMPTY=0, P1=1, P2=2 };

int N = BOARD_N;
int board[BOARD_N+1][BOARD_N+1];

typedef struct { int r1,c1,r2,c2,player; } Link;
Link links[MAX_LINKS];
int link_count = 0;

/* Enable colors on Windows */
#ifdef _WIN32
void enable_ansi_colors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#else
void enable_ansi_colors() {
    /* Linux/Mac terminals support ANSI colors by default */
}
#endif

/* Utility functions */
int abs_i(int x){ return x<0? -x : x; }
int valid_cell(int r,int c){ return r>=1 && r<=N && c>=1 && c<=N; }

int is_knight_move(int r1,int c1,int r2,int c2){
    int dr = abs_i(r1-r2);
    int dc = abs_i(c1-c2);
    return (dr==1 && dc==2) || (dr==2 && dc==1);
}

/* Intersection detection */
double orient(double ax,double ay,double bx,double by,double cx,double cy){
    return (bx-ax)*(cy-ay) - (by-ay)*(cx-ax);
}

int on_segment(double ax,double ay,double bx,double by,double px,double py){
    if (fabs(orient(ax,ay,bx,by,px,py)) > 1e-9) return 0;
    double minx = (ax < bx) ? ax : bx;
    double maxx = (ax > bx) ? ax : bx;
    double miny = (ay < by) ? ay : by;
    double maxy = (ay > by) ? ay : by;
    if (px < minx - 1e-9 || px > maxx + 1e-9) return 0;
    if (py < miny - 1e-9 || py > maxy + 1e-9) return 0;
    return 1;
}

int segments_intersect(int a1x,int a1y,int a2x,int a2y,int b1x,int b1y,int b2x,int b2y){
    double a1ax = a1x, a1ay = a1y;
    double a2ax = a2x, a2ay = a2y;
    double b1ax = b1x, b1ay = b1y;
    double b2ax = b2x, b2ay = b2y;

    double o1 = orient(a1ax,a1ay,a2ax,a2ay,b1ax,b1ay);
    double o2 = orient(a1ax,a1ay,a2ax,a2ay,b2ax,b2ay);
    double o3 = orient(b1ax,b1ay,b2ax,b2ay,a1ax,a1ay);
    double o4 = orient(b1ax,b1ay,b2ax,b2ay,a2ax,a2ay);

    if ((o1*o2 < -1e-9) && (o3*o4 < -1e-9)) return 1;
    if (fabs(o1) < 1e-9 && on_segment(a1ax,a1ay,a2ax,a2ay,b1ax,b1ay)) return 1;
    if (fabs(o2) < 1e-9 && on_segment(a1ax,a1ay,a2ax,a2ay,b2ax,b2ay)) return 1;
    if (fabs(o3) < 1e-9 && on_segment(b1ax,b1ay,b2ax,b2ay,a1ax,a1ay)) return 1;
    if (fabs(o4) < 1e-9 && on_segment(b1ax,b1ay,b2ax,b2ay,a2ax,a2ay)) return 1;
    return 0;
}

int link_crosses_existing(int r1,int c1,int r2,int c2){
    for(int i=0;i<link_count;i++){
        Link *L = &links[i];
        if ((L->r1==r1 && L->c1==c1) || (L->r1==r2 && L->c1==c2) || (L->r2==r1 && L->c2==c1) || (L->r2==r2 && L->c2==c2)) continue;
        if (segments_intersect(c1, r1, c2, r2, L->c1, L->r1, L->c2, L->r2)) return 1;
    }
    return 0;
}

int add_link(int r1,int c1,int r2,int c2,int player){
    if (link_count >= MAX_LINKS) return 0;
    if (r1 > r2 || (r1==r2 && c1>c2)){ int tr=r1, tc=c1; r1=r2; c1=c2; r2=tr; c2=tc; }
    links[link_count].r1 = r1;
    links[link_count].c1 = c1;
    links[link_count].r2 = r2;
    links[link_count].c2 = c2;
    links[link_count].player = player;
    link_count++;
    return 1;
}

void auto_link_for_new_peg(int r,int c,int player){
    for(int rr=1; rr<=N; rr++){
        for(int cc=1; cc<=N; cc++){
            if (rr==r && cc==c) continue;
            if (board[rr][cc] != player) continue;
            if (!is_knight_move(r,c,rr,cc)) continue;
            if (link_crosses_existing(r,c,rr,cc)) continue;
            int exists = 0;
            for(int i=0;i<link_count;i++){
                Link *L = &links[i];
                if ((L->r1==r && L->c1==c && L->r2==rr && L->c2==cc) || (L->r1==rr && L->c1==cc && L->r2==r && L->c2==c)) { exists=1; break; }
            }
            if (!exists) add_link(r,c,rr,cc,player);
        }
    }
}

int player_has_won(int player){
    int visited[BOARD_N+1][BOARD_N+1]; 
    memset(visited,0,sizeof(visited));
    int queue_sz = (N*N)+5; 
    int qrs[queue_sz], qcs[queue_sz]; 
    int qh=0, qt=0;

    if (player==P1){
        for(int c=1;c<=N;c++){
            if (board[1][c]==P1){ qrs[qt]=1; qcs[qt]=c; visited[1][c]=1; qt++; }
        }
    } else {
        for(int r=1;r<=N;r++){
            if (board[r][1]==P2){ qrs[qt]=r; qcs[qt]=1; visited[r][1]=1; qt++; }
        }
    }

    while(qh<qt){
        int r=qrs[qh], c=qcs[qh]; qh++;
        if (player==P1 && r==N) return 1;
        if (player==P2 && c==N) return 1;
        for(int i=0;i<link_count;i++){
            Link *L = &links[i]; 
            if (L->player != player) continue;
            int nr=-1, nc=-1;
            if (L->r1==r && L->c1==c){ nr=L->r2; nc=L->c2; }
            else if (L->r2==r && L->c2==c){ nr=L->r1; nc=L->c1; }
            if (nr!=-1){ if (!visited[nr][nc]){ visited[nr][nc]=1; qrs[qt]=nr; qcs[qt]=nc; qt++; } }
        }
    }
    return 0;
}

int link_exists(int r1, int c1, int r2, int c2, int player){
    for(int i=0; i<link_count; i++){
        Link *L = &links[i];
        if (L->player != player) continue;
        if ((L->r1==r1 && L->c1==c1 && L->r2==r2 && L->c2==c2) || 
            (L->r1==r2 && L->c1==c2 && L->r2==r1 && L->c2==c1)) 
            return 1;
    }
    return 0;
}

char get_link_char(int player){
    return (player == P1) ? '*' : '#';
}

void print_board(){
    printf("\n");
    
    /* Create expanded canvas for drawing links */
    int canvas_rows = 2*N + 1;
    int canvas_cols = 4*N + 1;
    char canvas[canvas_rows][canvas_cols];
    
    /* Initialize canvas */
    for(int i = 0; i < canvas_rows; i++){
        for(int j = 0; j < canvas_cols; j++){
            canvas[i][j] = ' ';
        }
    }
    
    /* Place pegs on canvas */
    for(int r = 1; r <= N; r++){
        for(int c = 1; c <= N; c++){
            int row = 2*(r-1);
            int col = 4*(c-1);
            if(board[r][c] == P1) canvas[row][col] = 'X';
            else if(board[r][c] == P2) canvas[row][col] = 'O';
            else canvas[row][col] = '.';
        }
    }
    
    /* Draw links on canvas */
    for(int i = 0; i < link_count; i++){
        Link *L = &links[i];
        int r1 = L->r1, c1 = L->c1;
        int r2 = L->r2, c2 = L->c2;
        int player = L->player;
        
        int y1 = 2*(r1-1);
        int x1 = 4*(c1-1);
        int y2 = 2*(r2-1);
        int x2 = 4*(c2-1);
        
        /* Draw line from (x1,y1) to (x2,y2) */
        int steps = (abs_i(x2-x1) > abs_i(y2-y1)) ? abs_i(x2-x1) : abs_i(y2-y1);
        
        for(int s = 1; s < steps; s++){
            int x = x1 + (x2-x1) * s / steps;
            int y = y1 + (y2-y1) * s / steps;
            
            if(x >= 0 && x < canvas_cols && y >= 0 && y < canvas_rows){
                if(canvas[y][x] == ' '){
                    if(player == P1) canvas[y][x] = '~';
                    else canvas[y][x] = '=';
                }
            }
        }
    }
    
    /* Top boundary */
    printf("  %s╔", CYAN);
    for(int c=1; c<=N; c++){
        printf("════");
    }
    printf("╗%s\n", RESET);
    
    /* Column headers */
    printf("  %s║%s", CYAN, RESET);
    for(int c=1;c<=N;c++) printf(" %s%2d%s ", YELLOW, c, RESET);
    printf("%s║%s\n", CYAN, RESET);
    
    /* Row separator after header */
    printf("  %s╠", CYAN);
    for(int c=1; c<=N; c++){
        printf("════");
    }
    printf("╣%s\n", CYAN, RESET);
    
    /* Print board with links */
    for(int r = 1; r <= N; r++){
        int canvas_row = 2*(r-1);
        
        printf("%s%2d%s %s║%s", YELLOW, r, RESET, CYAN, RESET);
        
        for(int c = 1; c <= N; c++){
            int canvas_col = 4*(c-1);
            char piece = canvas[canvas_row][canvas_col];
            
            char *color = WHITE;
            if(piece == 'X') color = RED;
            else if(piece == 'O') color = BLUE;
            else if(piece == '~') color = RED;
            else if(piece == '=') color = BLUE;
            
            printf(" %s%c%s", color, piece, RESET);
            
            /* Print middle parts of links */
            if(c < N){
                char mid1 = canvas[canvas_row][canvas_col+1];
                char mid2 = canvas[canvas_row][canvas_col+2];
                
                char *mid1_color = WHITE;
                char *mid2_color = WHITE;
                
                if(mid1 == '~' || mid1 == '=') mid1_color = (mid1 == '~') ? RED : BLUE;
                if(mid2 == '~' || mid2 == '=') mid2_color = (mid2 == '~') ? RED : BLUE;
                
                if(mid1 == ' ') mid1 = ' ';
                if(mid2 == ' ') mid2 = ' ';
                
                printf("%s%c%c%s", mid1_color == WHITE ? "" : mid1_color, mid1, mid2, RESET);
            }
        }
        printf(" %s║%s\n", CYAN, RESET);
        
        /* Print vertical middle rows */
        if(r < N){
            int mid_canvas_row = 2*(r-1) + 1;
            printf("   %s║%s", CYAN, RESET);
            
            for(int c = 1; c <= N; c++){
                int canvas_col = 4*(c-1);
                char piece = canvas[mid_canvas_row][canvas_col];
                
                char *color = WHITE;
                if(piece == '~') color = RED;
                else if(piece == '=') color = BLUE;
                
                if(piece == ' ') piece = ' ';
                printf(" %s%c%s", color, piece, RESET);
                
                /* Print middle parts */
                if(c < N){
                    char mid1 = canvas[mid_canvas_row][canvas_col+1];
                    char mid2 = canvas[mid_canvas_row][canvas_col+2];
                    
                    char *mid1_color = WHITE;
                    char *mid2_color = WHITE;
                    
                    if(mid1 == '~' || mid1 == '=') mid1_color = (mid1 == '~') ? RED : BLUE;
                    if(mid2 == '~' || mid2 == '=') mid2_color = (mid2 == '~') ? RED : BLUE;
                    
                    if(mid1 == ' ') mid1 = ' ';
                    if(mid2 == ' ') mid2 = ' ';
                    
                    printf("%s%c%c%s", mid1_color == WHITE ? "" : mid1_color, mid1, mid2, RESET);
                }
            }
            printf(" %s║%s\n", CYAN, RESET);
        }
    }
    
    /* Bottom boundary */
    printf("  %s╚", CYAN);
    for(int c=1; c<=N; c++){
        printf("════");
    }
    printf("╝%s\n", RESET);
    
    printf("\n%sTOTAL LINKS: %d%s (Red ~ for X, Blue = for O)\n", GREEN, link_count, RESET);
}

void list_links(){
    printf("\n%s=== Existing Links ===%s\n", BOLD, RESET);
    if (link_count == 0){
        printf("  (no links)\n");
        return;
    }
    for(int i=0;i<link_count;i++){
        Link *L=&links[i];
        char *player_color = (L->player==P1) ? RED : BLUE;
        char player_char = (L->player==P1) ? 'X' : 'O';
        printf("  %d: (%d,%d) %s──%s (%d,%d)  player=%s%c%s\n", 
               i+1, L->r1, L->c1, player_color, RESET, L->r2, L->c2, 
               player_color, player_char, RESET);
    }
}

void show_help(){
    printf("\n%s╔════════════════════════════════╗%s\n", CYAN, RESET);
    printf("%s║   TWIXT GAME - COMMANDS        ║%s\n", CYAN, RESET);
    printf("%s╚════════════════════════════════╝%s\n", CYAN, RESET);
    printf("  %sp r c%s    -- place a peg at row r col c (1-based)\n", BOLD, RESET);
    printf("  %sshow%s     -- print board\n", BOLD, RESET);
    printf("  %slinks%s    -- list all links\n", BOLD, RESET);
    printf("  %shelp%s     -- show this help\n", BOLD, RESET);
    printf("  %squit%s     -- exit game\n", BOLD, RESET);
}

int main(){
    enable_ansi_colors();
    
    printf("\n");
    printf("%s╔════════════════════════════════╗%s\n", MAGENTA, RESET);
    printf("%s║     WELCOME TO TWIXT GAME      ║%s\n", MAGENTA, RESET);
    printf("%s╚════════════════════════════════╝%s\n\n", MAGENTA, RESET);
    
    printf("Board size: %s%dx%d%s\n", BOLD, N, N, RESET);
    printf("%sPlayer X (RED)%s   - Connect Top to Bottom (rows 1 to %d)\n", RED, RESET, N);
    printf("%sPlayer O (BLUE)%s  - Connect Left to Right (columns 1 to %d)\n", BLUE, RESET, N);
    printf("\nType '%shelp%s' for commands.\n\n", BOLD, RESET);

    memset(board, 0, sizeof(board));
    char line[256];
    int turn = 0;
    
    print_board();
    
    while(1){
        int player = (turn==0)?P1:P2;
        char *player_color = (player==P1) ? RED : BLUE;
        char player_char = (player==P1) ? 'X' : 'O';
        
        printf("\n%s%c's Turn%s - Enter command: ", player_color, player_char, RESET);
        fflush(stdout);
        
        if (!fgets(line, sizeof(line), stdin)) break;
        
        char cmd[16]; 
        int r=0, c=0;
        sscanf(line, "%15s %d %d", cmd, &r, &c);
        
        for(char *p=cmd; *p; ++p) *p = tolower(*p);
        
        if (strcmp(cmd,"p")==0 || strcmp(cmd,"place")==0){
            if (r==0 || c==0){ 
                printf("%s[ERROR]%s Usage: p r c (both 1..%d)\n", RED, RESET, N); 
                continue; 
            }
            if (!valid_cell(r,c)){ 
                printf("%s[ERROR]%s Cell out of bounds. Valid range: 1..%d\n", RED, RESET, N); 
                continue; 
            }
            if (board[r][c] != EMPTY){ 
                printf("%s[ERROR]%s Cell already occupied.\n", RED, RESET); 
                continue; 
            }
            
            board[r][c] = player;
            auto_link_for_new_peg(r,c,player);
            print_board();
            
            if (player_has_won(player)){
                printf("\n%s╔═══════════════════════════════╗%s\n", GREEN, RESET);
                printf("%s║  %s*** PLAYER %c WINS! ***%s         ║%s\n", GREEN, BOLD, player_char, RESET, GREEN, RESET);
                printf("%s╚════════════════════════════════╝%s\n\n", GREEN, RESET);
                break;
            }
            
            turn = 1-turn;
        } 
        else if (strcmp(cmd,"show")==0){
            print_board();
        } 
        else if (strcmp(cmd,"links")==0){
            list_links();
        } 
        else if (strcmp(cmd,"help")==0){
            show_help();
        } 
        else if (strcmp(cmd,"quit")==0 || strcmp(cmd,"exit")==0){
            printf("\n%sGoodbye!%s\n\n", GREEN, RESET);
            break;
        } 
        else {
            printf("%s[ERROR]%s Unknown command '%s'. Type 'help' for options.\n", RED, RESET, cmd);
        }
    }
    
    return 0;
}