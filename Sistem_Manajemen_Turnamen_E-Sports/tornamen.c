#include <stdio.h>    // Diperlukan untuk fungsi input-output seperti printf dan scanf
#include <stdlib.h>   // Diperlukan untuk manajemen memori seperti malloc dan free
#include <string.h>   // Diperlukan untuk operasi string seperti strcpy dan strcmp
#include <time.h>     // Diperlukan untuk fungsi srand agar pengacakan lebih baik

#define MAKS_TIM 32     // Jumlah maksimum tim yang bisa didaftarkan
#define NAMA_MAKS 50    // Panjang maksimum untuk nama tim dan pemain

// ===================================================
//  DEFINISI STRUKTUR DATA (ADT - Abstract Data Type)
// ===================================================

// ADT untuk Pemain, digunakan sebagai node untuk Linked List
typedef struct Pemain {
    char nama[NAMA_MAKS];       // Variabel untuk menyimpan nama pemain
    struct Pemain *berikutnya;  // Pointer ke pemain selanjutnya dalam daftar (Linked List)
} Pemain;

// ADT untuk Tim
typedef struct Tim {
    int id;                     // ID unik untuk setiap tim
    char nama[NAMA_MAKS];       // Nama tim
    int poinKualifikasi;        // Poin yang digunakan untuk seeding/peringkat
    Pemain *kepalaPemain;       // Pointer ke awal (kepala) dari Linked List pemain
} Tim;

// ADT untuk Pertandingan
typedef struct Pertandingan {
    Tim *tim1;                  // Pointer ke data tim pertama
    Tim *tim2;                  // Pointer ke data tim kedua
    Tim *pemenang;              // Pointer ke tim yang menang, NULL jika belum ada
} Pertandingan;

// ADT untuk Node Pohon (Bagan Pertandingan)
typedef struct NodeBagan {
    Pertandingan laga;              // Data pertandingan di node ini
    struct NodeBagan *kiri;         // Pointer ke pertandingan anak di sebelah kiri
    struct NodeBagan *kanan;        // Pointer ke pertandingan anak di sebelah kanan
    struct NodeBagan *induk;        // Pointer ke pertandingan induk (level di atasnya)
} NodeBagan;

// ===========================================
//  IMPLEMENTASI STRUKTUR DATA ANTREN (QUEUE)
// ===========================================

// Node untuk Antrean, menyimpan pointer ke NodeBagan
typedef struct NodeAntrean {
    NodeBagan *nodeBagan;       // Data yang disimpan adalah node dari bagan
    struct NodeAntrean *berikutnya; // Pointer ke node antrean selanjutnya
} NodeAntrean;

// Struktur utama Antrean
typedef struct Antrean {
    NodeAntrean *depan;         // Pointer ke elemen depan (yang akan keluar pertama)
    NodeAntrean *belakang;      // Pointer ke elemen belakang (yang baru masuk)
} Antrean;

// Fungsi untuk membuat antrean baru yang kosong
Antrean* buatAntrean() {
    Antrean *q = (Antrean*)malloc(sizeof(Antrean)); // Alokasikan memori untuk Antrean
    q->depan = q->belakang = NULL;                  // Awalnya, depan dan belakang adalah NULL
    return q;                                       // Kembalikan pointer ke antrean yang baru dibuat
}

// Fungsi untuk menambahkan elemen ke belakang antrean (enqueue)
void masukAntrean(Antrean *q, NodeBagan *nodeBagan) {
    NodeAntrean *temp = (NodeAntrean*)malloc(sizeof(NodeAntrean)); // Buat node antrean baru
    temp->nodeBagan = nodeBagan;                                   // Isi data
    temp->berikutnya = NULL;                                       // Node baru akan menjadi yang terakhir, jadi 'berikutnya' adalah NULL
    if (q->belakang == NULL) {                                     // Jika antrean kosong
        q->depan = q->belakang = temp;                             // Node baru menjadi depan dan belakang
        return;
    }
    q->belakang->berikutnya = temp; // Hubungkan node terakhir saat ini ke node baru
    q->belakang = temp;             // Pindahkan pointer belakang ke node baru
}

