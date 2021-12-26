// python intcode_cc.py -i math.c string.c sort.c test.c -o test.txt
// intcode_vm test.txt

int _test_core_OK[] = "OK";
int _test_core_FAIL[] = "FAIL";
int _test_core_comparisons[] = "Comparisons               : ";
int _test_core_comparisons_negative[] = "Comparisons with negative : ";
int _test_core_expressions[] = "Expressions               : ";
int _test_core_for_loop_empty_cond[] = "For-loop with empty cond. : ";


void test_core() {
    int a;
    int b[10];
    int res;

    a = 10;
    prints(_test_core_comparisons);
    res = (a==10)+(a<11)+(a>9)+(a>=10)+(a<=10);
    if(res == 5) {
        prints(_test_core_OK);
    }
    else {
        prints(_test_core_FAIL);
    }
    printlf();

    a = -10;
    prints(_test_core_comparisons_negative);
    res = (a==-10)+(a>-11)+(a<-9)+(a<=-10)+(a>=-10);
    if(res == 5) {
        prints(_test_core_OK);
    }
    else {
        prints(_test_core_FAIL);
    }
    printlf();

    prints(_test_core_expressions);
    b[0] = 1;
    b[1] = 1;
    for(a=2; a<10; a+=1) {
        // 1 1 2 3 5 8 13 21 34 55
        b[a]=b[a-1]+b[a-2];
    }
    res = b[9] == 55;
    res += 10*(20 - 7) + (4 - 3*(9 - 6))*2 == 120;
    res += b[b[10*(20 - 7) - 124] - 5] == 34;
    res += -2*b[b[10*(20 + -7) - 124] + -5] + -2 == -70;
    if(res == 4) {
        prints(_test_core_OK);
    }
    else {
        prints(_test_core_FAIL);
    }
    printlf();

    prints(_test_core_for_loop_empty_cond);
    for(a=0; ; a+=1) {
        if(a==5) {
            break;
        }
    }
    if(a == 5) {
        prints(_test_core_OK);
    }
    else {
        prints(_test_core_FAIL);
    }
    printlf();
}

void main() {
    test_core();
    test_math();
    test_sort();
}