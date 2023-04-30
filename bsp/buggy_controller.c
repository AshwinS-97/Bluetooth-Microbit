#include <stdint.h>
#include <string.h>
#include "../rtx/cmsis_os2.h"
#include "../bsp/serial.h"
#include "board.h"
#include "buggy_controller.h"
#include "utility.h"
#include "../ble/ble_uart.h"
#include "motion.h"

#define MSGQUEUE_LENGTH 10
char* msg ="[refHeading,currHeading,error,ctrlSignal,lctrlSignal,rctrlSignal]\n";

extern float referenceHeading;
extern float currentHeading;
extern float controlSignal;
extern int error;
extern int lControlSignal;
extern int rControlSignal;
extern float kP;

osThreadId_t tid_log;
osThreadId_t tid_forward;
osThreadId_t tid_backward;
osThreadId_t tid_left;
osThreadId_t tid_right;


osThreadId_t active_thread[10];
int active_count = -1;


osMessageQueueId_t controller_MsgQueue;

uint8_t fwd[5][5] = {{0, 0, 1, 0, 0},
                     {0, 1, 1, 1, 0},
                     {0, 0, 1, 0, 0},
                     {0, 0, 1, 0, 0},
                     {0, 0, 1, 0, 0}};

uint8_t rev[5][5] = {{0, 0, 1, 0, 0},
                     {0, 0, 1, 0, 0},
                     {0, 0, 1, 0, 0},
                     {0, 1, 1, 1, 0},
                     {0, 0, 1, 0, 0}};

uint8_t right[5][5] = {{1, 1, 1, 1, 0},
                       {1, 0, 0, 1, 0},
                       {1, 0, 0, 1, 0},
                       {1, 0, 1, 1, 1},
                       {1, 0, 0, 1, 0}};

uint8_t left[5][5] = {{0, 1, 1, 1, 1},
                      {0, 1, 0, 0, 1},
                      {0, 1, 0, 0, 1},
                      {1, 1, 1, 0, 1},
                      {0, 1, 0, 0, 1}};

typedef struct
{
  char *command;
  int *values[];
} ControllerMsg;

int init_controller(void)
{
  puts1("init controller\n\r");
  controller_MsgQueue = osMessageQueueNew(MSGQUEUE_LENGTH, sizeof(ControllerMsg), NULL);
  if (controller_MsgQueue == NULL)
  {
    puts1("Message Queue object not created, handle failure\n\r");
  }

  return 0;
}

void check_controllerMsg(void)
{
  ControllerMsg msg;
  osStatus_t status;
  uint16_t msgCount = osMessageQueueGetCount(controller_MsgQueue);

  // puts1("in check_controllerMsg\n\r");
  for (int i = 0; i < msgCount; i++)
  {
    status = osMessageQueueGet(controller_MsgQueue, &msg, NULL, 0U);
    if (status == osOK)
    {
      char *command = msg.command;
      puts1("command in Cmd Message Queue: ");
      puts1(command);
      puts1("\n\r");
      execute_driver(command);
    }
  }
}

void add_controllerMsg(char *command)
{
  ControllerMsg msg;
  msg.command = command;
  osMessageQueuePut(controller_MsgQueue, &msg, 0U, 0U);
  puts1("added the message in the queue with command : ");
  puts1(command);
  puts1("\n\r");
}

void execute_driver(char *command)
{

  if (strcasecmp(command, "FORWARD") == 0)
  {

    puts1("MOVING FORWARD\n\r");
    task_stop();
    tid_forward = osThreadNew(forward_motion, NULL, NULL);
    osThreadSetPriority(forward_motion, osPriorityLow);
    active_thread[++active_count] = tid_forward;
  }
  else if (strcasecmp(command, "BACKWARD") == 0)
  {

    puts1("MOVING BACKWARD\n\r");
    task_stop();
    tid_backward = osThreadNew(backward_motion, NULL, NULL);
    osThreadSetPriority(backward_motion, osPriorityLow);
    active_thread[++active_count] = tid_backward;
  }
  else if (strcasecmp(command, "LEFT") == 0)
  {

    puts1("MOVING LEFT\n\r");
    task_stop();
    tid_left = osThreadNew(spot_left, NULL, NULL);
    osThreadSetPriority(spot_left, osPriorityLow);
    active_thread[++active_count] = tid_left;

  }
  else if (strcasecmp(command, "RIGHT") == 0)
  {

    puts1("MOVING RIGHT\n\r");
    task_stop();
    tid_right = osThreadNew(spot_right, NULL, NULL);
    osThreadSetPriority(spot_right, osPriorityLow);
    active_thread[++active_count] = tid_right;

  }
  else if (strcasecmp(command, "STOP") == 0)
  {

    puts1("STOPPING\n\r");
    task_stop();
  }
  else if (strcasecmp(command, "LOG") == 0)
  {
    
    puts1("LOGGING\n\r");
    tid_log = osThreadNew(task_log, NULL, NULL);
    osThreadSetPriority(task_log, osPriorityLow);
  }
  else if (strcasecmp(command, "STOP LOG") == 0)
  {

    puts1("STOPINGS LOGS\n\r");
    task_stop_log();
  }
}

void task_log(void *args)
{
  ble_send((uint8_t *)msg, strlen((char *)msg));
  while (1)
  {
    char* msg="";
    printMetrics(referenceHeading, currentHeading, error, controlSignal,lControlSignal,rControlSignal, msg);
    puts1("msg = ");
    puts1(msg);
    ble_send((uint8_t *)msg, strlen((char *)msg));
    osDelay(1000);
  }
}

void task_stop_log(void)
{

  osStatus_t status = osThreadTerminate(tid_log); // stops the thread
  if (status == osOK)
  {
    puts("Thread was terminated successfully/n/r");
  }
  else
  {
    puts("Failed to terminate a thread/n/r");
  }
}

void task_stop(void)
{
  int len = active_count;
  if (len >= 0)
  {
    stop_motion();
    for (int i = 0; i <= len; i++)
    {
      osStatus_t status = osThreadTerminate(active_thread[i]); // stops the thread
      if (status == osOK)
      {
        puts("Thread was terminated successfully/n/r");
        active_count--;
      }
      else
      {
        puts("Thread was not terminated/n/r");
      }
    }
  }
}