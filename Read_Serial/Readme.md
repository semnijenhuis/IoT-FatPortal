# Read Serial port
In this program the serial interface of the MPPT is read and saved. The MPPT has a VE.Direct protocol which is send over rs232. The MPPT will send the data every second over this protocol. In this program their are 2 messages long received, and these are trimt to 1 full message. Next this message is saved in a character device

## Program
This program will read the incomming rs232 messages from the Victron MPPT. Because the rs232 is a device you can read this information. A message of the MPPT has a size of about 180 bytes. To save 2 full messages the device is read 2 times with the size of 180 bytes. Next the start and the end of the message are found and returned to the main function. From here the message will be saved in the serial character device driver.

## Cross compile program
Go to the direction of the program. Next define the environment variables. This is explained in the main readme and looks something like this: 
```bash
export STAGING_DIR=~/Documents/rutos-ath79-trb2-sdk/staging_dir
export TOOLCHAIN=${STAGING_DIR}/toolchain-mips_24kc_gcc-8.4.0_musl
export CC=${TOOLCHAIN}/bin/mips-openwrt-linux-musl-gcc
```
Finally, you can simply call the make command in the terminal and the programs will be created. To check if the compilation was succesfull you can run `readelf -h readSerial` and if the output shows ELF32 and MIPS R3000 the program compiled succesfully. 

![Output of readelf -h libmodbus.a](/Images/Compiled_Program.png)
