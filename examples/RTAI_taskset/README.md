Objective: Implement tasks 3, 4 and 5 from "Putting Preemptive Time Petri Nets to Work in a V-Model SW Life Cycle" by Carnevali, Ridi, Vicario, 2011.  
  
How to compile read.c : gcc -I . -fno-stack-protector read.c -o read  
How to compile kernel module: make -f Makefile  

In order to run the module, first run ```startRTAI``` then ``` sudo insmod taskset.ko```.  
When the experiment is completed, type ``` sudo rmmod taskset`` and ``` stopRTAI```. 
