#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef LIBRARY_H
#define LIBRARY_H
#define BUFFER_SIZE 2048
#define MOV 0
#define CAL 1
#define RET 2
#define REF 3
#define ADD 4
#define PRINT 5
#define NOT 6
#define EQU 7
#define INT_MAX_BIN_WIDTH (sizeof(int) * CHAR_BIT)

#define VALUE 0
#define REGISTER_ADDRESS 1
#define STACK_SYMBOL 2
#define POINTER_VALUED 3
#define max_struct 32

extern struct operation{
    char opcode[256];
    char type1[256];
    unsigned char val1;
    char type2[256];
    unsigned char val2;
}op;

extern struct function{
    unsigned char label;
    unsigned char num_of_instructions;
    struct operation operations[max_struct];
}fun;




//***************************************
// FUNCTION USED FOR GENERAL OPERATIONS**
// **************************************


//***************************************
// FUNCTION USED FOR GENERAL OPERATIONS**
// **************************************

//

#endif