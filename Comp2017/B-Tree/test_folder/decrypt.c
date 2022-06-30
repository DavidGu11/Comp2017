#include "btreestore.h"
#include "../btreestore.c"
#include <stdio.h>
int main(){
    void* helper = init_store(3,6);
    uint64_t plaintext[4];
    strcpy((char *) plaintext, "Hell");
    uint64_t nonce = 111;
    // uint32_t key = 2;
    // uint32_t key1 = 3;
    // uint32_t key2 = 4;
    void* out;
    uint32_t encryption_key[4] = {123,456,256,0};
    btree_insert(1, plaintext, 4, encryption_key, nonce, helper);
 


    
 
    // free(found);
    int b = btree_decrypt(1,out,helper);
    printf("decryption value: %s\n",(char*)out);
    close_store(helper);
    

    return 0;

}