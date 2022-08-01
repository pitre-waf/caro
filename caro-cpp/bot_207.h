#ifndef BOTBASELINE
#define BOTBASELINE

#include <iostream>
#include <stdlib.h>
#include <vector>
#include "config.h"

bool emptyBoard = true;

// player_id = 1 || -1
Point player_rand(int board_game[][WIDTH], int player_id){
    int row, col;
    row = rand() % HEIGHT;
    col = rand() % WIDTH;
    return Point(row, col);
}


// tìm điểm đánh mà win luôn
// tìm điểm 0 trên board
// nước đánh tạo thành 6 cờ liên tiếp => WIN luôn
// nước đánh tạo thành 5 cờ liên tiếp, và 1 hướng mở => WIN luôn
// nước đánh tạo thành 4 cờ liên tiếp, 2 hướng mở, và 1 hướng mở có 2 ô trống
// nước đánh tạo thành nước đôi 3 cờ liên tiếp, 2 hướng mở, 2 hướng mở có 2 ô trống
// thì => OKE< WIN.
Point check_win(int board_game[][WIDTH], int player_id) {
    for (int i=0; i < HEIGHT; i++) {
        for (int j=0; j < WIDTH; j++) {
            if (board_game[i][j] == 0) {

                // gán giá trị player_id cho vị trí [i, j]
                board_game[i][j] = player_id;

                int ci =i;
                int cj = j;
                int low, up;            // upper limit, lower limit

                /*================ CHECK NGANG ==================*/
                low = cj, up = cj;

                // loop dừng khi ko gặp cờ giống tại [ci, cj]
                // cout << board_game[ci][low] << " " << low  << " " << up << "\n";
                while (board_game[ci][low] == player_id) 
                {
                    if (low == -1) break;
                    low --;
                }
                while (board_game[ci][up] == player_id) {
                    if (up == WIDTH) break;
                    up ++;
                }

                // cout << "low:" << low << ", up:" << up << "\n";

                // nước đánh tạo thành 6 cờ liên tiếp
                if (up - low == 7) {
                    board_game[i][j] = 0;
                    return Point(i, j);
                }

                // nước đánh tạo thành 5 cờ liên tiếp, và 1 hướng mở
                if (up - low == 6) {
                    // kiểm tra xuất hiện 1 trong 2 hướng mở
                    if (low >= 0 && board_game[ci][low] == 0 || up <= WIDTH-1 && board_game[ci][up] == 0) {
                        board_game[i][j] = 0;
                        return Point(i, j);
                    }
                } 

                // nước đánh tạo thành 4 cờ liên tiếp, 2 hướng mở, và cả 2 hướng mở có 2 ô trống
                if (up - low == 5) {
                    // kiểm tra 2 hướng mở có ô trống đầu tiên
                    if (low >= 0 && board_game[ci][low] == 0 && up <= WIDTH-1 && board_game[ci][up] == 0) {
                        // kiểm tra 2 hướng mở có ô trống thứ 2
                        if (low-1 >= 0 && board_game[ci][low-1] == 0 && up+1 <= WIDTH-1 && board_game[ci][up+1] == 0) {
                            board_game[i][j] = 0;
                            return Point(i, j);
                        }
                    }
                }


                /*================= CHECK DỌC ===================*/
                low = ci, up = ci;

                // loop dừng khi ko gặp cờ giống tại [ci, cj]
                while (board_game[low][cj] == player_id) {
                    if (low == -1) break;
                    low --;
                }
                while (board_game[up][cj] == player_id) {
                    if (up == HEIGHT) break;
                    up ++;
                }

                // nước đánh tạo thành 6 cờ liên tiếp 
                if (up - low == 7) {
                    board_game[i][j] = 0;
                    return Point(i, j);
                }

                // nước đánh tạo thành 5 cờ liên tiếp, và 1 hướng mở
                if (up - low == 6) {
                    if (low >= 0 && board_game[low][cj] == 0 || up <= HEIGHT-1 && board_game[up][cj] == 0) {
                        board_game[i][j] = 0;
                        return Point(i, j);
                    }
                }

                // nước đánh tạo thành 4 cờ liên tiếp, 2 hướng mở, và cả 2 hướng mở có 2 ô trống
                if (up - low == 5) {
                    // kiểm tra 2 hướng mở có ô trống đầu tiên
                    if (low >= 0 && board_game[ci][low] == 0 && up <= HEIGHT-1 && board_game[ci][up] == 0) {
                        // kiểm tra 2 hướng mở có ô trống thứ 2
                        if (low-1 >= 0 && board_game[ci][low-1] == 0 && up+1 <= HEIGHT-1 && board_game[ci][up+1] == 0) {
                            board_game[i][j] = 0;
                            return Point(i, j);
                        }
                    }
                }
                

                /*============== CHECK CHÉO ================*/
                int low_i, low_j, up_i, up_j;

                /*============== TRÊN CHÉO XUỐNG ================*/
                low_i = ci, low_j = cj, up_i = ci, up_j = cj;

                while (board_game[low_i][low_j] == player_id) {
                    /* tìm chặn dưới */
                    if (low_i == -1 || low_j == -1)  break;
                    low_i --;
                    low_j --;
                }
                while (board_game[up_i][up_j] == player_id) {
                    /* tìm chặn trên */
                    if (up_i == HEIGHT || up_j == WIDTH) break;
                    up_i ++;
                    up_j ++;

                }

                // nước đánh tạo thành 6 cờ liên tiếp
                if (up_i - low_i == 7) {
                    board_game[i][j] = 0;
                    return Point(i, j);
                }

                // nước đánh tạo thành 5 cờ liên tiếp, và 1 hướng mở
                if (up_i - low_i == 6) {
                    // check chặn dưới mở
                    if (low_i >= 0 && low_j >= 0 && board_game[low_i][low_j] == 0) {
                        board_game[i][j] = 0;
                        return Point(i, j);
                    }
                    // check chặn trên mở
                    if (up_i <= HEIGHT-1 && up_j <= HEIGHT-1 && board_game[up_i][up_j] == 0) {
                        board_game[i][j] = 0;
                        return Point(i, j);
                    }
                }

                // nước đánh tạo thành 4 cờ liên tiếp, 2 hướng mở, và cả 2 hướng mở có 2 ô trống
                if (up_i - low_i == 5) {
                    // kiểm tra 2 hướng mở có ô trống đầu tiên
                    if (low_i >= 0 && low_j >= 0 && board_game[low_i][low_j] == 0 && 
                        up_i <= HEIGHT-1 && up_j <= HEIGHT-1 && board_game[up_i][up_j] == 0) {
                        
                        // kiểm tra 2 hướng mở có ô trống thứ 2
                        if (low_i-1 >= 0 && low_j-1 >= 0 && board_game[low_i-1][low_j-1] == 0 &&
                            up_i+1 <= HEIGHT-1 && up_j+1 <= HEIGHT-1 && board_game[up_i+1][up_j+1] == 0) {
                                board_game[i][j] = 0;
                                return Point(i, j);
                        }
                    }
                }


                /*=============== DƯỚI CHÉO LÊN =================*/
                low_i = ci, low_j = cj, up_i = ci, up_j = cj;

                while (board_game[low_i][low_j] == player_id) {
                    if (low_i == HEIGHT || low_j == -1) break;
                    low_i ++;
                    low_j --;

                }
                while (board_game[up_i][up_j] == player_id) {
                    if (up_i == -1 || up_j == WIDTH) break;
                    up_i --;
                    up_j ++;
                }

                // cout << "low:(" << low_i << ", " << low_j << "), up:(" << up_i << ", " << up_j << ")" << "\n";

                // nước đánh tạo thành 6 cờ liên tiếp
                if (up_j - low_j == 7) {
                    board_game[i][j] = 0;
                    return Point(i, j);
                }
                // nước đánh tạo thành 5 cờ liên tiếp, và 1 hướng mở
                if (up_j - low_j == 6) {
                    // check chặn dưới mở
                    if (low_i <= HEIGHT-1 && low_j >= 0 && board_game[low_i][low_j] == 0) {
                        board_game[i][j] = 0;
                        return Point(i, j);
                    } 
                    // check chặn trện mở
                    if (up_i >= 0 && up_j <= WIDTH-1 && board_game[up_i][up_j] == 0) {
                        board_game[i][j] = 0;
                        return Point(i, j);
                    }
                }

                // nước đánh tạo thành 4 cờ liên tiếp, 2 hướng mở, và cả 2 hướng mở có 2 ô trống
                if (up_j - low_j == 5) {
                    // kiểm tra 2 hướng mở có ô trống đầu tiên
                    if (low_i <= HEIGHT-1 && low_j >= 0 && board_game[low_i][low_j] == 0 && 
                        up_i >= 0 && up_j <= WIDTH-1 && board_game[up_i][up_j] == 0) {
                        
                        // kiểm tra 2 hướng mở có ô trống thứ 2
                        if (low_i-1 <= HEIGHT-1 && low_j-1 >= 0 && board_game[low_i-1][low_j-1] == 0 &&
                            up_i+1 >= 0 && up_j+1 <= WIDTH-1 && board_game[up_i+1][up_j+1] == 0) {
                                board_game[i][j] = 0;
                                return Point(i, j);
                        }
                    }
                }

                // đặt lại đúng giá trị khởi tạo cho board_game
                board_game[i][j] = 0;
            }
        }
    }
    
    return Point(-1, -1);
}


