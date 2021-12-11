// python intcode_cc.py -i math.c string.c sort.c 2021/day09.c -o 2021/day09.txt
// intcode_vm 2021/day09.txt < 2021/input09.txt

int input[10000]; // 100*100
int dx[] = {1, 0, -1, 0};
int dy[] = {0, 1, 0, -1};

void main() {
    int L;
    int l;
    int c;
    int counter;
    int minc;
    int H;
    int W;
    int basins[1000];
    int N_basins;
    H = 100;
    W = 100;
    N_basins=0; // 253
    
    for(L=0; L<H; L+=1) {
        for(l=0; l<W; l+=1) {
            scan(&input[W*L + l]);
        }
        for(c=1; ((c==10)+(c==0))==0; scan(&c)) {
        }
    }

    counter = 0;
    for(L=0; L<H; L+=1) {
        for(l=0; l<W; l+=1) {
            minc = 0;
            // Allow that checks may happen outside of input-buffer
            // (Easier implemented, and _should_ be ok).
            minc += (l>0)*(input[W*L + l]<input[W*L + l-1]);
            minc += (l<W-1)*(input[W*L + l]<input[W*L + l+1]);
            minc += (L>0)*(input[W*L + l]<input[W*L-W + l]);
            minc += (L<H-1)*(input[W*L + l]<input[W*L+W + l]);
            minc += (l==0) + (l==W-1) + (L==0) + (L==H-1);
            if(minc == 4) {
                counter += input[W*L + l] + 1 - 48;
                basins[N_basins] = 1000*L + l;
                N_basins += 1;
            }
        }
    }
   
    // Part 1
    printd(counter); // 633
    printlf();

    // Part 2
    int areas[1000];
    int stack[1000];
    int res[2];
    for(L=0; L<N_basins; L+=1) {
        // decode y & x
        divmod(res, basins[L], 1000);
        input[W*res[0] + res[1]] = 0;

        areas[L] = 0;
        
        // Setup stack
        int sp;
        sp = 0;
        stack[sp] = 1000*res[0] + res[1];
        sp += 1;

        for(; sp>0;) {
            // decode y & x
            sp -= 1;
            divmod(res, stack[sp], 1000);

            // Increase area
            areas[L] += 1;

            //input[W*res[0] + res[1]] = 0;
            int d;
            for(d=0; d<4; d+=1) {
                int x;
                int y;
                x = res[1]+dx[d];
                y = res[0]+dy[d];
                if((0<=x) * (x<W) * (0<=y) * (y<W)) {
                    if((input[W*y + x] == 0) + (input[W*y + x] == 57) == 0) {
                        // Consume position and push to stack
                        input[W*y + x] = 0;
                        stack[sp] = 1000*(res[0]+dy[d]) + res[1]+dx[d];
                        sp += 1;
                    }
                }
            }
        }
    }

    // Yes, let's sort!
    sort_inplace(areas, stack, N_basins);
    printd(areas[N_basins-1]*areas[N_basins-2]*areas[N_basins-3]); // 1050192
    printlf();
}