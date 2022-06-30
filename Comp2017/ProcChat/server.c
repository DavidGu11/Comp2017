

#include "server.h"

int ends_with(const char *str, const char *suffix) {
    //if a string ends with 
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    return (str_len >= suffix_len) &&
            (!memcmp(str + str_len - suffix_len, suffix, suffix_len));
}

void signalHandler(int signalNum) {
    //handling sigusr1
  if (signalNum == SIGUSR1) {
      wait(NULL);
  }
}

int isDirectoryEmpty(char *dirname) {
    //determine if a dir is empty
    int n = 0;
    struct dirent *d;
    DIR *dir = opendir(dirname);
    if (dir == NULL) //Not a directory or doesn't exist
    return 1;
    while ((d = readdir(dir)) != NULL) {
    if(++n > 2)
        break;
    }
    closedir(dir);
    if (n <= 2) //Directory Empty
    return 1;
    else
    return 0;
    }

int main()
{
    //looking at signal
    signal(SIGUSR1, signalHandler);

    char * myfifo = global_proc;

    //create global pipe
    if(mkfifo(myfifo,0666)<0)
    {   
        printf("Error in creating named pipe\n");   
        perror("fifo creation error: ");
        return -1;
    }  

    while(1){

        //if invalid == 1 which means message is invalid then continue
        int invalid = 0;

        //file descriptor for global pipe
        int fd; 

        //buffer for read 
        char arr1[2048];
        fd = open(myfifo, O_RDONLY);
        read(fd, arr1, 2048);

        //look at message to see if it starts with 0 then connect 
        if(((int)arr1[0]  == 0) && ((int)arr1[1]  == 0)){

            // find domain, RD and WR pipes 
            char current_client[256] = "";
            char iden[256] = "";
            char domain[1790] = "";
            char RD[256] = "./";
            char WR[256] = "./";
            struct stat st = {0};
            strncat(iden, &arr1[2], 256);
            strcpy(current_client,&arr1[2]);
            strncat(domain, &arr1[258], 1790);

            //continue if domain does not exist 
            if((int) domain[0] == 0){
                continue;
            }

            for(int i = 0;i < strlen(domain);i++){
                //path of domain and pipes cannot contain invalid entries 
                if((domain[i] == '\\') |  (domain[i] == '.')){
                    invalid = 1;
                }
                strncat(RD,&domain[i],1);
                strncat(WR,&domain[i],1);
            }

            if(invalid == 1){
                invalid = 0;
                continue;
            }

          

            // if domain does not exist then create the domain
            if (stat(RD, &st) == -1) {
                mkdir(RD, 0700);
            }

            strncat(RD,"/",3);
            strncat(WR,"/",3);
            
            for(int i = 0;i < strlen(iden);i++){
                strncat(RD,&iden[i],1);
                strncat(WR,&iden[i],1);
            }
            
            //create pipes
            strncat(RD,"_RD",4);
            strncat(WR,"_WR",4);

            if(mkfifo(RD,0666)<0)
            {   
                perror("fifo creation error: ");
                return -1;
            }  
            if(mkfifo(WR,0666)<0)
            {      
                perror("fifo creation error: ");
                return -1;
            }  

            // *************//
            // fork starting//
            // *************//
            pid_t pid = fork();
            int fd_rd;
            char temp[2048];
            
            if(pid == 0){

                close(fd);          
                fd_rd=open(WR, O_RDONLY);
                char message[2048];
                memset(temp,0,sizeof(temp));
                
                while(1){  

                    read(fd_rd, temp, 2048);
                    if((int)temp[0] == 0){
                        continue;
                    }
                  

                    //Say and receive
                    if(((int)temp[0]  == 1) && ((int)temp[1] == 0)){
        
                        DIR *d;
                        struct dirent *dir;
                        int fd_WR;
                        
                        strcpy(message, &temp[2]);
                        for(int i = 0;i < 1790;i++){
                            temp[0] = '\0';
                        }

                        //open directory to to progagate message to chat room
                        d = opendir(domain);
                       
                        while ((dir = readdir(d)) != NULL)
                        {

                            if(ends_with(dir->d_name,"RD")){
                            //make full path for the propagation
                                char full_address[2048] = "./";
                                strncat(full_address,domain,1790);
                                strncat(full_address,"/",3);
                                strncat(full_address,dir->d_name,258);

                                //skip self
                                if(strcmp(RD,full_address)== 0){     
                                    continue;
                                }

                                //relay message with the type 
                                char send[2048] = {[0] = 3};

                                int c = 0;
                                for(int i = 2; i < strlen(current_client)+2;i++){
                                    send[i] = current_client[c];
                                    c++;  
                                }

                                c = 0;
                                for(int i = 258; i < strlen(message)+258;i++){
                                    send[i] = message[c];
                                    c++;    
                                }
            


                                fd_WR = open(full_address,O_WRONLY);
                                //sent message
                                write(fd_WR,send,2048);
                                close(fd_WR);
                            }
                        }
                        closedir(d);
                    } 

                    //Saycont 
                    else if(((int)temp[0]  == 2) && ((int)temp[1] == 0)){
                        DIR *d;
                        struct dirent *dir;
                        int fd_WR;
                        
                        strcpy(message, &temp[2]);
                        for(int i = 0;i < 1790;i++){
                            temp[0] = '\0';
                        }
                        for(int i = 0; i < 2047;i++){
                            if(temp[i] == 255){
                                fprintf(stderr,"no %d",i);
                            }
                        }


                        //receive
                        d = opendir(domain);
                       
                        while ((dir = readdir(d)) != NULL)
                        {

                            if(ends_with(dir->d_name,"RD")){

                                char full_address[2048] = "./";
                                strncat(full_address,domain,1790);
                                strncat(full_address,"/",3);
                                strncat(full_address,dir->d_name,258);

                                if(strcmp(RD,full_address)== 0){
                                    continue;
                                }

                                //relaying message with type 
                                char send[2048] = {[0] = 4};

                                int c = 0;
                                for(int i = 2; i < strlen(current_client)+2;i++){
                                    send[i] = current_client[c];
                                    c++;  
                                }

                                c = 0;
                                for(int i = 258; i < strlen(message)+258;i++){
                                    send[i] = message[c];
                                    c++;    
                                }
                                
                                //append termination
                                send[2047] = temp[2047];

                                fd_WR = open(full_address,O_WRONLY);
                                write(fd_WR,send,2048);
                                // close(fd_WR);
                            }
                        }
                        close(fd_WR);
                        closedir(d);
 
                    }

                    //Disconnect tje
                    else if(((int)temp[0]  == 7) && ((int)temp[1] == 0)){
                        unlink(RD);
                        unlink(WR);
                        int i = isDirectoryEmpty(domain);
                        if(i == 1){
                            rmdir(domain);
                        }
                        kill(getppid(),SIGUSR1);
                        return 0;
                    }
                }
            }
            else if ((pid) == -1){
                fprintf(stderr, "Call to fork failed.\n"); 
                exit(1);
            }
        }
    }
    return 0;
}