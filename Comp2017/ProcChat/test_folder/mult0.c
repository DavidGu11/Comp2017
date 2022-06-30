// C program to implement one side of FIFO
// This side reads first, then reads

#include "servers.h"
int main()
{
	int fd1;
	char * myfifo = global_proc;
	char * RD = "./multi/BOM_RD";
    // char * myfifo = global_proc;
	// char * RD = "./ba/BOM_RD";
	unsigned char str2[5000] = {[2] = 'G',[3] = 'O',[4] = 'M',[258] = 'm',[259] = 'u',[260] = 'l',[261] = 't'};
    // unsigned char str2[5000] = {[2] = 'C',[3] = 'O',[4] = 'M',[258] = 'b',[259] = 'a'};
	fd1 = open(myfifo,O_WRONLY);	
	write(fd1, str2, 4000);
    sleep(1);
	if(mkdir("mult", 0700) == -1){
        printf("pipe in directory");
    }
	return 0;
	

	
}
