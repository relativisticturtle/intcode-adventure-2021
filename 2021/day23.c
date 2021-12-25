// python intcode_cc.py -i math.c string.c 2021/day23.c -o 2021/day23.txt
// intcode_vm 2021/day23.txt < 2021/input23.txt

// Pre-calculated 23x23 adjacency matrix
int adj[] = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,2,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0};
int X[] = {1, 10, 100, 1000};

//  #D#C#B#A#
//  #D#B#A#C#
int PT2[] = "DCBADBAC";

void min_cost_from(int state, int cost) {  // heuristic for A-star
    // check occupancy in each room
    int i;
    int j;
    int r;
    int M;
    cost[0] = 0;
    
    for(r=0; r<4; r+=1) {
        for(i=0; i<4; i+=1) {
            if((state[10+4*r-i]==65+r)==0) {
                break;
            }
        }
        for(; i<4; i+=1) {
            cost[0] += X[r]*(4-i); // cost for amph to move in
            if(64 < state[10+4*r-i]) {
                j = state[10+4*r-i]-65;
                cost[0] += X[j]*(4-i); // cost for amph to move out
                if(r<j) {
                    cost[0] += 2*X[j]*(j-r); // cost for amph to move in hallway
                }
                else {
                    cost[0] += 2*X[j]*(r-j);
                }
            }
        }
    }
    for(i=0; i<7; i+=1) {  // cost for amph to move in hallway
        if(64 < state[i]) {
            // find coords
            r = 2 + 2*(state[i] - 65); // 2, 4, 6, 8
            if(i<2) {
                j=i; // 0, 1
            }
            else if(i<5) {
                j = 2*i - 1; // 3, 5, 7, 9
            }
            else {
                j = 4+i;
            }
            if(r<j) {
                cost[0] += X[state[i] - 65]*(j-r);
            }
            else {
                cost[0] += X[state[i] - 65]*(r-j);
            }
        }
    }
}

void allowed_movement(int state, int from, int to) {
    int i;
    int bfsQ[23];
    int lenQ;

    // initialize
    for(i=0; i<23; i+=1) {
        to[i] = 0;
    }
    bfsQ[0] = from;
    lenQ = 1;

    // BFS-search
    for(; 0<lenQ;) {
        int p;
        lenQ -= 1;
        p = bfsQ[lenQ];

        for(i=0; i<23; i+=1) {
            if((0<state[i])+(0<to[i])+(adj[23*p+i]==0)) {
                // occupied, already visited, or not adjacent
                continue;
            }
            to[i] = to[p] + adj[23*p+i];
            bfsQ[lenQ] = i;
            lenQ += 1;
        }
    }

    int r;
    r = state[from] - 65;

    if(from<7) {
        // hallway, must move into room - furthest in    
        for(i=0; i<4; i+=1) {
            if((state[10 + 4*r - i] == state[from]) == 0) {
                break;
            }
        }
        int j;
        j = 23;
        if((i<4)*(state[10 + 4*r - i] == 0)) {
            // *only* allowed destination
            j = 10 + 4*r - i;
        }
        for(i=0; i<23; i+=1) {
            to[i] *= (i==j);
        }
    }
    else {
        // if in correct room and furthest in - do not move
        int res[2];
        int j;
        divmod(res, from-7, 4);
        if(res[0] == r) {
            for(j=res[1]; j<4; j+=1) {
                if((state[7+4*r+j] == state[from]) == 0) {
                    break;
                }
            }
            if(j==4) {
                for(i=0; i<23; i+=1) {
                    to[i] = 0;
                }
            }
        }
        else {
            // must move into hallway
            for(i=7; i<23; i+=1) {
                to[i] = 0;
            }
        }
    }
}

void hashcode(int state, int hash) {
    // 7*8 + 4*4*8 : 5 integers
    hash[0] = 0;
    int i;
    for(i=0; i<7; i+=1) {
        hash[0] *= 8;
        if(64 < state[i]) {
            hash[0] += 4 + state[i] - 65;
        }
    }
    int r;
    for(r=0; r<4; r+=1) {
        hash[r+1] = 0;
        for(i=0; i<4; i+=1) {
            hash[r+1] *= 8;
            if(64 < state[7 + 4*r + i]) {
                hash[r+1] += 4 + state[7 + 4*r + i] - 65;
            }
        }
    }
}

