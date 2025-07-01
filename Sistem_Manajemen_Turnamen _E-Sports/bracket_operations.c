#include "tournament.h"

// Variabel global dengan visibilitas terbatas pada file ini ('static').
static int match_counter = 0;

// Fungsi untuk membuat satu node Match baru.
Match* create_match_node() {
    Match* node = (Match*)malloc(sizeof(Match)); // Alokasikan memori untuk struct Match.
    node->match_id = ++match_counter; // Beri ID unik lalu tingkatkan counter.
    // Inisialisasi semua pointer member ke NULL.
    node->team1 = NULL;
    node->team2 = NULL;
    node->winner = NULL;
    node->child1 = NULL;
    node->child2 = NULL;
    return node;
}

// Fungsi untuk membangun seluruh struktur tree bracket dari antrean tim.
Match* create_bracket(Queue* team_queue) {
    Queue* match_queue = create_queue(); // Buat antrean sementara untuk menampung node Match.
    
    // Tahap 1: Membuat node daun (leaf) untuk pertandingan ronde pertama.
    while (!is_queue_empty(team_queue)) {
        Match* match_node = create_match_node(); // Buat node pertandingan.
        match_node->team1 = dequeue(team_queue); // Ambil tim pertama dari antrean pendaftaran.
        if (!is_queue_empty(team_queue)) {
            match_node->team2 = dequeue(team_queue); // Jika masih ada tim, ambil tim kedua.
        } else {
            // Jika jumlah tim ganjil, tim terakhir mendapat "bye" dan otomatis menang.
            match_node->team2 = NULL;
            match_node->winner = match_node->team1;
        }
        enqueue(match_queue, (Team*)match_node); // Masukkan node Match ke antrean pertandingan.
    }

    // Tahap 2: Membangun tree ke atas dengan menggabungkan pertandingan.
    while (match_queue->front->next != NULL) { // Loop selama ada lebih dari satu pertandingan di antrean.
        Match* child1 = (Match*)dequeue(match_queue); // Ambil pertandingan pertama sebagai anak kiri.
        Match* child2 = (Match*)dequeue(match_queue); // Ambil pertandingan kedua sebagai anak kanan.

        Match* parent_match = create_match_node(); // Buat node induk untuk ronde selanjutnya.
        parent_match->child1 = child1; // Tetapkan anak kiri.
        parent_match->child2 = child2; // Tetapkan anak kanan.
        
        enqueue(match_queue, (Team*)parent_match); // Masukkan node induk kembali ke antrean.
    }

    Match* root = (Match*)dequeue(match_queue); // Satu-satunya node yang tersisa adalah root dari tree.
    free_queue(match_queue); // Hapus antrean sementara yang sudah tidak terpakai.
    return root;
}

// Fungsi rekursif untuk menampilkan bracket (traversal reverse-in-order).
void display_bracket_recursive(Match* root, int level, int is_right_side) {
    if (root == NULL) return; // Basis rekursi: berhenti jika node tidak ada.

    display_bracket_recursive(root->child2, level + 1, 1); // 1. Panggil rekursif untuk sub-tree kanan.

    // 2. Proses node saat ini.
    for (int i = 0; i < level; i++) printf("        "); // Cetak spasi untuk indentasi.
    if (level > 0) {
       printf(is_right_side ? ".------ " : "`------ "); // Cetak garis penghubung.
    }
    printf("M%d: ", root->match_id); // Cetak ID pertandingan.
    // Cetak status pertandingan.
    if (root->winner) {
        printf("Pemenang: %s\n", root->winner->name);
    } else if (root->team1 && root->team2) {
        printf("%s vs %s\n", root->team1->name, root->team2->name);
    } else if (root->team1 && !root->team2){
         printf("%s vs (BYE)\n", root->team1->name);
    } else {
        printf("TBD vs TBD\n");
    }

    display_bracket_recursive(root->child1, level + 1, 0); // 3. Panggil rekursif untuk sub-tree kiri.
}

// Fungsi rekursif untuk menyebarkan pemenang ke node induk.
void propagate_winner(Match* root, Match* updated_match) {
    if (!root || !updated_match->winner) return; // Basis rekursi.

    // Cari node induk dari 'updated_match'.
    if (root->child1 == updated_match || root->child2 == updated_match) {
        if (root->child1 && root->child1->winner && root->child2 && root->child2->winner) {
             root->team1 = root->child1->winner; // Isi slot tim 1 di induk.
             root->team2 = root->child2->winner; // Isi slot tim 2 di induk.
        }
        return; // Hentikan pencarian.
    }
    propagate_winner(root->child1, updated_match); // Cari di sub-tree kiri.
    propagate_winner(root->child2, updated_match); // Cari di sub-tree kanan.
}

// Fungsi untuk mengatur pemenang sebuah pertandingan.
void update_match_winner(Match* root, Match* match, int winner_choice, Stack* undo_stack) {
    if (!match || match->winner || (!match->team1 || !match->team2)) {
        printf("Pertandingan tidak valid atau sudah ada pemenang.\n");
        return;
    }
    
    Team* chosen_winner = (winner_choice == 1) ? match->team1 : match->team2;
    
    push(undo_stack, match); // Simpan pointer pertandingan ke stack untuk fitur 'Undo'.
    
    match->winner = chosen_winner; // Tetapkan pemenang pada node match.
    printf("Pemenang Match M%d adalah %s.\n", match->match_id, match->winner->name);

    propagate_winner(root, match); // Panggil fungsi untuk mengisi data di ronde selanjutnya.
}


// Fungsi rekursif untuk mencari node pertandingan berdasarkan ID.
Match* find_match_by_id(Match* root, int id) {
    if (!root) return NULL; // Basis rekursi: node tidak ada, pencarian gagal.
    if (root->match_id == id) return root; // Basis rekursi: node ditemukan.
    
    Match* found = find_match_by_id(root->child1, id); // Cari di sub-tree kiri.
    if (found) return found; // Jika ditemukan di kiri, langsung kembalikan hasilnya.
    
    return find_match_by_id(root->child2, id); // Jika tidak, lanjutkan pencarian di sub-tree kanan.
}

// Fungsi rekursif untuk membebaskan memori tree (traversal post-order).
void free_bracket(Match* root) {
    if (root == NULL) return;     // Basis rekursi.
    free_bracket(root->child1); // 1. Bebaskan seluruh sub-tree kiri.
    free_bracket(root->child2); // 2. Bebaskan seluruh sub-tree kanan.
    free(root);                 // 3. Bebaskan node induk saat ini.
}