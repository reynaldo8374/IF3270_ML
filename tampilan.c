// File: tampilan.c

#include "sistem_penjadwalan.h"

// --- FUNGSI BAGIAN 3 & 4: Menampilkan Informasi dan Menyimpan ---

void tampilkan_statistik_jadwal() {
    if (jumlah_dokter == 0) {
        printf("\n[!] Tidak ada data dokter. Tidak dapat menampilkan statistik.\n");
        return;
    }

    printf("\n=== STATISTIK DOKTER TERJADWAL ===\n");
    printf("+----+--------------------------------+---------------------------+--------------------------+--------------------------+\n");
    printf("| No | Nama Dokter                    | Total Shift Dijadwalkan   | Maksimal Shift / Minggu  | Pelanggaran Preferensi   |\n");
    printf("+----+--------------------------------+---------------------------+--------------------------+--------------------------+\n");

    int total_pelanggaran_keseluruhan = 0;

    for (int i = 0; i < jumlah_dokter; i++) {
        Dokter d = daftar_dokter[i];
        printf("| %-2d | %-30s | %-25d | %-24d | %-24d |\n",
               i + 1,
               d.nama,
               d.total_shift,
               d.max_shift_per_minggu,
               d.pelanggaran_preferensi);
        total_pelanggaran_keseluruhan += d.pelanggaran_preferensi;
    }

    printf("+----+--------------------------------+---------------------------+--------------------------+--------------------------+\n");
    
    // Perbaikan UI/UX di bagian footer statistik
    const char *text_pelanggaran_awal = "**TOTAL KESELURUHAN PELANGGARAN PREFERENSI: ";
    const char *text_unassigned_awal = "**TOTAL SLOTS SHIFT TIDAK TERISI: ";

    char num_pelanggaran_str[15];
    snprintf(num_pelanggaran_str, sizeof(num_pelanggaran_str), "%d", total_pelanggaran_keseluruhan);
    
    int len_total_pelanggaran_line_content = strlen(text_pelanggaran_awal) + strlen(num_pelanggaran_str);
    int padding_pelanggaran = 115 - 1 - len_total_pelanggaran_line_content - 1 - 2 - 1; 
    
    printf("| %s%d%*s** |\n", text_pelanggaran_awal, total_pelanggaran_keseluruhan, padding_pelanggaran > 0 ? padding_pelanggaran : 0, "");

    char num_unassigned_str[15];
    snprintf(num_unassigned_str, sizeof(num_unassigned_str), "%d", unassigned_shifts);

    int len_total_unassigned_line_content = strlen(text_unassigned_awal) + strlen(num_unassigned_str);
    int padding_unassigned = 115 - 1 - len_total_unassigned_line_content - 1 - 2 - 1;

    printf("| %s%d%*s** |\n", text_unassigned_awal, unassigned_shifts, padding_unassigned > 0 ? padding_unassigned : 0, "");

    printf("+-------------------------------------------------------------------------------------------------------------------+\n");
    printf("\nCatatan: Pelanggaran preferensi terjadi saat dokter dijadwalkan di shift non-preferensi.\n");
    printf("         Shift tidak terisi adalah slot yang tidak ditemukan dokter yang memenuhi syarat.\n");
}


