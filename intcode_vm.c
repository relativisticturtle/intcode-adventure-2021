// python intcode_cc.py -i math.c string.c intcode_vm.c -o intcode_vm.txt

int msg_illegal_addr[] = "Illegal instruction-address: ";
int msg_invalid_op[] = "Invalid OP: ";
int msg_illegal_access[] = "Illegal access: code";
int msg_illegal_mode_eval[] = "Illegal mode for eval: ";
int msg_immediate_mode_forbidden[] = "Immediate mode forbidden for assignment";


void eval(int out, int code, int p, int mode, int rb, int error) {
    if(mode == 0) {
        if(p<0) {
            error[0] = 1001;
            prints(msg_illegal_access);
            print(91);
            printd(p);
            print(93);
            printlf();
            return;
        }
        out[0] = code[p];
    }
    else if(mode == 1) {
        out[0] = p;
    }
    else if(mode == 2) {
        if(rb+p<0) {
            error[0] = 1003;
            prints(msg_illegal_access);
            print(91);
            printd(rb);
            print(43);
            printd(p);
            print(93);
            printlf();
            return;
        }
        out[0] = code[rb + p];
    }
    else { 
        error[0] = 1004;
        prints(msg_illegal_mode_eval);
        printd(mode);
        printlf();
    }
}

void assign(int val, int code, int p, int mode, int rb, int error) {
    if(mode == 0) {
        if(p<0) {
            error[0] = 2001;
            prints(msg_illegal_access);
            print(91);
            printd(p);
            print(93);
            printlf();
            return;
        }
        code[p] = val;
    }
    else if(mode == 1) {
        error[0] = 2002;
        prints(msg_immediate_mode_forbidden);
        printlf();
    }
    else if(mode == 2) {
        if(rb+p<0) {
            error[0] = 2003;
            prints(msg_illegal_access);
            print(91);
            printd(rb);
            print(43);
            printd(p);
            print(93);
            printlf();
            return;
        }
        code[rb + p] = val;
    }
    else { 
        error[0] = 2004;
        prints(msg_illegal_mode_eval);
        printd(mode);
        printlf();
    }
}

void main() {
    int code;
    int i;
    int c;
    int decimal_mode;

    // Set code-ptr to unallocated area
    // - and hope no collision occurs!
    // 
    // Approx. 8100 integers for code and global memory.
    // Some extra for local memory. But this program has no recursion
    // or deep call stacks. Seems OK!
    code = 10000;

    // Decimal (non-ASCII) mode
    decimal_mode = 0;

    // Read IntCode
    c = 44;
    for(i=0; c==44; i+=1) {         // ','
        code[i] = 0;

        int sign;
        scan(&c);
        if(c==45) {                 // '-'
            sign = -1;
            c = 48;                 // '0'
        }
        else if((47<c)*(c<58)) {    // '0' - '9'
            sign = 1;
        }
        else {
            break;
        }
        for(; (47<c)*(c<58); scan(&c)) {
            code[i] = 10*code[i] + c - 48;
        }
        code[i] *= sign;
    }

    // Discard up to LF
    for(; (0<c)*((c==10)==0); scan(&c)) {
        if(c==68) {                // 'D'
            decimal_mode = 1;
        }
    }

    // Zero-set 100k integers beyond that
    //for(; i<100000; i+=1) {
    //    code[i]=0;
    //}

    // -------------
    //     Run!
    // -------------
    int rb;
    int stop;
    int error;
    int res[2];
    int buffer[20];
    i = 0;
    rb = 0;
    error = 0;
    
    for(stop=0; (stop==0)*(error==0);) {
        if((i<0)+(999999<i)) {
            error[0] = 1;
            prints(msg_illegal_addr);
            printd(i);
            printlf();
            break;
        }

        int OP;
        int A;
        int B;
        int C;

        divmod(res, code[i], 100);
        OP = res[1];    // code[i] % 100;
        divmod(res, res[0], 10);
        C = res[1];     // (code[i] / 100) % 10;
        divmod(res, res[0], 10);
        B = res[1];     // (code[i] / 1000) % 10;
        A = res[0];     // (code[i] / 10000) % 10;

        if(OP == 1) {
            eval(&C, code, code[i+1], C, rb, &error);
            if(error) {
                break;
            }
            eval(&B, code, code[i+2], B, rb, &error);
            if(error) {
                break;
            }
            assign(C+B, code, code[i+3], A, rb, &error);
            if(error) {
                break;
            }
            i += 4;
        }
        else if(OP == 2) {
            eval(&C, code, code[i+1], C, rb, &error);
            if(error) {
                break;
            }
            eval(&B, code, code[i+2], B, rb, &error);
            if(error) {
                break;
            }
            assign(C*B, code, code[i+3], A, rb, &error);
            if(error) {
                break;
            }
            i += 4;
        }
        else if(OP == 3) {
            int val;
            if(decimal_mode) {
                scans(buffer, 20, 32, &val);
                str2int(buffer, &val);
            }
            else {
                scan(&val);
            }
            assign(val, code, code[i+1], C, rb, &error);
            if(error) {
                break;
            }
            i += 2;
        }
        else if(OP == 4) {
            eval(&C, code, code[i+1], C, rb, &error);
            if(error) {
                break;
            }
            if(decimal_mode) {
                printd(C);
                printlf();
            }
            else {
                print(C);
            }
            i += 2;
        }
        else if((OP==5)+(OP==6)) {
            eval(&C, code, code[i+1], C, rb, &error);
            if(error) {
                break;
            }
            if((OP==6)==(C==0)) {
                eval(&B, code, code[i+2], B, rb, &error);
                if(error) {
                    break;
                }
                i = B;
            }
            else {
                i += 3;
            }
        }
        else if(OP == 7) {
            eval(&C, code, code[i+1], C, rb, &error);
            if(error) {
                break;
            }
            eval(&B, code, code[i+2], B, rb, &error);
            if(error) {
                break;
            }
            assign(C<B, code, code[i+3], A, rb, &error);
            if(error) {
                break;
            }
            i += 4;
        }
        else if(OP == 8) {
            eval(&C, code, code[i+1], C, rb, &error);
            if(error) {
                break;
            }
            eval(&B, code, code[i+2], B, rb, &error);
            if(error) {
                break;
            }
            assign(C==B, code, code[i+3], A, rb, &error);
            if(error) {
                break;
            }
            i += 4;
        }
        else if(OP == 9) {
            eval(&C, code, code[i+1], C, rb, &error);
            if(error) {
                break;
            }
            rb += C;
            i += 2;
        }
        else if(OP == 99) {
            stop = 1;
        }
        else {
            prints(msg_invalid_op);
            printd(OP);
            printlf();
            break;
        }
    }
    if(error) {
        printd(-error);
        printlf();
    }
}