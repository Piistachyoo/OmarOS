/*************************************************************************/
/* Author        : Omar Yamany                                    		 */
/* Project       : OmarOS  	                            				 */
/* File          : scheduler.c 			                           		 */
/* Date          : Nov 7, 2023                                           */
/* Version       : V1                                                    */
/* GitHub        : https://github.com/Piistachyoo             		     */
/*************************************************************************/

#include "scheduler.h"
#include "OmarOS_FIFO.h"

uint8 IdleTaskLED, SysTickLED;

struct{
	Task_ref *OS_Tasks[MAX_NO_TASKS]; /* Scheduling Table */
	uint8  NoOfActiveTasks;
	uint32 _S_MSP_OS;
	uint32 _E_MSP_OS;
	uint32 PSP_Task_Locator;
	Task_ref *CurrentTask;
	Task_ref *NextTask;
	enum{
		OS_Suspended,
		OS_Running,
		OS_Error
	}OS_ModeID;
}OS_Control;

typedef enum{
	SVC_ActivateTask,
	SVC_TerminateTask,
	SVC_TaskWaitingTime
}SVC_ID;

FIFO_Buf_t Ready_QUEUE;
Task_ref *Ready_QUEUE_FIFO[MAX_NO_TASKS];
static Task_ref IDLE_TASK;

static void OmarOS_IdleTask(void);
static void OmarOS_Create_TaskStack(Task_ref* newTask);
static void OmarOS_UpdateSchedulerTable(void);
static void OmarOS_BubbleSort(void);
static void OmarOS_DecideNextTask(void);
static void OmarOS_Update_TasksWaitingTime(void);

void OmarOS_Set_SVC (SVC_ID ID){
	switch(ID){
	case SVC_ActivateTask:
		__asm ("svc #0x00");
		break;
	case SVC_TerminateTask:
		__asm ("svc #0x01");
		break;
	case SVC_TaskWaitingTime:
		__asm ("svc #0x02");
		break;
	}
}

static void OmarOS_DecideNextTask(void){
	/* If Ready Queue is empty && OS_Control->CurrentTask != Suspended */
	if(Ready_QUEUE.counter == 0 && OS_Control.CurrentTask->TaskState != Suspended){
	OS_Control.CurrentTask->TaskState = Running;

	/* Add the current task again (Round Robin) */
	FIFO_enqueue(&Ready_QUEUE, OS_Control.CurrentTask);
	OS_Control.NextTask = OS_Control.CurrentTask;
	}
	else{
		FIFO_dequeue(&Ready_QUEUE, &OS_Control.NextTask);
		OS_Control.NextTask->TaskState = Running;

		/* Update Ready Queue (To keep round robin algorithm) */
		if((OS_Control.CurrentTask->Priority == OS_Control.NextTask->Priority) && (OS_Control.CurrentTask->TaskState != Suspended)){
			FIFO_enqueue(&Ready_QUEUE, OS_Control.CurrentTask);
			OS_Control.CurrentTask->TaskState = Ready;
		}
	}
}

/* Used to execute specific OS Services */
void OmarOS_SVC_services (uint32 *StackFramePointer){
	/* OS_SVC_Set Stack -> r0 -> argument0 = StackFramePointer
	   OS_SVC_Set : r0,r1,r2,r3,r12,LR,PC,xPSR */
	uint8 SVC_number;
	SVC_number = *((uint8*)((uint8*)(StackFramePointer[6])) - 2);
	switch(SVC_number){
	case SVC_ActivateTask:
	case SVC_TerminateTask:
	case SVC_TaskWaitingTime:
		/* Update Scheduler Table and Ready Queue */
		OmarOS_UpdateSchedulerTable();

		/* If OS is in running state -> Decide what next task */
		if(OS_Control.OS_ModeID == OS_Running){
			if(STRING_compare_caseSensitive((uint8*)OS_Control.CurrentTask->TaskName, (uint8*)"idletask") != 0){
				OmarOS_DecideNextTask();

				/* Switch/Restore Context */
				Trigger_OS_PendSV();
			}
		}
		break;
	}
}