void tampilkan_matriks_jadwal() {
    int any_shift_assigned = 0;
    for (int h = 0; h < MAX_HARI; h++) {
        for (int s = 0; s < MAX_SHIFT; s++) {
            if (jadwal_harian[h][s].jumlah_dokter_dijadwalkan > 0 &&
                strcmp(jadwal_harian[h][s].dokter_dijadwalkan[0], "-") != 0) {
                any_shift_assigned = 1;
                break;
            }
        }
        if (any_shift_assigned) break;
    }

    if (!any_shift_assigned) {
        printf("\n[!] Jadwal belum dibuat atau masih kosong. Mohon buat jadwal terlebih dahulu.\n");
        return;
    }

    int pil;
    int minggu_pilihan, hari_pilihan;

    printf("\n=== PILIH TAMPILAN JADWAL ===\n");
    printf("1. Jadwal Bulanan (Total %d Hari)\n", MAX_HARI);
    printf("2. Jadwal Mingguan (Per 7 Hari)\n");
    printf("3. Jadwal Harian (Per 1 Hari)\n");
    printf("4. Kembali ke Menu Utama\n");
    printf("-----------------------------\n");
    printf("Pilihan Anda: ");
    scanf("%d", &pil);
    clearInputBuffer();

    printf("\n");

    if (pil == 1) { // Bulanan
        printf("Jadwal Bulanan (Hari 1 - %d)\n", MAX_HARI);
        printf("+-----+--------------------------------+--------------------------------+--------------------------------+\n");
        printf("| Hari| %-30s | %-30s | %-30s |\n", "Pagi", "Siang", "Malam");
        printf("+-----+--------------------------------+--------------------------------+--------------------------------+\n");

        for (int h = 0; h < MAX_HARI; h++) {
            printf("| %-4d|", h + 1);
            for (int s = 0; s < MAX_SHIFT; s++) {
                printf(" ");
                if (jadwal_harian[h][s].jumlah_dokter_dijadwalkan > 0) {
                    char temp_str[55];
                    snprintf(temp_str, sizeof(temp_str), "1. %s", jadwal_harian[h][s].dokter_dijadwalkan[0]);
                    printf("%-30s |", temp_str);
                } else {
                    printf(" %-29s |", "KOSONG");
                }
            }
            printf("\n");

            for (int k = 1; k < MAKS_PER_SHIFT; k++) {
                printf("|     |");
                for (int s = 0; s < MAX_SHIFT; s++) {
                    printf(" ");
                    if (k < jadwal_harian[h][s].jumlah_dokter_dijadwalkan) {
                        char temp_str[55];
                        snprintf(temp_str, sizeof(temp_str), "%d. %s", k + 1, jadwal_harian[h][s].dokter_dijadwalkan[k]);
                        printf("%-30s |", temp_str);
                    } else {
                        printf("%-30s |", "");
                    }
                }
                printf("\n");
            }
        }
        printf("+-----+--------------------------------+--------------------------------+--------------------------------+\n");
    }
    else if (pil == 2) { // Mingguan
        printf("Pilih Minggu (1-%d): ", MAX_MINGGU);
        scanf("%d", &minggu_pilihan);
        clearInputBuffer();

        if (minggu_pilihan < 1 || minggu_pilihan > MAX_MINGGU) {
            printf("[!] Input minggu tidak valid!\n");
            return;
        }
        int start_hari = (minggu_pilihan - 1) * 7;
        int end_hari = start_hari + 7;
        if (end_hari > MAX_HARI) end_hari = MAX_HARI;

        printf("Jadwal Mingguan ke-%d (Hari %d - %d)\n", minggu_pilihan, start_hari + 1, end_hari);
        printf("+-----+--------------------------------+--------------------------------+--------------------------------+\n");
        printf("| Hari| %-30s | %-30s | %-30s |\n", "Pagi", "Siang", "Malam");
        printf("+-----+--------------------------------+--------------------------------+--------------------------------+\n");

        for (int h = start_hari; h < end_hari; h++) {
             printf("| %-4d|", h + 1);
            for (int s = 0; s < MAX_SHIFT; s++) {
                printf(" ");
                if (jadwal_harian[h][s].jumlah_dokter_dijadwalkan > 0) {
                    char temp_str[55];
                    snprintf(temp_str, sizeof(temp_str), "1. %s", jadwal_harian[h][s].dokter_dijadwalkan[0]);
                    printf("%-30s |", temp_str);
                } else {
                    printf(" %-29s |", "KOSONG");
                }
            }
            printf("\n");

            for (int k = 1; k < MAKS_PER_SHIFT; k++) {
                printf("|     |");
                for (int s = 0; s < MAX_SHIFT; s++) {
                    printf(" ");
                    if (k < jadwal_harian[h][s].jumlah_dokter_dijadwalkan) {
                        char temp_str[55];
                        snprintf(temp_str, sizeof(temp_str), "%d. %s", k + 1, jadwal_harian[h][s].dokter_dijadwalkan[k]);
                        printf("%-30s |", temp_str);
                    } else {
                        printf("%-30s |", "");
                    }
                }
                printf("\n");
            }
        }
        printf("+-----+--------------------------------+--------------------------------+--------------------------------+\n");
    }
    else if (pil == 3) { // Harian
        printf("Pilih Hari (1-%d): ", MAX_HARI);
        scanf("%d", &hari_pilihan);
        clearInputBuffer();

        if (hari_pilihan < 1 || hari_pilihan > MAX_HARI) {
            printf("[!] Input hari tidak valid!\n");
            return;
        }
        int h = hari_pilihan - 1;

        printf("Jadwal Harian ke-%d\n", hari_pilihan);
        printf("+--------+---------------------------------------------------------------+\n");
        printf("| Shift  | Dokter                                                          |\n");
        printf("+--------+---------------------------------------------------------------+\n");

        for (int s = 0; s < MAX_SHIFT; s++) {
            printf("| %-6s |", nama_shift_global[s]);
            if (jadwal_harian[h][s].jumlah_dokter_dijadwalkan > 0) {
                char temp_str[65];
                snprintf(temp_str, sizeof(temp_str), "1. %s", jadwal_harian[h][s].dokter_dijadwalkan[0]);
                printf(" %-62s|\n", temp_str);
            } else {
                printf(" %-62s|\n", "KOSONG");
            }

            for (int k = 1; k < MAKS_PER_SHIFT; k++) {
                printf("|        |");
                if (k < jadwal_harian[h][s].jumlah_dokter_dijadwalkan) {
                    char temp_str[65];
                    snprintf(temp_str, sizeof(temp_str), "%d. %s", k + 1, jadwal_harian[h][s].dokter_dijadwalkan[k]);
                    printf(" %-62s|\n", temp_str);
                } else {
                    printf(" %-62s|\n", "");
                }
            }
        }
        printf("+--------+---------------------------------------------------------------+\n");
    }
    else if (pil == 4) {
        printf("Kembali ke menu utama.\n");
        return;
    }
    else {
        printf("[!] Pilihan tidak valid! Mohon pilih antara 1-4.\n");
    }
}

