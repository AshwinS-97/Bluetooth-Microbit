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