#ifndef JADWAL_H
#define JADWAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ===========================================
// KONSTANTA GLOBAL
// ===========================================

#define MAX_DOKTER 20
#define MAX_LINE 100
#define MAX_HARI 30

// ===========================================
// DEFINISI STRUCT
// ===========================================

typedef struct {
    char nama[50];
    int max_shift;
    char preferensi[10]; // "Pagi", "Siang", "Malam"
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

// ===========================================
// DEKLARASI VARIABEL GLOBAL (EXTERN)
// ===========================================

extern Dokter dokter[MAX_DOKTER];
extern int total_dokter;
extern JadwalHari jadwal[MAX_HARI];

// ===========================================
// PROTOTIPE FUNGSI
// ===========================================

// --- Dari interface.c ---
void clear_screen();
void pause_screen();
void displayMainMenu();
void handleDoctorManagement();
void handleScheduleManagement();
void handleScheduleView();

// --- Dari dokter_handler.c ---
void trim_whitespace(char *str);
void initDokterArray();
void bacaFileCSV();
void simpanFileCSV();
void tambahDokterBaru();
void hapusDokter();
void tampilkanDaftarDokter();

// --- Dari jadwal_generator.c ---
void bersihkanSemuaJadwal();
void buatJadwalOtomatis();

// --- Dari jadwal_io.c ---
void cetakJadwalRange(int start_day, int end_day);
void cetakRingkasanShiftDokter();
void simpanJadwalKeCSV();

#endif // JADWAL_H