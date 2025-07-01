#include "tournament.h"

#define NUM_TEAMS 8             // Makro untuk jumlah tim.
#define INPUT_BUFFER_SIZE 100   // Makro untuk ukuran buffer input.

// Fungsi utama program.
int main() {
    srand(time(NULL)); // Inisialisasi seed untuk fungsi rand() menggunakan waktu saat ini.

    // 1. Inisialisasi data.
    Team* all_teams[NUM_TEAMS]; // Buat array yang berisi pointer ke struct Team.
    printf("--- Pendaftaran Tim ---\n");
    
    char* team_names[NUM_TEAMS] = {"Evos Legends", "RRQ Hoshi", "Onic Esports", "Bigetron Alpha", "Alter Ego", "Aura Fire", "Geek Fam", "Rebellion Zion"};
    for (int i = 0; i < NUM_TEAMS; i++) {
        // Alokasikan dan inisialisasi setiap tim dalam array.
        all_teams[i] = create_team(i + 1, team_names[i], rand() % 1000);
        add_player_to_team(all_teams[i], "Pemain5");
        add_player_to_team(all_teams[i], "Pemain4");
        add_player_to_team(all_teams[i], "Pemain3");
        add_player_to_team(all_teams[i], "Pemain2");
        add_player_to_team(all_teams[i], "Pemain1");
    }

    // 2. Lakukan sorting pada array tim.
    printf("\n--- Peringkat Tim Setelah diurutkan ---\n");
    qsort(all_teams, NUM_TEAMS, sizeof(Team*), compare_teams_by_seed); // Panggil fungsi qsort.
    for(int i = 0; i < NUM_TEAMS; i++) print_team(all_teams[i]); // Cetak hasil sort.

    // 3. Masukkan tim ke dalam antrean.
    Queue* registration_queue = create_queue();
    for (int i = 0; i < NUM_TEAMS; i++) {
        enqueue(registration_queue, all_teams[i]);
    }
    
    // 4. Buat struktur bracket dari antrean.
    printf("\nMembuat bracket turnamen...\n");
    Match* bracket_root = create_bracket(registration_queue);
    
    Stack* undo_stack = create_stack(); // Inisialisasi stack untuk fitur 'Undo'.

    int choice = -1; // Variabel untuk menyimpan pilihan menu.
    char input_buffer[INPUT_BUFFER_SIZE]; // Buffer untuk input pengguna yang aman.

    // Loop menu utama.
    do {
        printf("\n--- MENU MANAJEMEN TURNAMEN ---\n");
        printf("1. Tampilkan Bracket\n");
        printf("2. Input Hasil Pertandingan\n");
        printf("3. Undo Hasil Terakhir\n");
        printf("4. Cari Tim\n");
        printf("0. Keluar\n");
        printf("Pilihan: ");

        // Pola input aman.
        if (fgets(input_buffer, sizeof(input_buffer), stdin)) { // Baca satu baris input.
            if (sscanf(input_buffer, "%d", &choice) != 1) { // Coba parse integer dari buffer.
                choice = -1; // Jika gagal (input bukan angka), set pilihan ke invalid.
            }
        } else {
            choice = 0; // Jika terjadi error I/O (misal: EOF), keluar dari loop.
        }

        // Blok switch untuk menangani pilihan pengguna.
        switch (choice) {
            case 1: // Pilihan: Tampilkan Bracket.
                printf("\n--- BAGAN PERTANDINGAN ---\n\n");
                display_bracket_recursive(bracket_root, 0, 0);
                printf("\n---------------------------\n");
                break;
            case 2: { // Pilihan: Input Hasil.
                int match_id;
                printf("Masukkan ID pertandingan (M#): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin)) {
                    if (sscanf(input_buffer, "%d", &match_id) == 1) {
                        Match* match = find_match_by_id(bracket_root, match_id);
                        if (match && match->team1 && match->team2 && !match->winner) {
                            printf("Pilih pemenang untuk M%d:\n1. %s\n2. %s\n", match_id, match->team1->name, match->team2->name);
                            printf("Pilihan: ");
                            if (fgets(input_buffer, sizeof(input_buffer), stdin)) {
                                int winner_choice;
                                if (sscanf(input_buffer, "%d", &winner_choice) == 1 && (winner_choice == 1 || winner_choice == 2)) {
                                    update_match_winner(bracket_root, match, winner_choice, undo_stack);
                                } else {
                                    printf("Pilihan pemenang tidak valid.\n");
                                }
                            }
                        } else {
                            printf("ID Match tidak ditemukan, tidak valid, atau sudah selesai.\n");
                        }
                    } else {
                         printf("Input ID tidak valid.\n");
                    }
                }
                break;
            }
            case 3: // Pilihan: Undo.
                if (!is_stack_empty(undo_stack)) {
                    Match* last_match = pop(undo_stack); // Ambil pertandingan terakhir dari stack.
                    printf("Membatalkan hasil untuk M%d (%s vs %s). Pemenang sebelumnya: %s\n", 
                           last_match->match_id, last_match->team1->name, last_match->team2->name, last_match->winner->name);
                    last_match->winner = NULL; // Reset pointer pemenang ke NULL.
                    printf("Hasil dibatalkan. Silakan input ulang.\n");
                } else {
                    printf("Tidak ada aksi untuk di-undo.\n");
                }
                break;
            case 4: { // Pilihan: Cari Tim.
                char search_name[INPUT_BUFFER_SIZE];
                printf("Masukkan nama tim yang dicari: ");
                if (fgets(search_name, sizeof(search_name), stdin)) {
                    search_name[strcspn(search_name, "\n")] = 0; // Hapus karakter newline dari akhir string.
                    Team* found = find_team_by_name(all_teams, NUM_TEAMS, search_name);
                    if (found) {
                        printf("Tim ditemukan!\n");
                        print_team(found);
                    } else {
                        printf("Tim '%s' tidak ditemukan.\n", search_name);
                    }
                }
                break;
            }
            case 0: // Pilihan: Keluar.
                printf("Keluar dari program...\n");
                break;
            default: // Pilihan tidak valid.
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                break;
        }
    } while (choice != 0); // Kondisi untuk mengulang loop menu.
    
    printf("Membersihkan memori...\n");
    // Tahap pembersihan memori.
    free_bracket(bracket_root); // Bebaskan seluruh tree bracket.
    for (int i = 0; i < NUM_TEAMS; i++) {
        free_team(all_teams[i]); // Bebaskan setiap tim dalam array.
    }
    free_queue(registration_queue); // Bebaskan struct kontrol Queue.
    free_stack(undo_stack);         // Bebaskan struct kontrol Stack.

    printf("Program selesai.\n");
    return 0; // Menandakan eksekusi program berhasil.
}