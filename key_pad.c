#include "system_sam3x.h"
#include "at91sam3x8.h"

int key_pad(void){
  int key;
    int value = -1;
    *AT91C_PIOD_CODR  = (1<<2);  //Clear OE KEY BUS (Active Low)
    *AT91C_PIOC_OER= (7<<7);  //Make all Column pin as output
    *AT91C_PIOC_SODR= (7<<7); // Set all Column pin as high
    for(int col = 0; col <= 2; col++){
        int column_pin = 7 + col;
        *AT91C_PIOC_CODR  =(1<<column_pin); // clear one column
        for(int row= 0; row <= 3; row++){
            int row_pin = 2+ row;
            key = *AT91C_PIOC_PDSR;// read row
            if((~key & (1<<row_pin))== (1<<row_pin)){     //check if bit is zero
                value = row*3 + col+1;
            }
        }//end loop Row
        *AT91C_PIOC_SODR= (7<<7); //Set the column again
    }//End loop Column
    *AT91C_PIOC_ODR = (7<<7);  //Make all Column pin as input *
    *AT91C_PIOD_SODR= (1<<2);  //Set OE KEY BUS
    if(value == 11){
      value = 0;
    }
    return value;
}
