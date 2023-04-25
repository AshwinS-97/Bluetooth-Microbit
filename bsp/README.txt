
## BSP Setup
All the lines with “Interrupt related configuration” from gpio.c is deleted. (BLE code is using GPIO interrupt for detecting button-press to come out of sleep mode.)

#### Get ld files:
```b
cp ble/ble_app_uart_gcc_nrf52.ld ble_uart.ld
cp <nrf>/modules/nrfx/mdk/nrf_common.ld common.ld
```
Change the line ```“INCLUDE nrf_common.ld”``` to ```“INCLUDE common.ld”``` in ble_uart.ld file.\
Remove the following line from ble_uart.ld file:
```GROUP(-lgcc -lc -lnosys)```\
(We are using ld as linker, NRF Makefile uses gcc as linker. We are linking the libraries by providing their path in the linker command.)


#### Changes in startup file:
Rename ResetISR as Reset_Handler (as used in common.ld)\
Rename the section name of vector table from .vectors to .isr_vector (as used in common_ld)\
The linker file expects the stack to be allocated in .stack section. (In our case, we had allocated through linker command file.) We can change either the linker command file common.ld or change startup.c. Here, we will modify startup.c.

Add stack just before the vector table:\
```b
 #define STACK_SIZE  (8192/4)    // stack size in long words
__attribute__ ((section(".stack")))
uint32_t stack[STACK_SIZE];
```
Change the vector table entry for initial SP to:
```(pfn_t) (long) (stack + STACK_SIZE)```

Add these handlers in the vector table:
1.	RTC1_IRQHandler: used during Bluetooth pairing
2.	SWI2_EGU2_IRQHandler: used by BLE SoftDevice to send data to the app

\
BLE uses some initialized sections before and after .data section. So crt_init should copy memory after end of text (__etext) and before bss (__bss_start__).

    // Copy the data segment initializers from flash to SRAM.
    pui32Src = &__etext;
    pui32Dest = &__data_start__;
    while(pui32Dest < &__bss_start__)
        *pui32Dest++ = *pui32Src++;

Since we will use serial functions from BSP, we should remove UART related code from ble. From ble/ble_uart.c, remove uart_init function definition and its call inside ble_init. Also remove uart_event_handle.\
Copy main.c from ble directory for testing.

```cp ble/main.c main.c```
