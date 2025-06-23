#include "dokter_handler.h"
#include "interface.h"

void trim_whitespace(char *str) {
    if (str == NULL) return;
    int i = 0, j = 0;
    while (str[i] != '\0' && isspace((unsigned char)str[i])) { i++; }
    while (str[i] != '\0') { str[j++] = str[i++]; }
    str[j] = '\0';
    i = strlen(str) - 1;
    while (i >= 0 && isspace((unsigned char)str[i])) { str[i--] = '\0'; }
}

static int isNamaDokterExists(const char *nama_check) {
    for (int i = 0; i < total_dokter; i++) {
        if (strcmp(dokter[i].nama, nama_check) == 0) {
            return 1;
        }
    }
    return 0;
}

void initDokterArray() {
    total_dokter = 0;
    for (int i = 0; i < MAX_DOKTER; i++) {
        memset(&dokter[i], 0, sizeof(Dokter));
    }
}

void bacaFileCSV() {
    FILE *file = fopen("daftar_dokter.csv", "r");
    if (file == NULL) {
        printf("File daftar_dokter.csv tidak ditemukan. Akan dibuat jika ada data baru.\n");
        return;
    }

    char line[MAX_LINE];
    total_dokter = 0;

    fgets(line, sizeof(line), file); // Lewati baris header

    while (fgets(line, sizeof(line), file) && total_dokter < MAX_DOKTER) {
        if (strlen(line) < 3) continue;
        line[strcspn(line, "\n")] = 0;

        char temp_line[MAX_LINE];
        strcpy(temp_line, line);

        char *token = strtok(temp_line, ",");
        if (token == NULL) continue;
        trim_whitespace(token);
        strcpy(dokter[total_dokter].nama, token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        dokter[total_dokter].max_shift = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        trim_whitespace(token);

        dokter[total_dokter].pref_pagi = (strstr(token, "Pagi") != NULL);
        dokter[total_dokter].pref_siang = (strstr(token, "Siang") != NULL);
        dokter[total_dokter].pref_malam = (strstr(token, "Malam") != NULL);

        dokter[total_dokter].shift_terpakai = 0;
        dokter[total_dokter].pelanggaran = 0;
        total_dokter++;
    }
    fclose(file);
}

void simpanFileCSV() {
    FILE *file = fopen("daftar_dokter.csv", "w");
    if (file == NULL) {
        printf("Gagal menulis ke file daftar_dokter.csv\n");
        return;
    }

    fprintf(file, "Nama,Max Shift/Minggu,Preferensi Shift\n");
    for (int i = 0; i < total_dokter; i++) {
        char pref_str[50] = "";
        int first = 1;

        if (dokter[i].pref_pagi) {
            strcat(pref_str, "Pagi");
            first = 0;
        }
        if (dokter[i].pref_siang) {
            if (!first) strcat(pref_str, ",");
            strcat(pref_str, "Siang");
            first = 0;
        }
        if (dokter[i].pref_malam) {
            if (!first) strcat(pref_str, ",");
            strcat(pref_str, "Malam");
        }
        fprintf(file, "%s,%d,%s\n", dokter[i].nama, dokter[i].max_shift, pref_str);
    }
    fclose(file);
}

void tambahDokterBaru() {
    clear_screen();
    if (total_dokter >= MAX_DOKTER) {
        printf("Kapasitas dokter sudah penuh.\n");
        pause_screen();
        return;
    }

    printf("--- Tambah Dokter Baru ---\n");
    
    Dokter d;
    printf("Masukkan Nama Dokter: ");
    fgets(d.nama, sizeof(d.nama), stdin);
    d.nama[strcspn(d.nama, "\n")] = 0;
    trim_whitespace(d.nama);
    if(strlen(d.nama) == 0 || isNamaDokterExists(d.nama)) {
        printf("Nama tidak valid atau sudah ada.\n");
        pause_screen();
        return;
    }

    char input[10];
    printf("Masukkan Maksimal Shift per Minggu: ");
    fgets(input, sizeof(input), stdin);
    if(sscanf(input, "%d", &d.max_shift) != 1 || d.max_shift <= 0) {
        printf("Input shift tidak valid.\n");
        pause_screen();
        return;
    }

    char pref_input[50];
    printf("Masukkan Preferensi Shift (cth: Pagi,Malam): ");
    fgets(pref_input, sizeof(pref_input), stdin);
    d.pref_pagi = (strstr(pref_input, "Pagi") != NULL || strstr(pref_input, "pagi") != NULL);
    d.pref_siang = (strstr(pref_input, "Siang") != NULL || strstr(pref_input, "siang") != NULL);
    d.pref_malam = (strstr(pref_input, "Malam") != NULL || strstr(pref_input, "malam") != NULL);

    d.shift_terpakai = 0;
    d.pelanggaran = 0;
    dokter[total_dokter++] = d;

    printf("Dokter '%s' berhasil ditambahkan.\n", d.nama);
    simpanFileCSV();
    pause_screen();
}

void hapusDokter() {
    clear_screen();
    printf("--- Hapus Dokter ---\n");
    if (total_dokter == 0) {
        printf("Belum ada data dokter.\n");
        pause_screen();
        return;
    }

    tampilkanDaftarDokter(); // Tampilkan dulu biar user bisa lihat
    
    printf("\nMasukkan Nama Dokter yang akan dihapus: ");
    char nama_hapus[50];
    fgets(nama_hapus, sizeof(nama_hapus), stdin);
    nama_hapus[strcspn(nama_hapus, "\n")] = 0;
    trim_whitespace(nama_hapus);

    int found_index = -1;
    for (int i = 0; i < total_dokter; i++) {
        if (strcmp(dokter[i].nama, nama_hapus) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        for (int i = found_index; i < total_dokter - 1; i++) {
            dokter[i] = dokter[i + 1];
        }
        total_dokter--;
        printf("Dokter '%s' berhasil dihapus.\n", nama_hapus);
        simpanFileCSV();
    } else {
        printf("Dokter '%s' tidak ditemukan.\n", nama_hapus);
    }
    pause_screen();
}

void tampilkanDaftarDokter() {
    clear_screen();
    printf("--- Daftar Dokter ---\n");
    if (total_dokter == 0) {
        printf("Tidak ada dokter yang terdaftar.\n");
        pause_screen();
        return;
    }

    printf("+----+----------------------+-----------------+------------------------+\n");
    printf("| No | Nama Dokter          | Max Shift/Minggu| Preferensi             |\n");
    printf("+----+----------------------+-----------------+------------------------+\n");
    for (int i = 0; i < total_dokter; i++) {
        char pref_str[50] = "";
        int first = 1;

        if (dokter[i].pref_pagi) {
            strcat(pref_str, "Pagi");
            first = 0;
        }
        if (dokter[i].pref_siang) {
            if (!first) strcat(pref_str, ", ");
            strcat(pref_str, "Siang");
            first = 0;
        }
        if (dokter[i].pref_malam) {
            if (!first) strcat(pref_str, ", ");
            strcat(pref_str, "Malam");
        }
        printf("| %-2d | %-20s | %-15d | %-22s |\n", i + 1, dokter[i].nama, dokter[i].max_shift, pref_str);
    }
    printf("+----+----------------------+-----------------+------------------------+\n");
    pause_screen();
}