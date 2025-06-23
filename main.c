#include "definitions.h"
#include "interface.h"
#include "dokter_handler.h"
#include "jadwal_generator.h"

// DEFINISI VARIABEL GLOBAL
Dokter dokter[MAX_DOKTER];
int total_dokter = 0;
JadwalHari jadwal[MAX_HARI];

int main() {
    initDokterArray();
    bacaFileCSV();

    int choice;
    do {
        clear_screen();
        displayMainMenu();
        printf("Masukkan pilihan Anda: ");
        char input[10];
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &choice) != 1) {
            choice = -1;
        }

        switch (choice) {
            case 1:
                handleDoctorManagement();
                break;
            case 2:
                handleScheduleManagement();
                break;
            case 3:
                printf("Terima kasih telah menggunakan aplikasi penjadwalan dokter.\n");
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                pause_screen();
                break;
        }
    } while (choice != 3);

    bersihkanSemuaJadwal();
    return 0;
}