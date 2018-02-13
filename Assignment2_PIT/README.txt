this code is implementation of timer using 8254(Programmable Timer Interval)


here user can implement delay as his wish ,to do this you have to do:
insert module using insmod command
$ make
$ sudo insmod timer.ko

compile timer_user.c file 
$ gcc -o timer_user timer_user.c
$ sudo ./timer_user

dmesg

