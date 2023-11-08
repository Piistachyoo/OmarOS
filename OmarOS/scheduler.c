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

static struct{
	Task_ref *OS_Tasks[MAX_NO_TASKS]; /* Scheduling Table */
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

static FIFO_Buf_t Ready_QUEUE;
static Task_ref *Ready_QUEUE_FIFO[MAX_NO_TASKS];
static Task_ref IDLE_TASK;

static void OmarOS_IdleTask();
static void OmarOS_Create_TaskStack(Task_ref* newTask);

int OS_SVC_Set (int SVC_ID){
	int result;
	switch(SVC_ID){
	case 1: // ADD
		__asm ("svc #0x1");
		break;
	case 2: // SUB
		__asm ("svc #0x2");
		break;
	case 3: // MUL
		__asm ("svc #0x3");
		break;
	case 4: // PendSV
		__asm ("svc #0x4");
		break;
	}
	__asm ("mov %[result], r0": [result]"=r"(result));
	return result;
}

/* Used to execute specific OS Services */
void OS_SVC_services (uint32 *StackFramePointer){
	/* OS_SVC_Set Stack -> r0 -> argument0 = StackFramePointer
	   OS_SVC_Set : r0,r1,r2,r3,r12,LR,PC,xPSR */
	uint8 SVC_number;
	SVC_number = *((uint8*)((uint8*)(StackFramePointer[6])) - 2);
	switch(SVC_number){
	default: break;
	}
}

void PendSV_Handler(void){
	SCB->ICSR |= SCB_ICSR_PENDSVCLR_Msk;
}

void OmarOS_Create_MainStack(void){
	OS_Control._S_MSP_OS = (uint32)&_estack;
	OS_Control._E_MSP_OS = OS_Control._S_MSP_OS - MainStackSize;
	/* Allign 8 Bytes spaces between Main Stack and PSP Stack */
	OS_Control.PSP_Task_Locator = OS_Control._E_MSP_OS - 8;
}

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
		__asm ("NOP");
	}
}

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

	/* Task State Update */
	if(newTask->AutoStart == enabled){
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
