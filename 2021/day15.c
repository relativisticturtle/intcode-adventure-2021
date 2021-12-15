// python intcode_cc.py -i math.c string.c 2021/day15.c -o 2021/day15.txt
// intcode_vm 2021/day15.txt < 2021/input15.txt

int DX[] = {1, 0, -1, 0};
int DY[] = {0, 1, 0, -1};


void lowest_risk_path(int answer, int map, int H, int W, int buffer) {
    int res[2];
    int X;
    int Y;
    int d;
    int risk;
    int pQ;
    int pQ_len;
    int L;
    int newL;
    int l;
    int i;

    // allocate in buffer
    risk = buffer;
    pQ = buffer + W*H;

    for(L=0; L<W*H; L+=1) {
        risk[L] = -1;
    }

    // Enter first pos
    risk[0] = 0; //map[0] - 48;
    pQ[0] = 0;
    pQ_len = 1;

    for(; 0<pQ_len;) {
        // Take top
        pQ_len -= 1;
        L = pQ[pQ_len];
        if(L==W*H-1) {
            break;
        }
        divmod(res, L, W);
        Y = res[0];
        X = res[1];

        //printd(risk[W*Y + X]);
        //printlf();

        for(d=0; d<4; d+=1) {
            if((X + DX[d] < 0) + (W <= X + DX[d]) + (Y + DY[d] < 0) + (H <= Y + DY[d])) {
                continue;
            }
            if(-1 < risk[W*(Y+DY[d])+X+DX[d]] ) {
                continue;
            }
            newL = W*(Y+DY[d])+X+DX[d];
            risk[newL] = risk[L] + map[newL] - 48;
            
            // 1 iteration bubble sort 
            for(i=0; i<pQ_len; i+=1) {
                if((risk[newL] < risk[pQ[i]])==0) {
                    // swap L
                    l = pQ[i];
                    pQ[i] = newL;
                    newL = l;
                }
            }
            // insert last
            pQ_len += 1;
            pQ[i] = newL;
        }
    }
    answer[0] = risk[W*H - 1];
}


void main() {
    int map[20000];
    int i_read;
    int W;
    int H;
    int L;

    // First line - getting the width
    scans(map, 200, 10, &W);
    
    // Remaining lines
    for(H=1; H<200; H+=1) {
        scans(&map[W*H], 200, 10, &i_read);
        if((i_read == W) == 0) {
            break;
        }
    }
    
    // Part 1
    int answer;
    int buffer[22000];
    lowest_risk_path(&answer, map, H, W, buffer);
    printd(answer);
    printlf();


    // Part 2
    int megamap[500000];
    int megabuffer[550000];
    int tx;
    int ty;
    int x;
    int y;
    int X;
    int Y;
    for(ty=0; ty<5; ty+=1) {
        for(tx=0; tx<5; tx+=1) {
            for(y=0; y<H; y+=1) {
                for(x=0; x<W; x+=1) {
                    X = W*tx + x;
                    Y = H*ty + y;
                    megamap[5*W*Y + X] = map[W*y + x] + tx + ty;
                    if(57 < megamap[5*W*Y + X]) {
                        megamap[5*W*Y + X] -= 9;
                    }
                }
            }
        }
    }
    lowest_risk_path(&answer, megamap, 5*H, 5*W, megabuffer);
    printd(answer);
    printlf();

}
