// python intcode_cc.py -i math.c string.c 2021/day19.c -o 2021/day19.txt
// intcode_vm 2021/day19.txt < 2021/input19.txt

// 24 rotations x 9 elements per rotation-matrix
int R[] = {1,0,0,0,1,0,0,0,1,-1,0,0,0,-1,0,0,0,1,0,1,0,-1,0,0,0,0,1,0,-1,0,1,0,0,0,0,1,1,0,0,0,-1,0,0,0,-1,-1,0,0,0,1,0,0,0,-1,0,1,0,1,0,0,0,0,-1,0,-1,0,-1,0,0,0,0,-1,1,0,0,0,0,1,0,-1,0,-1,0,0,0,0,-1,0,-1,0,0,0,1,-1,0,0,0,-1,0,0,0,-1,1,0,0,0,-1,0,1,0,0,0,0,-1,0,1,0,-1,0,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1,0,0,0,-1,-1,0,0,0,1,0,0,1,0,0,0,1,1,0,0,0,-1,0,0,0,-1,1,0,0,0,0,1,0,-1,0,1,0,0,0,0,-1,0,1,0,1,0,0,0,1,0,0,0,-1,-1,0,0,0,-1,0,0,0,1,-1,0,0,0,0,1,0,1,0,-1,0,0,0,0,-1,0,-1,0,-1,0,0};
int msg_placing[] = "Placing scanner ";
int msg_success[] = "  Success! (#matches = ";
int msg_fail[] = "  No matches :(";
int msg_number_beacons[] = "Part 1 - Number of beacons: ";
int msg_distance[] = "Part 2 - Longest distance : ";

// --- IN ---
// P[3*N]        : determined beacon positions
// scanners[5*S] : determined scanner positions
// detected[3*D] : to be matched detections
// --- OUT ---
// best_transform : [rot_id, x, y, z]
// best_matches   : best match number
void find_best_overlap(int beacons, int B, int scanners, int S, int detected, int D, int best_transform, int best_matches) {
    int origin[3];
    int Q[3];
    int ri;

    best_matches[0] = 0;
    for(ri=0; ri<24; ri+=1) {  // for every rotation
        int r;
        r = &R[9*ri];
        int b;
        for(b=0; b<B; b+=1) {       //
            int d;                  // for every pair
            for(d=0; d<D; d+=1) {   //
                // Calculate would-be origin
                origin[0] = beacons[3*b  ] - (r[0]*detected[3*d] + r[1]*detected[3*d + 1] + r[2]*detected[3*d + 2]);
                origin[1] = beacons[3*b+1] - (r[3]*detected[3*d] + r[4]*detected[3*d + 1] + r[5]*detected[3*d + 2]);
                origin[2] = beacons[3*b+2] - (r[6]*detected[3*d] + r[7]*detected[3*d + 1] + r[8]*detected[3*d + 2]);
                
                // count number of matches - abort on violation
                int matches;
                matches = 0;
                int q;
                for(q=0; q<D; q+=1) {
                    // Calculate would-be beacon position
                    Q[0] = origin[0] + r[0]*detected[3*q] + r[1]*detected[3*q + 1] + r[2]*detected[3*q + 2];
                    Q[1] = origin[1] + r[3]*detected[3*q] + r[4]*detected[3*q + 1] + r[5]*detected[3*q + 2];
                    Q[2] = origin[2] + r[6]*detected[3*q] + r[7]*detected[3*q + 1] + r[8]*detected[3*q + 2];

                    // Skip if outside range of all previous scanners
                    int s;
                    for(s=0; s<S; s+=1) {
                        if(scanners[5*s] == 0) {
                            continue; // skip. not yet placed
                        }
                        int sc;
                        sc = &scanners[5*s + 2];
                        if((Q[0]<sc[0]-1000)+(sc[0]+1000<Q[0])+(Q[1]<sc[1]-1000)+(sc[1]+1000<Q[1])+(Q[2]<sc[2]-1000)+(sc[2]+1000<Q[2]) == 0) {
                            break; // in range
                        }
                    }
                    if(s==S) {
                        continue; // outside range of all scanners
                    }

                    // Now the beacon _must_ be in beacons, otherwise not valid
                    int p;
                    for(p=0; p<B; p+=1) { // for every beacon
                        if((Q[0]==beacons[3*p])*(Q[1]==beacons[3*p+1])*(Q[2]==beacons[3*p+2])) {
                            break;
                        }
                    }
                    if(p==B) {
                        break; // not found, not valid
                    }
                    matches += 1;
                }
                if(q==D) {
                    // every detection was either out of range or matched to a beacon
                    if(best_matches[0] < matches) {
                        best_matches[0] = matches;
                        best_transform[0] = ri;
                        best_transform[1] = origin[0];
                        best_transform[2] = origin[1];
                        best_transform[3] = origin[2];
                    }
                }
            }
        }
    }
}