// Fungsi untuk mengambil elemen dari depan antrean (dequeue)
NodeBagan* keluarAntrean(Antrean *q) {
    if (q->depan == NULL) return NULL; // Jika antrean kosong, tidak ada yang bisa diambil
    NodeAntrean *temp = q->depan;      // Simpan node depan sementara
    NodeBagan *nodeBagan = temp->nodeBagan; // Ambil datanya
    q->depan = q->depan->berikutnya;   // Pindahkan pointer depan ke node selanjutnya
    if (q->depan == NULL) {            // Jika setelah diambil antrean jadi kosong
        q->belakang = NULL;            // Set belakang juga menjadi NULL
    }
    free(temp);                        // Bebaskan memori dari node depan yang lama
    return nodeBagan;                  // Kembalikan data yang diambil
}

// =============================================
//  IMPLEMENTASI STRUKTUR DATA TUMPUKAN (STACK)
// =============================================

// Node untuk Tumpukan
typedef struct NodeTumpukan {
    NodeBagan *nodeLaga;             // Data yang disimpan adalah node pertandingan yang hasilnya di-input
    struct NodeTumpukan *berikutnya; // Pointer ke node di bawahnya
} NodeTumpukan;

// Struktur utama Tumpukan
typedef struct Tumpukan {
    NodeTumpukan *puncak;           // Pointer ke elemen paling atas dari tumpukan
} Tumpukan;

// Fungsi untuk membuat tumpukan baru yang kosong
Tumpukan* buatTumpukan() {
    Tumpukan *s = (Tumpukan*)malloc(sizeof(Tumpukan)); // Alokasikan memori
    s->puncak = NULL;                                  // Awalnya, puncak adalah NULL
    return s;                                          // Kembalikan tumpukan baru
}

// Fungsi untuk menambahkan elemen ke atas tumpukan (push)
void dorongKeTumpukan(Tumpukan *s, NodeBagan *node) {
    NodeTumpukan *nodeTumpukan = (NodeTumpukan*)malloc(sizeof(NodeTumpukan)); // Buat node tumpukan baru
    nodeTumpukan->nodeLaga = node;      // Isi data
    nodeTumpukan->berikutnya = s->puncak; // Hubungkan node baru ke puncak yang lama
    s->puncak = nodeTumpukan;           // Jadikan node baru sebagai puncak
}

// Fungsi untuk mengambil elemen dari atas tumpukan (pop)
NodeBagan* ambilDariTumpukan(Tumpukan *s) {
    if (s->puncak == NULL) return NULL; // Jika tumpukan kosong, tidak ada yang bisa diambil
    NodeTumpukan *temp = s->puncak;     // Simpan node puncak sementara
    NodeBagan *node = temp->nodeLaga;   // Ambil datanya
    s->puncak = s->puncak->berikutnya;  // Pindahkan puncak ke node di bawahnya
    free(temp);                         // Bebaskan memori dari puncak yang lama
    return node;                        // Kembalikan data yang diambil
}

// ===============================================
//  FUNGSI-FUNGSI BANTU (SORTING, SEARCHING, DLL)
// ===============================================

// --- FUNGSI UNTUK MEMBEBASKAN MEMORI ---
void bebaskanPemain(Pemain *kepala) {
    Pemain *saatIni = kepala;
    while (saatIni != NULL) {
        Pemain *berikutnya = saatIni->berikutnya;
        free(saatIni);
        saatIni = berikutnya;
    }
}

void bebaskanBagan(NodeBagan *akar) {
    if (akar == NULL) {
        return;
    }
    bebaskanBagan(akar->kiri);
    bebaskanBagan(akar->kanan);
    free(akar);
}

