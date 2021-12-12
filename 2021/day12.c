// python intcode_cc.py -i math.c string.c 2021/day12.c -o 2021/day12.txt
// intcode_vm 2021/day12.txt < 2021/input12.txt

int str_start[] = "start";
int str_end[] = "end";

void go(int G, int visited, int i, int counter) {
    visited[i] += 1;
    if(i==1) { // know 'end' is at 1
        counter[0] += 1;
    }
    int l;
    int j;
    for(l=10; (l<100)*(G[100*i + l]<9999); l+=1) {
        j = G[100*i + l];
        if((visited[j] == 0) + (G[100*j] < 97)) { // 'a'
            go(G, visited, j, counter);
        }
    }
    visited[i] -= 1;
}

void run(int G, int visited, int i, int counter, int sc2x, int level) {
    int l;

    if(i==1) { // know 'end' is at 1
        counter[0] += 1;
        return;
    }
    visited[i] += 1;

    
    int j;
    for(l=10; (l<100)*(G[100*i + l]<9999); l+=1) {
        j = G[100*i + l];
        if((visited[j] == 0) + (G[100*j] < 97)) {
            // not visited or large
            run(G, visited, j, counter, sc2x, level + 1);
        }
        else if((1<j)*(sc2x == 0)*(96 < G[100*j])) {
            // small cave, has been visited before
            run(G, visited, j, counter, 1, level + 1);
        }
    }
    visited[i] -= 1;
}

void main() {
    int G[10000];
    int buffer1[10];
    int buffer2[10];
    int i_read;
    int size;
    int L;
    int l;
    int res;
    int i;
    int j;

    // Reset G
    //   node[n] : G[100*i] : [name(10), neighbours(90)]
    for(l=0; l<10000; l+=1) {
        G[l] = 9999;
    }
    strcpy(G, str_start);
    strcpy(&G[100], str_end);

    // scan input
    size = 2;
    for(L=0; L<100; L+=1) {
        scans(buffer1, 10, 45, &i_read); // '-'
        if(i_read < 1) {
            break;
        }
        for(i=0; i<size; i+=1) {
            strcmp(&res, buffer1, &G[100*i]);
            if(res==0) {
                break;
            }
        }
        if(i==size) {
            size += 1;
            strcpy(&G[100*i], buffer1);
        }
        
        scans(buffer2, 10, 10, &i_read); // newline
        for(j=0; j<size; j+=1) {
            strcmp(&res, buffer2, &G[100*j]);
            if(res==0) {
                break;
            }
        }
        if(j==size) {
            size += 1;
            strcpy(&G[100*j], buffer2);
        }
        for(l=10; (l<100)*(G[100*i + l]<9999); l+=1) {
        }
        G[100*i + l] = j;
        for(l=10; (l<100)*(G[100*j + l]<9999); l+=1) {
        }
        G[100*j + l] = i;
    }

    // Part 1
    int counter;
    counter = 0;
    int visited[100];
    for(l=0; l<100; l+=1) {
        visited[l] = 0;
    }
    go(G, visited, 0, &counter);
    printd(counter); // 3369
    printlf();

    // Part 2
    counter = 0;
    for(l=0; l<100; l+=1) {
        visited[l] = 0;
    }
    run(G, visited, 0, &counter, 0, 0);
    printd(counter); // 85883
    printlf();
}
