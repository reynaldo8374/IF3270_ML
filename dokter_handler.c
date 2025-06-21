#include "jadwal.h"

// Fungsi utilitas hanya untuk modul ini
void trim_whitespace(char *str) {
    if (str == NULL) return;
    int i = 0, j = 0;
    while (str[i] != '\0' && isspace((unsigned char)str[i])) {
        i++;
    }
    while (str[i] != '\0') {
        str[j++] = str[i++];
    }
    str[j] = '\0';
    i = strlen(str) - 1;
    while (i >= 0 && isspace((unsigned char)str[i])) {
        str[i--] = '\0';
    }
}

// Fungsi internal, tidak perlu di header
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
        printf("File daftar_dokter.csv tidak ditemukan. Akan membuat file baru jika ada data ditambahkan.\n");
        return;
    }

    char line[MAX_LINE];
    int line_num = 0;
    total_dokter = 0;

    while (fgets(line, sizeof(line), file)) {
        if (line_num == 0 || strlen(line) < 3) {
            line_num++;
            continue;
        }
        line[strcspn(line, "\n")] = 0;
        char *token;
        char temp_line[MAX_LINE];
        strcpy(temp_line, line);

        token = strtok(temp_line, ",");
        if (token == NULL) continue;
        trim_whitespace(token);
        strcpy(dokter[total_dokter].nama, token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        dokter[total_dokter].max_shift = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        trim_whitespace(token);
        strcpy(dokter[total_dokter].preferensi, token);

        if (strcmp(dokter[total_dokter].preferensi, "Pagi") != 0 &&
            strcmp(dokter[total_dokter].preferensi, "Siang") != 0 &&
            strcmp(dokter[total_dokter].preferensi, "Malam") != 0) {
            printf("Peringatan: Preferensi '%s' untuk dokter '%s' tidak valid. Default ke 'Pagi'.\n", dokter[total_dokter].preferensi, dokter[total_dokter].nama);
            strcpy(dokter[total_dokter].preferensi, "Pagi");
        }

        dokter[total_dokter].shift_terpakai = 0;
        dokter[total_dokter].pelanggaran = 0;
        total_dokter++;

        if (total_dokter >= MAX_DOKTER) {
            printf("Batas maksimum dokter (%d) tercapai.\n", MAX_DOKTER);
            break;
        }
    }
    fclose(file);
    printf("Data dokter berhasil dimuat. Total: %d dokter.\n", total_dokter);
    pause_screen();
}

void simpanFileCSV() {
    FILE *file = fopen("daftar_dokter.csv", "w");
    if (file == NULL) {
        printf("Gagal menulis ke file daftar_dokter.csv\n");
        pause_screen();
        return;
    }
    fprintf(file, "Nama,Max Shift/Minggu,Preferensi Shift\n");
    for (int i = 0; i < total_dokter; i++) {
        fprintf(file, "%s,%d,%s\n", dokter[i].nama, dokter[i].max_shift, dokter[i].preferensi);
    }
    fclose(file);
    printf("Data dokter berhasil disimpan ke daftar_dokter.csv.\n");
    pause_screen();
}

void tambahDokterBaru() {
    clear_screen();
    printf("--- Tambah Dokter Baru ---\n");
    if (total_dokter >= MAX_DOKTER) {
        printf("Batas maksimum dokter (%d) telah tercapai.\n", MAX_DOKTER);
        pause_screen();
        return;
    }

    char nama[50];
    int max_shift;
    char preferensi[10];

    printf("Masukkan Nama Dokter: ");
    fgets(nama, sizeof(nama), stdin);
    nama[strcspn(nama, "\n")] = 0;
    trim_whitespace(nama);

    if (strlen(nama) == 0) {
        printf("Nama dokter tidak boleh kosong.\n");
        pause_screen();
        return;
    }
    if (isNamaDokterExists(nama)) {
        printf("Dokter dengan nama '%s' sudah ada.\n", nama);
        pause_screen();
        return;
    }

    printf("Masukkan Maksimal Shift per Minggu : ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%d", &max_shift) != 1 || max_shift <= 0) {
        printf("Maksimal shift tidak valid. Harus angka positif.\n");
        pause_screen();
        return;
    }

    printf("Masukkan Preferensi Shift (Pagi/Siang/Malam): ");
    fgets(preferensi, sizeof(preferensi), stdin);
    preferensi[strcspn(preferensi, "\n")] = 0;
    trim_whitespace(preferensi);

    if (strlen(preferensi) > 0) {
        preferensi[0] = toupper((unsigned char)preferensi[0]);
        for (int i = 1; preferensi[i]; i++) {
            preferensi[i] = tolower((unsigned char)preferensi[i]);
        }
    }

    if (strcmp(preferensi, "Pagi") != 0 && strcmp(preferensi, "Siang") != 0 && strcmp(preferensi, "Malam") != 0) {
        printf("Preferensi shift tidak valid. Pilih Pagi, Siang, atau Malam.\n");
        pause_screen();
        return;
    }

    strcpy(dokter[total_dokter].nama, nama);
    dokter[total_dokter].max_shift = max_shift;
    strcpy(dokter[total_dokter].preferensi, preferensi);
    dokter[total_dokter].shift_terpakai = 0;
    dokter[total_dokter].pelanggaran = 0;
    total_dokter++;

    printf("Dokter '%s' berhasil ditambahkan.\n", nama);
    simpanFileCSV();
}

void hapusDokter() {
    clear_screen();
    printf("--- Hapus Dokter ---\n");
    if (total_dokter == 0) {
        printf("Belum ada data dokter.\n");
        pause_screen();
        return;
    }
    tampilkanDaftarDokter(); // Gunakan fungsi yang sudah ada

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
        pause_screen();
    }
}

void tampilkanDaftarDokter() {
    clear_screen();
    printf("--- Daftar Dokter ---\n");
    if (total_dokter == 0) {
        printf("Tidak ada dokter yang terdaftar.\n");
        pause_screen();
        return;
    }
    printf("+----+--------------------------------+-----------------+---------------+\n");
    printf("| No | Nama Dokter                    | Max Shift/Minggu| Preferensi    |\n");
    printf("+----+--------------------------------+-----------------+---------------+\n");
    for (int i = 0; i < total_dokter; i++) {
        printf("| %-2d | %-30s | %-15d | %-13s |\n", i + 1, dokter[i].nama, dokter[i].max_shift, dokter[i].preferensi);
    }
    printf("+----+--------------------------------+-----------------+---------------+\n");
    pause_screen();
}