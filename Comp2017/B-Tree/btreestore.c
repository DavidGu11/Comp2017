#include "btreestore.h"

union
{
    uint64_t t_var;
    uint32_t a_var[2];
} u_var;

void * init_store(uint16_t branching, uint8_t n_processors) {
    struct helping* helper = (struct helping*)malloc(sizeof(struct helping));
    struct linked* root;
    root = (struct linked*)malloc(sizeof(struct linked));
    root->num_keys = 0;
    root->keys = (struct kkey **)malloc(sizeof(struct kkey)*(branching-1));
    // root->keys = NULL;
    // root->keys[0]->infos = (struct info *)malloc(sizeof(struct info)*branching);
    root->child_array = (struct linked **)malloc(sizeof(struct linked)*branching);
    root->parent = NULL;
    root->leaf = 1;
    root->num_child= 0;
    // helper->tree = (struct linked*)malloc(sizeof(struct linked));
    helper->num_nodes = 0;
    helper->branch = branching;
    helper->tree = root;
    root->is_root = 1;

    void * ptr = helper;


    // Your code here
    return ptr;
}

void * search(uint32_t target,void * helper){
    struct helping* help = (struct helping*) helper;
    struct linked* temp = help->tree; 
    uint32_t k_high,k_low;
    int high_in, low_in;
    struct linked* root = temp;
    while(1){
        high_in = -1;
        low_in = -1;
        for(int i = 0; i < root->num_keys;i++){
            switch(high_in){
                case -1:
                if(root->keys[i]->key > target){
                    k_high = root->keys[i]->key;
                    high_in = i;
                    break;
                }
                
                default:
                if((k_high > root->keys[i]->key) && (root->keys[i]->key > target)){
                    k_high = root->keys[i]->key;
                    high_in = i;
                    break;
                }
            }
            switch(low_in){
                case -1:
                if(root->keys[i]->key < target){
                    k_low = root->keys[i]->key;
                    low_in = i;
                    break;
                }
                
                default:
                if((k_low < root->keys[i]->key) && (root->keys[i]->key < target)){
                    k_low = root->keys[i]->key;
                    low_in = i;
                }
            }
            
          
            if(root->keys[i]->key == target){
                
                return (void*)(root->keys[i]);
            }
        }
        // if(target == 15){
        //     printf("high %d low %d\n",high_in,low_in);
        // }
        if(root->leaf == 1){
            return NULL;
        }
        if(root->num_keys == 1){
            high_in = 0;
        }
        if(low_in == -1){
            root = (root->child_array[high_in]);
        }
        else{
            
            root = (root->child_array[low_in+1]);
        }
    }
}

