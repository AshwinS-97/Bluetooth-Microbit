#include <stdint.h>
#include <string.h>
#include "../rtx/cmsis_os2.h"
#include "../bsp/serial.h"
#include "board.h"
#include "buggy_controller.h"

#define MSGQUEUE_LENGTH 10 


osMessageQueueId_t controller_MsgQueue;
 
uint8_t fwd[5][5] ={        {0, 0, 1, 0, 0},
                            {0, 1, 1, 1, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 1, 0, 0} 
                    };	
uint8_t rev[5][5] ={        {0, 0, 1, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 1, 1, 1, 0},
                            {0, 0, 1, 0, 0} 
                    };	
uint8_t right[5][5] ={        {1, 1, 1, 1, 0},
                            {1, 0, 0, 1, 0},
                            {1, 0, 0, 1, 0},
                            {1, 0, 1, 1, 1},
                            {1, 0, 0, 1, 0} 
                    };	
uint8_t left[5][5] ={        {0, 1, 1, 1, 1},
                            {0, 1, 0, 0, 1},
                            {0, 1, 0, 0, 1},
                            {1, 1, 1, 0, 1},
                            {0, 1, 0, 0, 1} 
                    };	


typedef struct{
    char* command;
    int* values[];
}ControllerMsg;

int init_controller(void){
  puts1("init controller\n\r");
     controller_MsgQueue = osMessageQueueNew(MSGQUEUE_LENGTH, sizeof(ControllerMsg), NULL);
  if (controller_MsgQueue == NULL) {
    puts1("Message Queue object not created, handle failure\n\r");
  }

  return 0;
 
}

void check_controllerMsg(void){
    ControllerMsg msg;
    osStatus_t status;
    uint16_t msgCount = osMessageQueueGetCount(controller_MsgQueue);

    //puts1("in check_controllerMsg\n\r");
    for(int i =0 ; i<msgCount; i++){
      status = osMessageQueueGet(controller_MsgQueue, &msg, NULL, 0U); 
      puts1("Debud :: inside for");
      if (status == osOK) {
        char* command = msg.command;
        puts1("command : ");
        puts1(command);
        puts1("\n\r");
        execute_driver(command);
        
      }
    }
}

void add_controllerMsg(char* command){
  ControllerMsg msg;
  msg.command = command;
  puts1("in add_controllerMsg\n\r");
  osMessageQueuePut(controller_MsgQueue, &msg, 0U, 0U);
  puts1("added the message in the queue with command :");
  puts1(command);
  puts1("\n\r");
  
}

void execute_driver(char* command){
  if(strcmp(command,"FORWARD")==0){
    //buggy_move_forward(100);
    led_display(fwd);
    puts1("MOVING FORWARD");
  }else if(strcmp(command,"BACKWARD")==0){
    //buggy_move_reverse(100);
    puts1("MOVING BACKWARD");
    led_display(rev);
  }else if(strcmp(command,"LEFT")==0){
    //buggy_spin_left();
    puts1("MOVING LEFT");
    led_display(left);
  }else if(strcmp(command,"RIGHT")==0){
    //buggy_spin_right();
    puts1("MOVING RIGHT");
    led_display(right);
  }else if(strcmp(command,"STOP")==0){
    //buggy_stop();
    puts1("STOPPING");
  }
          
}