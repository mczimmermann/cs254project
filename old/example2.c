//Insertion Sort
// MULLIFIER SUBSET OF C

#ifndef N
#define N 10
int vals[N] = {4,2,3,5,6,10,6,0,1,3};
#else
#include "data.h"
#endif

int insertion_sort() {
    for (int i = 1; i < N; i++) {
        int key = vals[i];
        int j = i - 1;
        while ((0 < j || j == 0) && key < vals[j]) {
            vals[j+1] = vals[j];
            j = j - 1;
        }
        vals[j+1] = key;
    }
    return 0;
}

int main(void) {
    insertion_sort();
    return 0;
}