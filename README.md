# OmarOS

#### ```⚠️ This project is made for educational and learning purposes and it may contain faults that I didn't  discover yet. If you find any, please do a pull request or fork the project to add the modifications.```

Omar OS is a Real-Time Operating System written from scratch, targeting embedded devices, and is built on the ARM CMSIS V5 Library for easy porting  

### Features:  
1- Fair scheduling policy: featuring a full-preemptive priority-based scheduler, OmarOS supports a round-robin policy for same-priority tasks.  
  
2- Low resource consumption: OmarOS only consumes 7% of the Cortex-M3 CPU usage in the idle state (no tasks are running), thanks to the "WFE" instruction which allows the CPU to enter sleep mode while not running any tasks  
  
3- Event and Time triggered scheduler: OmarOS allows the user to choose between letting the tasks activate/terminate either by themselves or by an event, or by using delays to run the tasks periodically! 
  
4- Easy task synchronization: OmarOS supports queued Mutexs, which allows one task to wait for an acquired mutex and to be activated once the mutex is released!  
  
5- No mutex releases by mistake: OmarOS respects the Mutex ownership concept and allows only the task that is holding the mutex to release it.   
  
6- Priority Inversion is no more: OmarOS features the priority ceiling technique which helps in avoiding the priority inversion problem!  
  

### Examples:  
In this example there are 3 tasks with the same priority, running sequentially with the round-robin scheduling policy   
![enter image description here](https://github.com/Piistachyoo/OmarOS/blob/main/RoundRobinExample.gif?raw=true)

This example shows 3 tasks, each running with a specific delay, task 1 runs every 100ms, task 2 runs every 300ms, and task 3 runs every 500 ms
![enter image description here](https://github.com/Piistachyoo/OmarOS/blob/main/Time_Delay_Example.gif?raw=true)

This example shows the CPU utilization when the OS is not running any tasks, the CPU load is at 7% so it is not consuming excessive energy or resources
![enter image description here](https://github.com/Piistachyoo/OmarOS/blob/main/CPU_Utilization_Example.gif?raw=true)

