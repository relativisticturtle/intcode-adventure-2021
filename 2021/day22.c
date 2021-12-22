// python intcode_cc.py -i math.c string.c 2021/day22.c -o 2021/day22.txt
// intcode_vm 2021/day22.txt < 2021/input22.txt

void main() {
    int buffer[100];
    int action;
    int i_read;
    int L;
    int l;
    int rs[7000];
    int i;
    int x;
    int y;
    int z;

    for(L=0; 1; L+=1) {
        if(999<L) {
            printd(-9999);
            return;
        }

        scans(buffer, 100, 32, &i_read);
        if(i_read<2) {
            break;
        }
        rs[7*L] = (i_read == 2);

        scans(buffer, 100, 61, &i_read);  // '='
        scans(buffer, 100, 46, &i_read);  // '.'
        str2int(buffer, rs + 7*L + 1);
        scan(buffer);
        scans(buffer, 100, 44, &i_read);  // ','
        str2int(buffer, rs + 7*L + 2);

        scans(buffer, 100, 61, &i_read);  // '='
        scans(buffer, 100, 46, &i_read);  // '.'
        str2int(buffer, rs + 7*L + 3);
        scan(buffer);
        scans(buffer, 100, 44, &i_read);  // ','
        str2int(buffer, rs + 7*L + 4);

        scans(buffer, 100, 61, &i_read);  // '='
        scans(buffer, 100, 46, &i_read);  // '.'
        str2int(buffer, rs + 7*L + 5);
        scan(buffer);
        scans(buffer, 100, 10, &i_read);  // LF
        str2int(buffer, rs + 7*L + 6);

        rs[7*L + 2] += 1;
        rs[7*L + 4] += 1;
        rs[7*L + 6] += 1;
    }

    // Part 1
    int V[1030301]; // 101*101*101

    for(i=0; i<1030301; i+=1) {
        V[i]=0;
    }

    for(l=0; l<L; l+=1) {
        int r;
        r = rs + 7*l;
        if((50<r[1])+(r[2]<-50)+(50<r[3])+(r[4]<-50)+(50<r[5])+(r[6]<-50)) {
            continue;
        }

        for(z=r[5]; z<r[6]; z+=1) {
            for(y=r[3]; y<r[4]; y+=1) {
                for(x=r[1]; x<r[2]; x+=1) {
                    V[10201*(z+50) + 101*(y+50) + x+50] = r[0];
                }
            }
        }
    }

    int counter;
    counter = 0;
    for(i=0; i<1030301; i+=1) {
        counter += V[i];
    }
    printd(counter);
    printlf();


    // Part 2
    int RS1[600000];
    int RS2[600000];
    int RS;
    int S1;
    int S2;
    S1=0;
    S2=0;
    
    int lim[12]; // 3 axes, 4 boundaries

    for(l=0; l<L; l+=1) {
        // new cube to consider: B
        int B;
        B = &rs[7*l];

        // walk through list so far
        int s;
        for(s=0; s<S1; s+=1) {
            // old cube to split
            int A;
            A = &RS1[6*s];

            // in every axis
            int a;
            for(a=0; a<3; a+=1) {
                lim[4*a] = A[2*a];

                if(B[2*a+1]<A[2*a]) {
                    lim[4*a + 1] = A[2*a];
                }
                else if(B[2*a+1]<A[2*a+1]) {
                    lim[4*a + 1] = B[2*a+1];
                }
                else {
                    lim[4*a + 1] = A[2*a+1];
                }

                if(B[2*a+2]<A[2*a]) {
                    lim[4*a + 2] = A[2*a];
                }
                else if(B[2*a+2]<A[2*a+1]) {
                    lim[4*a + 2] = B[2*a+2];
                }
                else {
                    lim[4*a + 2] = A[2*a+1];
                }

                lim[4*a + 3] = A[2*a + 1];
            }
            // check if no intersection
            for(a=0; a<3; a+=1) {
                if((lim[4*a] == lim[4*a+2]) + (lim[4*a+1] == lim[4*a+3])) {
                    break;
                }
            }
            if(a<3) {
                // no intersection, add cube as-is
                RS2[6*S2] = lim[0];
                RS2[6*S2 + 1] = lim[3];
                RS2[6*S2 + 2] = lim[4];
                RS2[6*S2 + 3] = lim[7];
                RS2[6*S2 + 4] = lim[8];
                RS2[6*S2 + 5] = lim[11];
                S2 += 1;
                if(99999<S2) {
                    printd(-9999);
                    printlf();
                    return;
                }
                continue;
            }

            int qz;
            for(qz=0; qz<3; qz+=1) {
                if((lim[8 + qz] < lim[8 + qz + 1]) == 0) {
                    continue;
                }
                int qy;
                for(qy=0; qy<3; qy+=1) {
                    if((lim[4 + qy] < lim[4 + qy + 1]) == 0) {
                        continue;
                    }
                    int qx;
                    for(qx=0; qx<3; qx+=1) {
                        if((lim[qx] < lim[qx + 1]) == 0) {
                            continue;
                        }
                        if((qz==1)*(qy==1)*(qx==1)) {
                            continue; // skip this cubic
                        }


                        RS2[6*S2] = lim[qx];
                        RS2[6*S2 + 1] = lim[qx + 1];
                        RS2[6*S2 + 2] = lim[4 + qy];
                        RS2[6*S2 + 3] = lim[4 + qy + 1];
                        RS2[6*S2 + 4] = lim[8 + qz];
                        RS2[6*S2 + 5] = lim[8 + qz + 1];
                        S2 += 1;
                        if(99999<S2) {
                            printd(-9999);
                            printlf();
                            return;
                        }
                    }
                }
            }
        }

        if(B[0]) {
            RS2[6*S2] = B[1];
            RS2[6*S2 + 1] = B[2];
            RS2[6*S2 + 2] = B[3];
            RS2[6*S2 + 3] = B[4];
            RS2[6*S2 + 4] = B[5];
            RS2[6*S2 + 5] = B[6];
            S2 += 1;
            if(99999<S2) {
                printd(-9999);
                printlf();
                return;
            }
        }

        // Do a switcheroo
        RS = RS1;
        RS1 = RS2;
        RS2 = RS;
        S1 = S2;
        S2 = 0;
    }

    counter = 0;
    for(i=0; i<S1; i+=1) {
        counter += (RS1[6*i+1] - RS1[6*i])*(RS1[6*i+3] - RS1[6*i+2])*(RS1[6*i+5] - RS1[6*i+4]);
    }
    printd(counter); // 1130514303649907
    printlf();
}