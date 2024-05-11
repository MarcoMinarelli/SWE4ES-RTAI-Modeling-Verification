# RTAI Rate Monotonic

## Objective
Create 3 periodic tasks with different frequencies and schedule them with a Rate Monotonic Scheduling Algorithm

## How to compile
make .  

## Execute the module
To start the module first load hal and sched module (```sudo insmod /usr/realtime/modules/rtai_hal.ko && sudo insmod /usr/realtime/modules/rtai_sched.ko``` or use ```startRTAI```), then type ```sudo insmod RTAI_RM.ko``` .  
To read the message written by printk, type ```sudo tail -f /var/log/kern.log``` (not the more elegant way to do it, but it works).
When you want to remove the module, type ```sudo rmmod RTAI_RM``` and ```stopRTAI```.  