__attribute ((naked)) void PendSV_Handler(void){
	/* Save context of current task */
	/* 1- Get the current task PSP */
	OS_GET_PSP(OS_Control.CurrentTask->Current_PSP);

	/* 2- Use the PSP to store R4 to R11 */
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile ("mov %0, r4" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile ("mov %0, r5" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile ("mov %0, r6" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile ("mov %0, r7" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile ("mov %0, r8" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile ("mov %0, r9" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile ("mov %0, r10" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile ("mov %0, r11" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));

	/* Restore context of next task */
	OS_Control.CurrentTask = OS_Control.NextTask;
	OS_Control.NextTask = NULL;

	__asm volatile ("mov r11, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile ("mov r10, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile ("mov r9, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile ("mov r8, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile ("mov r7, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile ("mov r6, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile ("mov r5, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	__asm volatile ("mov r4, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;

	/* Update PSP and exit */
	OS_SET_PSP(OS_Control.CurrentTask->Current_PSP);
	__asm volatile ("BX LR");

}

void SysTick_Handler(void){
	SysTickLED ^= 1;

	OmarOS_Update_TasksWaitingTime();

	/* Determine Current and Next tasks */
	OmarOS_DecideNextTask();

	/* Switch context and restore */
	Trigger_OS_PendSV();
}

static void OmarOS_UpdateSchedulerTable(void){
	Task_ref *temp = NULL;
	Task_ref *pTask, *pNextTask;
	uint8 i = 0;

	/* Bubble sort Scheduler Table */
	OmarOS_BubbleSort();

	/* Free Ready Queue */
	while(FIFO_dequeue(&Ready_QUEUE, &temp) != FIFO_EMPTY);

	/* Update Ready Queue */
	while(i < OS_Control.NoOfActiveTasks){
		pTask = OS_Control.OS_Tasks[i];
		pNextTask = OS_Control.OS_Tasks[i + 1];

		if(pTask->TaskState != Suspended){
			/* In case we reached to the end of available OSTasks */
			if(pNextTask->TaskState == Suspended){
				FIFO_enqueue(&Ready_QUEUE, pTask);
				pTask->TaskState = Ready;
				break;
			}
			/* Check for priority */
			if(pTask->Priority < pNextTask->Priority){
				/* Current task have higher priority */
				FIFO_enqueue(&Ready_QUEUE, pTask);
				pTask->TaskState = Ready;
				break;
			}
			else if(pTask->Priority == pNextTask->Priority){
				/* If both tasks have the same priority
				 * Push pTask to ready state
				 * Make the pTask = pNextTask and pNextTask++
				 */
				FIFO_enqueue(&Ready_QUEUE, pTask);
				pTask->TaskState = Ready;
			}
			else if(pTask->Priority > pNextTask->Priority){
				/* Not allowed to happen because we already did bubble sort */
				break;
			}
		}
		i++;
	}
}

static void OmarOS_BubbleSort(void){
	uint8 i, j, n;
	Task_ref *temp;
	n = OS_Control.NoOfActiveTasks;
	for(i = 0; i < (n - 1); i++){
		for(j = 0; j < (n - i - 1); j++){
			if(OS_Control.OS_Tasks[j]->Priority > OS_Control.OS_Tasks[j+1]->Priority){
				temp = OS_Control.OS_Tasks[j];
				OS_Control.OS_Tasks[j] = OS_Control.OS_Tasks[j+1];
				OS_Control.OS_Tasks[j+1] = temp;
			}
		}
	}
}

void OmarOS_Create_MainStack(void){
	OS_Control._S_MSP_OS = (uint32)&_estack;
	OS_Control._E_MSP_OS = OS_Control._S_MSP_OS - MainStackSize;
	/* Allign 8 Bytes spaces between Main Stack and PSP Stack */
	OS_Control.PSP_Task_Locator = OS_Control._E_MSP_OS - 8;
}

/**=============================================
 * @Fn			- OmarOS_Init
 * @brief 		- Initializes the OS control and buffers
 * @retval 		- Returns noError if no error happened or an error code if an error occured
 * Note			- Must be called before using any of the OS APIs
 */
OmarOS_errorTypes OmarOS_Init(void){
	OmarOS_errorTypes retval = noError;
	/* Update OS Mode -> OS_Suspended */
	OS_Control.OS_ModeID = OS_Suspended;

	/* Specify the Main Stack for OS */
	OmarOS_Create_MainStack();

	/* Create OS Ready Queue */
	if(FIFO_init(&Ready_QUEUE, Ready_QUEUE_FIFO, MAX_NO_TASKS) != FIFO_NO_ERROR){
		retval |= readyQueueInitError;
	}

	/* Configure IDLE Task */
	if(!retval){ /* No error */
		STRING_copy((uint8*)"idletask", (uint8*)&(IDLE_TASK.TaskName));
		IDLE_TASK.Priority = 255; // Max value for uint8 = highest priority
		IDLE_TASK.pf_TaskEntry = OmarOS_IdleTask;
		IDLE_TASK.Stack_Size = 300;
		retval |= OmarOS_CreateTask(&IDLE_TASK);
	}

	return retval;
}

static void OmarOS_IdleTask(){
	while(1){
		IdleTaskLED ^= 1;
		__asm ("wfe");
	}
}

/**=============================================
 * @Fn			- OmarOS_CreateTask
 * @brief 		- Creates the task object in the OS and initializes the task's stack area
 * @param [in] 	- newTask: Pointer to the task's configuration
 * @retval 		- Returns noError if no error happened or an error code if an error occured
 * Note			- Should only be called after calling "OmarOS_Init"
 */
OmarOS_errorTypes OmarOS_CreateTask(Task_ref* newTask){
	OmarOS_errorTypes retval = noError;

	/* Check if task stack size exceeds the PSP Stack */
	newTask->_S_PSP_Task = OS_Control.PSP_Task_Locator;
	newTask->_E_PSP_Task = newTask->_S_PSP_Task - newTask->Stack_Size;
	if(newTask->_E_PSP_Task < (uint32)&_eheap){
		return taskExceededStackSize;
	}

	/* Create the task stack area in PSP */
	OmarOS_Create_TaskStack(newTask);

	/* Allign 8Bytes spaces between Task PSP and new one */
	if(!retval){ /* No error */
		OS_Control.PSP_Task_Locator = newTask->_E_PSP_Task - 8;
	}

	OS_Control.OS_Tasks[OS_Control.NoOfActiveTasks] = newTask;
	OS_Control.NoOfActiveTasks++;

	/* Task State Update */
	if(newTask->AutoStart == Autostart_Enabled){
		newTask->TaskState = Ready;
	}
	else{
		newTask->TaskState = Suspended;
	}

	return retval;
}

static void OmarOS_Create_TaskStack(Task_ref* newTask){
	/*
	 * Task Frame
	 * ==========
	 * CPU Saved registers
	 *
	 * xPSR
	 * PC (Next instruction to be executed in the task)
	 * LR (return register which is saved in CPU before switching)
	 * r12
	 * r4
	 * r3
	 * r2
	 * r1
	 * r0
	 * ==========
	 * Manually Saved/Restored registers
	 *
	 * r5
	 * r6
	 * r7
	 * r8
	 * r9
	 * r10
	 * r11
	 */
	uint8 index;

	newTask->Current_PSP = (uint32*)newTask->_S_PSP_Task;

	/* xPSR dummy value -> T = 1 to avoid bus faults */
	newTask->Current_PSP--;
	*(newTask->Current_PSP) = 0x01000000;

	/* PC dummy value -> Start at task entry point*/
	newTask->Current_PSP--;
	*(newTask->Current_PSP) = (uint32)newTask->pf_TaskEntry;

	/* LR dummy value -> Return to thread mode with PSP */
	newTask->Current_PSP--;
	*(newTask->Current_PSP) = 0xFFFFFFFD;

	/* Zero CPU registers r0 -> r12 */
	for(index = 0; index < 13; index++){
		newTask->Current_PSP--;
		*(newTask->Current_PSP) = 0;
	}
}

/**=============================================
 * @Fn			- OmarOS_ActivateTask
 * @brief 		- Sends a task to the ready queue to be scheduled
 * @param [in] 	- pTask: Pointer to the task's configuration
 * @retval 		- None
 * Note			- Should only be called after calling "OmarOS_CreateTask"
 */
void OmarOS_ActivateTask(Task_ref* pTask){
	/* Change Task State */
	pTask->TaskState = Waiting;

	OmarOS_Set_SVC(SVC_ActivateTask);
}

/**=============================================
 * @Fn			- OmarOS_TerminateTask
 * @brief 		- Sends a task to the suspended state
 * @param [in] 	- pTask: Pointer to the task's configuration
 * @retval 		- None
 * Note			- Should only be called after calling "OmarOS_CreateTask"
 */
void OmarOS_TerminateTask(Task_ref* pTask){
	/* Change Task State */
	pTask->TaskState = Suspended;

	OmarOS_Set_SVC(SVC_TerminateTask);
}

/**=============================================
 * @Fn			- OmarOS_TaskWait
 * @brief 		- Sends a tasks to the waiting state for a specific amount of Ticks
 * @param [in] 	- Ticks: The amount of ticks the task should be suspended before running again
 * @param [in] 	- pTask: Pointer to the task's configuration
 * @retval 		- Returns noError if no error happened or an error code if an error occured
 * Note			- None
 */
void OmarOS_TaskWait(uint32 Ticks, Task_ref* pTask){
	pTask->TimeWaiting.Task_Block_State = enabled;
	pTask->TimeWaiting.Ticks_Count = Ticks;

	/* Task should be blocked */
	pTask->TaskState = Suspended;
	OmarOS_Set_SVC(SVC_TerminateTask);
}

/**=============================================
 * @Fn			- OmarOS_StartOS
 * @brief 		- Starts the OS scheduler to begin running tasks
 * @retval 		- None
 * Note			- Should only be called after calling "OmarOS_Init" and creating & activating tasks
 */
void OmarOS_StartOS(void){
	OS_Control.OS_ModeID = OS_Running;
	/* Set default "Current Task" */
	OS_Control.CurrentTask = &IDLE_TASK;

	OmarOS_ActivateTask(&IDLE_TASK);

	/* Start Ticker */
	Start_Ticker();

	/* Set PSP */
	OS_SET_PSP(OS_Control.CurrentTask->Current_PSP);
	OS_SWITCH_SP_to_PSP();

	/* Switch to thread mode and unprivileged */
	OS_SET_CPU_UNPRIVILIGED();
	OS_Control.CurrentTask->pf_TaskEntry();
}

void OmarOS_Update_TasksWaitingTime(void){
	uint8 index;
	for(index = 0; index < OS_Control.NoOfActiveTasks; index++){
		if(OS_Control.OS_Tasks[index]->TaskState == Suspended){
			if(OS_Control.OS_Tasks[index]->TimeWaiting.Task_Block_State == enabled){
				OS_Control.OS_Tasks[index]->TimeWaiting.Ticks_Count--;
				if(OS_Control.OS_Tasks[index]->TimeWaiting.Ticks_Count == 0){
					OS_Control.OS_Tasks[index]->TimeWaiting.Task_Block_State = disabled;
					OS_Control.OS_Tasks[index]->TaskState = Waiting;
					OmarOS_Set_SVC(SVC_TaskWaitingTime);
				}
			}
		}
	}
}

/**=============================================
 * @Fn			- OmarOS_AcquireMutex
 * @brief 		- Tries to acquire a mutex if available
 * @param [in] 	- pMutex: Pointer to the Mutex to be locked
 * @param [in] 	- pTask: Pointer to the task's configuration
 * @retval 		- Returns noError if no error happened or an error code if an error occured
 * Note			- A Mutex can be hold by 2 tasks only at the same time (as in a queue)
 */
OmarOS_errorTypes OmarOS_AcquireMutex(Mutex_ref* pMutex, Task_ref* pTask){
	OmarOS_errorTypes retval = noError;

	if(pMutex->CurrentTUser == NULL){
		pMutex->CurrentTUser = pTask;
		if(pMutex->PriorityCeiling.state == PriorityCeiling_enabled){
			/* Save current task priority */
			pMutex->PriorityCeiling.old_priority = pMutex->CurrentTUser->Priority;

			/* Change task priority to ceiling priority */
			pMutex->CurrentTUser->Priority = pMutex->PriorityCeiling.Ceiling_Priority;
		}
	}
	else if(pMutex->CurrentTUser == pTask){
		retval = MutexIsAlreadyAcquired;
	}
	else if(pMutex->NextTUser == NULL){
		pMutex->NextTUser = pTask;
		pMutex->NextTUser->TaskState = Suspended;
		OmarOS_Set_SVC(SVC_TerminateTask);
	}
	else{
		retval = MutexReachedMaxNoOfUsers;
	}

	return retval;
}

/**=============================================
 * @Fn			- OmarOS_ReleaseMutex
 * @brief 		- Releases a mutex and starts the next task that is in the queue (if found)
 * @param [in] 	- pMutex: Pointer to the Mutex to be locked
 * @retval 		- None
 * Note			- A mutex can only be released by the same task that acquired it
 */
void OmarOS_ReleaseMutex(Mutex_ref* pMutex){
	if((pMutex->CurrentTUser != NULL) && (pMutex->CurrentTUser == OS_Control.CurrentTask)){
		if(pMutex->PriorityCeiling.state == PriorityCeiling_enabled){
		/* Restore current task priority */
		pMutex->CurrentTUser->Priority = pMutex->PriorityCeiling.old_priority;
		}

		if(pMutex->NextTUser == NULL){
			if(pMutex->PriorityCeiling.state == PriorityCeiling_enabled){
			}
			pMutex->CurrentTUser = NULL;
		}
		else{
			pMutex->CurrentTUser = pMutex->NextTUser;
			pMutex->NextTUser = NULL;
			pMutex->CurrentTUser->TaskState = Waiting;
			if(pMutex->PriorityCeiling.state == PriorityCeiling_enabled){
				/* Save current task priority */
				pMutex->PriorityCeiling.old_priority = pMutex->CurrentTUser->Priority;

				/* Change task priority to ceiling priority */
				pMutex->CurrentTUser->Priority = pMutex->PriorityCeiling.Ceiling_Priority;
			}
			OmarOS_Set_SVC(SVC_ActivateTask);
		}
	}
}