void bebaskanTumpukan(Tumpukan *s) {
    NodeTumpukan *saatIni = s->puncak;
    while (saatIni != NULL) {
        NodeTumpukan *berikutnya = saatIni->berikutnya;
        free(saatIni);
        saatIni = berikutnya;
    }
    free(s);
}
// --- AKHIR FUNGSI MEMBEBASKAN MEMORI ---

// Fungsi untuk menambah pemain ke sebuah tim (menggunakan Linked List)
void tambahPemain(Tim *tim, const char *namaPemain) {
    Pemain *pemainBaru = (Pemain*)malloc(sizeof(Pemain)); // Alokasikan memori untuk pemain baru
    strcpy(pemainBaru->nama, namaPemain);                 // Salin nama pemain ke struct
    pemainBaru->berikutnya = NULL;                        // Awalnya, pointer 'berikutnya' adalah NULL

    if (tim->kepalaPemain == NULL) {                      // Jika belum ada pemain sama sekali
        tim->kepalaPemain = pemainBaru;                   // Jadikan pemain baru sebagai kepala
    } else {                                              // Jika sudah ada pemain
        Pemain *saatIni = tim->kepalaPemain;              // Mulai dari kepala
        while (saatIni->berikutnya != NULL) {             // Telusuri sampai akhir daftar
            saatIni = saatIni->berikutnya;
        }
        saatIni->berikutnya = pemainBaru;                 // Sambungkan pemain terakhir dengan pemain baru
    }
}

// Fungsi untuk menampilkan daftar pemain dari sebuah tim
void tampilkanPemain(Tim *tim) {
    if (tim == NULL) return; // Jika tim tidak ada, keluar
    printf("    Pemain: ");
    Pemain *saatIni = tim->kepalaPemain; // Mulai dari kepala linked list
    if (saatIni == NULL) {
        printf("Tidak ada pemain terdaftar.\n");
        return;
    }
    while (saatIni != NULL) {                   // Selama belum sampai di akhir daftar
        printf("%s", saatIni->nama);            // Cetak nama pemain
        if (saatIni->berikutnya != NULL) printf(", "); // Beri koma jika bukan pemain terakhir
        saatIni = saatIni->berikutnya;          // Pindah ke pemain selanjutnya
    }
    printf("\n"); // Baris baru di akhir
}

// Fungsi untuk mengurutkan tim berdasarkan poin kualifikasi (Bubble Sort)
void urutkanTim(Tim tim[], int n) {
    for (int i = 0; i < n - 1; i++) {           // Loop untuk setiap pass
        for (int j = 0; j < n - i - 1; j++) {   // Loop untuk perbandingan
            // Jika poin tim saat ini lebih kecil dari tim berikutnya, tukar posisi
            if (tim[j].poinKualifikasi < tim[j + 1].poinKualifikasi) {
                Tim temp = tim[j];              // Simpan tim[j] sementara
                tim[j] = tim[j + 1];            // Pindahkan tim[j+1] ke posisi tim[j]
                tim[j + 1] = temp;              // Pindahkan tim sementara ke posisi tim[j+1]
            }
        }
    }
    printf("Tim telah diurutkan berdasarkan poin kualifikasi (seeding).\n");
}

// Fungsi untuk mencari tim berdasarkan nama (Linear Search)
Tim* cariTim(Tim tim[], int n, const char* nama) {
    for (int i = 0; i < n; i++) { // Loop untuk setiap tim dalam array
        if (strcmp(tim[i].nama, nama) == 0) { // Bandingkan nama tim dengan nama yang dicari
            return &tim[i]; // Jika cocok, kembalikan alamat memori dari tim tersebut
        }
    }
    return NULL; // Jika tidak ditemukan setelah semua dicek, kembalikan NULL
}

// =================================================
//  FUNGSI INTI UNTUK MANAJEMEN BAGAN (POHON BINER)
// =================================================

