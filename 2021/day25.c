// python intcode_cc.py -i math.c string.c sort.c 2021/day25.c -o 2021/day25.txt
// intcode_vm 2021/day25.txt < 2021/input25.txt


void main() {
    int i_read;
    int H;
    int W;
    int map[100000];
    //int map_new

    // Read first line of image
    scans(map, 300, 10, &W);
    
    // Read remaining
    for(H=1; H<300; H+=1) {
        scans(&map[W*H], W+1, 10, &i_read);
        if((i_read == W)==0) {
            break;
        }
    }

    // Part 1
    // > : 62
    // v : 118
    // . : 46
    int s;
    int x;
    int y;
    int has_moved;
    has_moved = 1;
    for(s=0; (has_moved)*(s<1000); s+=1) {
        has_moved = 0;

        // East
        for(y=0; y<H; y+=1) { // mark for movement
            for(x=0; x<W-1; x+=1) {
                if((map[W*y+x] == 62) * (map[W*y+x+1] == 46)) {
                    map[W*y+x] = 63;
                }
            }
            if((map[W*y+x] == 62) * (map[W*y] == 46)) {
                map[W*y+x] = 63;
            }
        }
        for(y=0; y<H; y+=1) { // move
            for(x=0; x<W-1; x+=1) {
                if(map[W*y+x] == 63) {
                    map[W*y+x] = 46;
                    map[W*y+x+1] = 62;
                    has_moved = 1;
                }
            }
            if(map[W*y+x] == 63) {
                map[W*y+x] = 46;
                map[W*y] = 62;
                has_moved = 1;
            }
        }
        // South
        for(x=0; x<W; x+=1) { // mark for movement
            for(y=0; y<H-1; y+=1) {
                if((map[W*y+x] == 118) * (map[W*y+x+W] == 46)) {
                    map[W*y+x] = 119;
                }
            }
            if((map[W*y+x] == 118) * (map[x] == 46)) {
                map[W*y+x] = 119;
            }
        }
        for(x=0; x<W; x+=1) { // move
            for(y=0; y<H-1; y+=1) {
                if(map[W*y+x] == 119) {
                    map[W*y+x] = 46;
                    map[W*y+x+W] = 118;
                    has_moved = 1;
                }
            }
            if(map[W*y+x] == 119) {
                map[W*y+x] = 46;
                map[x] = 118;
                has_moved = 1;
            }
        }
    }

    printd(s);
    printlf();
}