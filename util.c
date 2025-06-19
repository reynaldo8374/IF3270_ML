// File: util.c

#include "sistem_penjadwalan.h"

// --- FUNGSI BANTUAN (HELPER FUNCTIONS) ---
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void acak_indeks(int indeks[], int n) {
    for (int i = 0; i < n; i++) {
        indeks[i] = i;
    }
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indeks[i];
        indeks[i] = indeks[j];
        indeks[j] = temp;
    }
}