#include "jadwal_generator.h"
#include "interface.h"

// --- FUNGSI-FUNGSI PEMBANTU (INTERNAL) ---

static void initShift(Shift *shift) {
    shift->jumlah_dokter = 0;
    shift->kapasitas = 1;
    shift->dokter_list = malloc(shift->kapasitas * sizeof(DokterShift));
    if (shift->dokter_list == NULL) {
        fprintf(stderr, "Gagal alokasi memori.\n");
        exit(EXIT_FAILURE);
    }
}

static void tambahDokterKeShift(Shift *shift, const char *nama) {
    if (shift->jumlah_dokter >= shift->kapasitas) {
        shift->kapasitas *= 2;
        DokterShift *temp = realloc(shift->dokter_list, shift->kapasitas * sizeof(DokterShift));
        if (temp == NULL) {
            fprintf(stderr, "Gagal realokasi memori.\n");
            exit(EXIT_FAILURE);
        }
        shift->dokter_list = temp;
    }
    shift->dokter_list[shift->jumlah_dokter].dokter = strdup(nama);
    shift->jumlah_dokter++;
}

static void bersihkanShift(Shift *shift) {
    if (shift->dokter_list) {
        for (int i = 0; i < shift->jumlah_dokter; i++) {
            free(shift->dokter_list[i].dokter);
        }
        free(shift->dokter_list);
        shift->dokter_list = NULL;
    }
    shift->jumlah_dokter = 0;
    shift->kapasitas = 0;
}

void bersihkanSemuaJadwal() {
    for (int i = 0; i < MAX_HARI; i++) {
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
    int shift_mingguan = 0;
    int hari_awal_minggu = minggu * 7;
    for(int d=hari_awal_minggu; d<=current_day; d++){
        if(isDokterInShift(&jadwal[d].pagi, dokter[dokter_idx].nama) ||
           isDokterInShift(&jadwal[d].siang, dokter[dokter_idx].nama) ||
           isDokterInShift(&jadwal[d].malam, dokter[dokter_idx].nama)) {
            shift_mingguan++;
        }
    }
    if (shift_mingguan >= dokter[dokter_idx].max_shift) {
        return 0;
    }
    return 1;
}


// --- FUNGSI UTAMA PEMBUATAN JADWAL ---

void buatJadwalOtomatis() {
    clear_screen();
    printf("--- Membuat Jadwal Otomatis ---\n");
    if (total_dokter == 0) {
        printf("Tidak ada dokter terdaftar.\n");
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

    Shift *shifts_today[3];
    int gagal = 0;

    for (int hari = 0; hari < MAX_HARI; hari++) {
        shifts_today[0] = &jadwal[hari].pagi;
        shifts_today[1] = &jadwal[hari].siang;
        shifts_today[2] = &jadwal[hari].malam;

        for (int s = 0; s < 3; s++) { // 0: Pagi, 1: Siang, 2: Malam
            Shift *current_shift = shifts_today[s];
            
            // Tahap 1: Isi dengan dokter preferensi
            for (int i = 0; i < total_dokter; i++) {
                int is_preferred = (s == 0 && dokter[i].pref_pagi) ||
                                   (s == 1 && dokter[i].pref_siang) ||
                                   (s == 2 && dokter[i].pref_malam);
                
                if (is_preferred && isDokterAvailable(i, hari) && !isDokterInShift(current_shift, dokter[i].nama)) {
                    tambahDokterKeShift(current_shift, dokter[i].nama);
                    dokter[i].shift_terpakai++;
                }
            }

            // Tahap 2: Jika shift masih kosong, isi dengan non-preferensi
            if (current_shift->jumlah_dokter < 1) {
                int terisi = 0;
                for (int i = 0; i < total_dokter; i++) {
                     int is_preferred = (s == 0 && dokter[i].pref_pagi) ||
                                       (s == 1 && dokter[i].pref_siang) ||
                                       (s == 2 && dokter[i].pref_malam);

                    if (!is_preferred && isDokterAvailable(i, hari) && !isDokterInShift(current_shift, dokter[i].nama)) {
                        tambahDokterKeShift(current_shift, dokter[i].nama);
                        dokter[i].shift_terpakai++;
                        dokter[i].pelanggaran++;
                        terisi = 1;
                        break;
                    }
                }
                if(!terisi) gagal = 1; // Tandai jika tetap gagal mengisi
            }
        }
    }

    if(gagal){
        printf("Peringatan: Beberapa shift mungkin kosong karena kekurangan dokter yang tersedia.\n");
    } else {
        printf("Jadwal 30 hari berhasil dibuat.\n");
    }
    pause_screen();
}
