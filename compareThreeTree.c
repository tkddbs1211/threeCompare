#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_INSERTIONS 100
#define NUM_SEARCHES 50
#define MAX_VAL 1000

typedef struct Node {
    int key;
    int height;
    struct Node* left;
    struct Node* right;
} Node;

Node* create_node(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) return NULL;
    node->key = key;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int max_val(int a, int b) {
    return (a > b) ? a : b;
}

int get_height(Node* node) {
    if (node == NULL) return 0;
    return node->height;
}

int get_balance(Node* node) {
    if (node == NULL) return 0;
    return get_height(node->left) - get_height(node->right);
}

void update_height(Node* node) {
    if (node != NULL) {
        node->height = 1 + max_val(get_height(node->left), get_height(node->right));
    }
}

Node* right_rotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    update_height(y);
    update_height(x);

    return x;
}

Node* left_rotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    update_height(x);
    update_height(y);

    return y;
}

int insert_array(int arr[], int* size, int key, long long* comp_cnt) {
    for (int i = 0; i < *size; i++) {
        (*comp_cnt)++;
        if (arr[i] == key) {
            return 0; 
        }
    }
    arr[*size] = key;
    (*size)++;
    return 1;
}

Node* insert_bst(Node* node, int key, long long* comp_cnt, int* is_inserted) {
    if (node == NULL) {
        *is_inserted = 1;
        return create_node(key);
    }

    (*comp_cnt)++;
    if (key == node->key) {
        *is_inserted = 0; 
        return node;
    }
    else if (key < node->key) {
        node->left = insert_bst(node->left, key, comp_cnt, is_inserted);
    }
    else {
        node->right = insert_bst(node->right, key, comp_cnt, is_inserted);
    }

    update_height(node);
    return node;
}

Node* insert_avl(Node* node, int key, long long* comp_cnt, int* is_inserted) {
    if (node == NULL) {
        *is_inserted = 1;
        return create_node(key);
    }

    (*comp_cnt)++;
    if (key == node->key) {
        *is_inserted = 0; 
        return node;
    }
    else if (key < node->key) {
        node->left = insert_avl(node->left, key, comp_cnt, is_inserted);
    }
    else {
        node->right = insert_avl(node->right, key, comp_cnt, is_inserted);
    }

    update_height(node);

    int balance = get_balance(node);

    if (balance > 1 && key < node->left->key) {
        return right_rotate(node);
    }
    if (balance < -1 && key > node->right->key) {
        return left_rotate(node);
    }
    if (balance > 1 && key > node->left->key) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    if (balance < -1 && key < node->right->key) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

int search_array(int arr[], int size, int key, int* comp_cnt) {
    *comp_cnt = 0;
    for (int i = 0; i < size; i++) {
        (*comp_cnt)++;
        if (arr[i] == key) {
            return 1;
        }
    }
    return 0;
}

int search_tree(Node* root, int key, int* comp_cnt) {
    *comp_cnt = 0;
    Node* curr = root;

    while (curr != NULL) {
        (*comp_cnt)++;
        if (key == curr->key) {
            return 1;
        }
        else if (key < curr->key) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }
    return 0;
}

void free_tree(Node* node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

int main() {
    srand((unsigned int)time(NULL));

    int insert_data[NUM_INSERTIONS];
    int search_data[NUM_SEARCHES];

    int array[NUM_INSERTIONS];
    int array_size = 0;
    Node* bst_root = NULL;
    Node* avl_root = NULL;

    long long array_insert_comp = 0;
    long long bst_insert_comp = 0;
    long long avl_insert_comp = 0;

    int duplicates = 0;

    printf("=== 1. Generating Insert Data (100 integers) ===\n");
    for (int i = 0; i < NUM_INSERTIONS; i++) {
        insert_data[i] = rand() % (MAX_VAL + 1);
    }

    for (int i = 0; i < NUM_INSERTIONS; i++) {
        int val = insert_data[i];
        int inserted = 0;

        insert_array(array, &array_size, val, &array_insert_comp);
        bst_root = insert_bst(bst_root, val, &bst_insert_comp, &inserted);
        avl_root = insert_avl(avl_root, val, &avl_insert_comp, &inserted);

        if (!inserted) {
            duplicates++;
        }
    }

    printf("\nStored values : %d (Duplicates skipped: %d)\n\n", array_size, duplicates);
    printf("Construction\n");
    printf("Array comparisons : %lld\n", array_insert_comp);
    printf("BST comparisons   : %lld\n", bst_insert_comp);
    printf("AVL comparisons   : %lld\n\n", avl_insert_comp);

    printf("Structure\n");
    printf("Array length : %d\n", array_size);
    printf("BST height   : %d\n", get_height(bst_root));
    printf("AVL height   : %d\n\n", get_height(avl_root));

    printf("=== 2. Generating Search Data (50 integers) ===\n");
    for (int i = 0; i < NUM_SEARCHES; i++) {
        search_data[i] = rand() % (MAX_VAL + 1);
    }

    long long seq_total_comp = 0;
    long long bst_total_comp = 0;
    long long avl_total_comp = 0;

    printf("\n--- Detailed Search Results ---\n");
    for (int i = 0; i < NUM_SEARCHES; i++) {
        int target = search_data[i];
        int seq_comp = 0, bst_comp = 0, avl_comp = 0;

        int seq_res = search_array(array, array_size, target, &seq_comp);
        int bst_res = search_tree(bst_root, target, &bst_comp);
        int avl_res = search_tree(avl_root, target, &avl_comp);

        seq_total_comp += seq_comp;
        bst_total_comp += bst_comp;
        avl_total_comp += avl_comp;

        printf("Search Key : %d\n", target);
        printf("  Sequential Search -> Result: %-9s | Comparisons: %d\n", seq_res ? "Found" : "Not Found", seq_comp);
        printf("  BST Search        -> Result: %-9s | Comparisons: %d\n", bst_res ? "Found" : "Not Found", bst_comp);
        printf("  AVL Search        -> Result: %-9s | Comparisons: %d\n", avl_res ? "Found" : "Not Found", avl_comp);
        printf("--------------------------------------------------\n");
    }

    printf("\nSearches : %d\n\n", NUM_SEARCHES);
    printf("Sequential Search\n");
    printf("Total comparisons   : %lld\n", seq_total_comp);
    printf("Average comparisons : %.2f\n\n", (double)seq_total_comp / NUM_SEARCHES);

    printf("BST Search\n");
    printf("Total comparisons   : %lld\n", bst_total_comp);
    printf("Average comparisons : %.2f\n\n", (double)bst_total_comp / NUM_SEARCHES);

    printf("AVL Search\n");
    printf("Total comparisons   : %lld\n", avl_total_comp);
    printf("Average comparisons : %.2f\n", (double)avl_total_comp / NUM_SEARCHES);

    free_tree(bst_root);
    free_tree(avl_root);

    return 0;
}