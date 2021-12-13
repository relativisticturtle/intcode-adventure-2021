// python intcode_cc.py -i math.c string.c 2021/day13.c -o 2021/day13.txt
// intcode_vm 2021/day13.txt < 2021/input13.txt



void fold(int dots, int L, int fold) {
    int is_y;
    is_y = (fold[0] == 121);
    int d;
    for(d=0; d<L; d+=1) {
        if(fold[1] < dots[2*d + is_y]) {
            dots[2*d + is_y] = 2*fold[1] - dots[2*d + is_y];
        }
    }
}

void main() {
    int dots[2000];
    int folds[100];
    int buffer[20];
    int i_read;
    int L;
    int F;
    int l;
    int max_xy[2];
    
    // read dots
    for(L=0; L<1000; L+=1) {
        scans(buffer, 20, 10, &i_read);
        for(l=0; (l<i_read)*((buffer[l]==44)==0); l+=1) {
        }
        if(l==i_read) {
            break;
        }
        buffer[l]=0;
        str2int(buffer, &dots[2*L]);
        str2int(&buffer[l+1], &dots[2*L + 1]);
    }

    // read folds
    for(F=0; F<50; F+=1) {
        scans(buffer, 20, 10, &i_read);
        if(i_read<14) {
            break;
        }
        if(buffer[11]==120) { // 'x'
            folds[2*F] = 120;
            str2int(&buffer[13], &folds[2*F+1]);
            max_xy[0] = folds[2*F+1];
        }
        else if(buffer[11]==121) { // 'y'
            folds[2*F] = 121;
            str2int(&buffer[13], &folds[2*F+1]);
            max_xy[1] = folds[2*F+1];
        }
        else {
            printd(-9999);
        }
    }

    // Part 1
    fold(dots, L, folds);

    int counter;
    int k;
    counter = 1;
    for(l=1; l<L; l+=1) {
        for(k=0; k<l; k+=1) {
            if((dots[2*l] == dots[2*k])*(dots[2*l+1] == dots[2*k+1])) {
                break;
            }
        }
        if(k==l) {
            counter += 1;
        }
    }
    printd(counter);
    printlf();

    // Part 2
    for(l=1; l<F; l+=1) {
        fold(dots, L, &folds[2*l]);
    }
    int x;
    int y;
    for(y=0; y<max_xy[1]; y+=1) {
        for(x=0; x<max_xy[0]; x+=1) {
            for(l=0; l<L; l+=1) {
                if((dots[2*l]==x)*(dots[2*l+1]==y)) {
                    break;
                }
            }
            if(l==L) {
                print(46); // '.'
            }
            else {
                print(35); // '#'
            }
        }
        printlf();
    }
}
