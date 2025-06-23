#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// KONSTANTA GLOBAL
#define MAX_DOKTER 20
#define MAX_LINE 100
#define MAX_HARI 30

// DEFINISI STRUCT (versi multi-preferensi)
typedef struct {
    char nama[50];
    int max_shift;
    int pref_pagi;      // 1 jika preferensi, 0 jika tidak
    int pref_siang;     // 1 jika preferensi, 0 jika tidak
    int pref_malam;     // 1 jika preferensi, 0 jika tidak
    int shift_terpakai;
    int pelanggaran;
} Dokter;

typedef struct {
    char *dokter;
} DokterShift;

typedef struct {
    DokterShift *dokter_list;
    int jumlah_dokter;
    int kapasitas;
} Shift;

typedef struct {
    Shift pagi;
    Shift siang;
    Shift malam;
} JadwalHari;

// DEKLARASI VARIABEL GLOBAL (EXTERN)
extern Dokter dokter[MAX_DOKTER];
extern int total_dokter;
extern JadwalHari jadwal[MAX_HARI];

#endif // DEFINITIONS_H
