#include "btreestore.h"
#include "../btreestore.c"
#include <stdio.h>
int main(){
    void* helper = init_store(300,6);
    uint64_t plaintext[5];
    strcpy((char *) plaintext, "Hello");
    uint64_t nonce = 111;
    // uint32_t key = 2;
    // uint32_t key1 = 3;
    // uint32_t key2 = 4;
    uint32_t encryption_key[4] = {123,456,789,0};
    for(int i = 0;i < 299;i++){
        btree_insert(i, plaintext, sizeof(plaintext), encryption_key, nonce, helper);
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
    close_store(helper);
    

    return 0;

}