void reorder(struct linked* root){
    for (int d = 0; d < root->num_keys; ++d){

        for (int j = d + 1; j < root->num_keys; ++j)
        {            
            if (root->keys[d]->key > root->keys[j]->key) 
            {

                struct kkey* a = root->keys[d];
                root->keys[d] = root->keys[j];
                root->keys[j] = a;
            }
        }
         
    } 
}
void split(struct linked * root,struct helping * help,int target){
    struct linked * right = (struct linked*)malloc(sizeof(struct linked));
    int divide = 0;
    right->child_array = (struct linked **)malloc(sizeof(struct linked)*(help->branch));
    right->num_child = 0;
    right->num_keys = 0;
    if(root->parent == NULL){
        struct linked * left = (struct linked*)malloc(sizeof(struct linked));
        left->child_array = (struct linked **)malloc(sizeof(struct linked)*(help->branch));
        left->num_child = 0;
        left->num_keys = 0;
        if(root->num_keys % 2 == 0){
            divide = 1;
            
            // median = root->keys[root->num_keys/2 - 1];
            left->keys = (struct kkey **)malloc(sizeof(struct kkey)*(help->branch-1));
            for (int i = 0; i < root->num_keys/2 - 1;i++){
                
                left->keys[i] = root->keys[i];
                left->num_keys = i + 1;
            }
            
            right->keys = (struct kkey **)malloc(sizeof(struct kkey)*(help->branch-1));
            int tmp = 0;
            // printf("less %d %d\n",root->num_keys/2,root->num_keys);
            for (int i = root->num_keys /2 ; i < root->num_keys;i++){
                
                
                right->keys[tmp] = root->keys[i];
                tmp++;
                right->num_keys = tmp;
            }
            // printf("left %d\n right %d\n",left->num_keys,right->num_keys);

            
        }
        else{
            
            divide = 0;
            // median = root->keys[root->num_keys/2];
            left->keys = (struct kkey **)malloc(sizeof(struct kkey)*(help->branch-1));
            for (int i = 0; i < root->num_keys/2;i++){
            
                left->keys[i] = root->keys[i];
                left->num_keys = i + 1;
            }
            right->keys = (struct kkey **)malloc(sizeof(struct kkey)*(help->branch-1));
            int tmp = 0;
            for (int i = root->num_keys/2 + 1; i < root->num_keys;i++){
                right->keys[tmp] = root->keys[i];
                tmp++;
                right->num_keys = tmp;
            }   
        }
        if(divide == 1){
            root->keys[0] =  root->keys[root->num_keys/2 - 1];
        }
        else{
            root->keys[0] =  root->keys[root->num_keys/2];          
        }
        if(root->num_child != 0){
            for(int i = 0; i < root->num_keys/2+1;i++){
                left->child_array[i] = root->child_array[i];
              
                left->child_array[i]->num_child = 0;
                left->child_array[i]->leaf = 1;
                left->child_array[i]->parent = left;

                left->num_child++;
            }
            int j = 0;
            for(int i = root->num_keys/2+1; i < root->num_keys+1;i++){
                right->child_array[j] = root->child_array[i];
                right->child_array[j]->num_child = 0;
                right->child_array[j]->leaf = 1;
                right->child_array[j]->parent = right;

                j++;
                right->num_child++;
            }
        }

        // left->num_child = 2;
        left->parent = root;
        right->parent = root;
        if(left->num_child == 0){
            left->leaf = 1; 
        }
        if(right->num_child == 0){
            right->leaf = 1; 
        }
        // left->leaf = 1;
        // right->leaf = 1;
        root->num_keys = 1;
        root->num_child = 2;
        root->leaf = 0;
        root->child_array[0] = left;
        root->child_array[1] = right;

        help->num_nodes += 2;
        // if(target == 7){
        //     printf("sd %d\n",root->child_array[0]->child_array[0]->parent->keys[0]->key);
        // }
        
    }
    else{

        if(root->num_keys % 2 == 0){
            divide = 1;       
            right->keys = (struct kkey **)malloc(sizeof(struct kkey)*(help->branch-1));
            int tmp = 0;
            for (int i = root->num_keys/2; i < root->num_keys;i++){
                right->keys[tmp] = root->keys[i];
                tmp++;
                right->num_keys = tmp;
            }
        }
        else{
            
            divide = 0;
            right->keys = (struct kkey **)malloc(sizeof(struct kkey)*(help->branch-1));
            int tmp = 0;
            for (int i = root->num_keys/2 + 1; i < root->num_keys;i++){
                
                right->keys[tmp] = root->keys[i];
                tmp++;
                right->num_keys = tmp;
            }      
        }
        
        int median;
        struct linked * the_par = root->parent;
        if(divide == 1){
            median = root->num_keys/2 - 1;
            root->parent->keys[root->parent->num_keys] =  root->keys[root->num_keys/2 - 1];
        }
        else{
            median = root->num_keys /2;
            root->parent->keys[root->parent->num_keys] =  root->keys[root->num_keys/2];     
        }
        // if(target == 9){
        //     printf("%d",root->keys[root->num_keys/2]->key);
        //     printf("%d",root->parent->keys[root->parent->num_keys-1]->key);
        // }
        reorder(root->parent);
        int n = root->parent->num_child;
        int pos = median + root->parent->num_child - 1;
        for (int i = n; i >= pos; i--){
            // printf("key is %d\n",root->parent->child_array[i-1]->keys[0]->key);
            root->parent->child_array[i] = root->parent->child_array[i - 1];
        }
        right->parent = the_par;
        right->leaf = 1;
        
        root->parent->child_array[root->parent->num_child ] = right;
        root->parent->num_child += 1;
        root->parent->num_keys += 1;
        
        if(divide == 1){
            int c = 0;
            for (int i = 0; i < root->num_keys/2 - 1;i++){
                root->keys[i] = root->keys[i];
                c++;
            }      
            root->num_keys = c; 
        }
        else{
            int c = 0;
            for (int i = 0; i < root->num_keys/2;i++){
                
                root->keys[i] = root->keys[i];
                
                c++;
            }     
            root->num_keys = c;     
        }
        
        help->num_nodes++;
        root->leaf = 1;
        reorder(root->parent);

        
        // if(target == 6){
        //     printf("%d\n",root->parent->num_child );
        // }
        int math = root->parent->num_child - 2;
        // if(target == 11){
        //     printf("yes %d\n",root->parent->child_array[3]->num_keys);
        // }
        
        // if(root->parent->child_array[math]->keys[0]->key > root->parent->child_array[math+1]->keys[0]->key){
        //     // if(target == 11){
        //     //     printf("yes\n");
        //     // }
        //     struct linked * temp = root->parent->child_array[math];

        //     root->parent->child_array[math] = root->parent->child_array[math+1];
        //     root->parent->child_array[math+1] = temp;

        // }
        for(int i = 0; i < root->parent->num_child- 1; i++){
            if(root->parent->child_array[i]->keys[0]->key > root->parent->child_array[i+1]->keys[0]->key){
                // if(target == 11){
                //     printf("yes\n");
                // }
                struct linked * temp = root->parent->child_array[i];

                root->parent->child_array[i] = root->parent->child_array[i+1];
                root->parent->child_array[i+1] = temp;

            }
        }
        
        
       
        

    }
}


