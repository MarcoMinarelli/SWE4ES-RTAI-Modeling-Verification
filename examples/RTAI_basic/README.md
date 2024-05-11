# Basic RTAI tasks

## Objective
Create a one shot task that prints Hello World

## How to compile
make .

## Execute the module
In order to run the module, first run ```startRTAI``` then ```sudo insmod RTAI_basic.ko```.  
When you want to remove the module, type ```sudo rmmod RTAI_basic``` and ```stopRTAI```.   

To start the module first load hal and sched module (sudo insmod /usr/realtime/modules/rtai_hal.ko && sudo insmod /usr/realtime/modules/rtai_sched.ko or use startRTAI), then type sudo insmod .  
To read the message written by printk, type sudo tail -f /var/log/kern.log (not the more elegant way to do it, but it works).
