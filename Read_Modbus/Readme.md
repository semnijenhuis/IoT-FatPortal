# Read Modbus
This is a program which will read the modbus of the Teltonika TRB245. TODO describe how to turn on the modbus
To run this program you need to hava the libmodbus (cross-)compiled for the Teltonika TRB245. Next you need to either copy the modubs library to the staging_dir/toolchain/lib of the SDK, or set it in the libs folder at the head of the repository.

## Program
This program will read the modbus by using the modbus library to settle a communication with the modbus. The modbus will always return the values in 16 bit values. This values need to be converted to the desired values: characters of 8 bits for texts, 32 bit float values, and 32 bit (un)signed integers (long). After each piece of data the data is added as if it is a JSON. This is done, because the final data needs to be send as a JSON. At last the data will be saved in a character device. A connection will be made to the char device, and the data will be saved in that.

## Cross compile library
To cross-compile the library you first need to download the library. This can be done by cloning it from git with the command `git clone https://github.com/stephane/libmodbus.git`. Next go into the directory you just cloned with `libmodbus`. Next you need to compile the library for a ELF32 and MIPS3000 system which runs on the Teltonika router. To do this you first need to run the `autogen.sh` script by using `./autogen.sh`. Next a configure file is created. For the installation you need to enter a place where you want to install the library in. In the next example I am going to install the library in the documents/build directory. `./configure --host=mipsel --enable-static --prefix=/home/tjoostrik/Documents/build`. Next you need to run `make` and `make install` to install the library in the given directory. If you enter the library you specified to install you see 3 directories: include, libs, and share. For this program you need to copy the include/modbus folder into the include folder of this repo, and the .a and .la in the libs folder to the libs folder in the repo. To check if the library is cross-compiled succesfully you need to run `readelf -h libmodbus.a` and if you see the ELF32 and MIPS R3000 the library is compiled correctly.

![Output of readelf -h libmodbus.a](/Images/Compiled_Program.png)

## Turn on Modbus
Before you can connect to the modbus you need to active this. This can be done in the webUI of the router. In here you go to Services->Modbus->Modbus TCP Server. Make sure the port and device nnumber are the same as defined in the readModbus.c file on lines 39 aaand 46 respectively

 ![Settings of the Modbus](/Images/Screenshot_Modbus.png). 
 
 !!MAKE SURE THAT AFTER YOU MAKE A CHANGE YOU CLICK SAVE & APPLY!!

## Cross compile program
To cross-compile the programm make sure you have the libmodbus in the good place see the first alinea. Next define the environment variables. This is explained in the main readme and looks something like this: 
```bash
export STAGING_DIR=~/Documents/rutos-ath79-trb2-sdk/staging_dir
export TOOLCHAIN=${STAGING_DIR}/toolchain-mips_24kc_gcc-8.4.0_musl
export CC=${TOOLCHAIN}/bin/mips-openwrt-linux-musl-gcc
```
Next you can simply call the `make` command in the terminal and the programs will be created.

To check if the program succesfully compiled you can do `readelf -h ./readModbus`, and if you did it succesfully you will see the ELF32 and MIPS R3000 just as with the library compile.

# Install
Before you run the program you need to make sure the defined serial device driver exists. If you want to install the driver look at [router character device driver](../Save_Router_Data_Driver/Readme.md). Next you can either run the program manual or add it to the crontab, but before you can run it you need to copy it to the router with for example scp. Next you can run it with `./readModbus` if you are in the same directory as the program. If you want to add it to the crontab, you can run `crontab -e`. Next you click on  the `i` key to be able to edit the crontab, add the line `* * * * * /path/to/program/readModbus` to the bottom of the file, click on ESC, type `:wq`, and click on ENTER. Now the readModbus program will run every minute.
