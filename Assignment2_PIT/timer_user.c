#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>


#include "timer.h"

unsigned int val;

int delay_ms(int fd,unsigned int val)
{

if (ioctl(fd, delay, &val) == -1)
    {
        perror("device failed");
exit(-1);
    }
    else
    {
        printf("delay %ums implemented \n",val);
       
         return 0;
   }

}


int main(int argc,char *argv[])
{
	int fd,ip,reg1,count;
	char buf[2];
	//ch = atoi(argv[1]);
	fd = open("/dev/TIMER0",0);
	if(fd<0){
	printf("cannot open file: %d\n",fd);
	exit(-1);
	}
	
	printf(" Enter 1 to read counter and 2 for using delay\n");
	
	scanf("%d",&ip);
	switch(ip)
	{
          case 1:
	  reg1=read(fd,buf,2);
	  count=(unsigned char)buf[0];
	  count=count|((unsigned char)buf[1]<<8);
	  printf("counter value is %d\n",count);
	  break;
	  case 2:
	 
	  printf(" enter required delay \n");
	
	  scanf("%u",&val);
  	  delay_ms(fd,val);	
	  break;
	  default:
	  printf("command not found\n");
	}
	
  	return 0;
}
