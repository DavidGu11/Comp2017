#include "btreestore.h"
#include "btreestore.c"
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
    btree_insert(15, plaintext, 40, encryption_key, nonce, helper);


    struct node * list = NULL;
    void* out = malloc(100000);
    
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

    // btree_decrypt(2,out,helper);
    free(out);

    close_store(helper);
    

    return 0;

}