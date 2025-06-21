#include "jadwal.h"

// ===========================================
// UTILITY FUNCTIONS
// ===========================================

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause_screen() {
    printf("\nTekan ENTER untuk melanjutkan...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    // tambahan getchar() untuk menangani buffer dari input sebelumnya
    if (c == EOF) {
        // Handle end of file
    } else {
       getchar();
    }
}

// ===========================================
// MENU HANDLERS
// ===========================================

void displayMainMenu() {
    printf("==============================================\n");
    printf("|       APLIKASI PENJADWALAN DOKTER          |\n");
    printf("==============================================\n");
    printf("| 1. Manajemen Dokter                        |\n");
    printf("| 2. Manajemen Jadwal                        |\n");
    printf("| 3. Keluar                                  |\n");
    printf("==============================================\n");
}

void handleDoctorManagement() {
    int choice;
    do {
        clear_screen();
        printf("==============================================\n");
        printf("|           MENU MANAJEMEN DOKTER            |\n");
        printf("==============================================\n");
        printf("| 1. Tampilkan Daftar Dokter                 |\n");
        printf("| 2. Tambah Dokter Baru                      |\n");
        printf("| 3. Hapus Dokter                            |\n");
        printf("| 4. Kembali ke Menu Utama                   |\n");
        printf("==============================================\n");
        printf("Masukkan pilihan Anda: ");
        char input[10];
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &choice) != 1) {
            choice = -1;
        }

        switch (choice) {
            case 1:
                tampilkanDaftarDokter();
                break;
            case 2:
                tambahDokterBaru();
                break;
            case 3:
                hapusDokter();
                break;
            case 4:
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                pause_screen();
                break;
        }
    } while (choice != 4);
}

void handleScheduleManagement() {
    int choice;
    do {
        clear_screen();
        printf("==============================================\n");
        printf("|           MENU MANAJEMEN JADWAL            |\n");
        printf("==============================================\n");
        printf("| 1. Buat Jadwal Otomatis (30 Hari)          |\n");
        printf("| 2. Tampilkan Jadwal                        |\n");
        printf("| 3. Tampilkan Ringkasan Shift Dokter        |\n");
        printf("| 4. Simpan Jadwal ke CSV                    |\n");
        printf("| 5. Kembali ke Menu Utama                   |\n");
        printf("==============================================\n");
        printf("Masukkan pilihan Anda: ");
        char input[10];
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &choice) != 1) {
            choice = -1;
        }

        switch (choice) {
            case 1:
                buatJadwalOtomatis();
                break;
            case 2:
                handleScheduleView();
                break;
            case 3:
                cetakRingkasanShiftDokter();
                break;
            case 4:
                simpanJadwalKeCSV();
                break;
            case 5:
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                pause_screen();
                break;
        }
    } while (choice != 5);
}

void handleScheduleView() {
    int choice;
    do {
        clear_screen();
        printf("==============================================\n");
        printf("|           MENU TAMPILAN JADWAL             |\n");
        printf("==============================================\n");
        printf("| 1. Tampilkan Jadwal Bulanan (30 Hari)      |\n");
        printf("| 2. Tampilkan Jadwal Per Minggu             |\n");
        printf("| 3. Tampilkan Jadwal Harian                 |\n");
        printf("| 4. Kembali ke Menu Manajemen Jadwal        |\n");
        printf("==============================================\n");
        printf("Masukkan pilihan Anda: ");
        char input[10];
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &choice) != 1) {
            choice = -1;
        }

        switch (choice) {
            case 1: {
                clear_screen();
                printf("\n--- Jadwal Dokter Bulanan (Hari 1 - 30) ---\n");
                cetakJadwalRange(1, MAX_HARI);
                pause_screen();
                break;
            }
            case 2: {
                int week;
                printf("\nMasukkan nomor minggu (1-4): ");
                char week_input[10];
                fgets(week_input, sizeof(week_input), stdin);
                if (sscanf(week_input, "%d", &week) != 1 || week < 1 || week > 4) {
                    printf("Input minggu tidak valid. Pilih antara 1 sampai 4.\n");
                    pause_screen();
                    break;
                }
                int start_day = (week - 1) * 7 + 1;
                int end_day = week * 7;
                if (end_day > MAX_HARI) end_day = MAX_HARI;

                clear_screen();
                printf("\n--- Jadwal Dokter Minggu %d (Hari %d - %d) ---\n", week, start_day, end_day);
                cetakJadwalRange(start_day, end_day);
                pause_screen();
                break;
            }
            case 3: {
                int day;
                printf("\nMasukkan nomor hari (1-30): ");
                char day_input[10];
                fgets(day_input, sizeof(day_input), stdin);
                if (sscanf(day_input, "%d", &day) != 1 || day < 1 || day > MAX_HARI) {
                    printf("Input hari tidak valid. Pilih antara 1 sampai 30.\n");
                    pause_screen();
                    break;
                }
                clear_screen();
                printf("\n--- Jadwal Dokter Hari %d ---\n", day);
                cetakJadwalRange(day, day);
                pause_screen();
                break;
            }
            case 4:
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                pause_screen();
                break;
        }
    } while (choice != 4);
}