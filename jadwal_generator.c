#include "jadwal.h"

// ===========================================
// FUNGSI PEMBANTU (HELPER) - INTERNAL
// ===========================================

static void initShift(Shift *shift) {
    shift->jumlah_dokter = 0;
    shift->kapasitas = 1;
    shift->dokter_list = malloc(shift->kapasitas * sizeof(DokterShift));
    if (shift->dokter_list == NULL) {
        fprintf(stderr, "Alokasi memori gagal untuk Shift.\n");
        exit(EXIT_FAILURE);
    }
}

static void tambahDokterKeShift(Shift *shift, const char *nama, const char *pref) {
    if (shift->jumlah_dokter >= shift->kapasitas) {
        shift->kapasitas *= 2;
        DokterShift *temp = realloc(shift->dokter_list, shift->kapasitas * sizeof(DokterShift));
        if (temp == NULL) {
            fprintf(stderr, "Re-alokasi memori gagal.\n");
            exit(EXIT_FAILURE);
        }
        shift->dokter_list = temp;
    }
    shift->dokter_list[shift->jumlah_dokter].dokter = strdup(nama);
    shift->dokter_list[shift->jumlah_dokter].preferensi = strdup(pref);
    shift->jumlah_dokter++;
}

static void bersihkanShift(Shift *shift) {
    if (shift->dokter_list) {
        for (int i = 0; i < shift->jumlah_dokter; i++) {
            free(shift->dokter_list[i].dokter);
            free(shift->dokter_list[i].preferensi);
        }
        free(shift->dokter_list);
        shift->dokter_list = NULL;
    }
    shift->jumlah_dokter = 0;
    shift->kapasitas = 0;
}

void bersihkanSemuaJadwal() {
    for (int i = 0; i < MAX_HARI; i++) {
        // Cek jika sudah diinisialisasi sebelum dibersihkan
        if (jadwal[i].pagi.kapasitas > 0) bersihkanShift(&jadwal[i].pagi);
        if (jadwal[i].siang.kapasitas > 0) bersihkanShift(&jadwal[i].siang);
        if (jadwal[i].malam.kapasitas > 0) bersihkanShift(&jadwal[i].malam);
    }
}

static void resetDokterStats() {
    for (int i = 0; i < total_dokter; i++) {
        dokter[i].shift_terpakai = 0;
        dokter[i].pelanggaran = 0;
    }
}

static int isDokterInShift(Shift *shift, const char *dokter_nama) {
    for (int i = 0; i < shift->jumlah_dokter; i++) {
        if (strcmp(shift->dokter_list[i].dokter, dokter_nama) == 0) {
            return 1;
        }
    }
    return 0;
}

static int isDokterAvailable(int dokter_idx, int current_day) {
    int minggu = current_day / 7;
    if (dokter[dokter_idx].shift_terpakai >= (dokter[dokter_idx].max_shift * (minggu + 1))) {
        return 0;
    }
    return 1;
}

// ===========================================
// FUNGSI UTAMA PEMBUATAN JADWAL
// ===========================================

void buatJadwalOtomatis() {
    int gagal = 0;
    char list_gagal[100][100];
    int jumlah_gagal = 0;

    clear_screen();
    printf("--- Membuat Jadwal Otomatis ---\n");
    if (total_dokter == 0) {
        printf("Tidak ada dokter terdaftar. Mohon tambahkan dokter.\n");
        pause_screen();
        return;
    }
    
    bersihkanSemuaJadwal();
    resetDokterStats();

    for (int hari = 0; hari < MAX_HARI; hari++) {
        initShift(&jadwal[hari].pagi);
        initShift(&jadwal[hari].siang);
        initShift(&jadwal[hari].malam);
    }

    const char *shift_names[] = {"Pagi", "Siang", "Malam"};
    Shift *shifts_today[3];

    for (int hari = 0; hari < MAX_HARI; hari++) {
        shifts_today[0] = &jadwal[hari].pagi;
        shifts_today[1] = &jadwal[hari].siang;
        shifts_today[2] = &jadwal[hari].malam;

        for (int s = 0; s < 3; s++) {
            Shift *current_shift = shifts_today[s];
            const char *nama_shift = shift_names[s];
            
            for (int i = 0; i < total_dokter; i++) {
                if (strcmp(dokter[i].preferensi, nama_shift) == 0 &&
                    isDokterAvailable(i, hari) &&
                    !isDokterInShift(current_shift, dokter[i].nama)) {
                    tambahDokterKeShift(current_shift, dokter[i].nama, dokter[i].preferensi);
                    dokter[i].shift_terpakai++;
                }
            }

            if (current_shift->jumlah_dokter < 1) {
                int assigned_non_preferred = 0;
                for (int i = 0; i < total_dokter; i++) {
                    if (strcmp(dokter[i].preferensi, nama_shift) != 0 &&
                        isDokterAvailable(i, hari) &&
                        !isDokterInShift(current_shift, dokter[i].nama)) {
                        tambahDokterKeShift(current_shift, dokter[i].nama, dokter[i].preferensi);
                        dokter[i].shift_terpakai++;
                        dokter[i].pelanggaran++;
                        assigned_non_preferred = 1;
                        break;
                    }
                }

                if (!assigned_non_preferred) {
                    snprintf(list_gagal[jumlah_gagal++], sizeof(list_gagal[0]),
                             "Gagal: Kekurangan dokter untuk hari %d, shift %s.",
                             hari + 1, nama_shift);
                    gagal = 1;
                    continue;
                }
            }
        }
    }

    if (gagal) {
        for (int i = 0; i < jumlah_gagal; i++) {
            printf("%s\n", list_gagal[i]);
        }
        printf("\nMohon tambah dokter atau sesuaikan batas shift.\n");
        bersihkanSemuaJadwal();
    } else {
        printf("Jadwal 30 hari berhasil dibuat.\n");
    }
    pause_screen();
}