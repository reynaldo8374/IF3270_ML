# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 -g

# Daftar file source (.c)
SRCS = main.c interface.c dokter_handler.c jadwal_generator.c jadwal_io.c

# Daftar file header (.h) untuk dependensi
HEADERS = definitions.h interface.h dokter_handler.h jadwal_generator.h jadwal_io.h

# Konfigurasi berdasarkan Sistem Operasi (Windows atau lainnya)
ifeq ($(OS),Windows_NT)
    # Pengaturan untuk Windows (cmd.exe atau PowerShell)
    TARGET = jadwal_app.exe
    RM = del /Q
    RUN_CMD = .\\$(TARGET)
else
    # Pengaturan untuk Linux, macOS, atau Git Bash/WSL di Windows
    TARGET = jadwal_app
    RM = rm -f
    RUN_CMD = ./$(TARGET)
endif

# Membuat file object (.o) dari file source (.c)
OBJS = $(SRCS:.c=.o)


# ===================================================================
# TARGETS (Aturan-aturan untuk 'make')
# ===================================================================

# Aturan default (dijalankan jika Anda hanya mengetik 'make')
# Sama dengan 'make all'
all: $(TARGET)

# Aturan untuk membuat file executable dari file-file object
$(TARGET): $(OBJS)
	@echo "Linking semua object file menjadi -> $@"
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo "Build selesai. Executable '$(TARGET)' siap."

# Aturan umum untuk membuat file .o dari file .c
# Ini memberitahu 'make' bahwa setiap file .o bergantung pada file .c-nya
# dan SEMUA file header. Ini cara aman untuk memastikan kompilasi ulang jika ada header berubah.
%.o: %.c $(HEADERS)
	@echo "Compiling $< -> $@"
	$(CC) $(CFLAGS) -c $< -o $@

# --- TARGET BARU ANDA ---
# Aturan untuk menjalankan program
run: all
	@echo "--- Menjalankan Program ---"
	$(RUN_CMD)
	@echo "--- Program Selesai ---"

# Aturan untuk membersihkan file yang dibuat (object dan executable)
clean:
	@echo "--- Membersihkan Build Files ---"
	-$(RM) $(OBJS) $(TARGET)

# Memberitahu 'make' bahwa target ini bukan nama file, melainkan perintah
.PHONY: all clean run