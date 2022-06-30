#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "library.h"
#include "extra.c"











//***************************************
// FUNCTION USED FOR GENERAL OPERATIONS**
// **************************************

//


int main(int argc, char* argv[]){
    char all_bits[BUFFER_SIZE];

    // printf("s\n");
    FILE *fp;
    unsigned char buffer;
    fp = fopen(argv[1],"rb");
    char* p;
    if (!fp) {
        fprintf (stderr, "error: file open failed.\n");
        return 1;
    }
    while (fread(&buffer, sizeof(unsigned char), 1, fp) != 0) {
        p = decimal_to_binary(buffer,all_bits);
        // printf("%s\n",p);
        free(p);
    }
    // printf("%s",all_bits);
    test(all_bits);
    return 0;
}
