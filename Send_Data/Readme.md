# Send serial data
In this program the serial and router data are retrieved from the character device drivers and send to the FATportal with the curl command. The curl library is not used, because this will exceed the storage space of the Teltonika TRB245. This programma requires that you both have a serial character device driver, and router character device driver installed. To install this drivers check [router character device driver](../Save_Router_Data_Driver/Readme.md) and [serial character device driver](../Save_Serial_Data_Driver/Readme.md).

## Program
This program will read the character devices of the serial and router data, and will send this data to the FATportal. The data of the router will be saved as JSON without the brackets, and the data of the serial is saved as it is recieved from the MPPT. This program will convert the serial data to JSON data without the brackets. Next a string is created with a starting bracket, followed by the serial data, a comma, the router data, and at last a closing bracket. This will then be send to FATportal with the URL and PRODUCTID which are defined in the main.c at the top of the program. In the top of json.c you can change which parameters will be send from the serial data.

## Cross compile program
Go to the direction of the program. Next define the environment variables. This is explained in the main readme and looks something like this: 
```bash
export STAGING_DIR=~/Documents/rutos-ath79-trb2-sdk/staging_dir
export TOOLCHAIN=${STAGING_DIR}/toolchain-mips_24kc_gcc-8.4.0_musl
export CC=${TOOLCHAIN}/bin/mips-openwrt-linux-musl-gcc
```
Finally, you can simply call the `make` command in the terminal and the programs will be created. 

To check if the compilation was succesfull you can run `readelf -h sendData` and if the output shows ELF32 and MIPS R3000, as shown in the picture below, the program compiled succesfully. 

![Output of readelf -h sendData](/Images/Compiled_Program.png)


# Install
Before you run the program you need to make sure the defined serial and router device driver exists. If you want to install the drivers look at [router character device driver](../Save_Router_Data_Driver/Readme.md) and [serial character device driver](../Save_Serial_Data_Driver/Readme.md). Next you can either run the program manual or add it to the crontab, but before you can run it you need to copy it to the router with for example scp. Next you can run it with `./sendData` if you are in the same directory as the program. If you want to add it to the crontab, you can run `crontab -e`. Next you click on the `i` key to be able to edit the crontab, add the line `*/5 * * * * /path/to/program/sendData` to the bottom of the file, click on ESC, type `:wq`, and click on ENTER. Now the readModbus program will run every 5 minutes.
