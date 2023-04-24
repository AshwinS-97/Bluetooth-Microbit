## MICROBIT - BLUETOOTH
-- Vector table is controlled by the soft device and our defined vector table is called with indirection through soft device vector table.

nRF5 SDK downloaded from:
https://www.nordicsemi.com/Products/Development-software/nRF5-SDK/Download

Change gcc path and version in <nrf>/components/toolchain/gcc/Makefile.posix to path where our gcc toolchain is installed.

pca10100.h board uses the same processor part number as in Microbit, so we use that code and change the pin numbers. (Added microbit.h to the board)