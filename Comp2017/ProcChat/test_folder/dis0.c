// C program to implement one side of FIFO
// This side reads first, then reads

#include "servers.h"
int main()
{
	int fd1;
	int fd2;
	DIR *d;
    struct dirent *dir;
	char * myfifo = global_proc;
	char * WR = "./dis/BOM_WR";
	unsigned char str2[5000] = {[2] = 'B',[3] = 'O',[4] = 'M',[258] = 'd',[259] = 'i',[260] = 's'};
    unsigned char msg[5000] = {[0] = '7' - '0'};
	fd1 = open(myfifo,O_WRONLY);	
	write(fd1, str2, 4000);
	fd2=open(WR, O_WRONLY);
	write(fd2, msg, 2048);
    if(mkdir("dis", 0700) != -1){
        printf("disconnect successful");
    }
	return 0;
}
