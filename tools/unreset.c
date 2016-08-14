#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(unsigned int argc,unsigned char* arglist[])
{
	int fd;
	int Value=0;
	if( (fd=open(arglist[1],O_RDWR)) !=-1 ) 
	{
		printf("Inicio\x07\n");
		Value |= TIOCM_DTR;
		ioctl(fd,TIOCMSET,&Value);
		printf("Reset\n");
		Value |= TIOCM_RTS;
		ioctl(fd,TIOCMSET,&Value);
		printf("Unboot 3v\n");
		Value &= ~TIOCM_DTR;
		ioctl(fd,TIOCMSET,&Value);
		printf("Unreset\x07\n");
		sleep(2);
 	}
	else
		printf("Error\n");
}
