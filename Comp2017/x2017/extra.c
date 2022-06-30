#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "library.h"


//***************************************
// STANDARD FUNCTIONS********************
// **************************************

extern struct function find_counter(struct function functions[],int num_of_func){//find the label 0 
    for(int i = 0;i < num_of_func;i++){
        if(functions[i].label == 0){
            return functions[i];
        }
    }
    fprintf(stderr, "NO LABEL 0");
    exit(1);
}


extern void ReverseSTRUCT1(struct function functions[], int n) //reverse struct array of function
{
    struct function temp;
  
    // Move from begin and end. Keep
    // swapping strings. 
    int j = n - 1;
    for (int i = 0; i < j; i++) {
        temp = functions[i];
        functions[i] = functions[j];
        functions[j] = temp;
        j--;
    }
}

extern void ReverseSTRUCT2(struct operation functions[], int n)//reverse struct array of operations
{
    struct operation temp;
  
    // Move from begin and end. Keep
    // swapping strings. 
    int j = n - 1;
    for (int i = 0; i < j; i++) {
        temp = functions[i];
        functions[i] = functions[j];
        functions[j] = temp;
        j--;
    }
}


//Reverse a string
extern void reverse_string(char str[]) //reverse a string
{
    char temp;
    char *p, *q;

    p = str;
    if (!p)
        return;

    q = p + 1;
    if (*q == '\0')
        return;

    temp = *p;
    reverse_string(q);

    while (*q != '\0') {
        *p = *q;
        p++;
        q++;
    }
    *p = temp;

    return;
}

extern char* rev_str(char* s)
{ // return a revserved string
    size_t l = strlen(s);
    char* r = malloc((l + 1));
    r[l] = '\0';
    int i;
    for(i = 0; i < l; i++) {
        r[i] = s[l - 1 - i];
    }
    return r;
}

//concatenate char to char array
extern void strcat_c (char *str, char c)
{
    for (;*str;str++);  
    *str++ = c; 
    *str++ = 0; 
}

//Converting decimal to binary
extern char *decimal_to_binary(int n,char* all_bits)
{
    int t = 0;
    char* ptr = (char*)malloc(32+1);
    if (ptr == NULL){
        exit(EXIT_FAILURE);
    }   
    for (int i = 7 ; i >= 0 ; i--){
        int d = n >> i;        
        if (d & 1){
            *(ptr+t) = 1 + '0';
            strcat(all_bits,"1");
        }
            
        else{
            *(ptr+t) = 0 + '0';
            strcat(all_bits,"0");
        }         
        t++;
    }
    *(ptr+t) = '\0';
    return ptr;
}



//***************************************
// FUNCTION USED FOR COMPARISONS********
// **************************************

//Comparing a bit with one of the opcodes, returning 1 if equal
extern int op_cmp(char* reversed_binstr,int opcode){
    char* b = rev_str(reversed_binstr);
       
    // strcpy(binstr, b);
    char *pp;
    // reverse_string(binstr);
    int bin = strtol(b, &pp, 2);
    if(bin == opcode){
        free(b);
        
        return 1;
    }
    free(b);
    return 0;

    
}

//converting string with to int
extern int str2bin(char* line, int cut)
{
    char temp[60];
    char* pp;
    for(int i = 0;i < cut;i++){
        strcat_c(temp,line[i]);
    }
    reverse_string(temp);
    int bin = strtol(temp, &pp, 2);
    return bin;
}

//see if the temporary bits equals any one of the opcode value
extern int com_all_op(char* tmp,char* bits_cpy){
    for(int i = 0 ; i < 3; i++){
            strcat_c(tmp,bits_cpy[i]);
        }

    if(op_cmp(tmp,MOV) == 1){
        return 0;
    }
    else if(op_cmp(tmp,CAL) == 1){
        return 1;
    }
    else if (op_cmp(tmp,RET) == 1){
        return 2;
    }
    else if (op_cmp(tmp,REF) == 1){
        return 3;
    }
    else if (op_cmp(tmp,ADD) == 1){
        return 4;
    }
    else if (op_cmp(tmp,PRINT) == 1){
        return 5;
    }
    else if (op_cmp(tmp,NOT) == 1){
        return 6;
    }
    else if (op_cmp(tmp,EQU) == 1){
        return 7;
    }
    return -1;
}

