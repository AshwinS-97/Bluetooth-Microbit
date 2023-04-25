## RTX Download
Download CMSIS_5 repository from https://github.com/ARM-software/CMSIS_5

Copy the following files:

#### RTX source code and internal header files
```bash
cp CMSIS_5-develop/CMSIS/RTOS2/RTX/Source/*.c .
cp CMSIS_5-develop/CMSIS/RTOS2/RTX/Source/*.h .
cp CMSIS_5-develop/CMSIS/RTOS2/RTX/Include/* .
```

#### RTX main header file
```b
cp CMSIS_5-develop/CMSIS/RTOS2/Include/cmsis_os2.h .
```

#### We use SysTick for timer tick
```b
cp CMSIS_5-develop/CMSIS/RTOS2/Include/os_tick.h .
cp CMSIS_5-develop/CMSIS/RTOS2/Source/os_systick.c .
```

#### OS configuration file and stubs
```b
cp CMSIS_5-develop/CMSIS/RTOS2/RTX/Config/RTX_Config.h .
cp CMSIS_5-develop/CMSIS/RTOS2/RTX/Config/RTX_Config.c .
/* (In RTX_Config.h, change OS_ROBIN_ENABLE from 1 to 0.) */
```

#### architecture specific header files
``` mkdir arch```

#### compiler and architecture specific header files
```bash
cp CMSIS_5-develop/CMSIS/Core/Include/cmsis_version.h arch
cp CMSIS_5-develop/CMSIS/Core/Include/cmsis_compiler.h arch
cp CMSIS_5-develop/CMSIS/Core/Include/cmsis_gcc.h arch
cp CMSIS_5-develop/CMSIS/Core/Include/core_cm4.h arch
cp CMSIS_5-develop/CMSIS/Core/Include/mpu_armv7.h arch
```
#### Cortex-M4 device specific header files
```b
cp CMSIS_5-develop/Device/ARM/ARMCM4/Include/ARMCM4.h arch
cp CMSIS_5-develop/Device/ARM/ARMCM4/Include/ARMCM4_FP.h arch
cp CMSIS_5-develop/Device/ARM/ARMCM4/Include/system_ARMCM4.h arch
```
#### assembly files for exception handlers
```b
cp CMSIS_5-develop/CMSIS/RTOS2/RTX/Source/GCC/irq_armv7m.S .
```
#### empty RTE_Components.h
```touch RTE_Components.h```



## RTX Setup

#### Edit the following files:


os_systick.c\
rtx_core_c.h\
rtx_core_cm.h\
rtx_lib.c\
Change:```#include CMSIS_device_header``` to ```#include "ARMCM4_FP.h"```


#### Modify bsp/startup.c to match the handler names used in rtx:

 SVC_Handler\
 PendSV_Handler\
 SysTick_Handler

#### Copy the Linker file
``` cp bare.ld rtos.ld ```

If you get a linker error about ARM.extab sections. To resolve this,
remove the lines containing .fnstart, .fnend and .cantunwind in irq_armv7m.S
(total 9 lines).

BLE softdevice is implemented using SVCall and requires priority to be 4. To do so, we need to change the following line in rtx_core_cm.h.
```line
SCB->SHP[7] = (uint8_t)(0xFCU << n) #Change 0xFEU to 0xFCU to change priority from 6 to 4.
```

Add ```RTX``` in build.sh.\
Add ```-I$RTXDIR``` to CFLAGS\
Add ```$RTXDIR/librtx.a``` to linker command line.
