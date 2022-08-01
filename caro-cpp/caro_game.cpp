#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <unistd.h>     // sleep function

#include "config.h"
#include "bot_207.h"

using namespace std;

int board_game[HEIGHT][WIDTH];
Point win_path[5];
Point mostRecentMove;
int mostRecentPlayer;

void init_board_game();
void go_to_xy(Point p);
void set_text_color(int color);
void draw_tile(Point p, int color);
int who_win();
void draw_win_path(int winner);
void play_game();
Point player1_run();
Point player2_run();

void draw_background();

int main(){
    srand (time(NULL));

    set_text_color(WHITE_COLOR);
    char c;
    do{
        cout<<"Are you ready?(y/n)"<<endl;
        cin>>c;
    }while(c != 'y');

    play_game();
    return 0;
}

/* 
 khởi tạo bàn cờ tất cả các ô lưu giá trị 0 
 các ô đã được đi sẽ lưu giá trị 1, -1 tương ứng với 2 người chơi
*/
void init_board_game(){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            board_game[i][j] = 0;
            draw_tile(Point(BLOCK_RATIO*j, i), BLACK_COLOR);
        }
    }
}

void set_text_color(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/* chắc là di chuyển con trỏ dến đây */
void go_to_xy(Point p){
    COORD coord;
    coord.X = p.x;
    coord.Y = p.y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/* Vẽ ra win path */
void draw_win_path(int winner){

    bool turn = true;
    int color;
    int blink_count = 10;
    while(blink_count >= 0){
        if(winner == 1){
            if(turn) color = WHITE_COLOR;
            else color = BLACK_COLOR;
        } else {
            if(turn) color = RED_COLOR;
            else color = BLACK_COLOR;
        }
        for(int i = 0; i < 5; i++){
            draw_tile(win_path[i], color);
        }
        Sleep(500);
        turn = !turn;
        blink_count--;
    }

}

/* Vẽ kí hiệu `O| X` */
void draw_tile(Point p, int color){
    go_to_xy(p);
    set_text_color(color);
    for(int i = 0; i < BLOCK_RATIO; i++) {
        if(color == WHITE_COLOR)
            cout<<"O";
        else cout<<"X";
    }
}

// 1. player 1 win . 0 means hoa, -1. player 2 win
int who_win(){
    // YOUR CODE HERE
    // check list con (1| -1) kề liền đạt đến 5 element chưa?
    // và thêm điều kiện là 1 trong trong 2 đầu là 0
    int ci = mostRecentMove.x;
    int cj = mostRecentMove.y;

    // upper limit, lower limit
    int low, up;                 

    /*================ CHECK NGANG ==================*/
    low = cj, up = cj;

    // loop dừng khi ko gặp cờ giống tại [ci, cj]
    // cout << board_game[ci][low] << " " << low  << " " << up << "\n";
    while (board_game[ci][low] == mostRecentPlayer) 
    {
        if (low == -1) break;
        low --;
    }
    while (board_game[ci][up] == mostRecentPlayer) {
        if (up == WIDTH) break;
        up ++;

    }

    // cout << "low:" << low << ", up:" << up << "\n";

    // xuất hiện liên tiếp > 6 cờ
    if (up - low >= 7) return mostRecentPlayer;

    // xuất hiện liên tiếp 5 cờ, và 1 hướng mở
    if (up - low == 6) {
        if (low >= 0 && board_game[ci][low] == 0) {
            return mostRecentPlayer;
        } 
        if (up <= WIDTH-1 && board_game[ci][up] == 0) {
            return mostRecentPlayer;
        }
    } 


    /*================= CHECK DỌC ===================*/
    low = ci, up = ci;

    // loop dừng khi ko gặp cờ giống tại [ci, cj]
    while (board_game[low][cj] == mostRecentPlayer) {
        if (low == -1) break;
        low --;
    }
    while (board_game[up][cj] == mostRecentPlayer) {
        if (up == HEIGHT) break;
        up ++;
    }

    // xuất hiện liên tiếp > 6 cờ
    if (up - low >= 7) return mostRecentPlayer;

    // xuất hiện liên tiếp 5 cờ, và 1 hướng mở
    if (up - low == 6) {
        if (low >= 0 && board_game[low][cj] == 0) {
            return mostRecentPlayer;
        }
        if (up <= HEIGHT-1 && board_game[up][cj] == 0) {
            return mostRecentPlayer;
        }
    }


    /*============== CHECK CHÉO ================*/
    int low_i, low_j, up_i, up_j;

    /*============== TRÊN CHÉO XUỐNG ================*/
    low_i = ci, low_j = cj, up_i = ci, up_j = cj;

    while (board_game[low_i][low_j] == mostRecentPlayer) {
        /* tìm chặn dưới */
        if (low_i == -1 || low_j == -1)      break;
        low_i --;
        low_j --;
    }
    while (board_game[up_i][up_j] == mostRecentPlayer) {
        /* tìm chặn trên */
        if (up_i == HEIGHT || up_j == WIDTH) break;
        up_i ++;
        up_j ++;

    }

    // xuất hiện liên tiếp > 6 cờ
    if (up_i - low_i >= 7) return mostRecentPlayer;

    // xuất hiện liên tiếp 5 cờ, và 1 hướng mở
    if (up_i - low_i == 6) {
        // check chặn dưới mở
        if (low_i >= 0 && low_j >= 0 && board_game[low_i][low_j] == 0) {
            return mostRecentPlayer;
        }
        // check chặn trên mở
        if (up_i <= HEIGHT-1 && up_j <= HEIGHT-1 && board_game[up_i][up_j] == 0) {
            return mostRecentPlayer;
        }
    }


    /*=============== DƯỚI CHÉO LÊN =================*/
    low_i = ci, low_j = cj, up_i = ci, up_j = cj;

    // cout << board_game[ci][cj] << endl;

    while (board_game[low_i][low_j] == mostRecentPlayer) {
        if (low_i == HEIGHT || low_j == -1)      break;
        low_i ++;
        low_j --;

    }
    while (board_game[up_i][up_j] == mostRecentPlayer) {
        if (up_i == -1 || up_j == WIDTH)         break;
        up_i --;
        up_j ++;
    }

    // cout << "low:(" << low_i << ", " << low_j << "), up:(" << up_i << ", " << up_j << ")" << "\n";

    // xuất hiện liên tiếp > 6 cờ
    if (up_j - low_j >= 7) return mostRecentPlayer;

    // xuất hiện liên tiếp 5 cờ, và 1 hướng mở
    if (up_j - low_j == 6) {
        // check chặn dưới mở
        if (low_i <= HEIGHT-1 && low_j >= 0 && board_game[low_i][low_j] == 0) {
            return mostRecentPlayer;
        } 
        // check chặn trện mở
        if (up_i >= 0 && up_j <= WIDTH-1 && board_game[up_i][up_j] == 0) {
            return mostRecentPlayer;
        }
    }

    // HÒA
    return 0;
}

//goc toa do (0;0) o goc tren ben trai
void draw_background(){
    set_text_color(BLUE_COLOR);

    //Ve khung cho ban co
    for(int i=0; i <= WIDTH; i++){
        go_to_xy(Point(10+i, 3));
        cout << char(220);
        go_to_xy(Point(10+i, HEIGHT));
        cout << char(220);
    }

    for(int i=4 ;i<= HEIGHT;i++){
        go_to_xy(Point(10, i));
        cout << char(219);
        go_to_xy(Point(10+WIDTH,i));
        cout << char(219);
    }

    // Ve cac o trong ban co
    Point p;
    int x, y;
    for(int i = 11; i <= WIDTH; i += 2){
        for(int j = 4; j <= HEIGHT; j++){
            x = (i-1)/2;
            y = j;
            p.x = i;
            p.y = j;
            if(x % 2==0){
                if(y % 2==0){
                    draw_tile(p, BLACK_COLOR);
                }else{
                    draw_tile(p, WHITE_COLOR);
                }
            }
            else{
                if(y % 2==0){
                    draw_tile(p, WHITE_COLOR);
                }
                else{
                    draw_tile(p, BLACK_COLOR);
                }
            }
        }
    }
    set_text_color(WHITE_COLOR);
}

Point player1_run(){
    return player_rand(board_game, 1);
    //return player_baseline(board_game, 1);
}

Point player2_run(){
     return player_rand(board_game, -1);
    //return player_baseline(board_game, -1);
}

void play_game(){
    bool turn_player1 = true;
    int turn_limit = 3000;
    int row, col, winner, repeat_pos;
    Point position;
    char c;
    do{
        init_board_game();
        turn_player1 = true;
        turn_limit = 3000;

        // run bàn cờ
        while(turn_limit > 0){
            repeat_pos = 5;

            // lượt người chơi 1
            if(turn_player1){
                mostRecentPlayer = 1;

                // nhận vị trí đặt cờ chưa được viết, 
                // thằng ngu đi quá 5 lần vào vị trí đã viết thì reset game
                do{
                    position = player1_run();
                    if(repeat_pos == 0){
                        cout<<"player 1 is so stupid"<<endl;
                        goto reset_game;
                    }
                    repeat_pos--;
                }while(board_game[position.x][position.y] != 0);

                // đánh dấu vị trí đã đi bằng giá trị 1 trong board
                board_game[position.x][position.y] = 1;
                draw_tile(Point(BLOCK_RATIO*position.y, position.x), WHITE_COLOR);
            }

            // lượt người chơi 2 
            else {
                mostRecentPlayer = -1;
                do{
                    position = player2_run();
                    if(repeat_pos == 0){
                        cout<<"player 2 is so stupid"<<endl;
                        goto reset_game;
                    }
                    repeat_pos--;
                }while(board_game[position.x][position.y] != 0);
                board_game[position.x][position.y] = -1;
                draw_tile(Point(BLOCK_RATIO*position.y, position.x), RED_COLOR);
            }

            // kiểm tra ai win
            mostRecentMove = position;
            winner = who_win();
            if(winner != 0){
                go_to_xy(Point(WIDTH/2, HEIGHT+5));
                cout<<winner<<" win"<<endl;
                draw_win_path(winner);
                break;
            }

            // đặt lại lượt, đến lượt đối thủ chơi
            turn_player1 = !turn_player1;

            turn_limit--;
            Sleep(PAUSE_TIME);
        }
        reset_game:
        cin>>c;
    }while(c == 'y');
}

