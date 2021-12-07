// python intcode_cc.py -i math.c string.c 2021/day07.c -o 2021/day07.txt
// intcode_vm 2021/day07.txt < 2021/input07.txt

int input[1000];

void main() {
    int buffer[20];
    int i_read;
    int L;
    int l;
    
    // read input
    for(L=0; L<999; L+=1) {
        scans(buffer, 20, 44, &i_read);
        str2int(buffer, &input[L]);
    }
    scans(buffer, 20, 10, &i_read);
    str2int(buffer, &input[999]);

    // Part 1
    int least_fuel;
    least_fuel = 99999999;
    for(l=0; l<1000; l+=1) {
        int fuel;
        fuel = 0;
        
        for(L=0; L<1000; L+=1) {
            if(input[L] < l) {
                fuel += l - input[L];
            }
            else {
                fuel += input[L] - l;
            }
        }
        if(fuel < least_fuel) {
            least_fuel = fuel;
        }
    }
    printd(least_fuel);
    printlf();

    // Part 2
    least_fuel = 999999999;
    for(l=0; l<1000; l+=1) {
        int fuel;
        fuel = 0;
        
        int steps;
        for(L=0; L<1000; L+=1) {
            if(input[L] < l) {
                steps = l - input[L];
            }
            else {
                steps = input[L] - l;
            }
            // 1+2+3+...+n = n*(n+1)/2
            // postpone the 2-division to after the loops
            fuel += steps * (steps+1);
        }
        if(fuel < least_fuel) {
            least_fuel = fuel;
        }
    }
    // divide by 2 (see above)
    int res[2];
    divmod(res, least_fuel, 2);
    least_fuel = res[0];
    printd(least_fuel);
    printlf();
}