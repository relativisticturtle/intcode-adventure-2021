//---------------------------------------------------
//
//  divmod(*res, a, b)
//
//    res[0]: a / b (integer division)
//    res[1]: a % b (modulus)
//
//---------------------------------------------------

int _xp2[32] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648};

void divmod(int res, int a, int b) {
    res[0] = 0;
    int p;
    for(p=31; p>=0; p-=1) {
        if(b*_xp2[p] <= a) {
            res[0] += _xp2[p];
            a -= b*_xp2[p];
        }
    }
    res[1] = a;
}
