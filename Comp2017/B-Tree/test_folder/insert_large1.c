#include "btreestore.h"
#include "../btreestore.c"
#include <stdio.h>
int main(){
    void* helper = init_store(7,6);
    uint64_t plaintext[5];
    strcpy((char *) plaintext, "Hello");
    uint64_t nonce = 111;
    // uint32_t key = 2;
    // uint32_t key1 = 3;
    // uint32_t key2 = 4;
    uint32_t encryption_key[4] = {123,456,789,0};
    btree_insert(42, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    btree_insert(9, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
   
    btree_insert(3, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    btree_insert(99, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    btree_insert(800,plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    btree_insert(402, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    btree_insert(2, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    
    btree_insert(888, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    btree_insert(999, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    btree_insert(22,plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    btree_insert(39, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    btree_insert(28, plaintext, sizeof(plaintext), encryption_key, nonce, helper);

    struct node * list = NULL;
    
    int g = btree_export(helper,&list);
    
    for(int i = 0; i < g; i++){
        for(int j = 0; j < list[i].num_keys;j++){
            printf("node: %d keys %d\n", i, list[i].keys[j]);
        }
    }

    for(int i = 0; i < g;i++){
        free(list[i].keys);
    }
    free(list);
    close_store(helper);
    

    return 0;
}