#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct dan variabelnya
struct Langkah {
    char namaTugas[100];
    struct Langkah* prasyaratKiri;
    struct Langkah* prasyaratKanan;
};

// fungsi Buat Node
struct Langkah* buatLangkahBaru(char* nama) {
    struct Langkah* langkahBaru = (struct Langkah*)malloc(sizeof(struct Langkah));
    strcpy(langkahBaru->namaTugas, nama);
    langkahBaru->prasyaratKiri = NULL;
    langkahBaru->prasyaratKanan = NULL;
    return langkahBaru;
}

// Fungsi untuk mencari sebuah langkah berdasarkan nama
struct Langkah* cariLangkah(struct Langkah* saatIni, char* nama) {
    if (saatIni == NULL) {
        return NULL;
    }
    if (strcmp(saatIni->namaTugas, nama) == 0) {
        return saatIni;
    }
    struct Langkah* ditemukan = cariLangkah(saatIni->prasyaratKiri, nama);
    if (ditemukan != NULL) {
        return ditemukan;
    }
    return cariLangkah(saatIni->prasyaratKanan, nama);
}

// --- FUNGSI KUNJUNGAN ---

// PreOrder: Menampilkan struktur dekomposisi perakitan
void tampilkanStrukturPerakitan(struct Langkah* tugas) {
    if (tugas == NULL) return;
    printf(" -> %s", tugas->namaTugas);
    tampilkanStrukturPerakitan(tugas->prasyaratKiri);
    tampilkanStrukturPerakitan(tugas->prasyaratKanan);
}

// InOrder: Menampilkan urutan alternatif (kurang bermakna)
void tampilkanUrutanAlternatif(struct Langkah* tugas) {
    if (tugas == NULL) return;
    tampilkanUrutanAlternatif(tugas->prasyaratKiri);
    printf(" -> %s", tugas->namaTugas);
    tampilkanUrutanAlternatif(tugas->prasyaratKanan);
}

// PostOrder: Menampilkan DAFTAR KERJA yang logis
void tampilkanDaftarKerjaLogis(struct Langkah* tugas) {
    if (tugas == NULL) return;
    tampilkanDaftarKerjaLogis(tugas->prasyaratKiri);
    tampilkanDaftarKerjaLogis(tugas->prasyaratKanan);
    printf(" -> %s", tugas->namaTugas);
}

// --- FUNGSI UTAMA (INTERAKTIF) ---

int main() {
    struct Langkah* tugasUtama = NULL;
    int pilihan;
    char namaBaru[100], namaInduk[100], posisi;

    printf("============================================\n");
    printf("   PROGRAM SIMULATOR MERAKIT KOMPUTER\n");
    printf("============================================\n");

    while (1) {
        printf("\nMenu:\n");
        printf("1. Tambah Langkah Perakitan Baru\n");
        printf("2. Tampilkan Daftar Kerja (PostOrder)\n");
        printf("3. Tampilkan Struktur Perakitan (PreOrder)\n");
        printf("4. Tampilkan Urutan Alternatif (InOrder)\n");
        printf("5. Keluar\n");
        printf("Pilih Opsi: ");
        scanf("%d", &pilihan);
        getchar(); // Membersihkan buffer newline

        switch (pilihan) {
            case 1:
                printf("Masukkan nama langkah baru: ");
                fgets(namaBaru, 100, stdin);
                namaBaru[strcspn(namaBaru, "\n")] = 0; // Menghapus newline

                if (tugasUtama == NULL) {
                    tugasUtama = buatLangkahBaru(namaBaru);
                    printf("Langkah '%s' telah ditetapkan sebagai tugas utama (root).\n", namaBaru);
                } else {
                    printf("Masukkan nama langkah induknya: ");
                    fgets(namaInduk, 100, stdin);
                    namaInduk[strcspn(namaInduk, "\n")] = 0;

                    struct Langkah* induk = cariLangkah(tugasUtama, namaInduk);
                    if (induk == NULL) {
                        printf("Error: Langkah induk '%s' tidak ditemukan!\n", namaInduk);
                    } else {
                        printf("Letakkan sebagai prasyarat Kiri (L) atau Kanan (R)? ");
                        scanf(" %c", &posisi);
                        getchar();

                        if (posisi == 'L' || posisi == 'l') {
                            if (induk->prasyaratKiri != NULL) {
                                printf("Error: Prasyarat Kiri sudah terisi!\n");
                            } else {
                                induk->prasyaratKiri = buatLangkahBaru(namaBaru);
                                printf("Langkah '%s' ditambahkan sebagai prasyarat kiri dari '%s'.\n", namaBaru, namaInduk);
                            }
                        } else if (posisi == 'R' || posisi == 'r') {
                            if (induk->prasyaratKanan != NULL) {
                                printf("Error: Prasyarat Kanan sudah terisi!\n");
                            } else {
                                induk->prasyaratKanan = buatLangkahBaru(namaBaru);
                                printf("Langkah '%s' ditambahkan sebagai prasyarat kanan dari '%s'.\n", namaBaru, namaInduk);
                            }
                        } else {
                            printf("Error: Posisi tidak valid.\n");
                        }
                    }
                }
                break;

            case 2:
                if (tugasUtama == NULL) {
                    printf("Pohon masih kosong. Tambahkan langkah terlebih dahulu.\n");
                } else {
                    printf("\n## Daftar Kerja Logis (PostOrder) ##\nUrutan:");
                    tampilkanDaftarKerjaLogis(tugasUtama);
                    printf("\n");
                }
                break;
            
            case 3:
                 if (tugasUtama == NULL) {
                    printf("Pohon masih kosong. Tambahkan langkah terlebih dahulu.\n");
                } else {
                    printf("\n## Struktur Perakitan (PreOrder) ##\nUrutan:");
                    tampilkanStrukturPerakitan(tugasUtama);
                    printf("\n");
                }
                break;

            case 4:
                if (tugasUtama == NULL) {
                    printf("Pohon masih kosong. Tambahkan langkah terlebih dahulu.\n");
                } else {
                    printf("\n## Urutan Alternatif (InOrder) ##\nUrutan:");
                    tampilkanUrutanAlternatif(tugasUtama);
                    printf("\n");
                }
                break;
            
            case 5:
                printf("Terima kasih telah menggunakan program ini!\n");
                exit(0);

            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    }

    return 0;
}