void simpan_jadwal_ke_csv(const char* filename) {
    int any_shift_assigned = 0;
    for (int h = 0; h < MAX_HARI; h++) {
        for (int s = 0; s < MAX_SHIFT; s++) {
            if (jadwal_harian[h][s].jumlah_dokter_dijadwalkan > 0 &&
                strcmp(jadwal_harian[h][s].dokter_dijadwalkan[0], "-") != 0) {
                any_shift_assigned = 1;
                break;
            }
        }
        if (any_shift_assigned) break;
    }

    if (!any_shift_assigned) {
        printf("\n[!] Jadwal belum dibuat atau masih kosong. Tidak ada yang disimpan.\n");
        return;
    }

    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("\n[!] Gagal membuat file '%s'. Pastikan Anda memiliki izin tulis pada direktori ini.\n", filename);
        return;
    }

    fprintf(file, "Hari,Pagi,Siang,Malam\n");

    for (int h = 0; h < MAX_HARI; h++) {
        fprintf(file, "%d", h + 1);
        for (int s = 0; s < MAX_SHIFT; s++) {
            fprintf(file, ",");
            if (jadwal_harian[h][s].jumlah_dokter_dijadwalkan > 0) {
                for (int k = 0; k < jadwal_harian[h][s].jumlah_dokter_dijadwalkan; k++) {
                    fprintf(file, "%s", jadwal_harian[h][s].dokter_dijadwalkan[k]);
                    if (k < jadwal_harian[h][s].jumlah_dokter_dijadwalkan - 1) {
                        fprintf(file, " & ");
                    }
                }
            } else {
                fprintf(file, "KOSONG");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("[v] Jadwal berhasil disimpan ke file: '%s'\n", filename);
}