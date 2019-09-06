static inline int binaryAdd(int *v, int size, int val) {
    int i, j;

    for (i = 0; i < size; i++) {
        if (val < v[i] || val == v[i]) {
            int prec, temp;
            temp = val;

            for (j = i; j < size + 1; j++) { // shift
                prec = v[j];
                v[j] = temp;
                temp = prec;
            }

            return i;
        } 
    }

    v[size] = val;

    return size;
}

static inline int unsortedSearch(int *v, int size, int val) {
    // Start searching from both ends 
    int front = 0, back = size - 1; 
  
    // Keep searching while two indexes 
    // do not cross. 
    while (front <= back) {
        if (v[front] == val) return front;
        if (v[back] == val) return back;
        front++; 
        back--; 
    }

    return -1; 
}

static inline int binarySearch(int *v, int size, int val) {
    int l = 0, r = size - 1;

    while (l <= r) {
        int m = l + (r - l) / 2;

        // Check if x is present at mid
        if (val == v[m])
            return m;

        // If x greater, ignore left half
        if (val > v[m])
            l = m + 1;

            // If x is smaller, ignore right half
        else
            r = m - 1;
    }
    
    return -1;
}

static inline int binaryDelete(int *v, int size, int val) {
    int i = binarySearch(v, size, val);

    if (i < 0) 
        return 0;
    
    for (; i < size - 1; i++)
        v[i] = v[i+1];

    return 1;
}
