#include "tournament.h"

// Fungsi untuk membuat dan menginisialisasi struct Queue.
Queue* create_queue() {
    Queue* q = (Queue*)malloc(sizeof(Queue)); // Alokasikan memori untuk struct kontrol Queue.
    q->front = NULL; // Inisialisasi pointer 'front' ke NULL.
    q->rear = NULL;  // Inisialisasi pointer 'rear' ke NULL.
    return q;
}

// Fungsi untuk menambahkan elemen (enqueue) ke akhir antrean.
void enqueue(Queue* q, Team* t) {
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode)); // Alokasikan memori untuk node baru.
    new_node->team = t;   // Tetapkan data pada node baru.
    new_node->next = NULL; // Node baru akan menjadi yang terakhir, 'next' harus NULL.

    if (q->rear == NULL) { // Kondisi: jika antrean saat ini kosong.
        q->front = new_node; // Node baru menjadi 'front'.
        q->rear = new_node;  // Node baru juga menjadi 'rear'.
        return;
    }
    
    q->rear->next = new_node; // Tautkan 'next' dari node 'rear' lama ke node baru.
    q->rear = new_node;       // Perbarui pointer 'rear' untuk menunjuk ke node baru.
}

// Fungsi untuk mengambil elemen (dequeue) dari depan antrean.
Team* dequeue(Queue* q) {
    if (is_queue_empty(q)) return NULL; // Jika antrean kosong, kembalikan NULL.

    QueueNode* temp = q->front; // Simpan pointer ke node 'front' saat ini.
    Team* team = temp->team;    // Ambil data dari node tersebut.

    q->front = q->front->next; // Majukan pointer 'front' ke node selanjutnya.

    if (q->front == NULL) { // Kondisi: jika antrean menjadi kosong setelah dequeue.
        q->rear = NULL; // Pastikan 'rear' juga diatur ke NULL.
    }

    free(temp); // Bebaskan memori dari node yang telah diambil.
    return team; // Kembalikan data yang telah diambil.
}

// Fungsi untuk memeriksa apakah antrean kosong.
int is_queue_empty(const Queue* q) {
    return q->front == NULL; // Antrean kosong jika pointer 'front' bernilai NULL.
}

// Fungsi untuk membebaskan seluruh memori yang digunakan oleh antrean.
void free_queue(Queue* q) {
    while (!is_queue_empty(q)) { // Lakukan dequeue pada semua elemen yang tersisa.
        dequeue(q);
    }
    free(q); // Bebaskan memori dari struct kontrol Queue itu sendiri.
}