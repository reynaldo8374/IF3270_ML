# ELXX08

Tugas besar ELXX08 oleh 13223083, 13223090, 13223103, 13223109, 13223111.

## Aplikasi Penjadwalan Dokter Sederhana

Program ini adalah program C sederhana untuk membuat jadwal jaga dokter selama 30 hari secara otomatis. Program ini membaca daftar dokter dari file CSV, lengkap dengan batasan jumlah shift per minggu dan preferensi shift mereka.

Tujuan utamanya adalah menghasilkan jadwal yang adil dan meminimalkan pelanggaran preferensi dokter.

## Fitur Utama

* **Manajemen Dokter**: Bisa menambah, menghapus, dan melihat daftar dokter.
* **Penyimpanan Data**: Data dokter disimpan dalam file `daftar_dokter.csv`.
* **Pembuatan Jadwal Otomatis**: Menghasilkan jadwal 30 hari untuk 3 shift (Pagi, Siang, Malam).
* **Laporan**: Bisa menampilkan ringkasan total shift dan total pelanggaran preferensi untuk setiap dokter.
* **Ekspor Jadwal**: Jadwal yang sudah jadi bisa disimpan ke file `jadwal.csv`.

## Kebutuhan

Sebelum menggunakan, pastikan komputer memiliki:

* Compiler C, misalnya GCC.
* Utilitas `make`.

## Cara Menjalankan Program

Program ini udah dilengkapi `Makefile`...

1.  **Untuk Kompilasi**

    Perintah ini akan building `aplikasi_jadwal.exe` (di Windows) atau `aplikasi_jadwal` (di Linux/macOS).

    ```bash
    make
    ```

2.  **Untuk Kompilasi & Langsung Run**

    Perintah ini akan mengkompilasi (kalau ada perubahan kode) sekaligus menjalankan programnya.

    ```bash
    make run
    ```

3.  **Untuk Clean/Delete**

    Jika ingin menghapus file kompilasi (`.o` dan `.exe`), gunakan perintah ini.

    ```bash
    make clean
    ```

## File Penting

Ada dua file CSV utama yang digunakan program ini:

### `daftar_dokter.csv`

Ini adalah file **INPUT**. Jika filenya belum ada, program akan otomatis membuatnya saat user menambahkan dokter pertama kali.

**Format:**
`Nama,Max Shift/Minggu,Preferensi Shift`

**Contoh isi `daftar_dokter.csv`:**

```csv
Nama,Max Shift/Minggu,Preferensi Shift
dr. Budi,5,Pagi
dr. Citra,4,Pagi,Malam
dr. Anisa,5,Siang
dr. Doni,6,Malam
dr. Eka,5,Pagi,Siang
```

### `jadwal.csv`

Ini adalah file **OUTPUT**, jika dibutuhkan user.