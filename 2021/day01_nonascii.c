// python intcode_cc.py -i 2021/day01_nonascii.c -o 2021/day01_nonascii.txt
// intcode_vm -d 2021/day01_nonascii.txt < 2021/input01.txt

int input[2000];

void main() {
    int L;
    int counter;
    
    for(L=0; L<2000; L+=1) {
        scan(&input[L]);
    }
    
    // Part 1
    counter=0;
    for(L=1; L<2000; L+=1) {
        if(input[L-1]<input[L]) {
            counter += 1;
        }
    }
    print(counter); // 1448

    // Part 2
    counter=0;
    for(L=3; L<2000; L+=1) {
        if(input[L-3]<input[L]) {
            counter += 1;
        }
    }
    print(counter); // 1471
}