int insert_search(uint32_t target,void * helper,struct kkey* insert_key){
    struct helping* help = (struct helping*) helper;
    struct linked* root = help->tree; 
    uint32_t k_high,k_low;
    int high_in, low_in;
    while(1){
        high_in = -1;
        low_in = -1;
        for(int i = 0; i < root->num_keys;i++){
            switch(high_in){
                case -1:
                if(root->keys[i]->key > target){
                    // if(target == 6){
                    //     printf("yes");
                    // }
                    k_high = root->keys[i]->key;
                    high_in = i;
                    break;
                }
                
                default:
                if((k_high > root->keys[i]->key) && (root->keys[i]->key > target)){
                    // if(target == 6){
                    //     printf("no\n");
                    // }
                    k_high = root->keys[i]->key;
                    high_in = i;
                    break;
                }
            }
            switch(low_in){
                case -1:
                if(root->keys[i]->key < target){
                    k_low = root->keys[i]->key;
                    low_in = i;
                    break;
                }
                
                default:
                if((k_low < root->keys[i]->key) && (root->keys[i]->key < target)){
                    k_low = root->keys[i]->key;
                    low_in = i;
                }
            }
            
          
            if(root->keys[i]->key == target){
                // printf("key is %d\n",root->keys[i]->key);
                return 0;
            }
        }
        // if(target == 8){
        //     printf("high %d low %d \n",high_in,low_in);
        // }
        if(root->leaf == 1){
            
            
            root->keys[root->num_keys] = insert_key;
            // if(target == 14){
            //     printf("%d\n",root->num_keys);
            // }
            root->num_keys += 1;
            reorder(root);
            if(root->num_keys <= help->branch - 1){
                return 0;
            }
            struct linked * tempp = root;
            // printf("%d\n",root->parent->num_keys);
            // if(target == 9){
            //     printf("%d\n",root->keys[1]->key);
            // }
            split(tempp,help,target);
            if(tempp->parent != NULL){
                while(tempp->parent->num_keys == help->branch){
                    tempp = tempp ->parent;
                    split(tempp,help,target);
                    // tempp = tempp->parent;
                    break;
                }
            }
            // if(target == 80){
            //     split(tempp->parent,help,target);
            // }
            // split(root,help,target);
            
            
            
            // split(root,help,target);
            
            return 0;  
        }
        if(root->num_keys == 1){
            high_in = 0;
        }
        if(low_in == -1){
            root = (root->child_array[high_in]);
        }
        else{
            root = (root->child_array[low_in+1]);
        }
        
        
    }
}

void free_help(struct linked* root,struct helping* help){
    for(int i = 0; i < root->num_keys; i++){
        // printf("by: %d\n",root->keys[i]->key);
        free(root->keys[i]->infos->data);
        free(root->keys[i]->infos);
        free(root->keys[i]);
    }
    free(root->keys);
    help->num_nodes -= 1;
    // free(root);

    // free(root->keys->infos);

    for(int i = 0; i < (root->num_child);i++){
        free_help(root->child_array[i],help);      
    }
    // if(help->num_nodes == 0){
    //     free(root->child_array);
    //     free(root);
    //     return;
    // }

    free(root->child_array);
    free(root);

}
void close_store(void * helper) {
    struct helping* help = (struct helping*) helper;
    struct linked* root = help->tree; 
    help->count = 0;
    free_help(root,help);
    // free(help->tree);
    free(help);

    return;
}

int roundUp(int numToRound, int multiple)
{
    if (multiple == 0)
        return numToRound;

    int remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;

    return numToRound + multiple - remainder;
}

struct kkey * make_keys(uint32_t key, void * plaintext, size_t count, uint32_t encryption_key[4], uint64_t nonce){
    
