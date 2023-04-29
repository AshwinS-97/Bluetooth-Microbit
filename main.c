#include <stdint.h>
#include <string.h>
#include "cmsis_os2.h"
#include "ble_uart.h"
#include "board.h"
#include "lib.h"
#include "buggy_controller.h"
#include "utility.h"

/* OS objects */
osThreadId_t tid_ctrl;
osThreadId_t tid_disp;


/* Buffer to hold the command received from UART or BLE
 * We use single buffer assuming command-response protocol,
 * that the next command will be sent after receiving the
 * response for the current command.
 */
uint8_t cmd_buf[256];
uint32_t cmd_len;

uint8_t pic[5][5] ={        {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0} 
                    };	




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
    osThreadFlagsSet(tid_ctrl, 1); 
}

void task_ctrl(void *arg)
{
    while (1)
    {
        /* Receive a command from BLE */
        osThreadFlagsWait(1, osFlagsWaitAny, osWaitForever);

        /* Echo on UART */
        puts1((char *) cmd_buf);
        puts1("\n");

        ftoa(12.7777, (char *)cmd_buf, 4);
        add_controllerMsg((char *) cmd_buf);
      

        /* Echo on BLE */
        ble_send((uint8_t *) cmd_buf, strlen((char *) cmd_buf));
    }
}

// Display Task

void task_disp(void *arg)
{
    while (1)
    {
        
        led_display(pic); 
        check_controllerMsg();   
           
    }
}
// Display Task

int main(void)
{
    /* BSP initializations before BLE because we are using printf from BSP */
    board_init();
    ble_init(ble_recv_handler);
    

    /* Greetings */
    //printf("hello, world!\n");
    audio_sweep(500, 2000, 100);

    /* Initialize and start the kernel */
    osKernelInitialize();
    init_controller();

    /* controller task */
    tid_ctrl = osThreadNew(task_ctrl, NULL, NULL);
    osThreadSetPriority(tid_ctrl, osPriorityLow1);

    /* Display Task */
    tid_disp = osThreadNew(task_disp, NULL, NULL);
    osThreadSetPriority(tid_disp, osPriorityLow);

    osKernelStart();
    /* never returns */

    led_blink(2, 2, BLINK_FOREVER);

    return 0;
}
