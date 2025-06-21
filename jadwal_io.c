#include "jadwal.h"

// Fungsi internal, tidak perlu di header
static int get_max_lines_in_shift_display(int countP, int countS, int countM) {
    int max = countP;
    if (countS > max) max = countS;
    if (countM > max) max = countM;
    return max > 0 ? max : 1; // Pastikan minimal 1 baris untuk cetak
}

void cetakJadwalRange(int start_day, int end_day) {
    if (jadwal[0].pagi.kapasitas == 0) { // Cek apakah jadwal sudah di-init
        printf("Jadwal belum dibuat. Mohon buat jadwal terlebih dahulu.\n");
        return;
    }

    printf("+------+--------------------------------+--------------------------------+--------------------------------+\n");
    printf("| Hari |              Pagi              |              Siang             |              Malam             |\n");
    printf("+------+--------------------------------+--------------------------------+--------------------------------+\n");

    for (int hari = start_day - 1; hari < end_day; hari++) {
        if (hari < 0 || hari >= MAX_HARI) continue;

        int countP = jadwal[hari].pagi.jumlah_dokter;
        int countS = jadwal[hari].siang.jumlah_dokter;
        int countM = jadwal[hari].malam.jumlah_dokter;
        int maxLines = get_max_lines_in_shift_display(countP, countS, countM);

        for (int line = 0; line < maxLines; line++) {
            printf("| %-4s ", (line == 0) ? " " : "");
            if(line == 0) printf("\b\b\b\b%d", hari + 1);


            char buffer[35];

            // Pagi
            if (line < countP) {
                snprintf(buffer, sizeof(buffer), "%d. %s", line + 1, jadwal[hari].pagi.dokter_list[line].dokter);
                printf("| %-30s ", buffer);
            } else {
                printf("| %-30s ", "");
            }

            // Siang
            if (line < countS) {
                snprintf(buffer, sizeof(buffer), "%d. %s", line + 1, jadwal[hari].siang.dokter_list[line].dokter);
                printf("| %-30s ", buffer);
            } else {
                printf("| %-30s ", "");
            }

            // Malam
            if (line < countM) {
                snprintf(buffer, sizeof(buffer), "%d. %s", line + 1, jadwal[hari].malam.dokter_list[line].dokter);
                printf("| %-30s |\n", buffer);
            } else {
                printf("| %-30s |\n", "");
            }
        }
        printf("+------+--------------------------------+--------------------------------+--------------------------------+\n");
    }
    printf("\n");
}

void cetakRingkasanShiftDokter() {
    clear_screen();
    printf("--- Ringkasan Shift Dokter ---\n");
    if (total_dokter == 0) {
        printf("Tidak ada dokter terdaftar.\n");
        pause_screen();
        return;
    }
    if (jadwal[0].pagi.kapasitas == 0) {
        printf("Jadwal belum dibuat.\n");
        pause_screen();
        return;
    }

    printf("+--------------------------------+-----------------+-----------------+\n");
    printf("| Nama Dokter                    | Total Shift     | Total Pelanggaran |\n");
    printf("+--------------------------------+-----------------+-----------------+\n");

    int total_pelanggaran_global = 0;
    for (int i = 0; i < total_dokter; i++) {
        printf("| %-30s | %-15d | %-15d |\n", dokter[i].nama, dokter[i].shift_terpakai, dokter[i].pelanggaran);
        total_pelanggaran_global += dokter[i].pelanggaran;
    }
    printf("+--------------------------------+-----------------+-----------------+\n");
    printf("\nTotal Pelanggaran Preferensi Keseluruhan: %d\n", total_pelanggaran_global);
    pause_screen();
}

void simpanJadwalKeCSV() {
    clear_screen();
    printf("--- Menyimpan Jadwal ke CSV ---\n");
    if (jadwal[0].pagi.kapasitas == 0) {
        printf("Jadwal belum dibuat.\n");
        pause_screen();
        return;
    }

    FILE *file = fopen("jadwal.csv", "w");
    if (file == NULL) {
        printf("Gagal membuat file jadwal.csv\n");
        pause_screen();
        return;
    }

    fprintf(file, "Hari,Shift Pagi,Shift Siang,Shift Malam\n");
    for (int hari = 0; hari < MAX_HARI; hari++) {
        fprintf(file, "%d,", hari + 1);

        for (int i = 0; i < jadwal[hari].pagi.jumlah_dokter; i++) {
            fprintf(file, "%s%s", jadwal[hari].pagi.dokter_list[i].dokter, (i == jadwal[hari].pagi.jumlah_dokter - 1) ? "" : "; ");
        }
        fprintf(file, ",");

        for (int i = 0; i < jadwal[hari].siang.jumlah_dokter; i++) {
            fprintf(file, "%s%s", jadwal[hari].siang.dokter_list[i].dokter, (i == jadwal[hari].siang.jumlah_dokter - 1) ? "" : "; ");
        }
        fprintf(file, ",");

        for (int i = 0; i < jadwal[hari].malam.jumlah_dokter; i++) {
            fprintf(file, "%s%s", jadwal[hari].malam.dokter_list[i].dokter, (i == jadwal[hari].malam.jumlah_dokter - 1) ? "" : "; ");
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Jadwal berhasil disimpan ke jadwal.csv.\n");
    pause_screen();
}