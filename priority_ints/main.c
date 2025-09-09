/*.. Departamento de Electrónica, Universidad de Alcalá.
  .. Sistemas Digitales.
  .. Grado en Ingeniería Electrónica y Automática Industrial

Four interrupts are enabled:
SysTick, peripheral which is always present in Cortex-M3 devices, and external interrupts EINT0,
EINT1 and EINT2 (falling edge activated)

There is a ‘while’ loop in every EINTx ISR (Interrupt Service Routine), which forces the ISR to
wait for a specific GPIO port to go to low level (L):
An L level on P2.0 makes EINT0 ISR to finish
An L level on P2.1 makes EINT1 ISR to finish
An L level on P2.2 makes EINT2 ISR to finish

The purpose of this loop is simply to allow simulating the interrupts without having to take care
of the small execution times of these ISR, and observe comfortably how the interrupt controller
(NVIC) works: flags A (Active) and P (Pending) from each IRQ ID.

It is very important to remember that an ISR should last as short as possible, in order not to
block execution of other lower priority ISR and the main program. For this reason, this example
could never be part of a real system, because if P2.2 is kept at a high level, once
'EINT2_IRQHandler' starts executing, it will never allow any lower priority ISR or main program to
resume their execution.*/

#include <LPC17xx.h>

#define Ftick 100            // Frecuency IRQ SYSTICK  
volatile uint32_t a,b,c;     // Auxiliary vbles

//--------------------------------- CONFIG IRQS ----------------------------

void config_interrupts()
{
  // Configuración interrupciones externas
  LPC_PINCON->PINSEL4|=(0x01<<20);      // P2.10 INPUT EXT 0
  LPC_PINCON->PINSEL4|=(0x01<<22);      // P2.11 INPUT EXT 1
  LPC_PINCON->PINSEL4|=(0x01<<24);      // P2.12 INPUT EXT 2
  LPC_SC->EXTMODE|=(1<<2)|(1<<1)|(1<<0);   // Edge  
  LPC_SC->EXTPOLAR=0;                       // Falling

  // Asignación de prioridades
  NVIC_SetPriorityGrouping(2);        // 32 preemptive levels (without sub-priority)  
  NVIC_SetPriority(EINT0_IRQn, 7);    // Less priority!!!    NVIC->IP[18]=(4<<3); 
  NVIC_SetPriority(EINT1_IRQn, 6);    //                      NVIC->IP[19]=(3<<3);
  NVIC_SetPriority(EINT2_IRQn, 8);    //                      NVIC->IP[20]=(2<<3); 
  NVIC_SetPriority(SysTick_IRQn, 3);  // Most priority!   

  // Habilitación de las interrupciones
  NVIC_EnableIRQ(EINT0_IRQn); // sin CMSIS: NVIC->ISER[0]=(1<<18);
  NVIC_EnableIRQ(EINT1_IRQn); // sin CMSIS: NVIC->ISER[0]=(1<<19);    
  NVIC_EnableIRQ(EINT2_IRQn);  // sin CMSIS: NVIC->ISER[0]=(1<<20);
}

//--------------------------------- ISRs -------------------------------

void EINT0_IRQHandler()
{
  LPC_SC->EXTINT=(1<<0);         // clear flag Externa 0
  while(1)
  {    
    if(LPC_GPIO2->FIOPIN & 0x01){
      a++;
    }
    else
      return;
  }
}

void EINT1_IRQHandler()
{
  LPC_SC->EXTINT=(1<<1);         // clear flag Externa 1
  while(1) 
    if(LPC_GPIO2->FIOPIN & 2)
      b++;
    else
      return;
}

void EINT2_IRQHandler()
{
  LPC_SC->EXTINT=(1<<2);       // clear flag Externa 2
  while(1) 
    if(LPC_GPIO2->FIOPIN & 4)
      c++;
    else
      return;
}

void SysTick_Handler(void)     // Periodically run at Ftick (Hz)
{
  LPC_GPIO1->FIOPIN^=(1<<18);  // SWAP P1.18 
  a=0;
  b=0;
  c=0;
}

//--------------------------------- main ----------------------------------------
int main ()
{
  LPC_GPIO1->FIODIR|=(1<<18);             // P1.18 output
  SysTick_Config(SystemCoreClock/Ftick);  // RELOAD value. SystemCoreClock is LPC1768 freq 
                                          // CAREFUL enables IRQ with priority 31!!!
  config_interrupts();
  while(1);
}