// tìm điểm đánh để người chơi còn lại ko win luôn
Point defend(int board_game[][WIDTH], int player_id) {
    return check_win(board_game, -player_id);
}


// coi hướng 12h là nhìn lên camera của máy tính
int evaluateScoreOfPoint(int board_game[][WIDTH], int player_id, Point pos) {
    // int HEIGHT = 8, WIDTH = 8;
    int score = 0;
    int ci = pos.x, cj = pos.y;
    int coef_2_cell = 1;          // hệ số nhân điểm khi tạo thành 2 ô liền kề
    int coef_3_cell = 3;          // hệ số nhân điểm khi tạo thành 3 ô liền kề
    int coef_4_cell = 6;          // hệ số nhân điểm khi tạo thành 4 ô liền kề
    int bonus_unblock_score = 3;
    int bonus_super_unblock_score = 8;


    /* ================ ĐIỂM CHIỀU NGANG ======================= */

    // ĐIỂM THEO HƯỚNG 9h
    int _9h_low_j = cj;                         // giữ nguyên ci, giảm dần cj
    bool block_dir_9h = true;                   // xem có bị chặn bởi quân địch ko
    int score_9h = 0;
    int player_9h = 0;

    if (cj-1 >= 0) {
        // nếu ô kề cạnh POS, chỉ cần đặt cho là hướng mở
        if (board_game[ci][cj-1] == 0) {
            block_dir_9h = false;
        } 
        else {
            player_9h = board_game[ci][cj-1];
            _9h_low_j = cj-1;

            while (board_game[ci][_9h_low_j] == player_9h) {
                if (_9h_low_j == -1) break;
                if (_9h_low_j == cj - 6 ) break;
                _9h_low_j --;
            }
            // kiểm tra hướng 9h có bị chặn không
            if (_9h_low_j >= 0 && board_game[ci][_9h_low_j] == 0) {
                block_dir_9h = false;
            }
        }
        
    }
    // đếm ô theo hướng 9h
    score_9h = cj - _9h_low_j; 
    // cout << "player 9h: " << player_9h << "\n";
    // cout << "--> 9h, cj: " << _9h_low_j << " - " << cj << "\n";
    // cout << "score 9h: " << score_9h << "\n";
    // cout << "block_dir_9h: " << block_dir_9h << "\n\n"; 
    

    // ĐIỂM THEO HƯỚNG 3h
    int _3h_up_j = cj;                          // giữ nguyên ci, tăng dần cj;
    bool block_dir_3h = true;                   // xem có bị chặn bởi quân địch ko
    int score_3h = 0;
    int player_3h = 0;

    if (cj+1 <= WIDTH-1) {
        // nếu ô kề cạnh POS, chỉ cần đặt cho là hướng mở
        if (board_game[ci][cj+1] == 0) {
            block_dir_3h = false;
        }
        else {
            player_3h = board_game[ci][cj+1];
            _3h_up_j = cj+1;

            while (board_game[ci][_3h_up_j] == player_3h) {
                if (_3h_up_j == WIDTH) break;
                if (_3h_up_j == cj + 6) break;
                _3h_up_j ++;
            }
            // kiểm tra hướng 3h có bị chặn không
            if (_3h_up_j <= WIDTH-1 && board_game[ci][_3h_up_j] == 0) {
                block_dir_3h = false;
            }
        }
    }
    // đếm ô theo hướng 3h
    score_3h = _3h_up_j - cj;
    // cout << "player 3h: " << player_3h << "\n";
    // cout << "--> 3h, cj: " << cj << " - " << _3h_up_j << "\n";
    // cout << "score 3h: " << score_3h << "\n";
    // cout <<  "block_dir_3h: " << block_dir_3h << "\n\n";


    // 2 hướng trống
    if (player_9h == 0 && player_3h == 0) {
        // cout << "++2 huong trong \n";
        score += 1;
    }
    // hướng 3h trống
    else if (player_9h != 0 && player_3h == 0) {
        // cout << "++huong 3h trong \n";
        if (score_9h == 2) score += score_9h * coef_2_cell;
        if (score_9h == 3) score += score_9h * coef_3_cell;
        if (score_9h >= 4) score += score_9h * coef_4_cell;
        if (!block_dir_9h && !block_dir_3h) {
            score += bonus_super_unblock_score;
        }
    } 
    // hướng 9h trống
    else if (player_9h == 0 && player_3h != 0) {
        // cout << "++huong 9h trong \n";
        if (score_3h == 2) score += score_3h * coef_2_cell;
        if (score_3h == 3) score += score_3h * coef_3_cell;
        if (score_3h >= 4) score += score_3h * coef_4_cell;
        if (!block_dir_9h && !block_dir_3h) {
            score += bonus_super_unblock_score;
        }
    } 
    else {
        // hướng 3h-9h là cùng quân
        // cout << "++2 huong du, ";
        if (player_9h == player_3h) {
            // cout << "cung quan. \n";
            int num_move_39 = score_9h + score_3h - 1;
            // ko bị chặn ở 2 đầu
            if (num_move_39 == 2) score += num_move_39 * coef_2_cell;
            if (num_move_39 == 3) score += num_move_39 * coef_3_cell;
            if (num_move_39 >= 4) score += num_move_39 * coef_4_cell;

            // nếu cả 2 hướng ko bị chặn, đc siêu bonus
            if (!block_dir_3h && !block_dir_9h) {
                score += bonus_super_unblock_score;
            }
            // nếu cả 2 hướng bị chặn, - 2x bonus
            else if (block_dir_3h && block_dir_9h) {
                score -= 2*bonus_super_unblock_score;
            }
            // 1 hướng bị chặn, 1 hướng ko bị chặn
            else {
                score += bonus_unblock_score;
            }

        }
        // hướng 3h-9h là quân địch của nhau
        else {
            // cout << "khac quan. \n";
            if (score_9h == 2) score += score_9h * coef_2_cell;
            if (score_9h == 3) score += score_9h * coef_3_cell;
            if (score_9h >= 4) score += score_9h * coef_3_cell;

            if (score_3h == 2) score += score_3h * coef_2_cell;
            if (score_3h == 3) score += score_3h * coef_3_cell;
            if (score_3h >= 4) score += score_3h * coef_4_cell;
            // trừ điểm cho hướng 9h && 3h đã bị chặn
            if (block_dir_9h && block_dir_3h) score -= 2*bonus_unblock_score;
        }
    }

    // cout << score << "\n";

    // score = 0;
    /* ================ ĐIỂM CHIỀU DỌC ======================= */

    // ĐIỂM THEO HƯỚNG 12h
    int _12h_low_i = ci;                         // giữ nguyên ci, giảm dần cj
    bool block_dir_12h = true;                   // xem có bị chặn bởi quân địch ko
    int score_12h = 0;
    int player_12h = 0;

    if (ci-1 >= 0) {
        if (board_game[ci-1][cj] == 0) {
            block_dir_12h = false;
        }
        else {
            player_12h = board_game[ci-1][cj];
            _12h_low_i = ci - 1;
            while (board_game[_12h_low_i][cj] == player_12h) {
                if (_12h_low_i == -1) break;
                if (_12h_low_i == ci-6) break;
                _12h_low_i --; 
            }
            // kiểm tra hướng 12h có bị chặn không
            if (_12h_low_i >= 0 && board_game[_12h_low_i][cj] == 0) {
                block_dir_12h = false;
            }
        }
    }
    // đếm ô theo hướng 12h
    score_12h = ci - _12h_low_i; 
    // cout << "player 12h: " << player_12h << "\n";
    // cout << "--> 12h, ci: " << _12h_low_i << " - " << ci << "\n";
    // cout << "score 12h: " << score_12h << "\n";
    // cout << "block_dir_12h: " << block_dir_12h << "\n\n"; 


    // ĐIỂM THEO HƯỚNG 6H
    int _6h_up_i = ci;                         // giữ nguyên cJ, tăng dần ci
    bool block_dir_6h = true;                   // có bị chặn bởi quân địch
    int score_6h = 0;
    int player_6h = 0;

    if (ci + 1 <= HEIGHT-1) {
        if (board_game[ci+1][cj] == 0) {
            block_dir_6h = false;
        } 
        else {
            player_6h = board_game[ci+1][cj];
            _6h_up_i = ci + 1;

            while (board_game[_6h_up_i][cj] == player_6h) {
                if (_6h_up_i == HEIGHT) break;
                if (_6h_up_i == ci + 6) break;
                _6h_up_i ++;
            }
            // kiểm tra hướng 6h có bị chặn không
            if (_6h_up_i <= HEIGHT-1 && board_game[_6h_up_i][cj] == 0) {
                block_dir_6h = false;
            }
        }
    }
    // đến ô theo hướng 6h
    score_6h = _6h_up_i - ci;
    // cout << "player 6h: " << player_6h << "\n";
    // cout << "--> 6h, ci: " << ci << " - " << _6h_up_i << "\n";
    // cout << "score 6h: " << score_6h << "\n";
    // cout << "block_dir_6h: " << block_dir_6h << "\n\n"; 


    // 2 hướng trống
    if (player_12h == 0 && player_6h == 0) {
        // cout << "++2 huong trong \n";
        score += 1;
    }
    // hướng 6h trống
    else if (player_12h != 0 && player_6h == 0) {
        // cout << "++huong 6h trong \n";
        if (score_12h == 2) score += score_12h * coef_2_cell;
        if (score_12h == 3) score += score_12h * coef_3_cell;
        if (score_12h >= 4) score += score_12h * coef_4_cell;
        if (!block_dir_12h && !block_dir_6h) {
            score += bonus_super_unblock_score;
        }
    } 
    // hướng 12h trống
    else if (player_12h == 0 && player_6h != 0) {
        // cout << "++huong 12h trong \n";
        if (score_6h == 2) score += score_6h * coef_2_cell;
        if (score_6h == 3) score += score_6h * coef_3_cell;
        if (score_6h >= 4) score += score_6h * coef_4_cell;
        if (!block_dir_12h && !block_dir_6h) {
            score += bonus_super_unblock_score;
        }
    } 
    else {
        // hướng 12h-6h là cùng quân
        // cout << "++2 huong du, ";
        if (player_12h == player_6h) {
            // cout << "cung quan. \n";
            int num_move_39 = score_12h + score_6h - 1;
            // ko bị chặn ở 2 đầu
            if (num_move_39 == 2) score += num_move_39 * coef_2_cell;
            if (num_move_39 == 3) score += num_move_39 * coef_3_cell;
            if (num_move_39 >= 4) score += num_move_39 * coef_4_cell;

            // nếu cả 2 hướng ko bị chặn, đc siêu bonus
            if (!block_dir_12h && !block_dir_6h) {
                score += bonus_super_unblock_score;
            }
            // nếu cả 2 hướng bị chặn, - 2x bonus
            else if (block_dir_12h && block_dir_6h) {
                score -= 2*bonus_super_unblock_score;
            }
            // 1 hướng bị chặn, 1 hướng ko bị chặn
            else {
                score += bonus_unblock_score;
            }

        }
        // hướng 12h-6h là quân địch của nhau
        else {
            // cout << "khac quan. \n";
            if (score_12h == 2) score += score_12h * coef_2_cell;
            if (score_12h == 3) score += score_12h * coef_3_cell;
            if (score_12h >= 4) score += score_12h * coef_3_cell;

            if (score_6h == 2) score += score_6h * coef_2_cell;
            if (score_6h == 3) score += score_6h * coef_3_cell;
            if (score_6h >= 4) score += score_6h * coef_4_cell;
            // trừ điểm cho hướng 12h && 6h đã bị chặn
            if (block_dir_12h && block_dir_6h) score -= 2*bonus_unblock_score;
        }
    }

    // cout << score << "\n";

    // score = 0;
    /* ================ ĐIỂM TRÊN CHÉO XUỐNG ======================= */

    // ĐIỂM THEO HƯỚNG 10H30
    int _10h30_low_i = ci;                      // giảm dần ci          
    int _10h30_low_j = cj;                      // giảm dần cj
    bool block_dir_10h30 = true;                // có bị chặn bởi quân địch
    int score_10h30 = 0;
    int player_10h30 = 0;

    if (ci-1 >= 0 && cj-1 >= 0) {
        if (board_game[ci-1][cj-1] == 0) {
            block_dir_10h30 = false;
        } 
        else {
            _10h30_low_i = ci-1;
            _10h30_low_j = cj-1;
            player_10h30 = board_game[ci-1][cj-1];

            while (board_game[_10h30_low_i][_10h30_low_j] == player_10h30) {
                if (_10h30_low_i == -1 || _10h30_low_j == -1) break;
                if (_10h30_low_i == ci - 6) break;
                _10h30_low_i --;
                _10h30_low_j --;
            }
            // kiểm tra hướng 10h30 có bị chặn không
            if (_10h30_low_i >= 0 && _10h30_low_j >= 0 && board_game[_10h30_low_i][_10h30_low_j] == 0) {
                block_dir_10h30 = false;
            }
        }
    }
    // đến ô theo hướng 10h30
    score_10h30 = ci - _10h30_low_i;
    // cout << "player 10h30: " << player_10h30 << "\n";
    // cout << "--> 10h30, (ci-cj) : (" << _10h30_low_i << "-" << _10h30_low_j << ") - (" << ci << "-" << cj << ")\n";
    // cout << "score 10h30: " << score_10h30 << "\n";
    // cout << "block_dir_10h30: " << block_dir_10h30 << "\n\n";



    // ĐIỂM THEO HƯỚNG 4H30
    int _4h30_up_i = ci;                        // tăng dần ci
    int _4h30_up_j = cj;                        // tăng dần cj
    bool block_dir_4h30 = true;                 // có bị chặn bởi quân địch
    int score_4h30 = 0;
    int player_4h30 = 0;

    if (ci+1 <= HEIGHT-1 && cj+1 <= WIDTH-1) {
        if (board_game[ci+1][cj+1] == 0) {
            block_dir_10h30 = false;
        } 
        else {
            _4h30_up_i = ci+1;
            _4h30_up_j = cj+1;
            player_4h30 = board_game[ci+1][cj+1];

            while (board_game[_4h30_up_i][_4h30_up_j] == player_4h30) {
                if (_4h30_up_i == HEIGHT || _4h30_up_j == WIDTH) break;
                if (_4h30_up_i == ci + 6) break;
                _4h30_up_i ++;
                _4h30_up_j ++;
            }
            // kiểm tra hướng 10h30 có bị chặn không
            if (_4h30_up_i <= HEIGHT-1 && _4h30_up_j <= WIDTH-1 && board_game[_4h30_up_i][_4h30_up_j] == 0) {
                block_dir_4h30 = false;
            }
        }
    }
    // đến ô theo hướng 4h30
    score_4h30 = _4h30_up_i - ci;
    // cout << "player 4h30: " << player_4h30<< "\n";
    // cout << "--> 4h30, (ci-cj) : (" << ci << "-" << cj << ") - (" << _4h30_up_i << "-" << _4h30_up_j << ")\n";
    // cout << "score 4h30: " << score_4h30 << "\n";
    // cout << "block_dir_4h30: " << block_dir_4h30 << "\n\n";


    // 2 hướng trống
    if (player_10h30 == 0 && player_4h30 == 0) {
        // cout << "++2 huong trong \n";
        score += 1;
    }
    // hướng 4h30 trống
    else if (player_10h30 != 0 && player_4h30 == 0) {
        // cout << "++huong 6h trong \n";
        if (score_10h30 == 2) score += score_10h30 * coef_2_cell;
        if (score_10h30 == 3) score += score_10h30 * coef_3_cell;
        if (score_10h30 >= 4) score += score_10h30 * coef_4_cell;
        if (!block_dir_10h30 && !block_dir_4h30) {
            score += bonus_super_unblock_score;
        }
    } 
    // hướng 10h30 trống
    else if (player_10h30 == 0 && player_4h30 != 0) {
        // cout << "++huong 12h trong \n";
        if (score_4h30 == 2) score += score_4h30 * coef_2_cell;
        if (score_4h30 == 3) score += score_4h30 * coef_3_cell;
        if (score_4h30 >= 4) score += score_4h30 * coef_4_cell;
        if (!block_dir_10h30 && !block_dir_4h30) {
            score += bonus_super_unblock_score;
        }
    } 
    else {
        // hướng 10h30-4h30 là cùng quân
        // cout << "++2 huong du, ";
        if (player_10h30 == player_4h30) {
            // cout << "cung quan. \n";
            int num_move_39 = score_10h30 + score_4h30 - 1;
            // ko bị chặn ở 2 đầu
            if (num_move_39 == 2) score += num_move_39 * coef_2_cell;
            if (num_move_39 == 3) score += num_move_39 * coef_3_cell;
            if (num_move_39 >= 4) score += num_move_39 * coef_4_cell;

            // nếu cả 2 hướng ko bị chặn, đc siêu bonus
            if (!block_dir_10h30 && !block_dir_4h30) {
                score += bonus_super_unblock_score;
            }
            // nếu cả 2 hướng bị chặn, - 2x bonus
            else if (block_dir_4h30 && block_dir_4h30) {
                score -= 2*bonus_super_unblock_score;
            }
            // 1 hướng bị chặn, 1 hướng ko bị chặn
            else {
                score += bonus_unblock_score;
            }

        }
        // hướng 10h30-4h30 là quân địch của nhau
        else {
            // cout << "khac quan. \n";
            if (score_10h30 == 2) score += score_10h30 * coef_2_cell;
            if (score_10h30 == 3) score += score_10h30 * coef_3_cell;
            if (score_10h30 >= 4) score += score_10h30 * coef_3_cell;

            if (score_4h30 == 2) score += score_4h30 * coef_2_cell;
            if (score_4h30 == 3) score += score_4h30 * coef_3_cell;
            if (score_4h30 >= 4) score += score_4h30 * coef_4_cell;
            // trừ điểm cho hướng 10h30 && 4h30 đã bị chặn
            if (block_dir_10h30 && block_dir_4h30) score -= 2*bonus_unblock_score;
        }
    }

    // cout << score << "\n";

    // score = 0;
    /* ================ ĐIỂM DƯỚI CHÉO LÊN ======================= */

    // ĐIỂM THEO HƯỚNG 7H30
    int _7h30_low_i = ci;                      // tăng dần ci+1          
    int _7h30_low_j = cj;                      // giảm dần cj-1
    bool block_dir_7h30 = true;                // có bị chặn bởi quân địch
    int score_7h30 = 0;
    int player_7h30 = 0;

    if (ci+1 <= HEIGHT-1 && cj-1 >= 0) {
        if (board_game[ci+1][cj-1] == 0) {
            block_dir_7h30 = false;
        } 
        else {
            _7h30_low_i = ci+1;
            _7h30_low_j = cj-1;
            player_7h30 = board_game[ci+1][cj-1];

            while (board_game[_7h30_low_i][_7h30_low_j] == player_7h30) {
                if (_7h30_low_i == HEIGHT || _7h30_low_j == 0) break;
                if (_7h30_low_i == ci + 6) break;
                _7h30_low_i ++;
                _7h30_low_j --;
            }
            if (_7h30_low_i <= HEIGHT-1 && _7h30_low_j >= 0 && board_game[_7h30_low_i][_7h30_low_j] == 0) {
                block_dir_7h30 = false;
            }
        }
    }
    // đến ô theo hướng 7h30
    score_7h30 = _7h30_low_i - ci;
    // cout << "player 7h30: " << player_7h30<< "\n";
    // cout << "--> 7h30, (ci-cj) : (" << _7h30_low_i << "-" << _7h30_low_j << ") - (" << ci << "-" << cj << ")\n";
    // cout << "score 7h30: " << score_7h30 << "\n";
    // cout << "block_dir_7h30: " << block_dir_7h30 << "\n\n";




    // ĐIỂM THEO HƯỚNG 1H30
    int _1h30_up_i = ci;                      // giảm dần ci-1          
    int _1h30_up_j = cj;                      // tăng dần cj+1
    bool block_dir_1h30 = true;                // có bị chặn bởi quân địch
    int score_1h30 = 0;
    int player_1h30 = 0;

    if (ci-1 >= 0 && cj+1 <= WIDTH-1) {
        if (board_game[ci-1][cj+1] == 0) {
            block_dir_1h30 = false;
        }
        else {
            _1h30_up_i = ci-1;
            _1h30_up_j = cj+1;
            player_1h30 = board_game[ci-1][cj+1];
        
            while (board_game[_1h30_up_i][_1h30_up_j] == player_1h30) {
                if (_1h30_up_i == -1 || _1h30_up_j == WIDTH) break;
                if (_1h30_up_j == cj + 6) break;
                _1h30_up_i --;
                _1h30_up_j ++;
            }
            if (_1h30_up_i >= 0 && _1h30_up_j <= WIDTH-1 && board_game[_1h30_up_i][_1h30_up_j] == 0) {
                block_dir_1h30 = false;
            }
        }
    }
    // đến ô theo hướng 1h30
    score_1h30 = _1h30_up_j - cj;
    // cout << "player 1h30: " << player_1h30 << "\n";
    // cout << "--> 1h30, (ci-cj) : (" << ci << "-" << cj << ") - (" << _1h30_up_i << "-" << _1h30_up_j << ")\n";
    // cout << "score 1h30: " << score_1h30 << "\n";
    // cout << "block_dir_1h30: " << block_dir_1h30 << "\n\n";



    // 2 hướng trống
    if (player_7h30 == 0 && player_1h30 == 0) {
        // cout << "++2 huong trong \n";
        score += 1;
    }
    // hướng 1h30 trống
    else if (player_7h30 != 0 && player_1h30 == 0) {
        // cout << "++huong 6h trong \n";
        if (score_7h30 == 2) score += score_7h30 * coef_2_cell;
        if (score_7h30 == 3) score += score_7h30 * coef_3_cell;
        if (score_7h30 >= 4) score += score_7h30 * coef_4_cell;
        if (!block_dir_7h30 && !block_dir_1h30) {
            score += bonus_super_unblock_score;
        }
    } 
    // hướng 7h30 trống
    else if (player_7h30 == 0 && player_1h30 != 0) {
        // cout << "++huong 12h trong \n";
        if (score_1h30 == 2) score += score_1h30 * coef_2_cell;
        if (score_1h30 == 3) score += score_1h30 * coef_3_cell;
        if (score_1h30 >= 4) score += score_1h30 * coef_4_cell;
        if (!block_dir_7h30 && !block_dir_1h30) {
            score += bonus_super_unblock_score;
        }
    } 
    else {
        // hướng 7h30-1h30 là cùng quân
        // cout << "++2 huong du, ";
        if (player_7h30 == player_1h30) {
            // cout << "cung quan. \n";
            int num_move_39 = score_7h30 + score_1h30 - 1;
            // ko bị chặn ở 2 đầu
            if (num_move_39 == 2) score += num_move_39 * coef_2_cell;
            if (num_move_39 == 3) score += num_move_39 * coef_3_cell;
            if (num_move_39 >= 4) score += num_move_39 * coef_4_cell;

            // nếu cả 2 hướng ko bị chặn, đc siêu bonus
            if (!block_dir_7h30 && !block_dir_1h30) {
                score += bonus_super_unblock_score;
            }
            // nếu cả 2 hướng bị chặn, - 2x bonus
            else if (block_dir_7h30 && block_dir_1h30) {
                score -= 2*bonus_super_unblock_score;
            }
            // 1 hướng bị chặn, 1 hướng ko bị chặn
            else {
                score += bonus_unblock_score;
            }

        }
        // hướng 7h30-1h30 là quân địch của nhau
        else {
            // cout << "khac quan. \n";
            if (score_7h30 == 2) score += score_7h30 * coef_2_cell;
            if (score_7h30 == 3) score += score_7h30 * coef_3_cell;
            if (score_7h30 >= 4) score += score_7h30 * coef_3_cell;

            if (score_1h30 == 2) score += score_1h30 * coef_2_cell;
            if (score_1h30 == 3) score += score_1h30 * coef_3_cell;
            if (score_1h30 >= 4) score += score_1h30 * coef_4_cell;
            // trừ điểm cho hướng 7h30 && 1h30 đã bị chặn
            if (block_dir_7h30 && block_dir_1h30) score -= 2*bonus_unblock_score;
        }
    }

    return score;
}

