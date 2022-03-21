
#include "system_sam3x.h"
#include "at91sam3x8.h"
#include <stdio.h>
#include "newhave.h"
#include "print.h"
#include "ljus.h"
#include "temp.h"
#include <string.h>
char *Days_str[7] ={{"Mon"},{"Tue"},{"Wed"},{"Thu"},{"Fri"},{"Sat"},{"Sun"}};
int rows_postion[7] = {5,9,13,5,9,13,5};
int columns_positon[7]={5,5,5,14,14,14,22};
char *Statistic_str[3]={{"Min"},{"Max"},{"Ave"}};
int servo_done = 0;
char alarm_buffer[6];
int selected_mode = 100 ;
int sample_time = 100;
int alarm_index = 0;
float upper_limit;
float lower_limit;

void print_float(float value){
    int tal, ental, tiotal,decimaltal;
    tal = (int)(value *10.0);
    tiotal = (tal%1000)/100;
    ental = (tal%100)/10;
    decimaltal=(tal%10);
    Write_Data_2_Display(0x10+tiotal);
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0x10+ental);
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0x0c);
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0x10+decimaltal);
    Write_Command_2_Display(0xc0);
}

void print_degree(int tal){
    int ental, tiotal,hundratal;
    hundratal= (tal%1000)/100;
    tiotal = (tal%100)/10;
    ental = (tal%10);
    Write_Data_2_Display(0x10+hundratal);
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0x10+tiotal);
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0x10+ental);
    Write_Command_2_Display(0xc0);
}
void Print_Letter(char ch){
    
    if(ch > 31 && ch< 123){
        Write_Data_2_Display(ch-32);
        Write_Command_2_Display(0xc0);}
    
}
void print_str(char *ch)
{
    while(*ch!= NULL){
        
        Print_Letter(*ch);
        ch++;
    }
}

void move_ptr(int x, int y){
    unsigned char lowbyte,highbyte;
    int position = (x-1)*40 + (y-1);// startar from 1 not 0
    
    lowbyte = position & (0xff);
    highbyte = (position >> 8) & (0xff);
    Write_Data_2_Display(lowbyte);//low byte
    Write_Data_2_Display(highbyte);// high byte
    Write_Command_2_Display(0x24);
}

void clean(void){
      move_ptr(1,1);
   for(int j= 0; j<640; j++){    // col*rader = 16*40 = 640
        Write_Data_2_Display(0x00);
        Write_Command_2_Display(0xc0);
    }
}

void Print_Menu(void){
    move_ptr(1,13);
    print_str("Main Menu");
    move_ptr(3,1);
    print_str("1_Press 1 To Get Statistics for week");
    move_ptr(5,1);
    print_str("2_Press 2 for Orientation Of Sun");
    move_ptr(7,1);
    print_str("3-Press 3 To Set a alarm");
    move_ptr(9,1);
    print_str("4-Press 4 To Enter fastmode");
    move_ptr(11,1);
    print_str("5-Press 5 To Get Status");
   
}
void Welcome_Menu(void){
    move_ptr(4,15);
    print_str("Hello!");
    move_ptr(7,6);
    print_str("Welcome to my weather station!");
    move_ptr(16,1);
    print_str("Please Press # to start!");
    
    
}
void Statistic_menu(void){
   move_ptr(1,10);
   print_str("Statistic menu!");
   days_of_week();
   move_ptr(16,25);
   print_str("Press * to back");
}
void sun_position_menu(void){
   move_ptr(1,10);
   print_str("Sun postion menu!");
   if(servo_done == 0 ){
       sun_position();
       servo_done = 1;
       move_ptr(5,5);
       print_str("The volt is: ");
       print_float(light_sensor());
       print_str("V");
        move_ptr(7,5);
       print_str("The degree is: ");
       print_degree(sun_pos); 
       move_ptr(16,1);
       print_str("Press * to back");
          
   }
}
void Alarm_menu(void){
   move_ptr(1,10);
   print_str("Set Alarm menu!");
   if(new_button >= 0 && old_button == -1 && new_button < 12){
     alarm_buffer[alarm_index]= new_button;
     alarm_index = (alarm_index+1)%6;
   }
   move_ptr(5,10);
   print_str("The upper limit is: ");
   Print_Letter(alarm_buffer[0]+'0');
   Print_Letter(alarm_buffer[1]+'0');
   Print_Letter(',');
   Print_Letter(alarm_buffer[2]+'0');
   move_ptr(7,10);
   print_str("The lower limit is: ");
   Print_Letter(alarm_buffer[3]+'0');
   Print_Letter(alarm_buffer[4]+'0');
   Print_Letter(',');
   Print_Letter(alarm_buffer[5]+'0');
    move_ptr(16,1);
   print_str("Press * to back");
   move_ptr(16,25);
   print_str("Press # to enter");
}

void Mode_menu(void){
   move_ptr(1,10);
   print_str("Mode menu!");
     move_ptr(3,1);
   print_str("1_Press 1 for super fast mode!");
     move_ptr(5,1);
   print_str("2_Press 2 for fast mode!");
     move_ptr(7,1);
   print_str("3_Press 3 for slow mode!");
   
   if(new_button >= 0 && old_button == -1 && new_button < 12){
     if(new_button == 1){
      selected_mode = 100;
       move_ptr(9,1);
       print_str("Selected mode: 10 times per sec!");
   
     }
     else if(new_button == 2){
    selected_mode = 1000;
        move_ptr(9,1);
        print_str("Selected mode: 1 times per sec! ");
     }
      else if(new_button == 3){
      selected_mode = 60000;
        move_ptr(9,1);
        print_str("Selected mode: 1 times per min! ");
     }
   }    
   move_ptr(16,1);
   print_str("Press * to back");
   move_ptr(16,25);
   print_str("Press # to enter");
          
}
void Status_menu(void){
   move_ptr(1,10);
   print_str("Status menu!");
    move_ptr(3,1);
    print_str("The actual Temperature is: ");
    print_float( Temparature);
    print_str("C");
    move_ptr(5,1);
    print_str("The upper limit is:");
    print_float(upper_limit);
    move_ptr(7,1);
    print_str("The lower limit is:");
    print_float(lower_limit);
    if(Temparature > upper_limit){
       move_ptr(9,1);
       print_str("It's too HOT!");
    }
    else if(Temparature < lower_limit){
       move_ptr(9,1);
       print_str("It's too COLD!");
    }
    else{
      move_ptr(9,1);
      print_str("               ");
    }
      move_ptr(11,1);
      print_str("The sun postion is: ");
      print_degree(sun_pos);
      move_ptr(13,1);
      print_str("Selected sample time:");
      if(sample_time== 100){
       print_str("10 times per sec!");
   
     }
     else if(sample_time == 1000){
       print_str("1 times per sec! ");
     }
      else if(sample_time == 60000){
        print_str("1 times per min! ");
     }

    move_ptr(16,1);
    print_str("Press * to back");
   
}