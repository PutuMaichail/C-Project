# --- Variabel Definisi ---
CC = gcc # Menetapkan 'gcc' sebagai compiler C yang akan digunakan.
# Menetapkan flag (opsi) untuk compiler.
# -Wall -Wextra: Mengaktifkan set peringatan yang komprehensif.
# -std=c11: Menetapkan standar bahasa C ke versi 2011.
# -g: Menyertakan simbol debug dalam file executable untuk digunakan oleh GDB.
CFLAGS = -Wall -Wextra -std=c11 -g

# Menetapkan nama untuk file executable output.
TARGET = tournament_manager

# Membuat daftar semua file source (.c) yang menjadi bagian dari proyek.
SOURCES = main.c team_management.c queue_operations.c stack_operations.c bracket_operations.c

# Secara otomatis menghasilkan daftar file object (.o) dari daftar SOURCES.
# Contoh: main.c -> main.o
OBJECTS = $(SOURCES:.c=.o)


# --- Aturan Build ---

# Aturan 'all' adalah target default. 'make all' atau 'make' akan menjalankan ini.
# Target ini bergantung pada $(TARGET), yang berarti file executable harus dibuat.
all: $(TARGET)

# Aturan untuk membuat file executable (proses linking).
# Target ini bergantung pada semua file object.
# Perintah di bawahnya hanya akan dijalankan jika salah satu file .o lebih baru dari file TARGET.
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Aturan Pola (Pattern Rule) untuk kompilasi.
# Ini mendefinisikan cara membuat file .o dari file .c yang sesuai.
# %.o: Target (file .o apa pun).
# %.c: Dependensi (file .c dengan nama yang sama).
# tournament.h: Dependensi tambahan; jika header berubah, semua file akan dikompilasi ulang.
# Perintah ini dijalankan jika dependensi (.c atau .h) lebih baru dari target (.o).
%.o: %.c tournament.h
	# $(CC) $(CFLAGS): Panggil compiler dengan flag yang telah ditetapkan.
	# -c: Opsi untuk mengompilasi saja, tanpa melakukan linking.
	# $<: Variabel otomatis yang berarti "dependensi pertama" (file .c).
	# -o $@: Opsi output, $@ adalah variabel otomatis yang berarti "nama target" (file .o).
	$(CC) $(CFLAGS) -c $< -o $@

# Aturan untuk membersihkan direktori proyek.
# Perintah 'rm -f' akan menghapus file-file yang ditentukan tanpa meminta konfirmasi.
clean:
	rm -f $(OBJECTS) $(TARGET)

# Mendeklarasikan target yang bukan merupakan nama file.
# Ini mencegah 'make' bingung jika ada file di direktori yang bernama 'all' atau 'clean'.
.PHONY: all clean