#include <stdio.h>
#include <stdlib.h> 

// Struktur node AVL
typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

// Utility: maksimum
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Tinggi node
int height(Node* n) {
    return n ? n->height : 0;
}

// Buat node baru
Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    
    // --- PENANGANAN KEGAGALAN ALOKASI MEMORI ---
    // Periksa apakah alokasi memori berhasil. Jika gagal, program berhenti.
    if (node == NULL) {
        perror("Gagal mengalokasikan memori untuk node baru");
        exit(EXIT_FAILURE);
    }
    
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

// Rotasi kanan
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

// Rotasi kiri
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

// Balance factor
int getBalance(Node* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

// Insert key ke AVL, dengan rotasi jika perlu
Node* insert(Node* node, int key) {
    if (!node) return newNode(key);
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;  // duplikat diabaikan

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // LL
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
    // RR
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
    // LR
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // RL
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

// Inorder traversal
void inorder(Node* root) {
    if (root) {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

// --- MANAJEMEN MEMORI ---
// Fungsi untuk membebaskan semua memori yang dialokasikan untuk tree
void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }
    // Hapus subtree kiri dan kanan terlebih dahulu (post-order)
    freeTree(root->left);
    freeTree(root->right);
    // Baru hapus node root-nya
    free(root);
}


int main() {
    Node* root = NULL;
    int n, key;
    
    printf("Masukkan jumlah elemen: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Input jumlah elemen tidak valid. Program berhenti.\n");
        return 1; // Keluar dengan error jika input bukan angka atau <= 0
    }
    
    printf("Masukkan %d nilai (dipisah spasi):\n", n);
    for (int i = 0; i < n; i++) {
        // --- PEMERIKSAAN INPUT DI DALAM LOOP ---
        // Validasi setiap angka yang dimasukkan pengguna
        if (scanf("%d", &key) != 1) {
            printf("\nInput ke-%d tidak valid. Hanya angka yang diperbolehkan.\n", i + 1);
            printf("Program berhenti untuk mencegah error.\n");
            // Jika terjadi error, pastikan memori yang sudah ada dibebaskan
            freeTree(root);
            return 1; // Keluar dengan kode error
        }
        root = insert(root, key);
    }
    
    printf("Inorder traversal AVL tree (terurut):\n");
    inorder(root);
    printf("\n");
    
    // --- MANAJEMEN MEMORI ---
    // Bebaskan semua memori yang dialokasikan sebelum program berakhir
    freeTree(root);
    
    return 0;
}