//see if the temporary bits equals any one of the type value
extern int com_all_type(char* tmp,char* bits_cpy){
    for(int i = 0 ; i < 2; i++){
            strcat_c(tmp,bits_cpy[i]);
        }
    if(op_cmp(tmp,VALUE) == 1){
        return 0;
    }
    
    else if(op_cmp(tmp,REGISTER_ADDRESS) == 1){;
        return 1;
    }
    else if(op_cmp(tmp,STACK_SYMBOL) == 1){
        return 2;
    }
    else if(op_cmp(tmp,POINTER_VALUED) == 1){
        return 3;
    }
    return -1;
}

//does both empty the temporary bits which is used to store bits to compare with code/type/value and remove the bits
//that has completed comparison from the whole bits array 
extern char * clean_string(char* tmp_bits,char* revsersed_all_bits,int remove_bits){
    for(int i=0;i<strlen(tmp_bits);i++)
    {
        tmp_bits[i] = 0;
    }
    char* deleted = revsersed_all_bits + remove_bits;
    // return revsersed_all_bits;
    // memmove(revsersed_all_bits, reverse_string + remove_bits, strlen(revsersed_all_bits));
    return deleted;
}

//***************************************
// MAIN TESTING**************************
// **************************************

// Main loop examining binary and returns the number of functions
extern int examining(char* all_bits,struct function functions[]){
    int count = 0;
    int num_of_func = 0;
    long int instructions;
    char *bits_cpy = all_bits;
    char bit[256];
    char* newline;
    struct function afunc;
    struct operation aop;

    reverse_string(bits_cpy); //firstly reverse the bit array

    for(int i = 0 ; i < 5 ; i++){ //get the instructions
        strcat_c(bit,bits_cpy[i]);
    }
    reverse_string(bit);
    instructions = strtol(bit, &newline, 2);
    // printf("first: %s\n", bits_cpy);
    bits_cpy = clean_string(bit,bits_cpy,5); //remove the examined bits from the array
    // printf("%d\n",instructions);
    for(int i = 0 ; i < 3 ; i++){
        strcat_c(bit,bits_cpy[i]);
    }

    if(op_cmp(bit,RET) == 1){          // if EQUALS RET else exit failure
        bits_cpy = clean_string(bit,bits_cpy,3);
    }
    else{
        EXIT_FAILURE;
    }
    strcpy(aop.opcode, "RET");
    afunc.operations[count] = aop;
    count++;
    while(1){
        if(count == instructions){  //indicate the end of a function
            afunc.num_of_instructions = instructions;
            int num = str2bin(bits_cpy,3);
            afunc.label = num;
            functions[num_of_func] = afunc;
            num_of_func++;   
            bits_cpy = clean_string(bit,bits_cpy,3); // take 3 bits for the label


            for(int i = 0 ; i < 5 ; i++){ //get the padding, test if it is instruction or padding
                strcat_c(bit,bits_cpy[i]);
            }
            if(op_cmp(bit,0)){
                break;
            }
            else{
                instructions = 0;     //otherwise add a new function
                count = 0;
                reverse_string(bits_cpy);
                for(int i = 0 ; i < 5 ; i++){
                    strcat_c(bit,bits_cpy[i]);
                }
                reverse_string(bits_cpy);
                reverse_string(bit);

                instructions = strtol(bit, &newline, 2);
                bits_cpy = clean_string(bit,bits_cpy,5);

                for(int i = 0 ; i < 3 ; i++){
                    strcat_c(bit,bits_cpy[i]);
                }
                if(op_cmp(bit,RET) == 1){
                    bits_cpy = clean_string(bit,bits_cpy,3);
                }
                
                count++;
            }
        }
        
    
        int opcode = com_all_op(bit,bits_cpy);//comparing opcodes
        bool only_one = false;
        bits_cpy = clean_string(bit,bits_cpy,3);
        // printf("after opcode: %s\n", bits_cpy);
        if((opcode == 1) | (opcode == 5) | (opcode == 6) | (opcode == 7)){
            only_one = true;
        }
            

        switch(opcode){
            case 0:
            strcpy(aop.opcode, "MOV");
            break;

            case 1:
            strcpy(aop.opcode, "CAL");
            break;

            case 2:
            strcpy(aop.opcode, "RET");
            break;

            case 3:
            strcpy(aop.opcode, "REF");
            break;

            case 4:
            strcpy(aop.opcode, "ADD");
            break;

            case 5:
            strcpy(aop.opcode, "PRINT");
            break;

            case 6:
            strcpy(aop.opcode, "NOT");
            break;

            case 7:
            strcpy(aop.opcode, "EQU");
            break;

            default:
                printf("Invalid opcode");
        }
        if(opcode == 2){
            afunc.operations[count] = aop;
            count++;
            continue;
        }

        int type = com_all_type(bit,bits_cpy); //comparing types
        bits_cpy = clean_string(bit,bits_cpy,2);
        int bin;
        
        switch(type){
           case 0:
            strcpy(aop.type1, "VAL");          // allocate bits for each different type
            bin = str2bin(bits_cpy,8);
            aop.val1 = bin;
            bits_cpy = clean_string(bit,bits_cpy,8);
            break;

            case 1:
            strcpy(aop.type1, "REG");
            bin = str2bin(bits_cpy,3);
            aop.val1 = bin;
            bits_cpy = clean_string(bit,bits_cpy,3);
            break;

            case 2:
            strcpy(aop.type1, "STK");
            bin = str2bin(bits_cpy,5);
            aop.val1 = bin;
            bits_cpy = clean_string(bit,bits_cpy,5);
            
           
            break;

            case 3:
            strcpy(aop.type1, "PTR");
            bin = str2bin(bits_cpy,5);
            aop.val1 = bin;
            bits_cpy = clean_string(bit,bits_cpy,5);
            
            break;

            default:
                printf("Invalid type:%d",type);
        }

        if(only_one == true){//to see if its's the opcode that only accepts one type and one value
            only_one = false;
            afunc.operations[count] = aop;
            count++;
        
            continue;
        }
        
        type = com_all_type(bit,bits_cpy);
        bits_cpy = clean_string(bit,bits_cpy,2);
        // // printf("%s\n", bits_cpy);
        switch(type){
           case 0:
            // printf("0\n");
            strcpy(aop.type2, "VAL");  // second type and value
            // printf("%s\n",bits_cpy);
            bin = str2bin(bits_cpy,8);
            aop.val2 = bin;
            bits_cpy = clean_string(bit,bits_cpy,8);
            break;

            case 1:
            // printf("1\n");
            strcpy(aop.type2, "REG");
            bin = str2bin(bits_cpy,3);
            aop.val2 = bin;
            bits_cpy = clean_string(bit,bits_cpy,3);
            break;

            case 2:
            // printf("2\n");
            strcpy(aop.type2, "STK");
            bin = str2bin(bits_cpy,5);
            aop.val2 = bin;
            bits_cpy = clean_string(bit,bits_cpy,5);      
            break;

            case 3:
            // printf("3\n");
            strcpy(aop.type2, "PTR");
            bin = str2bin(bits_cpy,5);
            aop.val2 = bin;
            bits_cpy = clean_string(bit,bits_cpy,5);
            break;

            default:
                printf("Invalid type:%d",type);
        }
        afunc.operations[count] = aop;   // add the aop to the array of operations of a function

        
        // memset(line, 0, 255);
        count++;
        
    }  
    return num_of_func;
}

