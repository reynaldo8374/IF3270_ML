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

// DEFINISI STRUCT
typedef struct {
    char nama[50];
    int max_shift;
    char preferensi[10];
    int shift_terpakai;
    int pelanggaran;
} Dokter;

typedef struct {
    char *dokter;
    char *preferensi;
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