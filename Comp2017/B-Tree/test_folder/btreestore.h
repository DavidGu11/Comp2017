#ifndef BTREESTORE_H
#define BTREESTORE_H

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct info {
    uint32_t size;
    uint32_t key[4];
    uint64_t nonce;
    void * data;
};

struct node {
    uint16_t num_keys;
    uint32_t * keys;
};

struct helping{
    int num_nodes;
    uint16_t branch;
    struct linked * tree;
    int height;
    int count;

};

struct linked {
    uint16_t num_keys;
    struct kkey ** keys;
    struct linked ** child_array;
    struct linked *parent;
	int leaf;
    int num_child;
    int is_root;
};

struct kkey {
    uint32_t key;
    struct info * infos;
};

  

void * init_store(uint16_t branching, uint8_t n_processors);

void close_store(void * helper);

int btree_insert(uint32_t key, void * plaintext, size_t count, uint32_t encryption_key[4], uint64_t nonce, void * helper);

int btree_retrieve(uint32_t key, struct info * found, void * helper);

int btree_decrypt(uint32_t key, void * output, void * helper);

int btree_delete(uint32_t key, void * helper);

uint64_t btree_export(void * helper, struct node ** list);

void encrypt_tea(uint32_t plain[2], uint32_t cipher[2], uint32_t key[4]);

void decrypt_tea(uint32_t cipher[2], uint32_t plain[2], uint32_t key[4]);

uint64_t TEA_encrypt(uint32_t plain[2], uint32_t key[4]);

void encrypt_tea_ctr(uint64_t * plain, uint32_t key[4], uint64_t nonce, uint64_t * cipher, uint32_t num_blocks);

void decrypt_tea_ctr(uint64_t * cipher, uint32_t key[4], uint64_t nonce, uint64_t * plain, uint32_t num_blocks);

#endif
