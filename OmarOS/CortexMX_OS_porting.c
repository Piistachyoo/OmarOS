/*************************************************************************/
/* Author        : Omar Yamany                                    		 */
/* Project       : OmarOS  	                             				 */
/* File          : CortexMX_OS_porting.c 			                     */
/* Date          : Nov 7, 2023                                           */
/* Version       : V1                                                    */
/* GitHub        : https://github.com/Piistachyoo             		     */
/*************************************************************************/

#include "CortexMX_OS_porting.h"

void NMI_Handler(void){

}

void HardFault_Handler(void){

}

void MemManage_Handler(void){

}

void BusFault_Handler(void){

}

void UsageFault_Handler(void){

}

__attribute ((naked)) void SVC_Handler(void){
	__asm ("tst lr, #4 \n"
		   "ITE EQ \n"
		   "mrseq r0, msp \n"
		   "mrsne r0, psp \n"
		   "B OS_SVC_services");
}

void HW_Init(void){
	/* Initialize clock tree (RCC -> SysTick Timer & CPU 8MHz)
	 * 8MHz
	 * 1 Count -> 0.125us
	 * X count -> 1ms
	 * X = 8000 count
	 */
}
