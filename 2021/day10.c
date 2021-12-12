// python intcode_cc.py -i math.c string.c sort.c 2021/day10.c -o 2021/day10.txt
// intcode_vm 2021/day10.txt < 2021/input10.txt

int input[20000]; // 100*200

void main() {
    int L;
    int l;
    int c;
    int counter;
    int i_read;
    
    // Read input
    for(L=0; L<100; L+=1) {
        scans(&input[200*L], 200, 10, &i_read);
    }


    int score;
    score = 0;
    int ac_scores[100];

    int stack[200];
    for(L=0; L<100; L+=1) {
        ac_scores[L] = 0;

        // Setup stack
        int sp;
        sp = 0;

        for(l=0; l<200; l+=1) {
            // '()' 40, 41
            // '<>' 60, 62
            // '{}' 123, 125
            // '[]' 91, 93
            c = input[200*L + l];

            if((c == 40) + (c == 60) + (c == 123) + (c == 91)) {
                stack[sp] = c;
                sp += 1;
            }
            else if(c == 41) {
                if((0<sp) * (stack[sp-1] == 40)) {
                    sp -= 1;
                }
                else {
                    score += 3;
                    break;
                }
            }
            else if(c == 62) {
                if((0<sp) * (stack[sp-1] == 60)) {
                    sp -= 1;
                }
                else {
                    score += 25137;
                    break;
                }
            }
            else if(c == 125) {
                if((0<sp) * (stack[sp-1] == 123)) {
                    sp -= 1;
                }
                else {
                    score += 1197;
                    break;
                }
            }
            else if(c == 93) {
                if((0<sp) * (stack[sp-1] == 91)) {
                    sp -= 1;
                }
                else {
                    score += 57;
                    break;
                }
            }
            else if(c == 0) {
                int s;
                s = 0;
                for(; 0<sp; sp-=1) {
                    if(stack[sp-1] == 40) {
                        s = 5*s + 1;
                    }
                    else if(stack[sp-1] == 91) {
                        s = 5*s + 2;
                    }
                    else if(stack[sp-1] == 123) {
                        s = 5*s + 3;
                    }
                    else if(stack[sp-1] == 60) {
                        s = 5*s + 4;
                    }
                }
                ac_scores[L] = s;
                break;
            }
        }
    }

    // Part 1
    printd(score);  // 318081
    printlf();

    // Part 2
    sort_inplace(ac_scores, stack, 100);

    // Ignore 0s (invalid or complete)
    for(L=0; (L<100)*(ac_scores[L]==0); L+=1) {
    }
    // Find middle in remaining range
    int res[2];
    divmod(res, L+99, 2);
    printd(ac_scores[res[0]]); // 4361305341
    printlf();
}