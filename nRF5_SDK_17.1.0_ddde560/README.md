
## nRF5 SDK Setup
nRF5 SDK downloaded from:
https://www.nordicsemi.com/Products/Development-software/nRF5-SDK/Download

#### Modifications:
1.   gcc path and version in ```<nrf>/components/toolchain/gcc/Makefile.posix``` is changed to path where our gcc toolchain is installed.
2.   To support MICROBIT, we have to add our board to boards.h. Copy one of the existing board files (say, pca10100, beacuase it uses the same processor) and make changes as per Microbit schematics.
3. Modify ```<nrf>/components/boards/boards.h``` to include Microbit. Add the following lines after arduino_primo board:
```lines
#elif defined (BOARD_MICROBIT)
    #include "microbit.h"
```
4. Change NRF52832 to NRF52833 in ```modules/nrfx/mdk/nrf.h```:
```code
//Redefine "old" too-generic name NRF52 to NRF52833_XXAA to keep backwards compatibility. 
#if defined (NRF52)
    #ifndef NRF52833_XXAA		 //change here
        #define NRF52833_XXAA	 //change here
    #endif
#endif
```
5. We will ble_app_uart example and renamed as ble. Configuration of Bluetooth can be found here: