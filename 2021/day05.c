// python intcode_cc.py -i math.c string.c 2021/day05.c -o 2021/day05.txt
// intcode_vm 2021/day05.txt < 2021/input05.txt

int pipes[2000];    // 4*500
int grid[1000000];  // 1000*1000

void main() {
    int buffer[20];
    int i_read;
    int L;
    int i;
    int j;
    int x;
    int y;
    int x1;
    int x2;
    int y1;
    int y2;
    int counter;
    
    for(L=0; L<500; L+=1) {
        scans(buffer, 20, 44, &i_read);
        str2int(buffer, &pipes[4*L]);
        scans(buffer, 20, 32, &i_read);
        str2int(buffer, &pipes[4*L + 1]);
        scans(buffer, 20, 32, &i_read); // discard '-> '
        scans(buffer, 20, 44, &i_read);
        str2int(buffer, &pipes[4*L + 2]);
        scans(buffer, 20, 10, &i_read);
        str2int(buffer, &pipes[4*L + 3]);
    }

    
    // Part 1
    for(j=0; j<500; j+=1) {
        if(pipes[4*j] == pipes[4*j + 2]) {
            // vertical
            x=pipes[4*j];
            if(pipes[4*j + 1] < pipes[4*j + 3]) {
                y1 = pipes[4*j + 1];
                y2 = pipes[4*j + 3];
            }
            else {
                y1 = pipes[4*j + 3];
                y2 = pipes[4*j + 1];
            }
            for(i=y1; i<=y2; i+=1) {
                grid[1000*i + x] += 1;
            }
        }
        if(pipes[4*j + 1] == pipes[4*j + 3]) {
            // horizontal
            y=pipes[4*j + 1];
            if(pipes[4*j + 0] < pipes[4*j + 2]) {
                x1 = pipes[4*j + 0];
                x2 = pipes[4*j + 2];
            }
            else {
                x1 = pipes[4*j + 2];
                x2 = pipes[4*j + 0];
            }
            for(i=x1; i<=x2; i+=1) {
                grid[1000*y + i] += 1;
            }
        }
    }

    counter=0;
    for(y=0; y<1000; y+=1) {
        for(x=0; x<1000; x+=1) {
            if(1 < grid[1000*y + x]) {
                counter += 1;
            }
        }
    }

    printd(counter); // 7318
    printlf();


    // Part 2
    for(j=0; j<500; j+=1) {
        if(pipes[4*j + 0] < pipes[4*j + 2]) {
            x1 = pipes[4*j + 0];
            x2 = pipes[4*j + 2];
        }
        else {
            x1 = pipes[4*j + 2];
            x2 = pipes[4*j + 0];
        }
        if(pipes[4*j + 1] < pipes[4*j + 3]) {
            y1 = pipes[4*j + 1];
            y2 = pipes[4*j + 3];
        }
        else {
            y1 = pipes[4*j + 3];
            y2 = pipes[4*j + 1];
        }
        if((pipes[4*j + 2] - pipes[4*j + 0]) == (pipes[4*j + 3] - pipes[4*j + 1])) {
            // diagonal 1
            for(i=0; i<=x2-x1; i+=1) {
                grid[1000*y1 + 1000*i + x1 + i] += 1;
            }
        }
        if((pipes[4*j + 2] - pipes[4*j + 0]) == (pipes[4*j + 1] - pipes[4*j + 3])) {
            // diagonal 2
            for(i=0; i<=x2-x1; i+=1) {
                grid[1000*y1 + 1000*i + x2 - i] += 1;
            }
        }
    }

    counter=0;
    for(y=0; y<1000; y+=1) {
        for(x=0; x<1000; x+=1) {
            if(1 < grid[1000*y + x]) {
                counter += 1;
            }
        }
    }

    printd(counter); // 19939
    printlf();
}