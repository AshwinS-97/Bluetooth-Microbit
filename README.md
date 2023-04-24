## MICROBIT - BLUETOOTH
-- Vector table is controlled by the soft device and our defined vector table is called with indirection through soft device vector table.

nRF5 SDK downloaded from:
https://www.nordicsemi.com/Products/Development-software/nRF5-SDK/Download

Change gcc path and version in <nrf>/components/toolchain/gcc/Makefile.posix to path where our gcc toolchain is installed.

pca10100.h board uses the same processor part number as in Microbit, so we use that code and change the pin numbers. Copy one of the existing board files (say, pca10040) and make changes as per Microbit schematics.(Added microbit.h to the board)

Modify nrflib/components/boards/boards.h to include Microbit. Add the following lines after arduino_primo board:
#elif defined (BOARD_MICROBIT)
    #include "microbit.h"

Change NRF52832 to NRF52833 in modules/nrfx/mdk/nrf.h:
/* Redefine "old" too-generic name NRF52 to NRF52833_XXAA to keep backwards compatibility. */change here
#if defined (NRF52)
    #ifndef NRF52833_XXAA		 //change here
        #define NRF52833_XXAA	 //change here
    #endif
#endif

Copied ble_app_uart example and renamed as ble

We will use files from pca10100 board (it uses NRF52833, same as Microbit). We will simplify the directory structure and remove the unnecessary files.
```bash
cd ble`
mv pca10100/s113/armgcc/* .             # Makefile and linker script
mv pca10100/s113/config/sdk_config.h .  # Configuration
rm -rf pca* ble_app_uart.eww            # remove other boards and tools
```

Copy the correct softdevice hex file and remove others:
'''
cp <nrf>/components/softdevice/s113/hex/s113_nrf52_7.2.0_softdevice.hex hex
rm hex/ble*.hex		# remove other soft devices
'''

Modify configuration: pca10100 uses a crystal for low frequency clock used by BLE. Microbit uses internal RC oscillator. We need to reflect this change in sdk_config.h.
'''config
#define NRF_SDH_CLOCK_LF_SRC 0            # change from 1 to 0
#define NRF_SDH_CLOCK_LF_RC_CTIV 16       # change from 0 to 16
#define NRF_SDH_CLOCK_LF_RC_TEMP_CTIV 2   # change from 0 to 2
#define NRF_SDH_CLOCK_LF_ACCURACY 1       # change from 7 to 1
```

Make the following changes in Makefile:

Change BOARD_PCA10100 to BOARD_MICROBIT
Change SDK_ROOT := ../../../../../.. to (path of nRF5 SDK)
Change PROJ_DIR := ../../.. to .
INC_FOLDERS: change line containing ../config to $(PROJ_DIR)
After the following line:
CFLAGS += -Wall -Werror
Add the line below:
CFLAGS += -Wno-array-bounds
(We ignore this warning as the build fails with array-bounds error. Maybe the array-bounds
check was introduced in a later version of the tool-chain.)

