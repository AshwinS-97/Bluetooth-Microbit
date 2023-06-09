#include <string.h>
#include "cmsis_os2.h"
#include "ble_uart.h"
#include "board.h"
#include "lib.h"
#include "buggy_controller.h"
#include "utility.h"
#include "i2c.h"
#include "lsm303agr.h"
#include "adc.h"

/* OS objects */
//osThreadId_t tid_ctrl;
// osThreadId_t tid_disp;
osThreadId_t timer_id;
osThreadId_t Audio_processing;


extern uint32_t samples_collected; 


/* Collision detection */
// float acc_val[3] = {0.0,0.0,0.0};
// float acc_val_filtered[3] = {0.0,0.0,0.0};
// float gamma1 = 0.025;
// char buff[256];
// int counter = 0;
// #define UPPER_THRESHOLD 150
// #define LOWER_THRESHOLD 20

/* Buffer to hold the command received from UART or BLE
 * We use single buffer assuming command-response protocol,
 * that the next command will be sent after receiving the
 * response for the current command.
 */
uint8_t cmd_buf[256];
uint32_t cmd_len;

// uint8_t pic[5][5] ={        {0, 0, 0, 0, 0},
//                             {0, 0, 0, 0, 0},
//                             {0, 0, 0, 0, 0},
//                             {0, 0, 0, 0, 0},
//                             {0, 0, 0, 0, 0} 
//                     };	


void timer_callback(void *arg)
{
    while(1){
        //puts1("In timer Thread\r\n");
        // counter ++;
        // if(counter > 40) counter = 1;
        // accReadXYZ(acc_val);
        // for(int i = 0; i<3 ;++i)
        // {
        //     acc_val_filtered[i] = (1-gamma1)*acc_val_filtered[i] + gamma1*acc_val[i];             
        // }       
        // // ftoa(norm (acc_val_filtered, 3), buff, 2);
        // // if (counter == 1 ) puts1(buff);
        // // if (counter == 1 ) puts1(" Printin\r\n") ;
        // if (norm (acc_val_filtered, 3) > UPPER_THRESHOLD || norm (acc_val_filtered, 3) < LOWER_THRESHOLD)
        // puts1("Collision detected\r\n");
        // Clap detection
        //samples_collected = 0;
        get_audio_samples();
        osDelay(100);
    }
}


/* Called from BLE softdevice using SWI2_EGU2_IRQHandler */
static void ble_recv_handler(const uint8_t s[], uint32_t len)
{
    /* Make a local copy so that BLE can receive next characters */
    memcpy(cmd_buf, s, len);

    /* Remove trailing new line or carriange return characters. */
    while ((s[len - 1] == '\n') || (s[len - 1] == '\r'))
        len--;
    cmd_buf[len] = '\0';            // null-terminate the string
    cmd_len = len;

    /* Signal the waiting task. */
    //osThreadFlagsSet(tid_ctrl, 1); 
     /* Echo on BLE */
    ble_send((uint8_t *) cmd_buf, strlen((char *) cmd_buf));
}

// void task_ctrl(void *arg)
// {
//     while (1)
//     {
//         /* Receive a command from BLE */
//         osThreadFlagsWait(1, osFlagsWaitAny, osWaitForever);

//         /* Echo on UART */
//         puts1((char *) cmd_buf);
//         puts1("\n");

//         //ftoa(12.7777, (char *)cmd_buf, 4);
//         add_controllerMsg((char *) cmd_buf);
      

       
//     }
// }


// Audio_proc_thread 
void Audio_proc_thread(void *arg)
{
    while(1){
        //puts1("In Audio_proc_thread\r");
        osThreadFlagsWait(1, osFlagsWaitAny, osWaitForever);
        if(clap_detect()){
            puts1("clap Detected\r\n");
        }
        samples_collected = 0; 
       // once audio is processed set the global flag

    }
}

int main(void)
{
    /* BSP initializations before BLE because we are using printf from BSP */
    board_init();
    ble_init(ble_recv_handler);

    delay_ms(100);
    i2c_init(SCL_PIN,SDA_PIN);    
    accInit(LSM303_ACCEL_MODE_NORMAL, LSM303_ACCEL_RANGE_2G, LSM303_ACCEL_DATARATE_400HZ);
    delay_ms(100);
    
    

    /* Greetings */
    //printf("hello, world!\n");
    audio_sweep(500, 2000, 100);

    /* Initialize and start the kernel */
    osKernelInitialize();
    init_controller();

    // /* controller task */
    // tid_ctrl = osThreadNew(task_ctrl, NULL, NULL);
    // osThreadSetPriority(tid_ctrl, osPriorityLow2);

    // /* Display Task */
    // tid_disp = osThreadNew(task_disp, NULL, NULL);
    // osThreadSetPriority(tid_disp, osPriorityLow);

    // /* Audio_processing*/
    Audio_processing = osThreadNew(Audio_proc_thread, NULL, NULL);
    osThreadSetPriority(Audio_proc_thread, osPriorityLow1);

    // /* Timer Thread */
    timer_id = osThreadNew (timer_callback, NULL, NULL);
    osThreadSetPriority(timer_id, osPriorityNormal1);

    osKernelStart();
    /* never returns */

    led_blink(2, 2, BLINK_FOREVER);

    return 0;
}

// Function def
