// python intcode_cc.py -i math.c string.c 2021/day21.c -o 2021/day21.txt
// intcode_vm 2021/day21.txt < 2021/input21.txt

int scoreboard[1000000]; // 2 * 10 * 10 * 50 * 50 * 2  turn, pos1, pos2, score 1, score 2, unive1, unive2
int outcomes[] = {1, 3, 6, 7, 6, 3, 1};

void count_universes(int pos, int score, int x, int univ) {
    int hash;
    hash = x + 2*(score[0] + 50*(score[1] + 50*(pos[0]-1 + 10*(pos[1]-1))));
    if(scoreboard[2*hash] + scoreboard[2*hash+1]) {
        univ[0] = scoreboard[2*hash];
        univ[1] = scoreboard[2*hash + 1];
        return;
    }

    if(20<score[0]) {
        scoreboard[2*hash] = 1;
        univ[0] = 1;
        univ[1] = 0;
        return;
    }
    if(20<score[1]) {
        scoreboard[2*hash + 1] = 1;
        univ[0] = 0;
        univ[1] = 1;
        return;
    }
    
    int u[2];
    int p[2];
    int s[2];

    univ[0] = 0;
    univ[1] = 0;

    // 3x die outcomes:
    // 3 : 1
    // 4 : 3
    // 5 : 6
    // 6 : 7
    // 7 : 6
    // 8 : 3
    // 9 : 1
    // sum 27 - check!
    int dx3;
    //printlf();
    for(dx3=3; dx3<10; dx3+=1) {
        p[0] = pos[0];
        p[1] = pos[1];
        s[0] = score[0];
        s[1] = score[1];

        // roll 3x
        p[x] += dx3;
        if(10<p[x]) {
            p[x] -= 10;
        }
        s[x] += p[x];

        count_universes(p, s, 1-x, u);
        univ[0] += u[0] * outcomes[dx3-3];
        univ[1] += u[1] * outcomes[dx3-3];
    }
    scoreboard[2*hash] = univ[0];
    scoreboard[2*hash + 1] = univ[1];
}

void main() {
    int buffer[50];
    int i_read;
    int pos[2];
    int score[2];
    int p1;
    int p2;

    scans(buffer, 50, 58, &i_read);
    scans(buffer, 50, 10, &i_read);
    str2int(buffer, &p1);
    scans(buffer, 50, 58, &i_read);
    scans(buffer, 50, 10, &i_read);
    str2int(buffer, &p2);

    // Part 1
    int res[2];
    int x; // player: 0, 1
    int die;

    // Set game
    die = 1;
    pos[0] = p1;
    pos[1] = p2;

    int t;
    t=0;
    for(; 1;) {
        
        for(x=0; x<2; x+=1) {
            t+=3; // roll
            pos[x] += 3*die+3;
            for(die+=3; 100<die; die-=100) {
            }

            divmod(res, pos[x]-1, 10);
            pos[x] = res[1]+1;
            score[x] += pos[x];
            if(999 < score[x]) {
                break;
            }
        }
        if(x<2) {
            break;
        }
    }
    printd(score[1-x]*t); // 752247
    printlf();

    // Part 2

    // Prepare cache
    for(t=0; t<1000000; t+=1) {
        scoreboard[t] = 0;
    }

    // Reset game
    pos[0] = p1;
    pos[1] = p2;
    score[0] = 0;
    score[1] = 0;

    // Calc
    int univ[2];
    count_universes(pos, score, 0, univ);

    // 221109915584112
    if(univ[0] < univ[1]) {
        printd(univ[1]);
        printlf();
    }
    else    {
        printd(univ[0]);
        printlf();
    }
}