    int rounded;
    int add;
    if(count < 8){
        add = 8 - count;
    }
    // count = roundUp(count,8);
    struct kkey * akey = (struct kkey*)malloc(sizeof(struct kkey));
    akey->key = key;
    struct info * infoss = (struct info*)malloc(sizeof(struct info));
    rounded = roundUp(count,8);
    uint64_t * text = (uint64_t *) malloc(sizeof(uint64_t) * rounded/8 );
    uint32_t num_blocks = rounded/8;


    infoss->data = (uint64_t*)malloc(sizeof(uint64_t) * rounded/8 );
    memset(text,'\0',sizeof(uint64_t) * rounded/8);
    memcpy(text,plaintext,count);
    encrypt_tea_ctr(text,encryption_key,nonce,infoss->data,num_blocks);
    free(text);

    infoss->nonce = nonce;

    memcpy(infoss->key,encryption_key, 4*(sizeof(uint32_t)));
    infoss->size = count;
    akey->infos = infoss;

    return akey;
}

int btree_insert(uint32_t key, void * plaintext, size_t count, uint32_t encryption_key[4], uint64_t nonce, void * helper) {
    struct helping* help = (struct helping*) helper;
    struct linked* root = help->tree; 
    struct kkey * temp_key;

    void * search_result = search(key,helper);

    if(search_result != NULL){
        // printf(“)
        return 1;
    }
    if (help->num_nodes == 0){
        
        temp_key = make_keys(key, plaintext, count, encryption_key, nonce);
        root->keys[root->num_keys] = temp_key;
        root->num_keys += 1;
        help->num_nodes += 1;
        // free(temp_key);

        return 0;
    }
    else{
        temp_key = make_keys(key, plaintext, count, encryption_key, nonce);
        // if(key == 15){
        //     printf("%d",temp_key->infos->key[1]);
        // }
        
        insert_search(key,helper,temp_key);

        return 0;
    }
    return -1;
}

int btree_retrieve(uint32_t key, struct info * found, void * helper) {
    // struct helping* help = (struct helping*) helper;
    // struct linked* root = help->tree; 
    
    if(search(key,helper)!= NULL){
        struct kkey * fon = (struct kkey *)search(key,helper);
        struct info * fonndst = fon->infos;
        found->data = fonndst->data;
        memcpy(found->key,fonndst->key, 4*(sizeof(uint32_t)));
        // found->key = fonndst->key;
        found->nonce = fonndst->nonce;
        found->size = fonndst->size;
        // printf("key %d\n",fon->key);
        return 0;
    }

    // Your code here
    return 1;
}

int btree_decrypt(uint32_t key, void * output, void * helper) {
    // Your code here
    if(search(key,helper)!= NULL){
        // printf("key is %d\n",key);
        
        struct kkey * fon = (struct kkey *)search(key,helper);
        struct info * fonndst = fon->infos;

        uint64_t * temp = malloc(roundUp(fonndst->size,8));
        // uint64_t plain;
        // memcpy(temp,fonndst->data,fonndst->size);
        
        decrypt_tea_ctr(fonndst->data,fonndst->key,fonndst->nonce,temp,roundUp(fonndst->size,8)/8);
       

        memcpy(output,(void*)temp,fonndst->size);
        free(temp);

        return 0;
    }

    return 1;
}

int btree_delete(uint32_t key, void * helper) {
    if(search(key,helper) == NULL){
        return 1;
    }
    // Your code here
    return 0;
}

struct node newNode(uint16_t num_keys,uint32_t * keys){
    // allocate a new node in a heap and set its data
    struct node node;
    node.num_keys = num_keys;
 
    // `.next` pointer of the new node points to nothing
    node.keys = keys;
 
    return node;
}

void preorder(struct linked* root,struct node * export,struct helping * help){
    
    uint32_t * keys = malloc(sizeof(uint32_t)*(help->branch));
    struct node node1;
    node1.num_keys = root->num_keys;

    export[help->count] = node1;
    for(int i = 0; i < root->num_keys;i++){

   
        keys[i]= root->keys[i]->key;
    }
    export[help->count].keys = keys;
    help->count++;  

    
    // export[1] = node1;
    for(int i = 0; i < (root->num_child) ;i++){
         
        preorder(root->child_array[i],export,help);
        
        // break;
    }
    
    return;

}

uint64_t btree_export(void * helper, struct node ** list) {
    struct helping* help = (struct helping*) helper;
    struct linked* root = help->tree; 
    struct node * export = (struct node*)malloc(sizeof(struct node)*(help->num_nodes + 1));
    // printf("nodes: %d\n",root->child_array[1]->child_array[1]->keys[0]->key);

    
    help->count = 0;

    preorder(root,export,help);
    // printf("\nepxrt 1: %d",export[1].keys[0]);
    *list = export;


    return help->num_nodes;

}

