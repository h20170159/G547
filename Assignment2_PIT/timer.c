#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/kernel.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>

#include <asm/uaccess.h>
#include <linux/kdev_t.h>
#include <linux/random.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include "timer.h"


#define channel2 0x42
#define command_reg 0x43


static dev_t dev;
static struct cdev c_dev;
static struct class *cls;

unsigned int val;
unsigned int counter=0;

void delay_ms(int val)
{
	int dl,v;
	dl = counter + val;
	while(counter<dl)
	{
	   if(((v=inb(0x61))&0x20)==0x20)
		if(((v=inb(0x61))&0x20)==0x00)
		{
		counter++;
		
		}
	}
	counter =0;
}

long timer_ioctl(struct file *file,unsigned int cmd,unsigned long ioctl_param)
{
	if(cmd==delay){
	get_user(val,(unsigned int *)ioctl_param); 								
	delay_ms(val);
	}
	return 0;
}

static int timer_open(struct inode *i,struct file *f)
{
	printk(KERN_INFO "device opened\n");
	return 0;
}

static int timer_close(struct inode *i,struct file *f)
{
	printk(KERN_INFO "device closed\n");
	return 0;
}

static ssize_t timer_read(struct file *f,char __user *buf,size_t len,loff_t *off)
{
	char ret[2];
	printk(KERN_INFO "device read \n");
	outb(0x20,command_reg);
	ret[0]=inb(channel2);//lo-by
	put_user(ret[0],buf);
	ret[1]=inb(channel2);//hi-by
	put_user(ret[1],buf+1);
	return 0;
}

static ssize_t timer_write(struct file *f,const char __user *buf,size_t len,loff_t *off)
{
	printk(KERN_INFO "device write\n");
	return 0;
}

static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open = timer_open,
	.release = timer_close,
	.read = timer_read,
	.write = timer_write,
	.unlocked_ioctl = timer_ioctl
};


static int __init timer_init(void)
{	
	int count,q,q1,q2;
	printk(KERN_INFO "TIMER0 entered \n\n");
	
	if(alloc_chrdev_region(&dev,0,1,"TIMER0")<0)
	{
		return -1;
	}
	printk(KERN_INFO "Major and Minor are %d & %d\n",MAJOR(dev),MINOR(dev));
	
	
	
	if((cls = class_create(THIS_MODULE,"TIMERDRV")) == NULL)
	{
		unregister_chrdev_region(dev,1);
		return -1;
	}
	
	if(device_create(cls,NULL,dev,NULL,"TIMER0")==NULL)
	{
		class_destroy(cls);
		unregister_chrdev_region(dev,1);
		return -1;
	}
	
	
	cdev_init(&c_dev,&fops);
	
	if(cdev_add(&c_dev,dev,1) == -1)
	{
		device_destroy(cls,dev);
		class_destroy(cls);
		unregister_chrdev_region(dev,1);
		return -1;
	}
	
	printk(KERN_INFO "control word loaded\n");
	q=inb(0x61);
	
	
	count=1193;
	outb(0xB6,command_reg);// ch2,Mode 3 ,binary,access lo/hi
	outb((q | 0x01),0x61);
	outb(count&0xff,channel2);//lo_b	
	outb(count >> 8,channel2);//hi_b
	printk(KERN_INFO "control word loaded\n");
	q2=inb(0x61);
	printk(KERN_INFO "output = %d\n",(q2&0x20));	
	outb(0x20,command_reg);
	q1=inb(channel2);
	q1=q1|(inb(channel2)<<8);
	printk(KERN_INFO "counter =%d\n",q1);
	return 0;
}


static void __exit timer_exit(void)
{
	cdev_del(&c_dev);
	device_destroy(cls,dev);
	class_destroy(cls);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO "TIMER0 leaving");
}

module_init(timer_init);
module_exit(timer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MONIKA");
MODULE_DESCRIPTION("TIMER0");

