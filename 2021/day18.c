// python intcode_cc.py -i math.c string.c 2021/day18.c -o 2021/day18.txt
// intcode_vm 2021/day18.txt < 2021/input18.txt

int sn_sum[3000]; // 1000 x [in use?, v1, v2]

void snalloc(int p_ptr) {
    int j;
    for(j=0; sn_sum[3*j]; j+=1) {
    }
    if(j<1000) {
        sn_sum[3*j] = 1;
        p_ptr[0] = &sn_sum[3*j];
    }
    else {
        p_ptr[0] = -1;
        printd(-9998);
    }
}

void snfree(int ptr) {
    ptr[0] = 0;
}

void parse_sn(int string, int i_read, int result) {
    // expect: [ <...>, <...> ]
    // return: pointer to created pair
    if((string[0] == 91) == 0) { // '['
        printd(-9999);
        return;
    }
    int results[2];
    int offset;
    offset = 1;
    int p;
    for(p=0; p<2; p+=1) {
        if(string[offset] == 91) {
            int v1;
            int j_read;
            parse_sn(&string[offset], &j_read, &results[p]);
            if(results[p] < 0) {
                result[0] = -1;
                return;
            }
            offset += j_read;
        }
        else if((47 < string[offset])*(string[offset] < 58)) {
            int r;
            r = 0;
            for(; (47 < string[offset]) * (string[offset] < 58); offset+=1) {
                r = 10*r + string[offset] - 48;
            }
            results[p] = r;
        }
        if((p==0)*(string[offset]==44) + (p==1)*(string[offset]==93) == 1) {
            offset+=1;
        }
        else {
            result[0] = -1;
            printd(-9997);
            print(32);
            print(48 + p);
            print(32);
            print(string[offset]);
            printlf();
            return;
        }
    }
    
    // string[offset] == ']'
    // Two values / pointers in results[]
    int ptr;
    snalloc(&ptr);
    ptr[1] = results[0];
    ptr[2] = results[1];
    result[0] = ptr;
    i_read[0] = offset;
}

void print_sn(int ptr) {
    print(91);
    if(ptr[1] < 1000) {
        printd(ptr[1]);
    }
    else {
        print_sn(ptr[1]);
    }
    print(44);
    if(ptr[2] < 1000) {
        printd(ptr[2]);
    }
    else {
        print_sn(ptr[2]);
    }
    print(93);
}

void add_to_sn(int ptr, int side, int value) {
    if(ptr[1+side] < 1000) {
        ptr[1+side] += value;
    }
    else {
        add_to_sn(ptr[1+side], side, value);
    }
}

void explode_sn(int ptr, int level, int explosion, int reduction_occured) {
    if(level == 4) {
        explosion[0] = ptr[1]; // assume literals
        explosion[1] = ptr[2];
        snfree(ptr);
        reduction_occured[0] = 1;
        return;
    }
    if(1000 < ptr[1]) {
        // explode left side
        explode_sn(ptr[1], level+1, explosion, reduction_occured);
        if(level == 3) {
            // this pair must have exploded. replace with 0
            ptr[1] = 0;
        }
        if(0<explosion[1]) { // right-travelling explosion
            if(ptr[2]<1000) {
                // can be immediately put in right neighbour
                ptr[2] += explosion[1];
            }
            else {
                // traverse right neighbour's left side
                add_to_sn(ptr[2], 0, explosion[1]);
            }
            explosion[1] = 0;
        }
        // left-travelling explosion remains in explosion[0]
    }
    if((1000 < ptr[2])*(reduction_occured[0]==0)) {
        // explode right side
        explode_sn(ptr[2], level+1, explosion, reduction_occured);
        if(level == 3) {
            // this pair must have exploded. replace with 0
            ptr[2] = 0;
        }
        if(0<explosion[0]) { // left-travelling explosion
            if(ptr[1]<1000) {
                // can be immediately put in left neighbour
                ptr[1] += explosion[0];
            }
            else {
                // traverse left neighbour's right side
                add_to_sn(ptr[1], 1, explosion[0]);
            }
            explosion[0] = 0;
        }
        // right-travelling explosion remains in explosion[1]
    }
}

void split_sn(int ptr, int reduction_occured) {
    int i;
    for(i=1; i<3; i+=1) {
        if(1000 < ptr[i]) {
            split_sn(ptr[i], reduction_occured);
            if(reduction_occured[0]) {
                return;
            }
        }
        else if(9 < ptr[i]) {
            int p;
            snalloc(&p);
            int res[2];
            divmod(res, ptr[i], 2);
            p[1] = res[0];
            p[2] = res[0] + res[1];
            ptr[i] = p;
            reduction_occured[0] = 1;
            return;
        }
    }
}

void reduce_sn(int ptr) {
    int explosion[2];
    int reduction_occured;
    for(reduction_occured=1; reduction_occured;) {
        // explode
        reduction_occured = 0;
        explosion[0] = 0;
        explosion[1] = 0;
        explode_sn(ptr, 0, explosion, &reduction_occured);
        if(reduction_occured) {
            continue;
        }

        // split
        split_sn(ptr, &reduction_occured);
        if(reduction_occured) {
            continue;
        }
    }
}

void magnitude_of_sn(int ptr, int value) {
    int v;
    value[0] = 0;
    int i;
    for(i=1; i<3; i+=1) {
        if(1000 < ptr[i]) {
            magnitude_of_sn(ptr[i], &v);
            value[0] += (4-i)*v;
        }
        else {
            value[0] += (4-i)*ptr[i];
        }
    }
}


void main() {
    int buffer[20000];
    int L;
    int l;
    int root;
    int i;

    // init snail math memory (not needed?)
    for(i=0; i<3000; i+=1) {
        sn_sum[i] = 0;
    }
    
    // read input to buffer
    for(L=0; 1; L+=1) {
        scans(&buffer[L*100], 100, 10, &i);
        if(i<5) {
            break;
        }
    }
    
    // Part 1
    root = 0;
    for(l=0; l<L; l+=1) {
        if(l==0) {
            parse_sn(buffer, &i, &root);
        }
        else {
            // Add
            int new_term;
            int new_root;
            parse_sn(&buffer[100*l], &i, &new_term);
            snalloc(&new_root);
            new_root[1] = root;
            new_root[2] = new_term;
            root = new_root;
        }

        // Reduce
        reduce_sn(root);
    }
    int value;
    magnitude_of_sn(root, &value);
    printd(value); // 4207
    printlf();


    int l1;
    int l2;
    int max_val;
    max_val = 0;
    for(l1=0; l1<L; l1+=1) {
        for(l2=0; l2<L; l2+=1) {
            if(l1==l2) {
                continue;
            }
            for(i=0; i<3000; i+=1) {
                sn_sum[i] = 0;
            }
            // Add
            int term1;
            int term2;
            parse_sn(&buffer[100*l1], &i, &term1); // processing is destructive. must re-read
            parse_sn(&buffer[100*l2], &i, &term2);
            snalloc(&root);
            root[1] = term1;
            root[2] = term2;

            // Reduce
            reduce_sn(root);

            // Calculate magnitude - check if max
            magnitude_of_sn(root, &value);
            if(max_val < value) {
                max_val = value;
                
            }
        }
    }
    printd(max_val); // 4635
    printlf();
}
