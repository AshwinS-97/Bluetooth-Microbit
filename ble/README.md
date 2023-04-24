
## Bluetooth Setup
We will use files from pca10100 board (it uses NRF52833, same as Microbit). We will simplify the directory structure and remove the unnecessary files.
```bash
cd ble
mv pca10100/s113/armgcc/* .             # Makefile and linker script
mv pca10100/s113/config/sdk_config.h .  # Configuration
rm -rf pca* ble_app_uart.eww            # remove other boards and tools
```

Copy the correct softdevice hex file and remove others:
```bash
cp <nrf>/components/softdevice/s113/hex/s113_nrf52_7.2.0_softdevice.hex hex
rm hex/ble*.hex		# remove other soft devices
```

Modify configuration: pca10100 uses a crystal for low frequency clock used by BLE. Microbit uses internal RC oscillator. We need to reflect this change in sdk_config.h.
```config
#define NRF_SDH_CLOCK_LF_SRC 0            # change from 1 to 0
#define NRF_SDH_CLOCK_LF_RC_CTIV 16       # change from 0 to 16
#define NRF_SDH_CLOCK_LF_RC_TEMP_CTIV 2   # change from 0 to 2
#define NRF_SDH_CLOCK_LF_ACCURACY 1       # change from 7 to 1
```

Make the following changes in Makefile:
```Makefile
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
```

Run ```make```. It should generate ```_build/nrf52833_xxaa.hex```

We need a tool to merge soft device and application hex files. To do so, download command line tools from :

https://www.nordicsemi.com/Products/Development-tools/nrf-command-line-tools/download#infotabs


Download tar.gz and untar in ```/opt/```. Add ```/opt/nrf-command-line-tools/bin``` to PATH in your .bashrc.

Once done merge the hex with the softdevice hex (order is important)
```bash
mergehex -m hex/s113_nrf52_7.2.0_softdevice.hex _build/nrf52833_xxaa.hex -o MICROBIT.hex
```

Copy MICROBIT.hex to board. If Successfull top row Leds should blink

To test, download nRF Toolbox from Google store.

Run UART service and select Nordic_UART device. It opens up a message  window where you can type messages that will show up in UART terminal  window and vise-versa.