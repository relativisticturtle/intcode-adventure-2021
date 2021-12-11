//---------------------------------------------------
//
//  _xp[n]:  2^n, for 0 <= n <= 62
//
int _xp2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296, 8589934592, 17179869184, 34359738368, 68719476736, 137438953472, 274877906944, 549755813888, 1099511627776, 2199023255552, 4398046511104, 8796093022208, 17592186044416, 35184372088832, 70368744177664, 140737488355328, 281474976710656, 562949953421312, 1125899906842624, 2251799813685248, 4503599627370496, 9007199254740992, 18014398509481984, 36028797018963968, 72057594037927936, 144115188075855872, 288230376151711744, 576460752303423488, 1152921504606846976, 2305843009213693952, 4611686018427387904};

//---------------------------------------------------
//
//  divmod(*res, a, b)
//
//    res[0]: a / b (integer division)
//    res[1]: a % b (modulus)
//
void divmod(int res, int a, int b) {
    res[0] = 0;
    int p;
    // Guard against overflow:
    //   make sure b*_xp2[p] (see next loop) is < 2^62
    //   --> b < _xp2[62 - p]
    for(p=62; (p>=0)*(b > _xp2[62 - p]); p-=1) {
    }
    // Now it should be safe to multiply b and _xp2[p]
    for(; p>=0; p-=1) {
        if(b*_xp2[p] <= a) {
            res[0] += _xp2[p];
            a -= b*_xp2[p];
        }
    }
    res[1] = a;
}

//---------------------------------------------------
//
//  gcd(*res, a, b)
//
//    res[0]: a and b's greatest common divisor
//
void gcd(int res, int a, int b) {
    int r[2];
    // Swap a and b if necessary, so that a>b
    if(a<b) {
        r[1]=a;
        a=b;
        b=r[1];
    }
    // Euclidean algorithm
    for(; r[1];) {
        divmod(r, a, b);
        a=b;
        b=r[1];
    }
    res[0] = a;
}

//---------------------------------------------------
//
//  lcm(*res, a, b)
//
//    res[0]: a and b's least common multiple
//
void lcm(int res, int a, int b) {
    int r[2];
    gcd(r, a, b);
    divmod(r, a, r[0]);
    res[0] = r[0]*b;
}


//---------------------------------------------------
//
//                     TESTS
//
int _test_math_OK[] = "OK";
int _test_math_FAIL[] = "FAIL";
int _test_math_divmod_32bit[] = "Divide with 32bit numbers : ";
int _test_math_divmod_64bit[] = "Divide with 64bit numbers : ";
int _test_math_gcd[] = "Greatest common divisor   : ";
int _test_math_lcm[] = "Least common multiple     : ";

void test_math() {
    int res[2];

    prints(_test_math_divmod_32bit);
    divmod(res, 983746582, 569456);
    if((res[0] == 1727)*(res[1] == 296070)) {
        prints(_test_math_OK);
    }
    else {
        prints(_test_math_FAIL);
    }
    printlf();

    prints(_test_math_divmod_64bit);
    divmod(res, 983746582498848675, 569456897664);
    if((res[0] == 1727517)*(res[1] == 111017028387)) {
        prints(_test_math_OK);
    }
    else {
        prints(_test_math_FAIL);
    }
    printlf();

    prints(_test_math_gcd);
    gcd(res, 76779324006, 9145363612);
    if(res[0] == 154) {
        prints(_test_math_OK);
    }
    else {
        prints(_test_math_FAIL);
    }
    printlf();

    prints(_test_math_lcm);
    lcm(res, 128925, 898375);
    if(res[0] == 4632919875) {
        prints(_test_math_OK);
    }
    else {
        prints(_test_math_FAIL);
    }
    printlf();
}