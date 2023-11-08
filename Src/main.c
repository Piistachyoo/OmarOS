/*************************************************************************/
/* Author        : Omar Yamany                                    		 */
/* Project       : OmarOS  	                             				 */
/* File          : main.c 			                     				 */
/* Date          : Nov 7, 2023                                           */
/* Version       : V1                                                    */
/* GitHub        : https://github.com/Piistachyoo             		     */
/*************************************************************************/

#include "scheduler.h"

Task_ref Task1, Task2, Task3;

void Task_1(void);
void Task_2(void);
void Task_3(void);

int main(void)
{
	OmarOS_errorTypes retval = noError;
	/* HW_Init (Initialize Clock tree, Reset Controller) */
	HW_Init();
	retval |= OmarOS_Init();

	Task1.Stack_Size = 1024;
	Task1.pf_TaskEntry = Task_1;
	Task1.Priority = 3;
	Task1.AutoStart = FALSE;
	STRING_copy((uint8*)"Task_1", (uint8*)&Task1.TaskName);

	Task2.Stack_Size = 1024;
	Task2.pf_TaskEntry = Task_2;
	Task2.Priority = 3;
	Task2.AutoStart = FALSE;
	STRING_copy((uint8*)"Task_2", (uint8*)&Task2.TaskName);

	Task3.Stack_Size = 1024;
	Task3.pf_TaskEntry = Task_3;
	Task3.Priority = 3;
	Task3.AutoStart = FALSE;
	STRING_copy((uint8*)"Task_3", (uint8*)&Task3.TaskName);

	retval |= OmarOS_CreateTask(&Task1);
	retval |= OmarOS_CreateTask(&Task2);
	retval |= OmarOS_CreateTask(&Task3);

    while(1){

    }
}

void Task_1(void){
	while(1){
		/* Taskl Code */

	}
}

void Task_2(void){
	while(1){
		/* Task2 Code */

	}
}

void Task_3(void){
	while(1){
		/* Task3 Code */

	}
}
