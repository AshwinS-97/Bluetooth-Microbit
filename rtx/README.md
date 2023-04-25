2.	Modify bsp/startup.c to match the handler names used in rtx: SVC_Handler, PendSV_Handler, SysTick_Handler

4.	BLE softdevice is implemented using SVCall and requires priority to be 4. To do so, change the following line in rtx_core_cm.h.
SCB->SHP[7] = (uint8_t)(0xFCU << n);	 Change 0xFEU to 0xFCU to change priority from 6 to 4.

5.	Add RTX in build.sh.
Add RTXDIR variable.
Add -I$RTXDIR to CFLAGS
Add $RTXDIR/librtx.a to linker command line.
