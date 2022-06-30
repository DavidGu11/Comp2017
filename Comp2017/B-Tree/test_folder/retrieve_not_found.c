#include "btreestore.h"
#include "../btreestore.c"
#include <stdio.h>
int main(){
    void* helper = init_store(3,6);
    uint64_t plaintext[5];
    strcpy((char *) plaintext, "Hello");
    uint64_t nonce = 111;
    // uint32_t key = 2;
    // uint32_t key1 = 3;
    // uint32_t key2 = 4;
    uint32_t encryption_key[4] = {123,456,25645962,0};
    uint32_t encryption_skey[4] = {232123,4567,256462,233};
    btree_insert(1, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    btree_insert(3, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
   
    btree_insert(5, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
    btree_insert(7, plaintext, sizeof(plaintext), encryption_skey, nonce, helper);
    btree_insert(9, plaintext, sizeof(plaintext), encryption_key, nonce, helper);

    
    struct info found;
    struct helping * hel = (struct helping*) helper;
    struct linked * root = hel->tree;

    int gg = btree_retrieve(11,&found,helper);
    if(gg == 1){
        printf("cannot find the key");
        return 0;
    }
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