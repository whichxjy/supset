int is_subset(int* small, int small_size, int* large, int large_size) {
    for (int i = 0; i < small_size; i++) {
        int j;
        for (j = 0; j < large_size; j++) {
            if (large[j] == small[i]) {
                break;
            }
        }
        if (j == large_size) {
            return 0;
        }
    }
    return 1;
}