// Fungsi untuk membuat node bagan baru
NodeBagan* buatNodeBagan(Tim *t1, Tim *t2) {
    NodeBagan *node = (NodeBagan*)malloc(sizeof(NodeBagan)); // Alokasikan memori
    node->laga.tim1 = t1;       // Set tim 1
    node->laga.tim2 = t2;       // Set tim 2
    node->laga.pemenang = NULL; // Awalnya belum ada pemenang
    node->kiri = node->kanan = node->induk = NULL; // Pointer anak dan induk awalnya NULL
    return node;
}

// Fungsi utama untuk membangun keseluruhan bagan pertandingan dari daftar tim
NodeBagan* bangunBagan(Tim tim[], int jumlahTim) {
    if (jumlahTim == 0) return NULL; // Jika tidak ada tim, tidak ada bagan

    Antrean *q = buatAntrean(); // Buat antrean untuk membantu membangun pohon tingkat demi tingkat

    // Langkah 1: Buat node daun (ronde pertama) dan masukkan ke antrean
    for (int i = 0; i < jumlahTim; i += 2) {
        NodeBagan *nodeLaga = buatNodeBagan(&tim[i], &tim[i+1]);
        masukAntrean(q, nodeLaga);
    }

    // Langkah 2: Bangun pohon ke atas sampai hanya tersisa satu node di antrean (yaitu root/final)
    while (q->depan->berikutnya != NULL) {
        NodeBagan *anak1 = keluarAntrean(q); // Ambil pertandingan pertama dari antrean
        NodeBagan *anak2 = keluarAntrean(q); // Ambil pertandingan kedua dari antrean

        // Buat node induk untuk kedua pertandingan tadi. Timnya masih NULL karena menunggu pemenang.
        NodeBagan *lagaInduk = buatNodeBagan(NULL, NULL);
        lagaInduk->kiri = anak1;    // Jadikan laga pertama sebagai anak kiri
        lagaInduk->kanan = anak2;   // Jadikan laga kedua sebagai anak kanan
        anak1->induk = lagaInduk;   // Set induk dari anak1
        anak2->induk = lagaInduk;   // Set induk dari anak2
        masukAntrean(q, lagaInduk); // Masukkan laga induk kembali ke antrean untuk ronde selanjutnya
    }

    // Node terakhir yang tersisa di antrean adalah akar (root) dari pohon bagan
    NodeBagan *akar = keluarAntrean(q);
    free(q); // Bebaskan memori antrean karena sudah tidak dipakai
    return akar;
}

// Fungsi untuk menampilkan bagan secara visual (rekursif)
void tampilkanBagan(NodeBagan *akar, int tingkat) {
    if (akar == NULL) return; // Basis rekursif: jika node kosong, berhenti

    // Tampilkan anak kanan terlebih dahulu agar urutan visualnya benar seperti bagan
    tampilkanBagan(akar->kanan, tingkat + 1);

    // Cetak indentasi (tab) sesuai dengan tingkat kedalaman node di pohon
    for (int i = 0; i < tingkat; i++) printf("\t\t");

    // Cetak informasi pertandingan
    if (akar->kiri == NULL && akar->kanan == NULL) { // Jika ini node daun (ronde pertama)
        printf("--- %s\n", akar->laga.tim1 ? akar->laga.tim1->nama : "Menunggu");
        for (int i = 0; i < tingkat; i++) printf("\t\t");
        printf("   |\n");
        for (int i = 0; i < tingkat; i++) printf("\t\t");
        printf("--- %s\n", akar->laga.tim2 ? akar->laga.tim2->nama : "Menunggu");
    } else { // Jika ini node di tingkat atas
        printf("--- Pemenang\n");
    }
    
    // Tampilkan pemenang dari laga ini jika sudah ada
    if (akar->laga.pemenang != NULL) {
         for (int i = 0; i < tingkat; i++) printf("\t\t");
         printf("   |---> [%s]\n", akar->laga.pemenang->nama);
    }
    
    // Tampilkan anak kiri
    tampilkanBagan(akar->kiri, tingkat + 1);
}

