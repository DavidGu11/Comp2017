#include <stdio.h>
#include <stdlib.h>

#define BUFFERSIZE 4096

int str_len(char str[]){
    int count = 0;
    for (int i = 0; str[i] != '\0'; ++i){
        count++;
    }
    return count;
}

void strcat_c (char *str, char c)
{
    for (;*str;str++);  
    *str++ = c; 
    *str++ = 0; 
}

int str_cmp(char* str1,char* str2)
{
    int size1 = str_len(str1);
    int size2 = str_len(str2);

    if (size1 != size2){
        return 0;
    }

    for(int i=0;str1[i] !='\0' && str2[i] !='\0';i++)
    {
        if (str1[i] != str2[i])
            return 0;
    }

    return 1;
}

void change_fitime(int ch, int cm, int cs, int h, int m , int s, char* fitime,char* time)
{
    ch = h;
    cm = m;
    cs = s;
    for(int c = 0;c < str_len(fitime);c++){
        fitime[c] = '\0';
    }
    for(int c = 0;c < str_len(time);c++){
        strcat_c(fitime,time[c]);
    }
}

int main(int argc, char* argv[]) {
    char buffer[BUFFERSIZE];
    char source[4096];
    char dest[4096];
    char time[4096];
    char fitime[4096];
    char lowest[4096];
    int mo;
    int h;
    int m;
    int s; //seperated hour, minute and second for each line
    int ah;
    int am;
    int as; //command line entered time
    int ch;
    int cm;
    int cs; //the time that is going to be printed(final)
    int lh;
    int lm;
    int ls;  //lowest time, will be used if there's no available train today, it will be the earlist tomorrwow
 
    if(argc != 4){
        printf("Please provide <source> <destination> <time> as command line arguments");
        return 0;
    }
    
    
    while(fgets(buffer, BUFFERSIZE , stdin) != NULL)
    {
        // printf("%s",buffer);
        for(int j = 0;j < str_len(buffer)-1;j++){
            if(buffer[j] == ':' && buffer[j+1] == ':'){//if if the index and index + 1 all have delimiter then treat as seperation
                if(buffer[j+2] == ':'){//handle the situation of ':::'
                    break;
                }
                for(mo = 0;mo < j;mo++){
                    if(buffer[mo] == ':'){
                        break;
                    }
                    strcat_c(source, buffer[mo]);
                }
                while(buffer[j+2] != '\0'){
                    if(buffer[j+2] == ':' && buffer[j+3] == ':'){
                        if(buffer[j+4] == ':'){
                            break;
                        }
                        for(;j < str_len(buffer)-3;j++){
                            strcat_c(time,buffer[j+4]);
                        }
                        break;
                    }
                    strcat_c(dest, buffer[j+2]);
                    j++;
                }
                sscanf(time, "%d:%d:%d", &h, &m, &s);
                if((h > 23) | (m>59) | (s >59)){
                    break;
                }

                 if((str_cmp(argv[1],source) == 1) && (str_cmp(argv[2],dest))== 1){//comparing if source and dest equal
                    if((ch == 0) &&(cm == 0) && (cs == 0)){ //set the lowest time and final time to first line
                        ch = h;
                        cm = m;
                        cs = s;
                        lh = h;
                        lm = m;
                        ls = s;
                        for(int c = 0;c < str_len(time);c++){
                            strcat_c(lowest,time[c]);
                        }
                        for(int c = 0;c < str_len(time);c++){
                            strcat_c(fitime,time[c]);
                        }
                    }
                    else{
                        sscanf(argv[3], "%d:%d:%d", &ah, &am, &as); //read time in command line
                        if((h < lh) |((h == lh) && (m< lm)) | ((h == lh) && (m == lm) && (s<ls))){
                            //if current lowest time is higher than the line, then change the lowest time
                            lh = h;
                            lm = m;
                            ls = s;
                            for(int c = 0;c < str_len(lowest);c++){
                                lowest[c] = '\0';
                            }
                            for(int c = 0;c < str_len(time);c++){
                                strcat_c(lowest,time[c]);
                            }
                        }
                        if(h >= ah){ // compare hour
                            if(( (ch-ah < 0) | ((ch-ah == 0) & (cm-am <0)))&& (!(h-ah <0))){
                                ch = h;
                                cm = m;
                                cs = s;
                                for(int c = 0;c < str_len(fitime);c++){
                                    fitime[c] = '\0';
                                }
                                for(int c = 0;c < str_len(time);c++){
                                    strcat_c(fitime,time[c]);
                                }
                            }
                            else if((h - ah) == (ch -ah)){ //compare minute
                                if(((m-am)<(cm-am)) &&(!(m-am <0))){
                                    change_fitime(ch, cm, cs, h, m , s, fitime,time);
                                }
                                else if((m-am) == (cm-am)){ // compare second
                                    if((s-as)<(cs-as)&&(!(s-as <0))){
                                        change_fitime(ch, cm, cs, h, m , s, fitime,time);
                                    }         
                                }
                            }
                        }     
                    }                   
                }
                for(int c = 0;c < str_len(source);c++){//truncate source, dest and time
                    source[c] = '\0';
                }
                for(int c = 0;c < str_len(dest);c++){
                    dest[c] = '\0';
                }
                for(int c = 0;c < str_len(time);c++){
                    time[c] = '\0';
                }

            }
        }
    }
    int oh;
    int om;
    int os;
    sscanf(fitime, "%d:%d:%d", &oh, &om, &os);
    if(*fitime == 0){
        printf("No suitable trains can be found");
        printf("\n");
    }

    else if(((oh < ah) |((oh == ah) && (om< am)) | ((oh == ah) && (om == am) && (os<as)))){
        printf("The next train to %s from %s departs at %s",argv[2],argv[1],lowest);
    }
    else{
        printf("The next train to %s from %s departs at %s",argv[2],argv[1],fitime);
    }
    return 0;
}