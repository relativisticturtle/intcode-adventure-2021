//
// python intcode_cc.py -i math.c string.c 2021/day03.c -o 2021/day03.intc && intcode_vm 2021/day03.intc < 2021/input03.txt
//

int input[12000];

void main() {
    int buffer[20];
    int i_read;
    int L;
    int l;

    // Read input
    for(L=0; L<1000; L+=1) {
        for(l=0; l<12; l+=1) {
            int c;
            scan(&c);
            if(c==49) {         // '1'
                input[12*L + l] = 1;
            }
            else if(c == 48) {  // '0'
                input[12*L + l] = 0;
            }
            else {
                printd(-9999);
                return;
            }
        }
        // read remaining LF (and CR?)
        scans(buffer, 20, 10, &i_read);
    }
    
    // --- PART 1 ---

    // Count number of 1's in every position
    int counter[12];
    for(l=0; l<12; l+=1) {
        counter[l] = 0;
    }
    for(L=0; L<1000; L+=1) {
        for(l=0; l<12; l+=1) {
            counter[l] += input[12*L + l];
        }
    }

    int g_rate; // gamma-rate
    int e_rate; // epsilon-rate
    g_rate=0;
    e_rate=0;
    // Iterate from most-significant-bit (l=0) to least (l=11)
    // At every iteration:
    //  - "bit-shift" g & e upwards
    //  - use criteria to set next bit in g & e
    for(l=0; l<12; l+=1) {
        g_rate = 2*g_rate;
        e_rate = 2*e_rate;
        if(counter[l] > 500) {
            g_rate += 1;
        }
        else {
            e_rate += 1;
        }
    }
    printd(g_rate*e_rate);
    printlf();


    // --- PART 2 ---
    // Initialize o_keepers & c_keepers
    int o_keepers[1000];
    int c_keepers[1000];
    for(L=0; L<1000; L+=1) {
        o_keepers[L] = 1;
        c_keepers[L] = 1;
    }

    // Outer loop: bit-position to consider
    int n_keepers;
    int frequency;
    int one_is_more_frequent;
    for(l=0; l<12; l+=1) {
        // Check among o_keepers
        n_keepers = 0;
        frequency = 0;
        for(L=0; L<1000; L+=1) {
            if(o_keepers[L]) {
                n_keepers += 1;
                frequency += input[12*L + l];
            }
        }
        one_is_more_frequent = 2*frequency >= n_keepers;
        // Filter among o_keepers
        // (n_keepers >= 1 is guaranteed)
        for(L=0; L<1000; L+=1) {
            // Check if bit belongs to most-common set, otherwise exclude
            if((input[12*L + l] == one_is_more_frequent) == 0) {
                o_keepers[L] = 0;
            }
        }
    }
    for(l=0; l<12; l+=1) {
        // Check among c_keepers
        n_keepers = 0;
        frequency = 0;
        for(L=0; L<1000; L+=1) {
            if(c_keepers[L]) {
                n_keepers += 1;
                frequency += input[12*L + l];
            }
        }
        one_is_more_frequent = 2*frequency >= n_keepers;
        // Filter among c_keepers
        // (n_keepers will go to 1, break before 0!)
        if(n_keepers == 1) {
            break;
        }
        for(L=0; L<1000; L+=1) {
            // Check if bit belongs to least-common set, otherwise exclude
            // (when equal -> one_is_more_frequent = 1 -> keep 0's. check!)
            if((input[12*L + l] == one_is_more_frequent) == 1) {
                c_keepers[L] = 0;
            }
        }
    }

    // Find indices of the ratings
    int o_index;
    int c_index;
    for(L=0; L<1000; L+=1) {
        if(o_keepers[L] == 1) {
            o_index = L;
        }
        if(c_keepers[L] == 1) {
            c_index = L;
        }
    }
    int o_rate; // oxygen generator rating
    int c_rate; // CO2 scrubber rating
    o_rate=0;
    c_rate=0;
    // Iterate from most-significant-bit (l=0) to least (l=11)
    for(l=0; l<12; l+=1) {
        o_rate = 2*o_rate;
        c_rate = 2*c_rate;
        if(input[12*o_index + l]) {
            o_rate += 1;
        }
        if(input[12*c_index + l]) {
            c_rate += 1;
        }
    }
    printd(o_rate*c_rate);
    printlf();
}