void place_beacons(int beacons, int B, int detected, int D, int transform) {
    int r;
    int origin;
    r = &R[9*transform[0]];
    origin = &transform[1];

    int b;
    b=B[0];
    int Q;

    int q;
    for(q=0; q<D; q+=1) {
        // Calculate would-be beacon position
        Q = &beacons[3*b];
        Q[0] = origin[0] + r[0]*detected[3*q] + r[1]*detected[3*q + 1] + r[2]*detected[3*q + 2];
        Q[1] = origin[1] + r[3]*detected[3*q] + r[4]*detected[3*q + 1] + r[5]*detected[3*q + 2];
        Q[2] = origin[2] + r[6]*detected[3*q] + r[7]*detected[3*q + 1] + r[8]*detected[3*q + 2];

        // Check if already present
        int p;
        for(p=0; p<b; p+=1) {
            if((Q[0]==beacons[3*p])*(Q[1]==beacons[3*p+1])*(Q[2]==beacons[3*p+2])) {
                break;
            }
        }
        if(p==b) {
            b+=1; // not present. expand list
        }
    }
    B[0] = b;
}


void main() {
    int input[7550]; // (50 scanners) * (1 length + (50 beacons / scanner) * (3 positions / beacon))
    int buffer[20];
    int i_read;
    int S;
    int B;
    
    // Read input
    for(S=0; S<50; S+=1) {
        scans(buffer, 20, 10, &i_read); // Discard "--- scanner <S> ---"
        if(i_read < 17) {
            break;
        }
        for(B=0; B<50; B+=1) {
            scans(buffer, 20, 10, &i_read); // LF
            if(i_read < 5) {
                input[151*S] = B;
                break;
            }
            int b1; // find first ','
            for(b1=buffer; (b1[0]==44)==0; b1+=1) {
            }
            int b2; // find second ','
            for(b2=b1+1; (b2[0]==44)==0; b2+=1) {
            }
            // substitute the ',' for zero-terminations
            b1[0] = 0;
            b2[0] = 0;
            str2int(buffer, &input[151*S + 3*B + 1]);
            str2int(b1+1, &input[151*S + 3*B + 2]);
            str2int(b2+1, &input[151*S + 3*B + 3]);
        }
    }

    // Determined scanner positions (50 * [isplaced?, rot_id, x, y, z])
    int scanners[250];
    int has_been_placed[50];
    int s;
    for(s=0; s<S; s+=1) {
        has_been_placed[s] = 0;
    }

    // Determined beacon positions
    int beacons[7500]; // (2500 * [x, y, z])

    // Use scanner 0 as reference
    scanners[0] = 1;
    scanners[1] = 0;
    scanners[2] = 0;
    scanners[3] = 0;
    scanners[4] = 0;
    has_been_placed[0] = 1;

    for(B=0; B<input[0]; B+=1) {
        beacons[3*B] = input[3*B + 1];
        beacons[3*B+1] = input[3*B + 2];
        beacons[3*B+2] = input[3*B + 3];
    }
    

    int best_matches;
    s=0;
    for(;;) {
        int z;
        z = s;
        for(s=z+1; (s==z)==0; s+=1) {
            if(s==S) {
                s=0;
            }
            if(scanners[5*s] == 0) {
                break;
            }
        }
        if(s==z) {
            // Success! everyone are where they are supposed to be
            break;
        }
        prints(msg_placing);
        printd(s);
        print(46);
        print(46);
        print(46);
        print(10);
        find_best_overlap(beacons, B, scanners, S, &input[151*s + 1], input[151*s], &scanners[5*s+1], &best_matches);
        if(11<best_matches) {
            scanners[5*s] = 1;
            place_beacons(beacons, &B, &input[151*s + 1], input[151*s], &scanners[5*s+1]);
            prints(msg_success);
            printd(best_matches);
            print(41);
            printlf();
        }
        else {
            prints(msg_fail);
            printlf();
        }
    }

    // Part 1
    printlf();
    prints(msg_number_beacons);
    printd(B);
    printlf();

    // Part 2
    int longest_distance;
    longest_distance = 0;
    for(s=0; s<S; s+=1) {
        int z;
        for(z=0; z<s; z+=1) {
            int dx;
            int dy;
            int dz;
            dx = scanners[5*s + 2] - scanners[5*z + 2];
            dy = scanners[5*s + 3] - scanners[5*z + 3];
            dz = scanners[5*s + 4] - scanners[5*z + 4];
            int distance;
            distance = dx*(0<dx) - dx*(dx<0) + dy*(0<dy) - dy*(dy<0) + dz*(0<dz) - dz*(dz<0);
            if(longest_distance < distance) {
                longest_distance = distance;
            }
        }
    }
    prints(msg_distance);
    printd(longest_distance);
    printlf();
}