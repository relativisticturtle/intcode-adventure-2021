//
// python intcode_cc.py -i math.c string.c 2021/day03.c -o 2021/day03.intc && intcode_vm 2021/day03.intc < 2021/input03.txt
//


void check_bingo(int is_bingo, int board, int numbers, int upto) {
    int j;
    int i;
    int n;
    // --- rows ---
    for(j=0; j<5; j+=1) {
        for(i=0; i<5; i+=1) {
            for(n=0; n<upto; n+=1) {
                if(board[5*j + i] == numbers[n]) {
                    break; // is match
                }
            }
            if(n==upto) {
                break; // none matched
            }
        }
        if(i==5) {
            is_bingo[0] = 1;
            return; // bingo!
        }
    }

    // --- columns ---
    for(j=0; j<5; j+=1) {
        for(i=0; i<5; i+=1) {
            for(n=0; n<upto; n+=1) {
                if(board[j + 5*i] == numbers[n]) {
                    break; // is match
                }
            }
            if(n==upto) {
                break; // none matched
            }
        }
        if(i==5) {
            is_bingo[0] = 1;
            return; // bingo!
        }
    }

    is_bingo[0] = 0;
}

void calc_unmarked_sum(int score, int board, int numbers, int upto) {
    int j;
    int i;
    int n;
    score[0] = 0;

    for(j=0; j<25; j+=1) {
        for(n=0; n<upto; n+=1) {
            if(board[j] == numbers[n]) {
                break; // is match
            }
        }
        if(n==upto) {
            //was unmarked
            score[0]+=board[j];
        }
    }
}

void main() {
    int numbers[100];
    int boards[25000];
    int buffer[20];
    int i_read;
    int B;
    int L;
    int l;

    // Read numbers
    int l;
    for(L=0; L<99; L+=1) {
        scans(buffer, 3, 44, &i_read);
        str2int(buffer, &numbers[L]);
    }
    scans(buffer, 3, 10, &i_read);
    str2int(buffer, &numbers[99]);

    // Read boards
    for(B=0; B<100; B+=1) {
        for(L=0; L<25; L+=1) {
            // Get first non-ws char into buffer
            for(buffer[0]=32; (buffer[0]<48)*buffer[0]; ) {
                scan(&buffer[0]);
            }
            // Get 2nd (might be a ws, but str2int handles that).
            scan(&buffer[1]);
            buffer[2] = 0;
            
            str2int(buffer, &boards[25*B + L]);
        }
    }

    // --- PART 1 ---
    int upto;
    for(upto=5; upto<=100; upto+=1) {
        for(B=0; B<100; B+=1) {
            int is_bingo;
            check_bingo(&is_bingo, &boards[25*B], numbers, upto);
            if(is_bingo) {
                break;
            }
        }
        if(B<100) {
            break;
        }
    }
    int score;
    calc_unmarked_sum(&score, &boards[25*B], numbers, upto);
    printd(score*numbers[upto-1]); // 35670
    printlf();


    // --- PART 2 ---
    for(upto=100; upto>=5; upto-=1) {
        for(B=0; B<100; B+=1) {
            int is_bingo;
            check_bingo(&is_bingo, &boards[25*B], numbers, upto);
            if(is_bingo == 0) {
                break; // 1st board _not_ to bingo
            }
        }
        if(B<100) {
            break;
        }
    }
    upto += 1; // remember to re-draw winning number
    calc_unmarked_sum(&score, &boards[25*B], numbers, upto);

    // 25026 (too high)
    printd(score*numbers[upto-1]); 
    printlf();
}
