// C program to implement one side of FIFO
// This side reads first, then reads

#include "servers.h"
int main()
{
	int fd1;
	DIR *d;
    struct dirent *dir;
	char * myfifo = global_proc;
	char * RD = "./multi/BOM_RD";
	unsigned char str2[5000] = {[2] = 'B',[3] = 'O',[4] = 'M',[258] = 'm',[259] = 'u',[260] = 'l',[261] = 't'};
	fd1 = open(myfifo,O_WRONLY);	
	write(fd1, str2, 4000);
    
	
	return 0;
	

	
}