// tìm điểm tấn công tối ưu
Point attack(int board_game[][WIDTH], int player_id){

    int board_score[HEIGHT][WIDTH];

    for (int i=0; i < HEIGHT; i++) {
        for (int j=0; j < WIDTH; j++) {
            if (board_game[i][j] == 0) {
                board_score[i][j] = evaluateScoreOfPoint(board_game, player_id, Point(i, j));
            } else {
                board_score[i][j] = 0;
            }
        }
    }

    int max_score = 0;
    Point attack_point = player_rand(board_game, player_id);
    
    for (int i=0; i < HEIGHT; i++) {
        for (int j=0; j < WIDTH; j++) {
            if (max_score < board_score[i][j]) {
                max_score = board_score[i][j];
                attack_point = Point(i, j);
            }
        }
    }

    return attack_point;
}

Point check_n_tile(int board_game[][WIDTH], int player_id, int n){
    return player_rand(board_game, player_id);
}


Point player_207(int board_game[][WIDTH], int player_id){
    for (int i=0; i < HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            if (board_game[i][j] != 0) {
                emptyBoard = false;
            }
        }
    }

    if (emptyBoard == true) return Point(HEIGHT/2-1, WIDTH/2-1);

    Point p = check_win(board_game, player_id);
    if(p.x != -1 && p.y != -1){
        return p;
    } else {
        p = defend(board_game, player_id);
        if(p.x != -1 && p.y != -1){
            return p;
        } else {
            return attack(board_game, player_id);
        }
    }
}



#endif // BOTBASELINE

