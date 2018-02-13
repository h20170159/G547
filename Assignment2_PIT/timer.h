#ifndef TIMER_H
#define TIMER_H

#include <linux/ioctl.h>

#define MAGIC 'T'
 
 
#define delay _IOW(MAGIC,0,unsigned int)

#endif