// Fungsi untuk mencari pertandingan berikutnya yang siap dimainkan (rekursif)
NodeBagan* cariLagaSelanjutnya(NodeBagan* akar) {
    if (akar == NULL || akar->laga.pemenang != NULL) {
        return NULL; // Jika node kosong atau sudah ada pemenang, abaikan
    }

    // Cek anak-anaknya terlebih dahulu
    if (akar->kiri != NULL && akar->kanan != NULL) {
        // Jika anak kiri belum punya pemenang, cari di sub-pohon kiri
        if (akar->kiri->laga.pemenang == NULL) {
            return cariLagaSelanjutnya(akar->kiri);
        }
        // Jika anak kanan belum punya pemenang, cari di sub-pohon kanan
        if (akar->kanan->laga.pemenang == NULL) {
            return cariLagaSelanjutnya(akar->kanan);
        }
        // Jika kedua anak sudah punya pemenang, berarti laga ini (induk) siap dimainkan
        return akar;
    }
    
    // Jika ini adalah node daun (ronde 1), berarti laga ini siap dimainkan
    return akar;
}


// Fungsi untuk menginput hasil sebuah pertandingan
void inputHasilLaga(NodeBagan* node, int pilihanPemenang, Tumpukan* tumpukanUndo) {
    if (node == NULL || node->laga.pemenang != NULL) {
        printf("Pertandingan ini tidak valid atau sudah selesai.\n");
        return;
    }

    if (pilihanPemenang == 1) {
        node->laga.pemenang = node->laga.tim1; // Set tim 1 sebagai pemenang
    } else if (pilihanPemenang == 2) {
        node->laga.pemenang = node->laga.tim2; // Set tim 2 sebagai pemenang
    } else {
        printf("Pilihan tidak valid.\n");
        return;
    }
    
    printf("Pemenang dicatat: %s\n", node->laga.pemenang->nama);

    // Dorong laga ini ke tumpukan Undo untuk bisa dibatalkan nanti
    dorongKeTumpukan(tumpukanUndo, node);

    // "Naikkan" tim pemenang ke slot pertandingan induknya
    if (node->induk != NULL) {
        if (node == node->induk->kiri) { // Jika node ini adalah anak kiri dari induknya
            node->induk->laga.tim1 = node->laga.pemenang; // Isi slot tim1 di induk
        } else { // Jika node ini adalah anak kanan dari induknya
            node->induk->laga.tim2 = node->laga.pemenang; // Isi slot tim2 di induk
        }
    }
}

// Fungsi untuk membatalkan hasil pertandingan terakhir yang diinput
void batalkanHasilTerakhir(Tumpukan* tumpukanUndo) {
    if (tumpukanUndo->puncak == NULL) { // Cek apakah tumpukan kosong
        printf("Tidak ada riwayat pertandingan untuk dibatalkan.\n");
        return;
    }

    NodeBagan* nodeUntukBatal = ambilDariTumpukan(tumpukanUndo); // Ambil laga terakhir dari tumpukan
    printf("Membatalkan hasil untuk pertandingan: %s vs %s\n", nodeUntukBatal->laga.tim1->nama, nodeUntukBatal->laga.tim2->nama);

    // Hapus pemenang dari slot di induknya
    if (nodeUntukBatal->induk != NULL) {
        if (nodeUntukBatal == nodeUntukBatal->induk->kiri) {
            nodeUntukBatal->induk->laga.tim1 = NULL;
        } else {
            nodeUntukBatal->induk->laga.tim2 = NULL;
        }
    }
    
    // Hapus pemenang dari laga itu sendiri
    nodeUntukBatal->laga.pemenang = NULL;
    printf("Hasil pertandingan berhasil dibatalkan.\n");
}


