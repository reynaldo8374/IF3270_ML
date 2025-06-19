// File: dokter.c

#include "sistem_penjadwalan.h"

// --- FUNGSI BAGIAN 1: Pengelolaan Data Dokter ---
void tambahDokter()
{
    if (jumlah_dokter >= MAX_DOKTER)
    {
        printf("\n[!] Kapasitas dokter penuh. Tidak dapat menambah dokter baru.\n");
        return;
    }

    printf("\n=== TAMBAH DOKTER BARU ===\n");
    printf("Masukkan Nama Dokter: ");
    scanf(" %[^\n]", daftar_dokter[jumlah_dokter].nama);

    printf("Masukkan Maksimal Shift per Minggu (contoh: 5): ");
    scanf("%d", &daftar_dokter[jumlah_dokter].max_shift_per_minggu);
    clearInputBuffer();

    printf("Masukkan Preferensi Shift (Pagi/Siang/Malam): ");
    scanf(" %[^\n]", daftar_dokter[jumlah_dokter].preferensi);
    for (int i = 0; daftar_dokter[jumlah_dokter].preferensi[i]; i++){
        daftar_dokter[jumlah_dokter].preferensi[i] = tolower(daftar_dokter[jumlah_dokter].preferensi[i]);
    }

    daftar_dokter[jumlah_dokter].total_shift = 0;
    daftar_dokter[jumlah_dokter].pelanggaran_preferensi = 0;
    for (int i = 0; i < MAX_MINGGU; i++) {
        daftar_dokter[jumlah_dokter].shift_per_minggu[i] = 0;
    }

    jumlah_dokter++;
    printf("\n[v] Dokter berhasil ditambahkan: %s\n", daftar_dokter[jumlah_dokter-1].nama);
}

void displaydokter()
{
    if (jumlah_dokter == 0)
    {
        printf("\n[!] Belum ada data dokter yang terdaftar.\n");
        return;
    }
    printf("\n=== DAFTAR DOKTER ===\n");
    printf("+----+--------------------------------+--------------------------+------------------+\n");
    printf("| No | Nama Dokter                    | Maksimal Shift / Minggu  | Preferensi Shift |\n");
    printf("+----+--------------------------------+--------------------------+------------------+\n");

    for (int i = 0; i < jumlah_dokter; i++)
    {
        printf("| %-2d | %-30s | %-24d | %-16s |\n",
               i + 1,
               daftar_dokter[i].nama,
               daftar_dokter[i].max_shift_per_minggu,
               daftar_dokter[i].preferensi);
    }
    printf("+----+--------------------------------+--------------------------+------------------+\n");
}

void hapusDokter()
{
    char nama_dokter_hapus[50];
    if (jumlah_dokter == 0) {
        printf("\n[!] Tidak ada dokter untuk dihapus.\n");
        return;
    }
    
    displaydokter(); 
    printf("\n=== HAPUS DOKTER ===\n");
    printf("Masukkan nama dokter yang akan dihapus: ");
    scanf(" %[^\n]", nama_dokter_hapus);

    int nama_ada_indeks = -1;
    for (int i = 0; i < jumlah_dokter; i++)
    {
        if (strcmp(daftar_dokter[i].nama, nama_dokter_hapus) == 0)
        {
            nama_ada_indeks = i;
            break;
        }
    }

    if (nama_ada_indeks != -1)
    {
        for (int i = nama_ada_indeks; i < jumlah_dokter - 1; i++)
        {
            daftar_dokter[i] = daftar_dokter[i + 1];
        }
        jumlah_dokter--;
        printf("\n[v] Dokter '%s' berhasil dihapus.\n", nama_dokter_hapus);
    }
    else
    {
        printf("\n[!] Dokter dengan nama '%s' tidak ditemukan. Pastikan penulisan nama benar.\n", nama_dokter_hapus);
    }
}

void bacaFile()
{
    FILE *file = fopen("daftar_dokter.csv", "r");
    if (!file)
    {
        printf("[!] File 'daftar_dokter.csv' tidak ditemukan atau tidak dapat dibuka. Melanjutkan tanpa membaca file.\n");
        return;
    }

    jumlah_dokter = 0;
    char line[256];
    fgets(line, sizeof(line), file); // Skip header

    int dokter_terbaca = 0;
    while (fgets(line, sizeof(line), file)) {
        if (jumlah_dokter >= MAX_DOKTER) {
            printf("[!] Peringatan: Kapasitas dokter (%d) penuh. Tidak semua dokter dari file dapat dibaca.\n", MAX_DOKTER);
            break;
        }

        char *token;
        char temp_line[256];
        strcpy(temp_line, line);

        token = strtok(temp_line, ",");
        if (token) strcpy(daftar_dokter[jumlah_dokter].nama, token);
        else continue;

        token = strtok(NULL, ",");
        if (token) daftar_dokter[jumlah_dokter].max_shift_per_minggu = atoi(token);
        else continue;

        token = strtok(NULL, "\n");
        if (token) {
            token[strcspn(token, "\r\n")] = 0;
            strcpy(daftar_dokter[jumlah_dokter].preferensi, token);
            for (int i = 0; daftar_dokter[jumlah_dokter].preferensi[i]; i++){
                daftar_dokter[jumlah_dokter].preferensi[i] = tolower(daftar_dokter[jumlah_dokter].preferensi[i]);
            }
        } else continue;

        daftar_dokter[jumlah_dokter].total_shift = 0;
        daftar_dokter[jumlah_dokter].pelanggaran_preferensi = 0;
        for (int i = 0; i < MAX_MINGGU; i++) {
            daftar_dokter[jumlah_dokter].shift_per_minggu[i] = 0;
        }
        jumlah_dokter++;
        dokter_terbaca++;
    }

    fclose(file);
    printf("[v] Data dokter berhasil dibaca dari 'daftar_dokter.csv'. Total %d dokter ditemukan.\n", dokter_terbaca);
}

void simpandata_dokter()
{
    if (jumlah_dokter == 0) {
        printf("\n[!] Tidak ada data dokter untuk disimpan.\n");
        return;
    }
    
    FILE *file = fopen("daftar_dokter.csv", "w");
    if (!file)
    {
        printf("\n[!] Gagal membuka file 'daftar_dokter.csv' untuk ditulis. Pastikan file tidak sedang digunakan atau ada izin tulis.\n");
        return;
    }

    fprintf(file, "Nama,MaxShiftMingguan,Preferensi\n");

    for (int i = 0; i < jumlah_dokter; i++)
    {
        fprintf(file, "%s,%d,%s\n",
                daftar_dokter[i].nama,
                daftar_dokter[i].max_shift_per_minggu,
                daftar_dokter[i].preferensi);
    }

    fclose(file);
    printf("[v] Data dokter berhasil disimpan ke 'daftar_dokter.csv'.\n");
}