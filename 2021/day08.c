// python intcode_cc.py -i math.c string.c 2021/day08.c -o 2021/day08.txt
// intcode_vm 2021/day08.txt < 2021/input08.txt

void main() {
    int buffer[64];
    int i_read;
    int L;
    int counter_p1;
    int counter_p2;
    int l;
    int s;

    // Segments are identifiable with 2 pieces of information
    int segmf[7]; // how many digits do they appear in
    int segm4[7]; // do they appear in the "4"?

    // segmC maps from 'abcdefg' to segment position 0, 1, 2, 3, 4, 5, 6
    //    - 0 -
    //   1     2
    //    - 3 -
    //   4     5
    //    - 6 -
    int segmC[7];
    
    counter_p1=0;
    counter_p2=0;
    for(L=0; L<200; L+=1) {
        // Reset segment-counters
        for(l=0; l<7; l+=1) {
            segmf[l] = 0;
            segm4[l] = 0;
        }
        // Read 10 words
        for(l=0; l<10; l+=1) {
            scans(buffer, 64, 32, &i_read);
            for(s=0; buffer[s]; s+=1) {
                segmf[buffer[s]-97] += 1; // 97 == 'a'
                if(i_read == 4) {
                    segm4[buffer[s]-97] = 1;
                }
            }
        }

        // Use information from the segment-counters to identify all segments
        for(l=0; l<7; l+=1) {
            if((segmf[l] == 8)*(1-segm4[l])) {
                segmC[l] = 0;
            }
            else if((segmf[l] == 6)) {
                segmC[l] = 1;
            }
            else if((segmf[l] == 8)*segm4[l]) {
                segmC[l] = 2;
            }
            else if((segmf[l] == 7)*segm4[l]) {
                segmC[l] = 3;
            }
            else if((segmf[l] == 4)) {
                segmC[l] = 4;
            }
            else if((segmf[l] == 9)) {
                segmC[l] = 5;
            }
            else if((segmf[l] == 7)*(1-segm4[l])) {
                segmC[l] = 6;
            }
            else {
                printd(-segmf[l]);
                return;
            }
        }
        
        // Flush '|', and ' '
        scans(buffer, 64, 124, &i_read);
        scan(buffer);

        int segment_code;
        int number;
        number = 0;
        for(l=0; l<4; l+=1) {
            // LED-encoding (see segment map above)
            // >>> segment_on = ['1110111', '0100100', '1011101', '1101101', '0101110', '1101011', '1111011', '0100101', '1111111', '1101111']
            // >>> print(', '.join(['%d' % int(s,2) for s in segment_on]))  
            // 119, 36, 93, 109, 46, 107, 123, 37, 127, 111
            segment_code=0;
            scans(buffer, 64, 32, &i_read);
            for(s=0; buffer[s]; s+=1) {
                // borrow _xp2 from 'math.c' for fast binary encoding
                segment_code += _xp2[segmC[buffer[s]-97]];
            }

            number *= 10;
            if(segment_code == 119) {
                number += 0;
            }
            else if(segment_code == 36) {
                number += 1;
                counter_p1 += 1;
            }
            else if(segment_code == 93) {
                number += 2;
            }
            else if(segment_code == 109) {
                number += 3;
            }
            else if(segment_code == 46) {
                number += 4;
                counter_p1 += 1;
            }
            else if(segment_code == 107) {
                number += 5;
            }
            else if(segment_code == 123) {
                number += 6;
            }
            else if(segment_code == 37) {
                number += 7;
                counter_p1 += 1;
            }
            else if(segment_code == 127) {
                number += 8;
                counter_p1 += 1;
            }
            else if(segment_code == 111) {
                number += 9;
            }
            else {
                printd(-segment_code);
                return;
            }
        }
        counter_p2 += number;
    }
    
    // Part 1
    printd(counter_p1); // 310
    printlf();
    
    // Part 2
    printd(counter_p2); // 915941
    printlf();
}