#include<stdio.h>
#include <unistd.h>            
#include <sys/ioctl.h> 
#include<fcntl.h>
#include"adc_ioctl.h"
#include <sys/types.h>
#include<string.h>
#define DEVICE "/dev/adc0"
//#define M "MSB first"
//#define L "lSB first"

void get_vars(int fd)
{
    adc_arg_t q;
 
    if (ioctl(fd, ADC_GET_VARIABLES, &q) == -1)
    {
        perror("adc_apps ioctl get");
    }
    else
    {
        printf("channel: %d\n", q.channel);
        printf("alignment: %d\n", q.alignment);
        
    }
}
void set_vars(int fd)
{
    int v;
char u;
    adc_arg_t q;
    printf("Enter channel no: ");
    scanf("%d", &v);
while(v>8){
printf("enter a correct channel from 1 to 8\n");
 printf("Enter channel : ");
    scanf("%d", &v);

}
    getchar();
    q.channel = v;


printf("a=MSB first\nb=LSB first\nenter your choice of algnment:");
scanf("%c",&u);

 getchar();
    q.alignment = u;
 
    if (ioctl(fd, ADC_SET_VARIABLES, &q) == -1)
    {
        perror("adc_apps ioctl set");
    }
}

int main()
{
int i ,fd;
//char ch,write_buf[100],read_buf[100];
fd=open(DEVICE,0);
if(fd==-1){
printf("file %s does not exist\n",DEVICE);
}
printf("r=output from device\nw=input to device\nenter command:");
scanf("%c",&ch);

switch(ch)
{
case 'w':
	set_vars(fd);
	
	break;
case 'r':
	get_vars(fd);
	
	break;





default:
	printf("command not found\n");
	break;
}
close(fd);

return 0;
}
