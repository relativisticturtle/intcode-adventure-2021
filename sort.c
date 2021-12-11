void merge_into(int source, int target, int pivot, int size) {
    int a;
    int b;
    int c;
    a = 0;
    b = pivot;
    for(c=0; c<size; c+=1) {
        if( (b >= size) + (source[a] <= source[b]) * (a < pivot) ) {
            target[c] = source[a];
            a += 1;
        }
        else {
            target[c] = source[b];
            b += 1;
        }
    }
}


//---------------------------------------------------
//
//  sort_into(*source, *target, size)
//
//    sort numbers into ascending sequence into target
//    (note: numbers in source become scrambled)
//
void sort_into(int source, int target, int size) {
    int pivot;
    int res[2];
    divmod(res, size, 2);
    pivot = res[0];
    
    // Sort inplace
    if (pivot > 1) {
        sort_inplace(source, target, pivot);
    }
    if (size - pivot > 1) {
        sort_inplace(&source[pivot], &target[pivot], size - pivot);
    }

    // Merge into target
    merge_into(source, target, pivot, size);
}

//---------------------------------------------------
//
//  sort_inplace(*source, *buffer, size)
//
//    sort numbers into ascending inplace
//    (note: numbers in buffer become scrambled)
//
void sort_inplace(int source, int buffer, int size) {
    int pivot;
    int res[2];
    divmod(res, size, 2);
    pivot = res[0];
    
    // Sort into buffer
    if (pivot > 1) {
        sort_into(source, buffer, pivot);
    }
    else {
        buffer[0] = source[0];
    }
    if (size - pivot > 1) {
        sort_into(&source[pivot], &buffer[pivot], size - pivot);
    }
    else {
        buffer[pivot] = source[pivot];
    }

    // Merge back to source
    merge_into(buffer, source, pivot, size);
}

//---------------------------------------------------
//
//  argsort_into(*source, *target, size)
//
//    sort indices into ascending sequence into target
//    (note: numbers in source become scrambled)
//
void argsort_into(int source, int target, int size) {
    int pivot;
    int res[2];
    divmod(res, size, 2);
    pivot = res[0];
    
    // Sort inplace
    if (pivot > 1) {
        sort_inplace(source, target, pivot);
    }
    if (size - pivot > 1) {
        sort_inplace(&source[pivot], &target[pivot], size - pivot);
    }

    // Merge into target
    merge_into(source, target, pivot, size);
}

//---------------------------------------------------
//
//  argsort_inplace(*source, *buffer, size)
//
void argsort_inplace(int array, int target, int buffer, int size) {
    int pivot;
    int res[2];
    divmod(res, size, 2);
    pivot = res[0];
    
    // Sort into buffer
    if (pivot > 1) {
        argsort_into(array, target, buffer, pivot);
    }
    else {
        buffer[0] = target[0];
    }
    if (size - pivot > 1) {
        sort_into(&target[pivot], &buffer[pivot], size - pivot);
    }
    else {
        buffer[pivot] = target[pivot];
    }

    // Merge back to source
    merge_into(buffer, target, pivot, size);
}

//---------------------------------------------------
//
//  argsort(*array, *target, *buffer, size)
//
//    sort indices into target so that array[target[i]] is ascending
//
void argsort(int array, int target, int buffer, int size) {
    int pivot;
    int res[2];
    divmod(res, size, 2);
    pivot = res[0];

    // Init indexing
    int i;
    for(i=0; i<size; i+=1) {
        target[i]=i;
    }
    argsort_inplace(array, target, buffer, size);
}

//---------------------------------------------------
//
//                     TESTS
//
int _test_sort_OK[] = "OK";
int _test_sort_FAIL[] = "FAIL";
int _test_sort_sort_into[] =    "Sort into                 : ";
int _test_sort_sort_inplace[] = "Sort inplace              : ";

int _test_sequence[] = {1101,1,29,67,1102,0,1,65,1008,65,35,66,1005,66,28,1,67,65,20,4,-13,1001,65,1,65,1106,-10,8,99,35,67,101,99,105,32,110,39,101,115,116,32,112};
int _test_sequence_sorted[] = {-13,-10,0,1,1,1,1,4,8,20,28,29,32,32,35,35,39,65,65,65,65,65,66,66,67,67,67,99,99,101,101,105,110,112,115,116,1001,1005,1008,1101,1102,1106};

void test_sort() {
    int buffer[42];
    int sequence[42];
    int i;

    prints(_test_sort_sort_into);
    for(i=0; i<42; i+=1) {
        sequence[i] = _test_sequence[i];
    }
    sort_into(sequence, buffer, 42);
    for(i=0; (i<42)*(buffer[i] == _test_sequence_sorted[i]); i+=1) {
    }
    if(i==42) {
        prints(_test_sort_OK);
    }
    else {
        prints(_test_sort_FAIL);
    }
    printlf();

    prints(_test_sort_sort_inplace);
    for(i=0; i<42; i+=1) {
        sequence[i] = _test_sequence[i];
    }
    sort_inplace(sequence, buffer, 42);
    for(i=0; (i<42)*(sequence[i] == _test_sequence_sorted[i]); i+=1) {
    }
    if(i==42) {
        prints(_test_sort_OK);
    }
    else {
        prints(_test_sort_FAIL);
    }
    printlf();
}