// =============================
//  FUNGSI UTAMA PROGRAM (MAIN)
// =============================
int main() {
    // Deklarasi variabel utama
    Tim daftarTim[MAKS_TIM];          // Array untuk menyimpan semua data tim
    int jumlahTim = 0;                // Counter untuk jumlah tim yang terdaftar
    NodeBagan *akarTurnamen = NULL;   // Pointer ke akar (root) dari bagan turnamen, awalnya NULL
    Tumpukan *tumpukanUndo = buatTumpukan(); // Buat tumpukan untuk fitur Undo
    srand(time(NULL));                // Inisialisasi seed untuk angka acak

    int pilihan; // Variabel untuk menyimpan pilihan menu dari pengguna
    do {
        // Tampilkan menu utama
        printf("\n===== SISTEM MANAJEMEN TURNAMEN E-SPORTS =====\n");
        printf("1. Daftarkan Tim Baru\n");
        printf("2. Tampilkan Semua Tim Terdaftar\n");
        printf("3. Urutkan Tim (Seeding)\n");
        printf("4. Buat dan Mulai Turnamen\n");
        printf("5. Tampilkan Bagan Turnamen\n");
        printf("6. Input Hasil Pertandingan\n");
        printf("7. Batalkan Hasil Terakhir\n");
        printf("8. Cari Tim\n");
        printf("0. Keluar\n");
        printf("Pilihan Anda: ");
        scanf("%d", &pilihan); // Baca input pilihan dari pengguna
        getchar();             // Ambil karakter newline dari buffer agar tidak mengganggu fgets berikutnya

        // Proses pilihan pengguna menggunakan switch-case
        switch (pilihan) {
            case 1: { // Pilihan: Daftarkan Tim
                if (jumlahTim >= MAKS_TIM) {
                    printf("Pendaftaran sudah penuh!\n");
                    break;
                }
                Tim *timBaru = &daftarTim[jumlahTim]; // Ambil alamat memori dari elemen array berikutnya
                timBaru->id = jumlahTim + 1;         // Set ID
                timBaru->kepalaPemain = NULL;        // Inisialisasi daftar pemain kosong
                
                printf("Masukkan Nama Tim: ");
                fgets(timBaru->nama, NAMA_MAKS, stdin); // Baca nama tim
                timBaru->nama[strcspn(timBaru->nama, "\n")] = 0; // Hapus karakter newline dari akhir string

                printf("Masukkan Poin Kualifikasi: ");
                scanf("%d", &timBaru->poinKualifikasi); // Baca poin kualifikasi
                getchar(); // Ambil newline

                // Bagian input pemain dinamis
                int jumlahPemain;
                printf("Masukkan jumlah pemain untuk tim ini: ");
                scanf("%d", &jumlahPemain); // Baca jumlah pemain
                getchar(); // Ambil newline

                char namaPemain[NAMA_MAKS];
                for(int i = 0; i < jumlahPemain; i++){ // Loop sebanyak jumlah pemain
                     printf("Masukkan Nama Pemain %d: ", i + 1);
                     fgets(namaPemain, NAMA_MAKS, stdin);
                     namaPemain[strcspn(namaPemain, "\n")] = 0;
                     tambahPemain(timBaru, namaPemain); // Panggil fungsi untuk menambah pemain
                }
                
                jumlahTim++; // Tambah counter jumlah tim
                printf("\n    Tim '%s' dengan %d pemain berhasil didaftarkan!\n", timBaru->nama, jumlahPemain);
                break;
            }
            case 2: { // Pilihan: Tampilkan Semua Tim
                 printf("\n--- DAFTAR TIM ---\n");
                 for(int i=0; i<jumlahTim; i++){
                     printf("ID: %d, Nama: %s, Poin: %d\n", daftarTim[i].id, daftarTim[i].nama, daftarTim[i].poinKualifikasi);
                     tampilkanPemain(&daftarTim[i]); // Tampilkan juga daftar pemainnya
                 }
                 break;
            }
            case 3: { // Pilihan: Urutkan Tim
                urutkanTim(daftarTim, jumlahTim); // Panggil fungsi sorting
                break;
            }
            case 4: { // Pilihan: Buat Turnamen
                // Sistem gugur sederhana memerlukan jumlah tim kelipatan pangkat 2 (4, 8, 16, ...)
                if (jumlahTim < 2 || (jumlahTim & (jumlahTim - 1)) != 0) {
                    printf("Jumlah tim harus merupakan kelipatan pangkat 2 (contoh: 4, 8, 16) untuk membuat bagan.\n");
                    printf("Jumlah tim saat ini: %d\n", jumlahTim);
                    break;
                }
                akarTurnamen = bangunBagan(daftarTim, jumlahTim); // Panggil fungsi untuk membangun pohon bagan
                printf("Bagan turnamen berhasil dibuat!\n");
                break;
            }
            case 5: { // Pilihan: Tampilkan Bagan
                if (akarTurnamen == NULL) {
                    printf("Bagan belum dibuat. Silakan mulai turnamen terlebih dahulu (Pilihan 4).\n");
                } else {
                    printf("\n\n\n--- BAGAN TURNAMEN ---\n");
                    tampilkanBagan(akarTurnamen, 0); // Panggil fungsi untuk menampilkan bagan
                    printf("\n\n");
                }
                break;
            }
            case 6: { // Pilihan: Input Hasil
                if (akarTurnamen == NULL) {
                    printf("Bagan belum dibuat.\n");
                    break;
                }
                NodeBagan *lagaBerikutnya = cariLagaSelanjutnya(akarTurnamen); // Cari laga yang siap dimainkan
                if (lagaBerikutnya == NULL) {
                    printf("Semua pertandingan sudah selesai atau turnamen telah berakhir!\n");
                    if(akarTurnamen->laga.pemenang != NULL) {
                        printf("    JUARA TURNAMEN: %s \n", akarTurnamen->laga.pemenang->nama);
                    }
                    break;
                }
                printf("\n--- Input Hasil Pertandingan ---\n");
                printf("Pertandingan: %s vs %s\n", lagaBerikutnya->laga.tim1->nama, lagaBerikutnya->laga.tim2->nama);
                printf("Pilih pemenang (1 untuk %s, 2 untuk %s): ", lagaBerikutnya->laga.tim1->nama, lagaBerikutnya->laga.tim2->nama);
                int pemenang;
                scanf("%d", &pemenang);
                getchar();
                inputHasilLaga(lagaBerikutnya, pemenang, tumpukanUndo); // Input hasilnya
                break;
            }
            case 7: { // Pilihan: Undo
                batalkanHasilTerakhir(tumpukanUndo); // Panggil fungsi undo
                break;
            }
            case 8: { // Pilihan: Cari Tim
                 char namaCari[NAMA_MAKS];
                 printf("Masukkan nama tim yang ingin dicari: ");
                 fgets(namaCari, NAMA_MAKS, stdin);
                 namaCari[strcspn(namaCari, "\n")] = 0;
                 Tim *hasil = cariTim(daftarTim, jumlahTim, namaCari); // Panggil fungsi searching
                 if(hasil != NULL){
                     printf("Tim Ditemukan!\n");
                     printf("ID: %d, Nama: %s, Poin: %d\n", hasil->id, hasil->nama, hasil->poinKualifikasi);
                     tampilkanPemain(hasil);
                 } else {
                     printf("Tim dengan nama '%s' tidak ditemukan.\n", namaCari);
                 }
                 break;
            }
            case 0: // Pilihan: Keluar
                printf("Membersihkan memori dan keluar...\n");
                break;
            default: // Jika pilihan tidak ada di menu
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }

    } while (pilihan != 0); // Ulangi menu selama pengguna tidak memilih 0

    // Membersihkan semua memori yang dialokasikan sebelum program berakhir
    for (int i = 0; i < jumlahTim; i++) {
        bebaskanPemain(daftarTim[i].kepalaPemain);
    }
    bebaskanBagan(akarTurnamen);
    bebaskanTumpukan(tumpukanUndo);
    
    printf("Program selesai.\n");

    return 0; // Akhiri program
}