// python intcode_cc.py -i math.c string.c 2021/day17.c -o 2021/day17.txt
// intcode_vm 2021/day17.txt < 2021/input17.txt

void main() {
    int buffer[100];
    int i_read;
    int X1;
    int X2;
    int Y1;
    int Y2;
    
    scans(buffer, 100, 61, &i_read); // discard up to '='
    scans(buffer, 100, 46, &i_read); // read up to '.'
    str2int(buffer, &X1);

    scan(buffer); // discard one '.'
    scans(buffer, 100, 44, &i_read); // read up to ','
    str2int(buffer, &X2);

    scans(buffer, 100, 61, &i_read); // discard up to '='
    scans(buffer, 100, 46, &i_read); // read up to '.'
    str2int(buffer, &Y1);

    scan(buffer); // discard one '.'
    scans(buffer, 100, 10, &i_read); // read up to LF
    str2int(buffer, &Y2);
    
    // Part 1
    // vx = max(vx0 - t, 0)     "drag"
    // vy = vy0 - t             "gravity"
    //
    // 0, vy0, 2*vy0 - 1, 3*vy0 - 3, ... = t*vy0 - t*(t-1)/2
    // y(t)   = t*(vy0 - (t-1)/2)
    // --> 2nd root at t2 == 2*vy0 + 1
    //
    // Highest "allowed" vy0 if vy(t2) == Y1 
    //   vy == vy0 - t2 == Y1
    //   vy0 - (2*vy0 + 1) == Y1
    //   vy0 = -1 - Y1
    //
    // Maximum y at y(t2 // 2)

    int t2;
    int vy0_max;
    int ymax;
    int res[2];
    vy0_max = -1 - Y1;
    t2 = 2*vy0_max + 1;

    //ymax = t2*vy0 - t2*(t2+1)/2;
    int tmax;
    divmod(res, t2, 2);
    tmax = res[0];
    

    ymax = tmax*(tmax-1);
    divmod(res, ymax, 2);
    ymax = tmax*vy0_max - res[0];

    printd(ymax); // 6555
    printlf();


    int counter;
    counter = 0;
    int vx0;
    int vy0;

    for(vy0=Y1; vy0<=vy0_max; vy0+=1) {
        for(vx0=1; vx0<=X2; vx0+=1) {
            int vx;
            int vy;
            int x;
            int y;
            vx=vx0;
            vy=vy0;
            x=0;
            y=0;
            for(; Y1<=y;) {
                x+=vx;
                y+=vy;
                vx-=(0<vx);
                vy-=1;
                if((x<X1)+(X2<x)+(y<Y1)+(Y2<y) == 0) {
                    break;
                }
            }
            if((x<X1)+(X2<x)+(y<Y1)+(Y2<y) == 0) {
                counter += 1;
            }
        }
    }

    printd(counter); // 4973
    printlf();
}
