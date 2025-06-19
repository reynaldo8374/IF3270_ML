#include "sistem_penjadwalan.h"
#include <stdlib.h> // Diperlukan untuk qsort

// Struktur sementara untuk membantu proses sorting
typedef struct {
    int indeks_asli;
    int total_shift;
} DokterSortable;

// Fungsi pembanding untuk qsort. Mengurutkan dokter dari total shift terkecil ke terbesar.
int bandingkan_dokter(const void* a, const void* b) {
    DokterSortable* da = (DokterSortable*)a;
    DokterSortable* db = (DokterSortable*)b;
    return (da->total_shift - db->total_shift);
}

// Fungsi Bantuan: Cek apakah dokter sudah bekerja pada hari ini
static int sudah_bekerja_hari_ini(int hari, int indeks_dokter) {
    for (int s = 0; s < MAX_SHIFT; s++) {
        for (int k = 0; k < jadwal_harian[hari][s].jumlah_dokter_dijadwalkan; k++) {
            if (strcmp(daftar_dokter[indeks_dokter].nama, jadwal_harian[hari][s].dokter_dijadwalkan[k]) == 0) {
                return 1; // Ya, sudah bekerja
            }
        }
    }
    return 0; // Belum bekerja
}

// Fungsi Inti Penjadwalan yang Baru
static int coba_isi_slot(int hari, int indeks_shift, DokterSortable sorted_dokter[]) {
    int minggu = hari / 7;

    // Lakukan 3 Lapis Percobaan
    for (int lapis = 1; lapis <= 3; lapis++) {
        // Iterasi melalui dokter yang sudah diurutkan (dari shift paling sedikit)
        for (int j = 0; j < jumlah_dokter; j++) {
            int i = sorted_dokter[j].indeks_asli; // Indeks asli dokter di `daftar_dokter`

            // --- Kriteria Berdasarkan Lapis Percobaan ---
            if (lapis == 1 || lapis == 2) {
                // Lapis 1 & 2: Hanya untuk dokter yang BELUM bekerja hari ini
                if (sudah_bekerja_hari_ini(hari, i)) continue;
            }
            // Lapis 3: Khusus untuk dokter yang SUDAH bekerja hari ini (langkah darurat)
            if (lapis == 3) {
                 if (!sudah_bekerja_hari_ini(hari, i)) continue;
                 // Pastikan tidak menjadwalkan shift yang sama 2x di hari yang sama
                 int sudah_di_shift_ini = 0;
                 for (int k = 0; k < jadwal_harian[hari][indeks_shift].jumlah_dokter_dijadwalkan; k++) {
                     if (strcmp(daftar_dokter[i].nama, jadwal_harian[hari][indeks_shift].dokter_dijadwalkan[k]) == 0) {
                         sudah_di_shift_ini = 1;
                         break;
                     }
                 }
                 if(sudah_di_shift_ini) continue;
            }

            // === ATURAN KERAS (HARD RULES) - Selalu Berlaku ===
            // 1. Tidak melebihi kuota shift mingguan
            if (daftar_dokter[i].shift_per_minggu[minggu] >= daftar_dokter[i].max_shift_per_minggu) continue;

            // 2. Tidak boleh shift Malam (indeks 2) lalu Pagi (indeks 0)
            if (hari > 0 && indeks_shift == 0) {
                int kerja_malam_kemarin = 0;
                for (int k = 0; k < jadwal_harian[hari - 1][2].jumlah_dokter_dijadwalkan; k++) {
                    if (strcmp(jadwal_harian[hari - 1][2].dokter_dijadwalkan[k], daftar_dokter[i].nama) == 0) {
                        kerja_malam_kemarin = 1;
                        break;
                    }
                }
                if (kerja_malam_kemarin) continue;
            }
            
            // === ATURAN LUNAK (SOFT RULES) - Hanya di Lapis 1 ===
            if (lapis == 1) {
                char current_shift_lower[10];
                strcpy(current_shift_lower, nama_shift_global[indeks_shift]);
                for(int c=0; current_shift_lower[c]; c++) current_shift_lower[c] = tolower(current_shift_lower[c]);

                // Aturan preferensi
                if (strcmp(daftar_dokter[i].preferensi, current_shift_lower) != 0) continue;

                // Aturan tidak boleh shift sama berturut-turut
                if (hari > 0) {
                    int kerja_shift_sama_kemarin = 0;
                    for (int k = 0; k < jadwal_harian[hari-1][indeks_shift].jumlah_dokter_dijadwalkan; k++) {
                        if (strcmp(daftar_dokter[i].nama, jadwal_harian[hari-1][indeks_shift].dokter_dijadwalkan[k]) == 0) {
                            kerja_shift_sama_kemarin = 1;
                            break;
                        }
                    }
                    if (kerja_shift_sama_kemarin) continue;
                }
            }

            // === Jika lolos, jadwalkan dokter ini! ===
            strcpy(jadwal_harian[hari][indeks_shift].dokter_dijadwalkan[jadwal_harian[hari][indeks_shift].jumlah_dokter_dijadwalkan], daftar_dokter[i].nama);
            jadwal_harian[hari][indeks_shift].jumlah_dokter_dijadwalkan++;
            
            // Update statistik dokter
            daftar_dokter[i].total_shift++;
            daftar_dokter[i].shift_per_minggu[minggu]++;
            return 1; // BERHASIL MENGISI SLOT
        }
    }

    return 0; // GAGAL MENGISI SLOT SETELAH 3 LAPIS
}


