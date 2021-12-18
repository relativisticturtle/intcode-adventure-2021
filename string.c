int _buffer[20];
void prints(int s) {
    for(; s[0]; s+=1) {
        print(s[0]);
    }
}
void printd(int d) {
    if(d == 0) {
        print(48);
        return;
    }
    if(d < 0) {
        print(45);
        d = -d;
    }

    // buffer ptr - start from end
    int b;
    b = &_buffer[19];
    b[0] = 0;

    // div-by-10 loop
    int res[2];
    for(; d>0;) {
        b -= 1;
        divmod(res, d, 10);
        d = res[0];
        b[0] = 48 + res[1];
    }

    prints(b);
}
void printlf() {
    print(10);
}
void scans(int target, int i_max, int delim, int i_read) {
    int i;

    for(i=0; i<i_max;) {
        int c;
        scan(&c);
        if((c <= 0) + (c == delim)) {
            break;
        }
        if((delim == 32) * (c == 10)) {
            // treat lf as a ws
            break;
        }
        if(((delim == 10) + (delim == 32)) * (c == 13)) {
            // ignore cr when delim is lf or ws
            continue;
        }
        target[i] = c;
        i += 1;
    }
    target[i] = 0;
    i_read[0] = i;
}
void str2int(int s, int out) {
    for(; (s[0] == 32) + (s[0] == 9) + (s[0] == 10) + (s[0] == 13); s += 1) {
        // Skip whitespace characters
    }
    int sign;
    if(s[0] == 45) {
        sign = -1;
        s += 1;
    }
    else if(s[0] == 43) {
        sign = 1;
        s += 1;
    }
    else {
        sign = 1;
    }
    out[0] = 0;
    for(; (47 < s[0]) * (s[0] < 58); s+=1) {
        out[0] = 10*out[0] + s[0] - 48;
    }
    out[0] *= sign;
}
void strcmp(int res, int s1, int s2) {
    int i;
    for(i=0; s1[i]*(s1[i]==s2[i]); i+=1) {
    }
    res[0] = s2[i] - s1[i];
}
void strncmp(int res, int s1, int s2, int i_max) {
    int i;
    for(i=0; (i<i_max)*s1[i]*(s1[i]==s2[i]); i+=1) {
    }
    res[0] = s2[i] - s1[i];
}
void strcpy(int dest, int src) {
    int i;
    for(i=0; src[i]; i+=1) {
        dest[i] = src[i];
    }
    dest[i]=0;
}