void encrypt_tea(uint32_t plain[2], uint32_t cipher[2], uint32_t key[4]) {
    uint32_t sum = 0;
    uint32_t delta = 0x9E3779B9;
    cipher[0] = plain[0];

    cipher[1] = plain[1];
    long int power = (long int)pow(2,32);
    for(int i = 0; i < 1024;i++){       
        sum = (sum + delta) % power;
        uint32_t tmp1 = ((cipher[1] << 4) + key[0])% power;
        uint32_t tmp2 = (cipher[1] + sum)% power;
        uint32_t tmp3 = ((cipher[1] >> 5) + key[1])% power;
        cipher[0] = (cipher[0] + (tmp1 ^ tmp2 ^ tmp3))% power;
        uint32_t tmp4 = ((cipher[0] << 4) + key[2])% power;
        uint32_t tmp5 = (cipher[0] + sum)% power;
        uint32_t tmp6 = ((cipher[0] >> 5) + key[3])% power;
        cipher[1] = (cipher[1] + (tmp4 ^ tmp5^ tmp6))% power;
    }


    // Your code here
    return;
}

void decrypt_tea(uint32_t cipher[2], uint32_t plain[2], uint32_t key[4]) {
    uint32_t sum = 0xDDE6E400;
    uint32_t delta = 0x9E3779B9;
    long int power = (long int)pow(2,32);
    for(int i = 0; i < 1024;i++){
        uint32_t tmp4 = ((cipher[0] << 4) + key[2])% power;
        uint32_t tmp5 = (cipher[0] + sum)% power;
        uint32_t tmp6 = ((cipher[0] >> 5) + key[3])% power;
        cipher[1] = (cipher[1] - (tmp4 ^ tmp5 ^ tmp6))% power;
        uint32_t tmp1 = ((cipher[1] << 4) + key[0])% power;
        uint32_t tmp2 = (cipher[1] + sum)% power;
        uint32_t tmp3 = ((cipher[1] >> 5) + key[1])% power;
        cipher[0] = (cipher[0] - (tmp1 ^ tmp2 ^ tmp3))% power;
        sum = (sum - delta)% power;
        plain[0] = cipher[0];
        plain[1] = cipher[1];
    }

    return;
}


void encrypt_tea_ctr(uint64_t * plain, uint32_t key[4], uint64_t nonce, uint64_t * cipher, uint32_t num_blocks) {
    for(uint32_t i = 0; i < num_blocks;i++){
        uint64_t tmp1 = i ^ nonce;
        u_var.t_var = tmp1;
        uint64_t tmp2 = TEA_encrypt(u_var.a_var, key);
        cipher[i] = plain[i] ^ tmp2;
    }

    return;
}

void decrypt_tea_ctr(uint64_t * cipher, uint32_t key[4], uint64_t nonce, uint64_t * plain, uint32_t num_blocks) {
    for(uint32_t i = 0; i < num_blocks;i++){
        uint64_t tmp1 = i ^ nonce;
        u_var.t_var = tmp1;
        uint64_t tmp2 = TEA_encrypt(u_var.a_var, key);
        plain[i] = cipher[i] ^ tmp2;
    }
  
    return;
}

uint64_t TEA_encrypt(uint32_t plain[2], uint32_t key[4]){
    uint32_t sum = 0;
    uint32_t delta = 0x9E3779B9;
    uint32_t cipher[2];
    cipher[0] = plain[0];
    cipher[1] = plain[1];
    long int power = (long int)pow(2,32);
    for(int i = 0; i < 1024;i++){    
        sum = (sum + delta) % power;
        uint32_t tmp1 = ((cipher[1] << 4) + key[0])% power;
        uint32_t tmp2 = (cipher[1] + sum)% power;
        uint32_t tmp3 = ((cipher[1] >> 5) + key[1])% power;
        cipher[0] = (cipher[0] + (tmp1 ^ tmp2 ^ tmp3))% power;
        uint32_t tmp4 = ((cipher[0] << 4) + key[2])% power;
        uint32_t tmp5 = (cipher[0] + sum)% power;
        uint32_t tmp6 = ((cipher[0] >> 5) + key[3])% power;
        cipher[1] = (cipher[1] + (tmp4 ^ tmp5^ tmp6))% power;
    }
    u_var.a_var[0] = cipher[0];
    u_var.a_var[1] = cipher[1];

    return u_var.t_var;
}