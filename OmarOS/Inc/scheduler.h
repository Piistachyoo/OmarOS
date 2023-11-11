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
	taskExceededStackSize,
	MutexReachedMaxNoOfUsers,
	MutexIsAlreadyAcquired
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

typedef struct{
	uint8 *pPayload;
	uint32 PayloadSize;
	Task_ref* CurrentTUser;
	Task_ref* NextTUser;
	char MutexName[30];
	struct{
		enum{
			PriorityCeiling_enabled,
			PriorityCeiling_disabled
		}state;
		uint8 Ceiling_Priority;
		uint8 old_priority;
	}PriorityCeiling;
}Mutex_ref;

//----------------------------------------------
// Section: Macros Configuration References
//----------------------------------------------
#define MAX_NO_TASKS	100


/*
 * =============================================
 * APIs Supported by "OmarOS"
 * =============================================
 */

/**=============================================
 * @Fn			- OmarOS_Init
 * @brief 		- Initializes the OS control and buffers
 * @retval 		- Returns noError if no error happened or an error code if an error occured
 * Note			- Must be called before using any of the OS APIs
 */
OmarOS_errorTypes OmarOS_Init(void);

/**=============================================
 * @Fn			- OmarOS_CreateTask
 * @brief 		- Creates the task object in the OS and initializes the task's stack area
 * @param [in] 	- newTask: Pointer to the task's configuration
 * @retval 		- Returns noError if no error happened or an error code if an error occured
 * Note			- Should only be called after calling "OmarOS_Init"
 */
OmarOS_errorTypes OmarOS_CreateTask(Task_ref* newTask);

/**=============================================
 * @Fn			- OmarOS_ActivateTask
 * @brief 		- Sends a task to the ready queue to be scheduled
 * @param [in] 	- pTask: Pointer to the task's configuration
 * @retval 		- None
 * Note			- Should only be called after calling "OmarOS_CreateTask"
 */
void OmarOS_ActivateTask(Task_ref* pTask);

/**=============================================
 * @Fn			- OmarOS_TerminateTask
 * @brief 		- Sends a task to the suspended state
 * @param [in] 	- pTask: Pointer to the task's configuration
 * @retval 		- None
 * Note			- Should only be called after calling "OmarOS_CreateTask"
 */
void OmarOS_TerminateTask(Task_ref* pTask);

/**=============================================
 * @Fn			- OmarOS_StartOS
 * @brief 		- Starts the OS scheduler to begin running tasks
 * @retval 		- None
 * Note			- Should only be called after calling "OmarOS_Init" and creating & activating tasks
 */
void OmarOS_StartOS(void);

/**=============================================
 * @Fn			- OmarOS_TaskWait
 * @brief 		- Sends a tasks to the waiting state for a specific amount of Ticks
 * @param [in] 	- Ticks: The amount of ticks the task should be suspended before running again
 * @param [in] 	- pTask: Pointer to the task's configuration
 * @retval 		- Returns noError if no error happened or an error code if an error occured
 * Note			- None
 */
void OmarOS_TaskWait(uint32 Ticks, Task_ref* pTask);

/**=============================================
 * @Fn			- OmarOS_AcquireMutex
 * @brief 		- Tries to acquire a mutex if available
 * @param [in] 	- pMutex: Pointer to the Mutex to be locked
 * @param [in] 	- pTask: Pointer to the task's configuration
 * @retval 		- Returns noError if no error happened or an error code if an error occured
 * Note			- A Mutex can be hold by 2 tasks only at the same time (as in a queue)
 */
OmarOS_errorTypes OmarOS_AcquireMutex(Mutex_ref* pMutex, Task_ref* pTask);

/**=============================================
 * @Fn			- OmarOS_ReleaseMutex
 * @brief 		- Releases a mutex and starts the next task that is in the queue (if found)
 * @param [in] 	- pMutex: Pointer to the Mutex to be locked
 * @retval 		- None
 * Note			- A mutex can only be released by the same task that acquired it
 */
void OmarOS_ReleaseMutex(Mutex_ref* pMutex);

#endif /* INC_SCHEDULER_H_ */
