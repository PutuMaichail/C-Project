#include "tournament.h"

// Fungsi untuk mengalokasikan dan menginisialisasi struct Team.
Team* create_team(int id, const char* name, int seed_points) {
    Team* new_team = (Team*)malloc(sizeof(Team)); // Alokasikan memori seukuran struct Team.
    if (!new_team) { // Periksa kegagalan alokasi memori.
        perror("Gagal alokasi memori untuk tim");
        exit(EXIT_FAILURE); // Hentikan program jika memori tidak cukup.
    }
    // Inisialisasi nilai-nilai member struct.
    new_team->id = id;
    new_team->seed_points = seed_points;
    new_team->players = NULL; // Head dari list pemain diawali dengan NULL.

    // Alokasikan memori untuk string nama.
    new_team->name = malloc(strlen(name) + 1); // Alokasikan memori sesuai panjang string + 1 untuk '\0'.
    if (new_team->name) { // Periksa keberhasilan alokasi.
        strcpy(new_team->name, name); // Salin konten string nama ke memori yang baru dialokasikan.
    }

    return new_team; // Kembalikan pointer ke struct yang baru dibuat.
}

// Fungsi untuk menambahkan node pemain ke linked list sebuah tim.
void add_player_to_team(Team* team, const char* player_name) {
    Player* new_player = (Player*)malloc(sizeof(Player)); // Alokasikan memori untuk struct Player baru.
     if (!new_player) { // Periksa kegagalan alokasi.
        perror("Gagal alokasi memori untuk pemain");
        return; // Keluar dari fungsi jika gagal.
    }
    
    // Alokasikan memori untuk string nama pemain.
    new_player->name = malloc(strlen(player_name) + 1);
    if (new_player->name) { // Periksa keberhasilan alokasi.
        strcpy(new_player->name, player_name); // Salin konten string.
    }
    
    // Sisipkan node baru di kepala (head) dari linked list.
    new_player->next = team->players; // Pointer 'next' dari node baru menunjuk ke head lama.
    team->players = new_player;       // Pointer 'head' dari tim kini menunjuk ke node baru.
}

// Fungsi untuk mencetak detail tim ke konsol.
void print_team(const Team* team) {
    if (!team) return; // Pemeriksaan keamanan, jangan proses jika pointer NULL.
    printf("  ID: %d, Nama: %s, Seed: %d, Pemain: ", team->id, team->name, team->seed_points);
    Player* current = team->players; // Inisialisasi pointer iterasi ke head list pemain.
    if (!current) { // Jika tidak ada pemain.
        printf("(Belum ada)");
    }
    while (current) { // Lakukan iterasi selama pointer 'current' tidak NULL.
        printf("%s; ", current->name);
        current = current->next; // Pindahkan pointer iterasi ke node selanjutnya.
    }
    printf("\n");
}

// Fungsi untuk membebaskan semua memori yang dialokasikan untuk sebuah tim.
void free_team(Team* team) {
    if (!team) return; // Jangan proses jika pointer NULL.

    Player* current = team->players; // Inisialisasi pointer iterasi.
    while (current) { // Lakukan iterasi selama list belum berakhir.
        Player* to_delete = current; // Simpan pointer node saat ini.
        current = current->next;     // Majukan pointer iterasi ke node selanjutnya.
        free(to_delete->name);       // 1. Bebaskan memori string nama pemain.
        free(to_delete);             // 2. Bebaskan memori struct Player.
    }
    
    free(team->name); // 3. Bebaskan memori string nama tim.
    free(team);       // 4. Bebaskan memori struct Team.
}

// Fungsi pembanding yang dibutuhkan oleh `qsort` untuk mengurutkan tim.
int compare_teams_by_seed(const void* a, const void* b) {
    Team* teamA = *(Team**)a; // Cast dan dereferensi pointer ke elemen array pertama.
    Team* teamB = *(Team**)b; // Cast dan dereferensi pointer ke elemen array kedua.
    return (teamB->seed_points - teamA->seed_points); // Return > 0 jika B < A (untuk urutan menurun).
}

// Fungsi untuk mencari tim berdasarkan nama dalam sebuah array.
Team* find_team_by_name(Team** teams, int count, const char* name) {
    for (int i = 0; i < count; i++) { // Iterasi melalui array pointer tim.
        if (strcmp(teams[i]->name, name) == 0) { // Bandingkan string nama.
            return teams[i]; // Jika cocok, kembalikan pointer ke tim.
        }
    }
    return NULL; // Kembalikan NULL jika tidak ditemukan setelah seluruh array diperiksa.
}