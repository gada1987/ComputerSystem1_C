#ifndef TEMP_H
#define TEMP_H


void init_temp(void);
void Delay_temp(int Value);
void pulse(void);
void logging_temp(void);
void days_of_week(void);
void Set_Alarm(void);
extern float Temparature;
extern float Temp_buffer;
extern int Temp_buffer_index;
extern float Temp_alarm;
extern float Temp_upper;
extern float Temp_lower;

#define TC0_RA    (unsigned int *)     0x40080014 // (TC0) Register A
#define TC0_RB    (unsigned int *)     0x40080018 // (TC0) Register B

#endif