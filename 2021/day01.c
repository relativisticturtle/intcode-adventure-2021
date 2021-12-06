// python intcode_cc.py -i math.c string.c 2021/day01.c -o 2021/day01.txt
// intcode_vm 2021/day01.txt < 2021/input01.txt

int input[2000];

void main() {
    int buffer[20];
    int i_read;
    int L;
    int counter;
    
    for(L=0; L<2000; L+=1) {
        scans(buffer, 20, 10, &i_read);
        str2int(buffer, &input[L]);
    }
    
    // Part 1
    counter=0;
    for(L=1; L<2000; L+=1) {
        if(input[L-1]<input[L]) {
            counter += 1;
        }
    }
    printd(counter); // 1448
    printlf();

    // Part 2
    counter=0;
    for(L=3; L<2000; L+=1) {
        if(input[L-3]<input[L]) {
            counter += 1;
        }
    }
    printd(counter); // 1471
    printlf();
}