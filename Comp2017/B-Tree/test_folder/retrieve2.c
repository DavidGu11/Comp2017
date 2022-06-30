#include "btreestore.h"
#include "../btreestore.c"
#include <stdio.h>
int main(){
    void* helper = init_store(3,6);
    uint64_t plaintext[5],plaintext1[7];
    strcpy((char *) plaintext, "Hello");
    strcpy((char *) plaintext1, "Helsslo");
    uint64_t nonce = 111;
    // uint32_t key = 2;
    // uint32_t key1 = 3;
    // uint32_t key2 = 4;
    uint32_t encryption_key[4] = {123,456,25645962,0};
    uint32_t encryption_skey[4] = {232123,4567,256462,233};
    uint32_t encryption_sskey[4] = {2322123,4567,256462,233};
    btree_insert(2, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    btree_insert(4, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
   
    btree_insert(6, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    btree_insert(8, plaintext, sizeof(plaintext), encryption_skey, nonce, helper);
    btree_insert(10, plaintext1, sizeof(plaintext), encryption_sskey, nonce, helper);

    
    struct info found;
    struct helping * hel = (struct helping*) helper;
    struct linked * root = hel->tree;
    btree_retrieve(10,&found,helper);

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
    
    for(int i = 0; i < 4;i++){
        printf("encryption key: %d\n",found.key[i]);
    }
    printf("size: %d",found.size);
    // free(found);
    close_store(helper);
    

    return 0;

}