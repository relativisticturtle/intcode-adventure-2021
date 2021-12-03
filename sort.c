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