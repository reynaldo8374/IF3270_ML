// File: sistem_penjadwalan.h

#ifndef SISTEM_PENJADWALAN_H
#define SISTEM_PENJADWALAN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// --- Definisi Konstanta ---
#define MAX_DOKTER 100
#define MAX_HARI 30
#define MAX_SHIFT 3
#define MAX_MINGGU 5
#define MAKS_PER_SHIFT 5

// --- Struktur Data ---
typedef struct
{
    char nama[50];
    int max_shift_per_minggu;
    char preferensi[10];
    int total_shift;
    int pelanggaran_preferensi;
    int shift_per_minggu[MAX_MINGGU];
} Dokter;

typedef struct {
    char shift_nama[10];
    char dokter_dijadwalkan[MAKS_PER_SHIFT][50];
    int jumlah_dokter_dijadwalkan;
} JadwalShift;

// --- Deklarasi Variabel Global (extern) ---
// 'extern' memberitahu kompiler bahwa variabel ini ada,
// tetapi definisinya ada di file .c lain (dalam kasus ini, main.c)
extern Dokter daftar_dokter[MAX_DOKTER];
extern int jumlah_dokter;
extern JadwalShift jadwal_harian[MAX_HARI][MAX_SHIFT];
extern const char* nama_shift_global[MAX_SHIFT];
extern int kebutuhan_shift_global[MAX_SHIFT];
extern int unassigned_shifts;

// --- Prototipe Fungsi ---

// Dari util.c
void clearInputBuffer();
void acak_indeks(int indeks[], int n);

// Dari dokter.c
void tambahDokter();
void displaydokter();
void hapusDokter();
void bacaFile();
void simpandata_dokter();

// Dari penjadwalan.c
void buat_jadwal_otomatis();

// Dari tampilan.c
void tampilkan_statistik_jadwal();
void tampilkan_matriks_jadwal();
void simpan_jadwal_ke_csv(const char* filename);


#endif // SISTEM_PENJADWALAN_H