void calc_cost_from(int answer, int state) {
    int pQ[250000];
    int pQ_len;
    int i;
    int st[25];
    int to[23];
    int j;
    int k;
    int l;

    int hash;
    int H[5000000];
    int h;
    h=0;
    
    // Enter first state
    for(i=0; i<25; i+=1) {
        pQ[i] = state[i];
    }
    min_cost_from(&state[2], &state[1]);
    pQ_len = 1;

    for(; 0<pQ_len;) {
        // Take top
        pQ_len -= 1;
        for(i=0; i<25; i+=1) {
            st[i] = pQ[25*pQ_len+i];
        }

        // TODO: check if in cache!
        hash = &H[5*h];
        hashcode(&st[2], hash);
        if((hash[0]==0)*(hash[1]==2340)*(hash[2]==2925)*(hash[3]==3510)*(hash[4]==4095)) {
            answer[0] = st[0];
            return;
        }
        for(i=0; i<h; i+=1) {
            if((hash[0]==H[5*i])*(hash[1]==H[5*i+1])*(hash[2]==H[5*i+2])*(hash[3]==H[5*i+3])*(hash[4]==H[5*i+4])) {
                break;
            }
        }
        if(i<h) {
            continue;
        }
        else {
            h += 1;
            if(999999<h) {
                printd(-9999);
                printlf();
                return;
            }
        }

        // Consider moves:
        for(i=0; i<23; i+=1) {
            if(st[2+i] == 0) {
                continue;
            }
            int step_cost;
            step_cost = X[st[2+i]-65];

            allowed_movement(&st[2], i, to);
            for(j=0; j<23; j+=1) {
                if(to[j] == 0) {
                    continue;
                }

                for(k=0; k<25; k+=1) {
                    pQ[25*pQ_len+k] = st[k];
                }
                pQ[25*pQ_len+2+j] = st[2+i];
                pQ[25*pQ_len+2+i] = 0;
                pQ[25*pQ_len] += step_cost * to[j];
                min_cost_from(&pQ[25*pQ_len+2], &pQ[25*pQ_len+1]);

                // 1 iteration bubble sort 
                for(l=pQ_len; (0<l)*(pQ[25*(l-1)]+pQ[25*(l-1)+1]<pQ[25*l]+pQ[25*l+1]); l-=1) {
                    for(k=0; k<25; k+=1) {
                        int x;
                        x = pQ[25*l + k];
                        pQ[25*l + k] = pQ[25*(l-1) + k];
                        pQ[25*(l-1) + k] = x;
                    }
                }

                pQ_len += 1;
            }
        }
    }
}


void main() {
    int buffer[20];
    int i_read;
    int start_pos[8];
    int state[25];
    int i;
    int r;
    int answer;

    // Read input
    scans(buffer, 20, 10, &i_read); // First line - discard
    scans(buffer, 20, 10, &i_read); // 2nd line - discard
    scans(buffer, 20, 10, &i_read); // 3rd line
    start_pos[0] = buffer[3];
    start_pos[1] = buffer[5];
    start_pos[2] = buffer[7];
    start_pos[3] = buffer[9];
    scans(buffer, 20, 10, &i_read); // 4th line
    start_pos[4] = buffer[3];
    start_pos[5] = buffer[5];
    start_pos[6] = buffer[7];
    start_pos[7] = buffer[9];
    
    // Part 1
    for(i=0; i<9; i+=1) {
        state[i] = 0;
    }
    for(i=0; i<4; i+=1) {
        state[ 9+4*i] = start_pos[i];
        state[10+4*i] = start_pos[4 + i];
        state[11+4*i] = 65 + i;
        state[12+4*i] = 65 + i;
    }
    calc_cost_from(&answer, state);
    printd(answer); // Part 1 : 16489
    printlf();

    // Part 2
    for(i=0; i<9; i+=1) {
        state[i] = 0;
    }
    for(i=0; i<4; i+=1) {
        state[ 9+4*i] = start_pos[i];
        state[10+4*i] = PT2[i];
        state[11+4*i] = PT2[i+4];
        state[12+4*i] = start_pos[4 + i];
    }
    calc_cost_from(&answer, state);
    printd(answer); // Part 2 : 43413
    printlf();
}
