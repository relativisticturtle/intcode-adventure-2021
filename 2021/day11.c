// python intcode_cc.py -i math.c string.c 2021/day11.c -o 2021/day11.txt
// intcode_vm 2021/day11.txt < 2021/input11.txt

int input[100]; // 10*10
int DX[] = {-1, 0, 1, -1, 1, -1, 0, 1};
int DY[] = {-1, -1, -1, 0, 0, 1, 1, 1};


void perform_iteration(int flashes, int map) {
    int x;
    int y;
    int i;
    int f_before;

    for(x=0; x<100; x+=1) {
        map[x] += 1;
    }

    for(f_before = -1; f_before < flashes[0];) {
        f_before=flashes[0];

        for(y=0; y<10; y+=1) {
            for(x=0; x<10; x+=1) {
                if(map[10*y+x]<58) {
                    continue;
                }
                // flash!
                map[10*y+x] = 48;
                flashes[0] += 1;

                for(i=0; i<8; i+=1) {
                    if((0<=x+DX[i])*(x+DX[i]<10)*(0<=y+DY[i])*(y+DY[i]<10) == 0) {
                        continue;
                    }
                    if(map[10*y+10*DY[i]+x+DX[i]] == 48) {
                        continue;
                    }
                    map[10*y+10*DY[i]+x+DX[i]] += 1;
                }
            }
        }
    }
}

void main() {
    int L;
    int l;
    int c;
    int counter;
    
    // scan input
    for(L=0; L<10; L+=1) {
        for(l=0; l<10; l+=1) {
            scan(&input[10*L + l]);
        }
        for(c=1; ((c==10)+(c==0))==0; scan(&c)) {
        }
    }

    // Part 1
    int flashes;
    flashes=0;
    for(L=0; L<100; L+=1) {
        perform_iteration(&flashes, input);
    }
    printd(flashes); // 1697
    printlf();

    // Part 2
    for(; L<1000000; L+=1) {
        for(l=0; l<100; l+=1) {
            if(48 < input[l]) {
                break;
            }
        }
        if(l==100) {
            break;
        }
        perform_iteration(&flashes, input);
    }
    printd(L); // 1697
    printlf(); // 344
}
