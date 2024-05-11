# SWE4ES-RTAI-Modeling-Verification

Repo for keeping track of a project for the "Software Engineering For Embedded Systems" exam at the University of Florence.

## Project Objectives
The main objectives of this project are 
  1. Install Linux RTAI on Intel NUC hardware 
  2. Implement a busy_sleep function for the given hardware
  3. Model a task set on ORIS tool
  4. Implement the task set on Linux RTAI and verify all the deadlines are satisfied.
  
## Repository structure
Under the "examples" folder there will be some examples of RTAI-based kernel modules, while under "RTAI_Taskset" the code needed for the project (it shows how to use semaphores, too).  
In all folders there will be code and instruction on how to compile and to execute them.  
Both examples and taskset will be built using Make.  
In all folders there are two files, named ```startRTAI``` and ```stopRTAI``` that will load all the RTAI related kernel modules (needed for executing my custom modules). StartRTAI 
