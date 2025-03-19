# Save router data
In this character device driver the router data is saved. There are open, close, read, and write funtions to use the driver as desired. The router driver has mayor number of 250 to make it easier for setting the driver on startup. It also has a buffer size of 256, because that is enough for now to save the router data in. If necessary this can be easily exstended in the top of routerDriver.c. The data buffer is cleared with every new write to not interact with the new message

## Cross compile program
Go to the direction of the program. Next define the environment variables in the makefile. Here you can see the approximatly the location where the `KERNEL` should be. This can be different if you use a different version of the SDK. You likewise also need to change the `CROSS_COMPILER` should be. Make sure you don't add the last gcc part that is used in the other programs.

Finally, you can simply call the `make` command in the terminal and the programs will be created. 

To check if the compilation was succesfull you can run `readelf -h routerDriver.ko` and if the output shows ELF32 and MIPS R3000, as shown in the picture below, the program compiled succesfully. 

![Output of readelf -h libmodbus.a](/Images/Compiled_Program.png)

# Instal Driver
The only necessary file of the compiled files you need is the `routerDriver.ko` file. You can copy this file to the router with for example the scp command. When you are on the router in the same folder as the `routerDriver.ko` file you can simply type `insmod routerDriver.ko`. Next you can link the driver as a device with the command `mknod /dev/routerDriver c 250 0`. To check if the driver works you can simply put 1 character in the driver with `echo t\ > /dev/routerDriver`, and retrieve the character with `head -1 /dev/routerDriver`. As shown in the image below

![Output of readelf -h libmodbus.a](/Images/Set_Info_In_Char_Dev.png)

To remove the driver you can run `rmmod routerDriver`.
