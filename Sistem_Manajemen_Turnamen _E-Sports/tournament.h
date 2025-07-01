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
Team* create_team(int id, const char* name, int seed_points);
void add_player_to_team(Team* team, const char* player_name);
void print_team(const Team* team);
void free_team(Team* team);
int compare_teams_by_seed(const void* a, const void* b);
Team* find_team_by_name(Team** teams, int count, const char* name);

// -- dari queue_operations.c --
Queue* create_queue();
void enqueue(Queue* q, Team* t);
Team* dequeue(Queue* q);
int is_queue_empty(const Queue* q);
void free_queue(Queue* q);

// -- dari stack_operations.c --
Stack* create_stack();
void push(Stack* s, Match* m);
Match* pop(Stack* s);
int is_stack_empty(const Stack* s);
void free_stack(Stack* s);

// -- dari bracket_operations.c --
Match* create_bracket(Queue* team_queue);
void display_bracket_recursive(Match* root, int level, int is_right_side);
void update_match_winner(Match* root, Match* match, int winner_choice, Stack* undo_stack);
void free_bracket(Match* root);
Match* find_match_by_id(Match* root, int id);

#endif // Akhir dari blok header guard TOURNAMENT_H.