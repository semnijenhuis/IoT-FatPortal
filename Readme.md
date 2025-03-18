To create a program for the Teltonika TRB245 on a other platform (OS and/or CPU Architecture) you need to cross-compile the program. Reasons to do this is by not having access to  the hardware, or because their is limited recources on the system.

# Install SDK
This tutorial is about how to install the SDK on a Ubuntu environment.

## NodeJS and NPM
First open the terminal and enter `sudo apt install nodejs npm -y` to install nodejs and npm. (For installing this SDK, nodejs verion 20 or higher, and npm version 10 or higher are required)

To test if you have the required nodejs and npm version installed, run to commands `node -v` and `npm -v`.

To upgrade your node versjon you can install nvm. You can install nvm with `wget -qO- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.1/install.sh | bash`. !!RESTART TERMINAL AFTER INSTALLATION!!.

When you installed NVM, you can check the available notes with `nvm ls`. Next with the command `nvm install <VERSION>`, you can install the version. Note that instead of the &lt;VERSION&gt; you enter the version listed with `nvm ls` (and is 20 or higher). If you installed the new node version NPM also should have been upgraded.

## Create SDK

Download the TRB 245 RUTOS SDK from [here](https://wiki.teltonika-networks.com/view/Software_Development_Kit), and in the row TRB245 you click download.

Next enter your terminal, and enter cd ~/Downloads.

You can see the Downloaded SDK with `ls`. It looks something like `TRB2_R_GPL_00.07.12.3.tar.gz`. Next decompress it into a folder of you likings, for example `~/Documents` with the command `tar -xf <SDK> -C ~/Documents/` with the direction after the -C the place where the SDK will be uncompressed, and on &lt;SDK&gt; the SDK you just downloaded. 

### Required packages

If you have a new ubuntu environment you probably are missing some packages. To install the SDK you need the following packages, with next to it the command to install this packages.
|Package|command|
|-|-|
|Lz4|`sudo apt install liblz4-dev`|
|Expat|`sudo apt install libexpat-dev`|
|Ncursus|`sudo apt install libncursus-dev`|
|Zstd|`sudo apt install libzstd-dev`|
|Cmake|`sudo apt install cmake -y`|
|Gperf|`sudo apt install gperf`|
|Python3 disutils|`sudo apt install python3-disutils-extra -y`|
|GNU (awk)|`sudo apt install gawk -y`|

### Finally install

Next go to the place you uncompressed the SDK, In my case `~/Documents/<SDK>` with the &lt;SDK&gt; the sdk of you uncompressed (Something like `rutos-ath79-trb2-sdk`), and run the command `./scripts/feeds update -a` to update the feeds.
This will take about a minute. Make sure that the end of the output has no errors. After this you will create the SDK with `make`. Note that you can give more cores (if available) to creating the SDK by using `make -j2` with after the `-j` the number of cores you want to use. You can check the number of available cores with `nproc`. After you entered the make command you it can take a long time (Up to an hour). It may end in a error, but can still work.

# Cross-compile code
Once you installed the SDK on your OS you can create a program and compile it with the just created SDK.

## Environment variables

To use the created SDK you need to set some environment variables to the SDK. You can do this in the terminal. !!NOTE after you closed the terminal the environment variables  are gone, and you need to set them again!!. 

```bash
export STAGING_DIR=/dir/to/sdk/rutos-ath79-trb2-sdk/staging_dir
export TOOLCHAIN=${STAGING_DIR}/toolchain-mips_24kc_gcc-8.4.0_musl
export CC=${TOOLCHAIN}/bin/mips-openwrt-linux-musl-gcc
```
!Note you need to changes this variables a little bit, but the main directions should be the same.

## Compile a program
There are multiple example programs created to read the serial connection, read router information from the modbus, creatte drivers to save the data, and send the data to the FATportal of 100%FAT.

To compile such a program make sure you have set the environment variables, go to the disired directory and run `make`.

To check if the program is succesfully compiled you can run `readelf -h <OUTPUT>` with on the place of the &lt;OUTPUT&gt;, te compiled program. This should be something like `main` or `routerDriver.ko`.

And example of the output is shown in the image below.

![Output of readelf -h routerDriver.ko](/Images/Compiled_Program.png)

To check if it is compiled correctly you can look at the class (should be ELF32), and the Type (should be MIPS R3000).

# Programs
In this git repository are 5 programs which will do different things. All programs together make sure Serial data is written and saved from the RS232 port of the teltonika router, the router data is written and save, and all this data is send to the FATportal.

|Program|Description|
|-|-|
|Read_Modbus|Read the data from the Modbus, which is part of Teltonika's RUTOS to be able to control the router, converts the data to JSON, and saves it in the router data device driver|
|Read_Serial|Read the data from the Serial port, and save it in the serial data driver|
|Save_Router_Data_Driver|A character device driver where the router data is saved|
|Save_Serial_Data_Driver|A character device driver where the serial data is saved|
|Send_Data|A program which reads the data from the serial device driver, converts it to JSON, add the modbus JSON data from the device driver, and send it to the FATportal|

To find more information about the programs self their are more detailed readme's in the folders.
