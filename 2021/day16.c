// python intcode_cc.py -i math.c string.c 2021/day16.c -o 2021/day16.txt
// intcode_vm 2021/day16.txt < 2021/input16.txt

// -------------------------------
// v v v        : packet version
// 1 0 0        : type ID == 4
// 1 x x x x    : group 1
// 1 x x x x    : group 2
//  ...
// 0 x x x x    : last group

// -------------------------------
// v v v        : packet version
// t t t        : type ID != 4
// 0            : length type ID == 0
// d d d d d d
// d d d d d d  : (15 bits of) total length of sub-packets
// d d d
// <sub-packets>

// -------------------------------
// v v v        : packet version
// t t t        : type ID != 4
// 1            : length type ID == 1
// d d d d d d
// d d d d d    : (11 bits of) number of sub-packets
// <sub-packets>

void consume_packet(int bits, int bits_stop, int version_acc, int result) {
    int version;
    int type_id;
    int literal;

    // read header
    version = 4*bits[0] + 2*bits[1] + bits[2];
    type_id = 4*bits[3] + 2*bits[4] + bits[5];


    bits += 6;

    // version
    version_acc[0] += version;

    
    if(type_id == 4) {
        // literal
        for(literal=0; ; bits+=5) {
            literal = 16*literal + 8*bits[1] + 4*bits[2] + 2*bits[3] + bits[4];
            if(bits[0] == 0) {
                break;
            }
        }
        bits += 5;
        result[0] = literal;
    }
    else {
        // operation
        int length_type_id;
        int length;
        int l;
        int values[100];
        length_type_id = bits[0];
        if(length_type_id==0) {
            length = 16384*bits[1] + 8192*bits[2] + 4096*bits[3] + 2048*bits[4] + 1024*bits[5] + 512*bits[6] + 256*bits[7] + 128*bits[8] + 64*bits[9] + 32*bits[10] + 16*bits[11] + 8*bits[12] + 4*bits[13] + 2*bits[14] + bits[15];
            bits += 16;
            int bits_sub;
            bits_sub=bits;
            for(l=0; bits_sub<bits+length; l+=1) {
                consume_packet(bits_sub, &bits_sub, version_acc, &values[l]);
            }
            bits = bits_sub;
        }
        else {
            length = 1024*bits[1] + 512*bits[2] + 256*bits[3] + 128*bits[4] + 64*bits[5] + 32*bits[6] + 16*bits[7] + 8*bits[8] + 4*bits[9] + 2*bits[10] + bits[11];
            bits += 12;
            int bits_sub;
            bits_sub=bits;
            for(l=0; l<length; l+=1) {
                consume_packet(bits_sub, &bits_sub, version_acc, &values[l]);
            }
            bits = bits_sub;
        }

        // results from sub-package evaluation in: values[0], ..., values[l-1]
        int answer;
        int i;
        if(type_id == 0) {
            answer = 0;
            for(i=0; i<l; i+=1) {
                answer += values[i];
            }
        }
        else if(type_id == 1) {
            answer = 1;
            for(i=0; i<l; i+=1) {
                answer *= values[i];
            }
        }
        else if(type_id == 2) {
            answer = values[0];
            for(i=0; i<l; i+=1) {
                if(values[i]<answer) {
                    answer = values[i];
                }
            }
        }
        else if(type_id == 3) {
            answer = values[0];
            for(i=0; i<l; i+=1) {
                if(answer<values[i]) {
                    answer = values[i];
                }
            }
        }
        else if(type_id == 5) {
            answer = values[1] < values[0];
        }
        else if(type_id == 6) {
            answer = values[0] < values[1];
        }
        else if(type_id == 7) {
            answer = values[0] == values[1];
        }
        result[0] = answer;
    }
    bits_stop[0] = bits;
}


void main() {
    int message[3000];
    int bits[12000];
    int i_read;
    int i;
    int c;

    // Scan input
    scans(message, 3000, 10, &i_read);
    for(i=0; 47 < message[i]; i+=1) {
        c = message[i] - 48;
        if(9<c) {
            c -= 7;
        }
        bits[4*i+3] = c;
        divmod(&bits[4*i+2], bits[4*i+3], 2);
        divmod(&bits[4*i+1], bits[4*i+2], 2);
        divmod(&bits[4*i], bits[4*i+1], 2);
    }


    // Part 1 & 2
    int version_acc;
    int bits_stop;
    int result;
    version_acc = 0;
    consume_packet(bits, &bits_stop, &version_acc, &result);
    printd(version_acc);
    printlf();
    printd(result);
    printlf();
}
