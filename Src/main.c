/*************************************************************************/
/* Author        : Omar Yamany                                    		 */
/* Project       : OmarOS  	                             				 */
/* File          : main.c 			                     				 */
/* Date          : Nov 7, 2023                                           */
/* Version       : V1                                                    */
/* GitHub        : https://github.com/Piistachyoo             		     */
/*************************************************************************/

#include "scheduler.h"

Task_ref Task1, Task2, Task3, Task4;
uint8	 Task1LED, Task2LED, Task3LED, Task4LED;

void Task_1(void);

void Task_2(void);

void Task_3(void);

void Task_4(void);

int main(void)
{
	OmarOS_errorTypes retval = noError;
	/* HW_Init (Initialize Clock tree, Reset Controller) */
	HW_Init();
	retval |= OmarOS_Init();

	Task1.Stack_Size = 1024;
	Task1.pf_TaskEntry = Task_1;
	Task1.Priority = 3;
	Task1.AutoStart = Autostart_Disabled;
	STRING_copy((uint8*)"Task_1", (uint8*)&Task1.TaskName);

	Task2.Stack_Size = 1024;
	Task2.pf_TaskEntry = Task_2;
	Task2.Priority = 3;
	Task2.AutoStart = Autostart_Disabled;
	STRING_copy((uint8*)"Task_2", (uint8*)&Task2.TaskName);

	Task3.Stack_Size = 1024;
	Task3.pf_TaskEntry = Task_3;
	Task3.Priority = 3;
	Task3.AutoStart = Autostart_Disabled;
	STRING_copy((uint8*)"Task_3", (uint8*)&Task3.TaskName);

	Task4.Stack_Size = 1024;
	Task4.pf_TaskEntry = Task_4;
	Task4.Priority = 1;
	Task4.AutoStart = Autostart_Disabled;
	STRING_copy((uint8*)"Task_4", (uint8*)&Task4.TaskName);

	retval |= OmarOS_CreateTask(&Task1);
	retval |= OmarOS_CreateTask(&Task2);
	retval |= OmarOS_CreateTask(&Task3);
	retval |= OmarOS_CreateTask(&Task4);

	OmarOS_ActivateTask(&Task1);
	OmarOS_ActivateTask(&Task2);
	OmarOS_ActivateTask(&Task3);
//	OmarOS_ActivateTask(&Task4);

	OmarOS_StartOS();

    while(1){

    }
}

void Task_1(void){
	while(1){
		/* Taskl Code */
		Task1LED ^= 1;
		OmarOS_TaskWait(100, &Task1);
	}
}

void Task_2(void){
	while(1){
		/* Task2 Code */
		Task2LED ^= 1;
		OmarOS_TaskWait(250, &Task2);
	}
}

void Task_3(void){
	while(1){
		/* Task3 Code */
		Task3LED ^= 1;
		OmarOS_TaskWait(500, &Task3);
	}
}

void Task_4(void){
	static uint32 count = 0;
	while(1){
		/* Task4 Code */
		Task4LED ^= 1;
		OmarOS_TaskWait(500, &Task4);
	}
}
