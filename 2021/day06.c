// python intcode_cc.py -i math.c string.c 2021/day06.c -o 2021/day06.txt
// intcode_vm 2021/day06.txt < 2021/input06.txt

int input[300];

void main() {
    int buffer[20];
    int L;
    int d;
    int t;
    int counter;
    int lf_at[9];
    
    for(L=0; L<300; L+=1) {
        int c;
        scan(&c);
        input[L] = c - 48;
        scan(&c); // discard ','
    }

    // initialize lf_at
    for(L=0; L<9; L+=1) {
        lf_at[L] = 0;
    }
    for(L=0; L<300; L+=1) {
        lf_at[input[L]] += 1;
    }

    // Part 1
    for(d=0; d<80; d+=1) {
        int lf_at0;
        lf_at0 = lf_at[0];
        // everyone's a day older
        for(t=0; t<8; t+=1) {
            lf_at[t] = lf_at[t+1];
        }
        lf_at[8] = lf_at0;

        // day 6 gets extra contributions
        lf_at[6] += lf_at0;
        
    }

    counter = 0;
    for(t=0; t<9; t+=1) {
        counter += lf_at[t];
    }
    printd(counter); // 376194
    printlf();


    // Part 2
    
    // rather than re-initializing the simulation, continue from 80
    for(d=80; d<256; d+=1) {
        int lf_at0;
        lf_at0 = lf_at[0];
        // everyone's a day older
        for(t=0; t<8; t+=1) {
            lf_at[t] = lf_at[t+1];
        }
        lf_at[8] = lf_at0;

        // day 6 gets extra contributions
        lf_at[6] += lf_at0;
        
    }

    counter = 0;
    for(t=0; t<9; t+=1) {
        counter += lf_at[t];
    }
    printd(counter); // 1693022481538
    printlf();
}