// python intcode_cc.py -i math.c string.c sort.c 2021/day20.c -o 2021/day20.txt
// intcode_vm 2021/day20.txt < 2021/input20.txt

int imag0[10001]; // 100*100
int IEA[514];


void enhance_image(int source, int target, int W, int H, int padwith) {
    int x;
    int y;
    int binnum;
    for(y=0; y<H; y+=1) {
        for(x=0; x<W; x+=1) {
            // Extract binary number
            if((0<y)*(0<x)) {
                binnum = 256*(source[W*(y-1)+(x-1)]==35);
            }
            else {
                binnum = 256*padwith;
            }
            if(0<y) {
                binnum += 128*(source[W*(y-1)+x]==35);
            }
            else {
                binnum += 128*padwith;
            }
            if((0<y)*(x<W-1)) {
                binnum += 64*(source[W*(y-1)+(x+1)]==35);
            }
            else {
                binnum += 64*padwith;
            }
            if(0<x) {
                binnum += 32*(source[W*y+(x-1)]==35);
            }
            else {
                binnum += 32*padwith;
            }
            binnum += 16*(source[W*y+x]==35);
            if(x<W-1) {
                binnum += 8*(source[W*y+(x+1)]==35);
            }
            else {
                binnum += 8*padwith;
            }
            if((y<W-1)*(0<x)) {
                binnum += 4*(source[W*(y+1)+(x-1)]==35);
            }
            else {
                binnum += 4*padwith;
            }
            if(y<W-1) {
                binnum += 2*(source[W*(y+1)+x]==35);
            }
            else {
                binnum += 2*padwith;
            }
            if((y<W-1)*(x<W-1)) {
                binnum += (source[W*(y+1)+(x+1)]==35);
            }
            else {
                binnum += 1*padwith;
            }

            // Apply the image enhancement
            target[W*y+x] = IEA[binnum];
        }
    }
}


void main() {
    int i_read;
    int L;
    int l;
    int counter;
    int H;
    int W;
    int buffer[50000]; // > 200*200
    int bufnew[50000]; //

    // Read image enhancement algorithm
    scans(IEA, 513, 10, &i_read);

    // Discard empty line
    scans(imag0, 10000, 10, &i_read);

    // Read first line of image
    scans(imag0, 102, 10, &W);
    
    // Read remaining
    for(H=1; H<102; H+=1) {
        scans(&imag0[W*H], W+1, 10, &i_read);
        if((i_read == W)==0) {
            break;
        }
    }
    

    // Init buffer
    int x;
    int y;
    for(y=0; y<H+110; y+=1) {
        for(x=0; x<W+110; x+=1) {
            if(0<(y<55)+(H+54<y)+(x<55)+(W+54<x)) {
                buffer[(W+110)*y+x] = 46;
                //print(88);
            }
            else {
                buffer[(W+110)*y+x] = imag0[W*(y-55) + (x-55)];
                //print(imag0[W*(y-5) + (x-5)]);
            }
        }
        //printlf();
    }

    // Part 1
    enhance_image(buffer, bufnew, W+110, H+110, 0);
    enhance_image(bufnew, buffer, W+110, H+110, IEA[0]==35);
    counter=0;
    for(y=0; y<H+110; y+=1) {
        for(x=0; x<W+110; x+=1) {
            //print(buffer[(W+10)*y + x]);
            counter += buffer[(W+110)*y + x] == 35;
        }
        //printlf();
    }
    printd(counter); // 5597
    printlf();
    

    // Part 2
    for(l=0; l<24; l+=1) {
        enhance_image(buffer, bufnew, W+110, H+110, 0);
        enhance_image(bufnew, buffer, W+110, H+110, IEA[0]==35);
    }
    counter=0;
    for(y=0; y<H+110; y+=1) {
        for(x=0; x<W+110; x+=1) {
            //print(buffer[(W+10)*y + x]);
            counter += buffer[(W+110)*y + x] == 35;
        }
        //printlf();
    }
    printd(counter); // x 5568 too low, 5816 too high
    printlf();
}