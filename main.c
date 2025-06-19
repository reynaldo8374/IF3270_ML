// File: main.c

#include "sistem_penjadwalan.h"

// --- Definisi Variabel Global ---
// Variabel-variabel ini didefinisikan di sini.
// File lain mengaksesnya melalui deklarasi 'extern' di header.
Dokter daftar_dokter[MAX_DOKTER];
int jumlah_dokter = 0;
JadwalShift jadwal_harian[MAX_HARI][MAX_SHIFT];
const char* nama_shift_global[MAX_SHIFT] = {"Pagi", "Siang", "Malam"};
int kebutuhan_shift_global[MAX_SHIFT] = {3, 3, 3};
int unassigned_shifts = 0;


// --- FUNGSI UTAMA PROGRAM ---
int main()
{
    bacaFile(); // Baca data dokter saat program dimulai

    int pilihan_menu;
    do
    {
        printf("\n=============================================\n");
        printf("   SISTEM PENJADWALAN DOKTER RUMAH SAKIT ANJAY\n");
        printf("=============================================\n");
        printf("1. Pengelolaan Data Dokter\n");
        printf("2. Buat Jadwal Otomatis\n");
        printf("3. Tampilkan Informasi Jadwal\n");
        printf("4. Simpan Jadwal ke CSV\n");
        printf("5. Tampilkan Statistik Dokter\n");
        printf("6. Simpan Data Dokter ke CSV\n");
        printf("7. Keluar\n");
        printf("---------------------------------------------\n");
        printf("Pilihan Anda: ");
        scanf("%d", &pilihan_menu);
        clearInputBuffer();

        switch (pilihan_menu)
        {
        case 1:
            {
                int sub_pilihan;
                do {
                    printf("\n--- PENGELOLAAN DATA DOKTER ---\n");
                    printf("1. Tambah Dokter\n");
                    printf("2. Hapus Dokter\n");
                    printf("3. Tampilkan Semua Dokter\n");
                    printf("4. Kembali ke Menu Utama\n");
                    printf("--------------------------------\n");
                    printf("Pilihan Anda: ");
                    scanf("%d", &sub_pilihan);
                    clearInputBuffer();

                    switch (sub_pilihan) {
                        case 1: tambahDokter(); break;
                        case 2: hapusDokter(); break;
                        case 3: displaydokter(); break;
                        case 4: printf("\n[i] Kembali ke menu utama Sistem Penjadwalan.\n"); break;
                        default: printf("[!] Pilihan tidak valid. Silakan pilih 1-4.\n"); break;
                    }
                } while (sub_pilihan != 4);
            }
            break;
        case 2:
            buat_jadwal_otomatis();
            break;
        case 3:
            tampilkan_matriks_jadwal();
            break;
        case 4:
            simpan_jadwal_ke_csv("jadwal_rs_anjay.csv");
            break;
        case 5:
            tampilkan_statistik_jadwal();
            break;
        case 6:
            simpandata_dokter();
            break;
        case 7:
            printf("\n[i] Terima kasih telah menggunakan sistem penjadwalan rumah sakit ANJAY! Sampai jumpa.\n");
            break;
        default:
            printf("[!] Pilihan tidak valid. Silakan pilih angka antara 1-7.\n");
            break;
        }
    } while (pilihan_menu != 7);

    return 0;
}