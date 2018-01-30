#ifndef ADC_IOCTL_H
#define ADC_IOCTL_H
#include <linux/ioctl.h>
 
typedef struct
{
    int channel, alignment;
} adc_arg_t;
 
#define ADC_GET_VARIABLES _IOR('a', 'b', adc_arg_t *)

#define ADC_SET_VARIABLES _IOW('a', 'a', adc_arg_t *)
 
#endif
