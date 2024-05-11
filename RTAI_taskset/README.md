# RTAI Taskset based on selected paper

## Objective
Implement tasks 3, 4 and 5 from "Putting Preemptive Time Petri Nets to Work in a V-Model SW Life Cycle" by Carnevali, Ridi, Vicario, 2011.  

## How to compile  
How to compile read.c : gcc -I . -fno-stack-protector read.c -o read  
How to compile kernel module: make -f Makefile  

## Execute the module
In order to run the module, first run ```startRTAI``` then ```sudo insmod taskset.ko```.  
When the experiment is completed, type ```sudo rmmod taskset``` and ```stopRTAI```.  
In order to print the logs one must type ```./read```. The program will tell when all the logs have been written.

## Contents
In read.c file there's the code for reading the logging from the queue and write them into a texr file named log.txt.  
In calib.h there are the function needed for 1) computing the m and q constant used for implementing busy sleepand 2) function to actually use the ```busy_sleep()```. As now, those values are hard coded.  
values.h contains all the values and the structs needed for logging and computing parameters.  
Inside taskset.c there are the functions that implements all Linux module related stuff (such as ```init_module``` and ```clean_module```), for implementig and simulating tasks, for using RTAI API and for logging.  
In pdfs files there are report and slides used for show my work to the teachers (unluckily, only in italian, sorry!)
