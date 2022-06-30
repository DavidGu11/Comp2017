#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "library.h"
#include "extra.c"

int main(int argc, char* argv[]) {
    char all_bits[BUFFER_SIZE];
    int num_of_func = 0;
    struct function functions[8];
    unsigned char Register[8] = {0,0,0,0,254,0,0,0};
    //Register 7 stores the counter
    //Register 6 stores the stack_count(count the stack)
    //Register 5 stores the counter outside label 0
    //Register 4 stores the stack_count from the back
    unsigned char Stack[256];
    struct function newfunc; // temp func represents cal function
    struct function afunc; // temp func represents the main function
    struct operation aop; //temp instruction represents instruction that will be executed
    
    unsigned char buffer;
    FILE *fp = fopen(argv[1],"rb");
    char* p;
    if (!fp) {
        fprintf (stderr, "error: file open failed.\n");
        return 1;
    }
    while (fread(&buffer, sizeof(unsigned char), 1, fp) != 0) {
        p = decimal_to_binary(buffer,all_bits);
        free(p);
    }

    num_of_func = examining(all_bits,functions); // pass in the number of functions
    afunc = find_counter(functions,num_of_func);  // find the label 0 function
    aop = afunc.operations[afunc.num_of_instructions - 1]; //find the first instrction of the label 0 function 
    Stack[255] = 0; //the back of the Stack stores the boolean whether the instruction executed is in label 0,
    //0 is in labl 0, 1 if not

    // for (int i = 0; i < count - 1; i++) { // 
    //     for (int j = i + 1; j < count; j++) {
    //         if (functions[i].label == functions[j].label) {
    //             fprintf(stderr, "Having a same label");
    //             return 1;
    //         }
    //     }
    // }
    


    bool no_ret = true; // testing if the function executed has no return is so exit 
    for(int i = 0;i < afunc.num_of_instructions;i ++) {
        if(strcmp(afunc.operations[i].opcode,"RET") == 0){
            no_ret = false;
        }
    }
    if(no_ret == true){  
        EXIT_FAILURE;
    }
    else{
        no_ret = true;
    }
    while(1){

        if(Stack[255] == 1){
            for(int i = 0;i < newfunc.num_of_instructions;i ++) {
                if(strcmp(newfunc.operations[i].opcode,"RET") == 0){
                    no_ret = false;
                }
            }
            if(no_ret == true){
                no_ret = false;
                EXIT_FAILURE;
            }
            else{
                no_ret = true;
            }
        }

        if (Register[6] >= 255 ){
            fprintf(stderr, "STACK OVERFLOW");
            return 1;
        }
    
        if (num_of_func > 8){
            fprintf(stderr, "Too many func");
            return 1;
        }

        if (Register[7] >= 255){
            fprintf(stderr, "Stack Overflow");
            return 1;
            
        }   

        if (Stack[255] == 1){ //increment the cal_count if the the function is not label 0
            Register[5]++;
        }
        else{  //else increment the counter
            Register[7]++;
        } 
        if(strcmp(aop.opcode,"MOV") == 0){  //MOV changes the value
            if(strcmp(aop.type1,"REG") == 0){
                if(strcmp(aop.type2,"VAL") == 0){
                    Register[aop.val1] = aop.val2;
                }  
            }
            if(strcmp(aop.type1,"REG") == 0){
                if(strcmp(aop.type2,"REG") == 0){
                    Register[aop.val1] = Register[aop.val2];
                    if(aop.val1 == 7){
                        Register[7] = Register[aop.val2];
                    }
                }  
            }
            if(strcmp(aop.type1,"REG") == 0){
                if(strcmp(aop.type2,"STK") == 0){
                    for(int i = 0; i < Register[6];i++){
                        if(Stack[i] == aop.val2)
                        Register[aop.val1] = Stack[i+1];
                    }  
                }  
            }
            if(strcmp(aop.type1,"REG") == 0){
                if(strcmp(aop.type2,"PTR") == 0){
                    if(Stack[255] == 1){
                        for(int i = 254; i > Register[4] + 1;i--){
                            if(Stack[i] == aop.val2){       
                                Register[aop.val1] = Stack[i-1];                          
                            }
                        }
                    }
                    else
                    for(int i = 0; i < Register[6];i++){
                        if(Stack[i] == aop.val2){
                            Register[aop.val1] = Stack[i-1];     
                        }
                    }
                }  
            }


            if(strcmp(aop.type1,"STK") == 0){
                if(strcmp(aop.type2,"VAL") == 0){
                    Stack[Register[6]] = aop.val1;
                    Stack[Register[6] + 1] = aop.val2;
                    Register[6] += 2;
                    
                }  
            }
            if(strcmp(aop.type1,"STK") == 0){
                bool exist = false;
                if(strcmp(aop.type2,"STK") == 0){
                    for(int i = 0; i < Register[6];i++){
                        if(Stack[i] == aop.val1){           
                            exist = true;
                        }
                    }
                    if(exist == false){
                        for(int i = 0;i < Register[6];i++){
                            if(Stack[i] == aop.val2){
                                Stack[Register[6]] = aop.val1;
                                Stack[Register[6]+1] = Stack[i+1];
                                Register[6] += 2;
                            }
                        }
                    } 
                }  
            }
            if(strcmp(aop.type1,"STK") == 0){ 
                bool exist = false;
                if(strcmp(aop.type2,"REG") == 0){
                    if(Stack[255] == 1){   //if the function executed is outside label 1 
                        for(int i = 254; i > Register[4]; i--){ // if the temp stack exists it will change the stk address to the reg
                            if(Stack[i] == aop.val1){
                                printf("sds\n");
                                Stack[i+1] = Register[aop.val2];
                                exist = true;
                            }

                        }
                        
                        if(exist == false){  // if not then add the function to the back,    
                            Stack[Register[4]] = aop.val1; //Register[4] stores the address
                            Stack[Register[4] - 2] = Register[aop.val2]; // Register[4] -2 stores the value
                            for(int i = 0; i < Register[6]; i++){
                                if(Stack[i] == Register[aop.val2]){         
                                    Stack[Register[4] - 1] = Stack[i+1]; // Regitser[4] -1 stores the value it points to in label 0 

                                }
                            }
                            Register[4]-= 3;
                        }
                       
                    }
                    else{
                        for(int i = 0;i < Register[6];i++){
                            if(Stack[i] == aop.val1){
                                Stack[i] = Register[aop.val2];
                                exist = true;
                            }
                        }
                        if(exist == false){
                            Stack[Register[6]] = aop.val1;
                            Stack[Register[6]+1] = Register[aop.val2];
                            Register[6] += 2;
                        }
                    }
                }  
            }
            if(strcmp(aop.type1,"PTR") == 0){ // change the value which the address is pointing to to the reg
                int address;
                if(strcmp(aop.type2,"REG") == 0){
                    if(Stack[255] == 1){
                        for(int i = 254; i > Register[4];i--){
                            if(Stack[i] == aop.val1){    
                                address = Stack[i - 2]; 
                            }
                        }
                        for(int i = 0;i < Register[6];i++){
                            if(Stack[i] == address){       
                                Stack[i + 1] = Register[aop.val2]; 
                            }           
                        }
                    }
                    else
                    for(int i = 0;i < Register[6];i++){
                        if(Stack[i] == aop.val1){
                            
                            Stack[i + 1] = Register[aop.val2];
                           
                        }
                    }
                }   
            }
        }
        else if(strcmp(aop.opcode, "CAL") == 0){ 
            if(strcmp(aop.type1,"VAL") == 0){
               for(int i = 0;i < num_of_func;i++){
                   if(functions[i].label == aop.val1) {
                        newfunc = functions[i];
                        aop = newfunc.operations[newfunc.num_of_instructions - 1];
                        if(newfunc.label == 0){ //if function calls it self,
                            Stack[255] = 0;
                        }
                        else{ // if functions calls other function
                            Stack[255] = 1;
                        }   
                        break;
                   }
               }
               continue;
            }
        }
        else if(strcmp(aop.opcode, "RET") == 0){
            if(Stack[255] == 0){ //if the function is the main
                break;
            }        
            else{
                for(int i = 254; i > Register[4]; i --){ //clean the temp variables
                    Stack[i] = 0;
                }
                Register[4] = 254; //reset everything

                Stack[255] = 0;
                Register[5]= 0;
                aop = afunc.operations[afunc.num_of_instructions - 1 -Register[7]];   
                continue; 
            }
                 
        }
        else if(strcmp(aop.opcode,"REF") == 0){
            int temp1,temp2;
            bool exist;
            if(Stack[255] == 1){
                if(strcmp(aop.type1,"STK") == 0){
                    if(strcmp(aop.type2,"STK") == 0){
                        for(int i = 254;i > Register[4];i--){
                            if(aop.val2 == Stack[i]){
                                temp1 = Stack[i-2];
                                temp2 = Stack[i-1];
                                for(int c = 254; c > Register[4];c++){
                                    if(aop.val1 == Stack[c]){
                                        Stack[c] = Stack[i]; // change the address
                                        Stack[c-1] = Stack[i-1]; //change the value it points to 
                                        exist = true;
                                    }
                                }  
                            }
                        }
                        if(exist == false){
                        // printf("val1: %d stackcount = %d\n",aop.val1,stack_count);
                            Stack[Register[4]] = aop.val1;
                            Stack[Register[4] -1] = temp2;
                            Stack[Register[4] -2] = temp1;
                            Register[4] -= 3;
                        }
                    }
                }
            }
            else
            if(strcmp(aop.type1,"STK") == 0){
                if(strcmp(aop.type2,"STK") == 0){
                    for(int i = 0;i < Register[6];i++){
                        if(aop.val2 == Stack[i]){
                            temp1 = Stack[i+1];
                            for(int c = 0; c < Register[6];c++){
                                if(aop.val1 == Stack[c]){
                                    Stack[c] = Stack[i];
                                    exist = true;
                                }
                            }     
                        }   
                    }   
                    if(exist == false){
                        Stack[Register[6]] = aop.val1;
                        // printf("%d",Stack[2]);
                        Stack[Register[6] +1] = temp1;
                        Register[6] += 2;
                    }
                }  
            }
            
            if(strcmp(aop.type1, "REG") == 0){
                if(strcmp(aop.type2, "STK") == 0){
                    for(int c = 0;c < Register[6];c++){
                        if(Stack[c] == aop.val2){
                            Register[aop.val1] = Stack[c]; 
                        }
                    }  
                }
            }  
        }
        else if(strcmp(aop.opcode,"ADD") == 0){
            if(strcmp(aop.type1,"REG") == 0){
                if(strcmp(aop.type2,"REG") == 0){
                    
                    if(Stack[255] == 1){ //add the cal counter
                        if(aop.val1 == 7){
                            Register[5]+= Register[aop.val2];
                        }
                    }
                    if(aop.val1 != 7){
                        Register[aop.val1] += Register[aop.val2];
                    }                
                }  
            }
        }
            
        else if(strcmp(aop.opcode,"PRINT") == 0){
            if(strcmp(aop.type1,"REG") == 0){
                printf("%d\n",Register[aop.val1]);
            }
            if(strcmp(aop.type1,"VAL") == 0){
                printf("%d\n",aop.val1);
            }
            if(strcmp(aop.type1,"STK") == 0){
                if(Stack[255] == 1){//if in cal function print temp stk
                    for(int i = 254;i < Register[4];i++){
                        if(aop.val1 == Stack[i]){
                            printf("%d\n",Stack[i-2]);
                        }
                    }
                }
                else
                for(int i = 0;i < Register[6];i++){
                    if(aop.val1 == Stack[i]){
                        printf("%d\n",Stack[i+1]);
                    }
                }
            }
            if(strcmp(aop.type1,"PTR") == 0){
                if(Stack[255] == 1){ //if in cal function print temp ptr
                    for(int i = 254;i < Register[4];i++){
                        if(aop.val1 == Stack[i]){
                            printf("%d\n",Stack[i-1]);
                        }
                    }
                }
                else
                for(int i = 0;i < Register[6];i++){
                    if(aop.val1 == Stack[i]){
                        printf("%d\n",Stack[i+1]);
                    }
                }
            }
            
        }
        else if(strcmp(aop.opcode,"NOT") == 0){// bitwise operation not
            if(strcmp(aop.type1,"REG") == 0){
                unsigned int val = ~(Register[aop.val1]);
                // printf("%d\n",val);
                Register[aop.val1] = val;
            }
        }
        else if(strcmp(aop.opcode,"EQU") == 0){ //flip
            if(strcmp(aop.type1,"REG") == 0){
                if(Register[aop.val1] == 0){
                    Register[aop.val1] = 1;
                }
                else{
                    Register[aop.val1] = 0;
                }
            }
        }
        // printf("Stact %d %d, %d %d%d \n", Stack[0], Stack[1],Stack[252],Stack[253] ,Stack[254]);
        // // printf("After MOV: cal_counter %d\n",cal_count);
        // printf("After MOV: %d, %d %d %s %d\n",Register[0],Register[1],Register[5],aop.opcode, Stack[255]);
        // printf("\n");
        
        if(Stack[255] == 0){ //if the function executed is label one, aop becomes next instruction of main, else execute other label
            aop = afunc.operations[afunc.num_of_instructions - 1 -Register[7]]; 
            continue;
        }
            
    
        else if(Stack[255] == 1){
            aop = newfunc.operations[newfunc.num_of_instructions - 1 - Register[5]]; 
            continue;
        }
        
        
    return 0;
    }
}