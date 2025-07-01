#ifndef TOURNAMENT_H // Mencegah duplikasi definisi jika header di-include berkali-kali.
#define TOURNAMENT_H // Mendefinisikan makro header guard.

// --- Pustaka Standar ---
#include <stdio.h>   // Deklarasi fungsi-fungsi Standard Input/Output.
#include <stdlib.h>  // Deklarasi fungsi-fungsi utilitas umum (manajemen memori, konversi, dll).
#include <string.h>  // Deklarasi fungsi-fungsi untuk manipulasi string.
#include <time.h>    // Deklarasi fungsi-fungsi untuk manipulasi waktu.

// --- Definisi Tipe Data Abstrak (ADT) ---

// Definisi struct untuk data pemain (Node Linked List).
typedef struct Player {
    char* name;           // Pointer ke string nama (alokasi dinamis).
    struct Player* next;  // Pointer ke struct Player selanjutnya.
} Player;

// Definisi struct untuk data tim.
typedef struct Team {
    int id;               // Nomor identifikasi unik.
    char* name;           // Pointer ke string nama (alokasi dinamis).
    int seed_points;      // Nilai untuk pengurutan/seeding.
    Player* players;      // Pointer ke node pertama (head) dari linked list pemain.
} Team;

// Definisi struct untuk node pertandingan (Node Binary Tree).
typedef struct Match {
    int match_id;         // Nomor identifikasi unik untuk pertandingan.
    Team* team1;          // Pointer ke struct Team pertama.
    Team* team2;          // Pointer ke struct Team kedua.
    Team* winner;         // Pointer ke struct Team pemenang.
    struct Match* child1; // Pointer ke node anak kiri (pertandingan ronde sebelumnya).
    struct Match* child2; // Pointer ke node anak kanan (pertandingan ronde sebelumnya).
} Match;

// Definisi struct untuk node dari Antrean (Queue).
typedef struct QueueNode {
    Team* team;             // Pointer data (ke struct Team).
    struct QueueNode* next; // Pointer ke node selanjutnya.
} QueueNode;

// Definisi struct untuk kontrol Antrean (Queue).
typedef struct {
    QueueNode *front, *rear; // Pointer ke node depan dan belakang antrean.
} Queue;

// Definisi struct untuk node dari Tumpukan (Stack).
typedef struct StackNode {
    Match* match;           // Pointer data (ke struct Match).
    struct StackNode* next; // Pointer ke node di bawahnya.
} StackNode;

// Definisi struct untuk kontrol Tumpukan (Stack).
typedef struct {
    StackNode* top; // Pointer ke node teratas tumpukan.
} Stack;


// --- Prototipe Fungsi ---

// -- dari team_management.c --
Team* create_team(int id, const char* name, int seed_points);      // Mengalokasikan & menginisialisasi sebuah struct Team.
void add_player_to_team(Team* team, const char* player_name);      // Menambahkan sebuah node Player ke linked list dalam struct Team.
void print_team(const Team* team);                                  // Mencetak informasi detail sebuah tim ke konsol.
void free_team(Team* team);                                         // Membebaskan semua memori yang dialokasikan untuk sebuah Team (termasuk pemain).
int compare_teams_by_seed(const void* a, const void* b);            // Fungsi pembanding untuk digunakan oleh qsort dalam pengurutan tim.
Team* find_team_by_name(Team** teams, int count, const char* name); // Mencari sebuah tim berdasarkan nama dalam sebuah array pointer.

// -- dari queue_operations.c --
Queue* create_queue();      // Mengalokasikan & menginisialisasi sebuah struct Queue.
void enqueue(Queue* q, Team* t); // Menambahkan sebuah elemen ke akhir antrean (queue).
Team* dequeue(Queue* q);    // Mengambil & menghapus sebuah elemen dari depan antrean (queue).
int is_queue_empty(const Queue* q); // Memeriksa apakah antrean (queue) kosong.
void free_queue(Queue* q);  // Membebaskan semua memori yang digunakan oleh antrean (queue).

// -- dari stack_operations.c --
Stack* create_stack();      // Mengalokasikan & menginisialisasi sebuah struct Stack.
void push(Stack* s, Match* m); // Menambahkan sebuah elemen ke puncak tumpukan (stack).
Match* pop(Stack* s);       // Mengambil & menghapus sebuah elemen dari puncak tumpukan (stack).
int is_stack_empty(const Stack* s); // Memeriksa apakah tumpukan (stack) kosong.
void free_stack(Stack* s);  // Membebaskan semua memori yang digunakan oleh tumpukan (stack).

// -- dari bracket_operations.c --
Match* create_bracket(Queue* team_queue);                                 // Membangun struktur tree bracket dari antrean tim.
void display_bracket_recursive(Match* root, int level, int is_right_side); // Menampilkan struktur tree bracket secara visual di konsol.
void update_match_winner(Match* root, Match* match, int winner_choice, Stack* undo_stack); // Menetapkan pemenang untuk sebuah pertandingan dan menyimpannya untuk undo.
void free_bracket(Match* root);                                         // Membebaskan semua memori yang dialokasikan untuk tree bracket.
Match* find_match_by_id(Match* root, int id);                           // Mencari sebuah node pertandingan berdasarkan ID-nya di dalam tree.

#endif // Akhir dari blok header guard TOURNAMENT_H.