// C program to implement one side of FIFO
// This side reads first, then reads

#include "servers.h"
int main()
{
	int fd1;
	DIR *d;
    struct dirent *dir;
	char * myfifo = global_proc;
	char * RD = "./connect/BOM_RD";
	unsigned char str2[5000] = {[2] = 'B',[3] = 'O',[4] = 'M',[258] = 'c',[259] = 'o',[260] = 'n',[261] = 'n',[262] = 'e',[263] = 'c',[264] = 't'};
	fd1 = open(myfifo,O_WRONLY);	
	write(fd1, str2, 4000);
	sleep(1);
	if(mkdir("connect", 0700) == -1){
        printf("pipe in directory");
    }
	return 0;
}