void buat_jadwal_otomatis() {
    if (jumlah_dokter == 0) {
        printf("\n[!] Tidak ada data dokter untuk membuat jadwal.\n");
        return;
    }
    
    // Reset statistik dan jadwal
    unassigned_shifts = 0;
    for (int i = 0; i < jumlah_dokter; i++) {
        daftar_dokter[i].total_shift = 0;
        daftar_dokter[i].pelanggaran_preferensi = 0;
        for (int j = 0; j < MAX_MINGGU; j++) daftar_dokter[i].shift_per_minggu[j] = 0;
    }
    for (int h = 0; h < MAX_HARI; h++) {
        for (int s = 0; s < MAX_SHIFT; s++) {
            strcpy(jadwal_harian[h][s].shift_nama, nama_shift_global[s]);
            jadwal_harian[h][s].jumlah_dokter_dijadwalkan = 0;
        }
    }

    printf("\n[*] Membuat jadwal otomatis selama %d hari dengan strategi baru...\n", MAX_HARI);

    // Proses utama
    for (int hari = 0; hari < MAX_HARI; hari++) {
        for (int s = 0; s < MAX_SHIFT; s++) {
            // Untuk setiap slot, isi sesuai kebutuhan
            for (int slot = 0; slot < kebutuhan_shift_global[s]; slot++) {
                
                // 1. Buat daftar dokter yang bisa di-sort
                DokterSortable sortable_dokter_arr[MAX_DOKTER];
                for (int i = 0; i < jumlah_dokter; i++) {
                    sortable_dokter_arr[i].indeks_asli = i;
                    sortable_dokter_arr[i].total_shift = daftar_dokter[i].total_shift;
                }
                
                // 2. Sortir dokter berdasarkan total shift paling sedikit
                qsort(sortable_dokter_arr, jumlah_dokter, sizeof(DokterSortable), bandingkan_dokter);

                // 3. Coba isi slot menggunakan 3 lapis percobaan
                if (!coba_isi_slot(hari, s, sortable_dokter_arr)) {
                    // Jika semua percobaan gagal, tandai slot kosong
                    strcpy(jadwal_harian[hari][s].dokter_dijadwalkan[jadwal_harian[hari][s].jumlah_dokter_dijadwalkan], "-");
                    jadwal_harian[hari][s].jumlah_dokter_dijadwalkan++;
                    unassigned_shifts++;
                }
            }
        }
    }

    printf("[v] Jadwal otomatis selesai dibuat.\n");
    if (unassigned_shifts > 0) {
        printf("[!] Peringatan: Terdapat %d slot yang tidak bisa diisi bahkan dengan strategi darurat.\n", unassigned_shifts);
    }
    printf("Tips: Gunakan menu '3. Tampilkan Informasi Jadwal' untuk melihat hasilnya.\n");
}