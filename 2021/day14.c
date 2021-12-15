// python intcode_cc.py -i math.c string.c 2021/day14.c -o 2021/day14.txt
// intcode_vm 2021/day14.txt < 2021/input14.txt

void insertion(int pairs, int rules) {
    int new_pairs[676];
    int L;
    for(L=0; L<676; L+=1) {
        new_pairs[L] = 0;
    }
    for(L=0; L<676; L+=1) {
        // pair[L] --> 2 new pairs, as defined by rules
        new_pairs[rules[2*L]] += pairs[L];
        new_pairs[rules[2*L+1]] += pairs[L];
    }
    for(L=0; L<676; L+=1) {
        pairs[L] = new_pairs[L];
    }
}

void main() {
    int polymer_template[100];
    int pairs[676];
    int rules[1352]; // 26*26*2
    int buffer[26];
    int i_read;
    int L;
    int l;

    // read template
    scans(polymer_template, 100, 10, &i_read);
    for(L=0; L<676; L+=1) {
        pairs[L] = 0;
    }
    for(L=0; polymer_template[L+1]; L+=1) {
        l = 26*(polymer_template[L] - 65) + polymer_template[L+1] - 65;
        pairs[l] += 1;
    }

    scans(buffer, 100, 10, &i_read); // discard empty line
    
    // read rules
    for(L=0; L<1000; L+=1) {
        scans(buffer, 100, 10, &i_read);
        if(i_read < 7) {
            break;
        }
        int pair[3];
        pair[0] = 26*(buffer[0] - 65) + buffer[1] - 65;
        pair[1] = 26*(buffer[0] - 65) + buffer[6] - 65;
        pair[2] = 26*(buffer[6] - 65) + buffer[1] - 65;

        rules[2*pair[0]] = pair[1];
        rules[2*pair[0]+1] = pair[2];
    }

    for(L=0; L<10; L+=1) {
        insertion(pairs, rules);
    }

    // Part 1
    int elements[26];
    // Count 2nd in every pair
    for(l=0; l<26; l+=1) {
        elements[l] = 0;
        for(L=0; L<26; L+=1) {
            elements[l] += pairs[26*L + l];
        }
    }
    // Add the first element from template
    elements[polymer_template[0]-65] += 1;

    sort_inplace(elements, buffer, 26);
    for(L=0; elements[L]==0; L+=1) {
    }
    printd(elements[25] - elements[L]);
    printlf();

    // Part 2
    for(L=10; L<40; L+=1) {
        insertion(pairs, rules);
    }
    // Count 2nd in every pair
    for(l=0; l<26; l+=1) {
        elements[l] = 0;
        for(L=0; L<26; L+=1) {
            elements[l] += pairs[26*L + l];
        }
    }
    // Add the first element from template
    elements[polymer_template[0]-65] += 1;

    sort_inplace(elements, buffer, 26);
    for(L=0; elements[L]==0; L+=1) {
    }
    printd(elements[25] - elements[L]);
    printlf();
}
