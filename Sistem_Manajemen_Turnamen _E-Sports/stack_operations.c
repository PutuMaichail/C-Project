#include "tournament.h"

// Fungsi untuk membuat dan menginisialisasi struct Stack.
Stack* create_stack() {
    Stack* s = (Stack*)malloc(sizeof(Stack)); // Alokasikan memori untuk struct kontrol Stack.
    s->top = NULL; // Inisialisasi pointer 'top' ke NULL.
    return s;
}

// Fungsi untuk menambahkan elemen (push) ke puncak tumpukan.
void push(Stack* s, Match* m) {
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode)); // Alokasikan memori untuk node baru.
    new_node->match = m;     // Tetapkan data pada node baru.
    new_node->next = s->top; // Pointer 'next' dari node baru menunjuk ke 'top' yang lama.
    s->top = new_node;       // Perbarui 'top' untuk menunjuk ke node baru.
}

// Fungsi untuk mengambil elemen (pop) dari puncak tumpukan.
Match* pop(Stack* s) {
    if (is_stack_empty(s)) return NULL; // Jika tumpukan kosong, kembalikan NULL.

    StackNode* temp = s->top; // Simpan pointer ke node 'top' saat ini.
    Match* match = temp->match; // Ambil data dari node tersebut.

    s->top = s->top->next; // Mundurkan pointer 'top' ke node di bawahnya.

    free(temp); // Bebaskan memori dari node yang telah diambil.
    return match; // Kembalikan data yang telah diambil.
}

// Fungsi untuk memeriksa apakah tumpukan kosong.
int is_stack_empty(const Stack* s) {
    return s->top == NULL; // Tumpukan kosong jika pointer 'top' bernilai NULL.
}

// Fungsi untuk membebaskan seluruh memori yang digunakan oleh tumpukan.
void free_stack(Stack* s) {
    while (!is_stack_empty(s)) { // Lakukan pop pada semua elemen yang tersisa.
        pop(s);
    }
    free(s); // Bebaskan memori dari struct kontrol Stack itu sendiri.
}