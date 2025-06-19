# Nama compiler yang digunakan
CC = gcc

# Opsi/flag untuk compiler
CFLAGS = -Wall -g

# Nama file executable yang akan dihasilkan
TARGET = program_penjadwalan

# Daftar semua file sumber .c
SOURCES = main.c util.c dokter.c penjadwalan.c tampilan.c

# Secara otomatis mengubah daftar SOURCES (.c) menjadi daftar file objek (.o)
OBJECTS = $(SOURCES:.c=.o)

# Aturan default (yang dijalankan saat Anda hanya mengetik 'make')
all: $(TARGET)

# Aturan untuk membuat file executable dari file-file objek
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Aturan umum (pattern rule) untuk membuat file .o dari file .c
%.o: %.c sistem_penjadwalan.h
	$(CC) $(CFLAGS) -c $< -o $@

# Aturan untuk membersihkan direktori dari file hasil kompilasi
.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)