# Read Serial port
In this program the serial interface of the MPPT is read and saved. The MPPT has a VE.Direct protocol which is send over rs232. The MPPT will send the data every second over this protocol. In this program their are 2 messages long received, and these are trimt to 1 full message. Next this message is saved in a character device

## Program
This program will read the incomming rs232 messages from the Victron MPPT. Because the rs232 is a device you can read this information. A message of the MPPT has a size of about 180 bytes. To save 2 full messages the device is read 2 times with the size of 180 bytes. Next the start and the end of the message are found and returned to the main function. From here the message will be saved in the serial character device driver. This program can be started end will run until it will encounter a error or is exited.

## Cross compile program
Go to the direction of the program. Next define the environment variables. This is explained in the main readme and looks something like this: 
```bash
export STAGING_DIR=~/Documents/rutos-ath79-trb2-sdk/staging_dir
export TOOLCHAIN=${STAGING_DIR}/toolchain-mips_24kc_gcc-8.4.0_musl
export CC=${TOOLCHAIN}/bin/mips-openwrt-linux-musl-gcc
```
Finally, you can simply call the `make` command in the terminal and the programs will be created. 

To check if the compilation was succesfull you can run `readelf -h readSerial` and if the output shows ELF32 and MIPS R3000, as shown in the picture below, the program compiled succesfully. 

![Output of readelf -h readSerial](/Images/Compiled_Program.png)

# Install 
Before you run the program you need to make sure the defined serial device driver exists. If you want to install the driver look at [serial character device driver](../Save_Serial_Data_Driver/Readme.md). Next you can either run the program manual or add it as a startup script, but before you can run it you need to copy it to the router with for example scp. Next you can run it with `./readSerial` if you are in the same directory as the program, and stop the program using CTRL+C. If you want to run this from the startup go to router webUI->System->Maintenance->Custom Scripts, and add the following line to the script before `exit 0`: `/path/to/program/readSerial &`. The & at the and makes sure the router won't stay in the program itself and will put the program in the background. !!MAKE SURE YOU CLICK `SAVE & APPLY` BECAUSE OTHERWISE THESE CHANGES WILL NOT BE SAVED!!
