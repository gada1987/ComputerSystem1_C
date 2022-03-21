#ifndef PRINT_H
#define PRINT_H

void print_float(float value);
void Print_Letter(char ch);
void move_ptr(int x, int y);
void print_str(char *ch);
void Print_Menu(void);
void Welcome_Menu(void);
void Statistic_menu(void);
void sun_position_menu(void);
void Alarm_menu(void);
void Mode_menu(void);
void Status_menu(void);
void clean(void);
extern char *Days_str[7];
extern int rows_postion[7];
extern int columns_positon[7];
extern char *Statistic_str[3];
extern int servo_done;
extern int new_button;
extern int old_button;
extern char alarm_buffer[6];
extern float upper_limit;
extern float lower_limit;
extern int selected_mode;
extern int sample_time;

#endif
