/*************************************************************************/
/* Author        : Omar Yamany                                    		 */
/* Project       : OmarOS  	                                        	 */
/* File          : scheduler.h 			                           		 */
/* Date          : Nov 7, 2023                                           */
/* Version       : V1                                                    */
/* GitHub        : https://github.com/Piistachyoo             		     */
/*************************************************************************/
#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

//----------------------------------------------
// Section: Includes
//----------------------------------------------
#include "CortexMX_OS_porting.h"
#include "string_lib.h"

//----------------------------------------------
// Section: User type definitions
//----------------------------------------------
typedef enum{
	noError,
	readyQueueInitError,
	taskExceededStackSize
}OmarOS_errorTypes;

typedef struct{
	const char TaskName[30];
	enum{
		Autostart_Disabled,
		Autostart_Enabled
	}AutoStart;
	uint8 Priority;
	void (*pf_TaskEntry)(void); /* Pointer to Task C Function*/

	uint32 Stack_Size;
	uint32 _S_PSP_Task;	 /* Not entered by the user */
	uint32 _E_PSP_Task;	 /* Not entered by the user */
	uint32* Current_PSP; /* Not entered by the user */

	enum{
		Suspended,
		Running,
		Waiting,
		Ready
	}TaskState;

	struct{
		enum{
			enabled,
			disabled
		}Task_Block_State;
		uint32 Ticks_Count;
	}TimeWaiting;
}Task_ref;

//----------------------------------------------
// Section: Macros Configuration References
//----------------------------------------------
#define MAX_NO_TASKS	100


/*
 * =============================================
 * APIs Supported by "OmarOS"
 * =============================================
 */

OmarOS_errorTypes OmarOS_Init(void);
OmarOS_errorTypes OmarOS_CreateTask(Task_ref* newTask);
void OmarOS_ActivateTask(Task_ref* pTask);
void OmarOS_TerminateTask(Task_ref* pTask);
void OmarOS_StartOS(void);
void OmarOS_TaskWait(uint32 Ticks, Task_ref* pTask);

#endif /* INC_SCHEDULER_H_ */
