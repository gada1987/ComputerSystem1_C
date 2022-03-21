#ifndef TRAP_H
#define TRAP_H

void TC0_Handler(void);
void SysTick_Handler(void);
void delay_ms(int ms);
void PIOD_Handler(void);
extern int tick;
extern int tick2;
extern int tick3;
extern int tick4;
extern int tick5;
extern int Tc0_flag;
extern int run_program;

#endif
