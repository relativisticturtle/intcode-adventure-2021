//
// python intcode_cc.py -i math.c string.c 2021/day02.c -o 2021/day02.intc && intcode_vm 2021/day02.intc < 2021/input02.txt
//

void main() {
    int buffer[20];
    int b2[20];
    int i_read;
    int L;
    int counter;

    int x1;
    int y1;
    int x2;
    int y2;
    int aim;
    x1=0;
    y1=0;
    x2=0;
    y2=0;
    aim=0;

    int l;
    for(L=0; L<1000; L+=1) {
        scans(buffer, 20, 32, &i_read);
        scans(b2, 20, 10, &i_read);

        str2int(b2, &l);
        if(buffer[0] == 117) {      // u
            y1 -= l;
            aim -= l;
        }
        else if(buffer[0] == 100) { // d
            y1 += l;
            aim += l;
        }
        else if(buffer[0] == 102) { // f
            x1 += l;
            y2 += aim * l;
            x2 += l;
        }
        else {
            printd(9999);
            printlf();
        }
    }

    printd(x1 * y1); // 2120749
    printlf();
    printd(x2 * y2); // 4294967295
    printlf();
}
