// C program to implement one side of FIFO
// This side reads first, then reads
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "servers.h"
int main()
{
	int fd1;
	int fd2;
	char temp[2050];
	char * myfifo = global_proc;
	char * RD = "./Cle/BOM_RD";
	unsigned char str2[5000] = {[2] = 'B',[3] = 'O',[4] = 'M',[258] = 'C',[259] = 'l',[260] = 'e'};
	fd1 = open(myfifo,O_WRONLY);
	
	write(fd1, str2, 4000);
	int testInteger;
	
	while(1){	
		fd2 = open(RD,O_RDWR);
		// int flags = fcntl(fd1, F_GETFL, 0);
		// fcntl(fd1, F_SETFL, flags | O_NONBLOCK);
		// if (read(fd2,temp,2048) <=0)
		// 	continue;

		if((int)temp[0] == 0){
			continue;
		}
		for(int i =2; i < 2047;i++){
			printf("%c",temp[i]);
		}		
		close(fd1);
			
	}
	// }
	return 0;	
}
