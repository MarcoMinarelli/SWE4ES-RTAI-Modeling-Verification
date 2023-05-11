Objective: create 3 periodic tasks with different frequencies and schedule them with a Rate Monotonic Scheduling Algorithm
How to compile: make .  
To start the module first load hal and sched module (sudo insmod /usr/realtime/modules/rtai_hal.ko && sudo insmod /usr/realtime/modules/rtai_sched.ko), then type sudo insmod prova.ko.  
To read the message written by printk, type sudo tail -f /var/log/kern.log (not the more elegant way to do it, but it works).