//main testing to find out the paddings
extern void test(char* all_bits){
    struct function functions[8];
    int num_of_func = examining(all_bits,functions);
    ReverseSTRUCT1(functions,num_of_func);

    int stk = 0;
    char symbol;
    bool exist = false;
    int symbol_arr[52]; //the symbol array keeps tracking of the num of symbols
    int symbolin = 0;  //the length of symbol
    for(int i = 0;i < num_of_func;i++){
        printf("FUNC LABEL %d\n",functions[i].label);
        ReverseSTRUCT2(functions[i].operations,functions[i].num_of_instructions); //reverse struct in order to print 
        for(int j = 0; j < functions[i].num_of_instructions;j++){
            struct operation aop = functions[i].operations[j];
            if((strcmp(aop.type1,"STK") == 0) |((strcmp(aop.type1,"PTR") == 0))){ //handling the printing of symbols 
                for(int x = 0;x < symbolin;x++){
                    if(aop.val1 == symbol_arr[x]){ // if the address exist then symbol is the same 
                        stk = x;
                        exist = true;
                    }
                }
                if(exist == false){
                    symbol_arr[symbolin] = aop.val1; //if not print a new symbol
                    symbolin++;
                }
            }
            else if((strcmp(aop.type2,"STK") == 0) |(strcmp(aop.type2,"PTR") == 0)){
                for(int x = 0;x < symbolin;x++){
                    if(aop.val2 == symbol_arr[x]){
                        stk = x;
                        exist = true;
                    }
                }
                if(exist == false){
                    symbol_arr[symbolin] = aop.val2;
                    symbolin++;
                }
            }
            
            if((strcmp(aop.opcode,"PRINT") == 0) | (strcmp(aop.opcode,"EQU") == 0) 
            | (strcmp(aop.opcode,"NOT") == 0) |(strcmp(aop.opcode,"CAL") == 0)){
                if((strcmp(aop.type1,"STK") == 0) | (strcmp(aop.type1,"PTR") == 0)){
                    if(exist == true){
                        symbol = stk + 65;
                        exist = false;
                    }
                    else{
                        symbol = 64 + symbolin;
                    }
                    
                    printf("    %s %s %c\n",aop.opcode,aop.type1,symbol);
                }
                else
                printf("    %s %s %d\n",aop.opcode,aop.type1,aop.val1);
            }
            else if (strcmp(aop.opcode,"RET") == 0){
                printf("    %s\n",aop.opcode);
            }
            else{
                if((strcmp(aop.type1,"STK") == 0) | (strcmp(aop.type1,"PTR") == 0) && //circumstances accepting two types
                (strcmp(aop.type2,"STK") == 0) | (strcmp(aop.type2,"PTR") == 0)){
                    if(exist == true){
                        symbol = stk + 65;
                        exist = false;
                    }
                    else{
                        symbol = 64 + symbolin;
                    }
                    
                    printf("    %s %s %c ",aop.opcode,aop.type1,symbol);
                    fflush(stdout);
                    for(int x = 0;x < symbolin;x++){
                        if(aop.val2 == symbol_arr[x]){
                            stk = x;
                            exist = true;
                        }
                    }
                    if(exist == false){
                        symbol_arr[symbolin] = aop.val2;
                        symbolin++;
                    }
                    if(exist == true){
                        symbol = stk + 65;
                        exist = false;
                    }
                    else{
                        symbol = 64 + symbolin;
                    }
                    
                    printf("%s %c\n",aop.type2,symbol);
                }
                else if((strcmp(aop.type1,"STK") == 0) | (strcmp(aop.type1,"PTR") == 0) ){//without the need to handle stk and ptr
                    if(exist == true){
                        symbol = stk + 65;
                        exist = false;
                    }
                    else{
                        symbol = 64 + symbolin;
                    }
                    
                    printf("    %s %s %c %s %d\n",aop.opcode,aop.type1,symbol,aop.type2,aop.val2);
                }
                else if((strcmp(aop.type2,"STK") == 0) | (strcmp(aop.type2,"PTR") == 0) ){
                    if(exist == true){
                        symbol = stk + 65;
                        exist = false;
                    }
                    else{
                        symbol = 64 + symbolin;
                    }
                    
                    printf("    %s %s %d %s %c\n",aop.opcode,aop.type1,aop.val1,aop.type2,symbol);
                }
                else
                printf("    %s %s %d %s %d\n",aop.opcode,aop.type1,aop.val1,aop.type2,aop.val2);
            } 
            
        }
        for(int i = 0;i < symbolin;i++)
            symbol_arr[i] = 0;
        symbolin = 0;
    }
    // printf("%d",symbol_arr[1]);
}