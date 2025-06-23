#include "jadwal_io.h"
#include "interface.h"

static int get_max_lines(int c1, int c2, int c3) {
    int max = c1 > c2 ? c1 : c2;
    return max > c3 ? max : c3;
}

void cetakJadwalRange(int start_day, int end_day) {
    if (jadwal[0].pagi.kapasitas == 0) {
        printf("Jadwal belum dibuat.\n");
        pause_screen();
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
        int maxLines = get_max_lines(countP, countS, countM);
        if (maxLines == 0) maxLines = 1;

        for (int line = 0; line < maxLines; line++) {
            printf("| %-4s ", (line == 0) ? "" : " ");
            if (line == 0) {
                char day_str[5];
                sprintf(day_str, "%d", hari + 1);
                printf("\b\b\b\b%-4s", day_str);
            }

            char buffer[35];
            if (line < countP) {
                snprintf(buffer, sizeof(buffer), "%s", jadwal[hari].pagi.dokter_list[line].dokter);
                printf("| %-30s ", buffer);
            } else {
                printf("| %-30s ", "");
            }

            if (line < countS) {
                snprintf(buffer, sizeof(buffer), "%s", jadwal[hari].siang.dokter_list[line].dokter);
                printf("| %-30s ", buffer);
            } else {
                printf("| %-30s ", "");
            }

            if (line < countM) {
                snprintf(buffer, sizeof(buffer), "%s", jadwal[hari].malam.dokter_list[line].dokter);
                printf("| %-30s |\n", buffer);
            } else {
                printf("| %-30s |\n", "");
            }
        }
        printf("+------+--------------------------------+--------------------------------+--------------------------------+\n");
    }
}


void cetakRingkasanShiftDokter() {
    clear_screen();
    printf("--- Ringkasan Shift Dokter ---\n");
    if (total_dokter == 0) {
        printf("Tidak ada dokter yang terdaftar.\n");
        pause_screen();
        return;
    }
    if (jadwal[0].pagi.kapasitas == 0) {
        printf("Jadwal belum dibuat.\n");
        pause_screen();
        return;
    }

    printf("+----------------------+-----------------+-----------------+\n");
    printf("| Nama Dokter          | Total Shift     | Pelanggaran Pref. |\n");
    printf("+----------------------+-----------------+-----------------+\n");

    int total_pelanggaran_global = 0;
    for (int i = 0; i < total_dokter; i++) {
        printf("| %-20s | %-15d | %-15d |\n", dokter[i].nama, dokter[i].shift_terpakai, dokter[i].pelanggaran);
        total_pelanggaran_global += dokter[i].pelanggaran;
    }
    printf("+----------------------+-----------------+-----------------+\n");
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
        char shift_pagi_str[MAX_LINE] = "";
        char shift_siang_str[MAX_LINE] = "";
        char shift_malam_str[MAX_LINE] = "";

        for (int i = 0; i < jadwal[hari].pagi.jumlah_dokter; i++) {
            strcat(shift_pagi_str, jadwal[hari].pagi.dokter_list[i].dokter);
            if (i < jadwal[hari].pagi.jumlah_dokter - 1) strcat(shift_pagi_str, "; ");
        }
        for (int i = 0; i < jadwal[hari].siang.jumlah_dokter; i++) {
            strcat(shift_siang_str, jadwal[hari].siang.dokter_list[i].dokter);
            if (i < jadwal[hari].siang.jumlah_dokter - 1) strcat(shift_siang_str, "; ");
        }
        for (int i = 0; i < jadwal[hari].malam.jumlah_dokter; i++) {
            strcat(shift_malam_str, jadwal[hari].malam.dokter_list[i].dokter);
            if (i < jadwal[hari].malam.jumlah_dokter - 1) strcat(shift_malam_str, "; ");
        }
        fprintf(file, "\"%s\",\"%s\",\"%s\"\n", shift_pagi_str, shift_siang_str, shift_malam_str);
    }

    fclose(file);
    printf("Jadwal berhasil disimpan ke jadwal.csv.\n");
    pause_screen();
}
