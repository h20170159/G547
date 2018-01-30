#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include<linux/random.h> 
#include "adc_ioctl.h"
 
#define FIRST_MINOR 0
#define MINOR_CNT 1
 
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
static int channel = 1, alignment = 1;
 
static int my_open(struct inode *i, struct file *f)
{ printk(KERN_INFO "Device File Opened...!!!\n");
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{printk(KERN_INFO "Device File Closed...!!!\n");
    return 0;
}
static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Read Function\n");
    return 0;
}
 
static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Write function\n");
    return 0;
}
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    adc_arg_t q;
int i,val;
get_random_bytes(&i,sizeof(i));
val=(i%1024);
printk(KERN_INFO "value=%d",val);

 
    switch (cmd)
    {
        case ADC_GET_VARIABLES:
            q.channel = channel;
            q.alignment = alignment;
            if (copy_to_user((adc_arg_t *)arg, &q, sizeof(adc_arg_t)))
            {
                return -EACCES;
            }
            break;
        case ADC_SET_VARIABLES:
            if (copy_from_user(&q, (adc_arg_t *)arg, sizeof(adc_arg_t)))
            {
                return -EACCES;
            }
 printk(KERN_INFO "q= %d\n", q);
            channel = q.channel;
            alignment= q.alignment;
            break;
        default:
            return -EINVAL;
    }
 
    return 0;
}
 
static struct file_operations fops =
{   
    .owner = THIS_MODULE,
    .read  = my_read,
    .write = my_write,
    .open = my_open,
    .release = my_close,
    .unlocked_ioctl = my_ioctl

};
 
static int __init adc_ioctl_init(void)
{
 if ((alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "adc_ioctl")) < 0)
    {
 
        return -1;
    }
  printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
    
cdev_init(&c_dev, &fops);
c_dev.owner = THIS_MODULE;
        c_dev.ops = &fops;
 
    if ((cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
    {
        printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
    }
     
    if ((cl = class_create(THIS_MODULE, "char"))==NULL)
    { printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
}
     //   cdev_del(&c_dev);
       // unregister_chrdev_region(dev, MINOR_CNT);
        //return PTR_ERR(cl);
    //}
    if ((device_create(cl, NULL, dev, NULL, "adc0"))==NULL)
    {printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
    }
 
    printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;

r_device:
        class_destroy(cl);
r_class:
        unregister_chrdev_region(dev, MINOR_CNT);
        return -1;
}
 
static void __exit adc_ioctl_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
 
module_init(adc_ioctl_init);
module_exit(adc_ioctl_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MONIKA VIJAY");
MODULE_DESCRIPTION("ADC ioctl() Char Driver");
