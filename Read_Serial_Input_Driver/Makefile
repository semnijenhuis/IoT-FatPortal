BINARY 			:= mychardev
KERNEL 			:= /home/tjoostrik/Downloads/rutos-ath79-trb2-sdk/build_dir/target-mips_24kc_musl/linux-ath79_24kc/linux-5.4.285
ARCH			:= mips
OBJECTS 		:= hello_cdev.o
CROSS_COMPILE	:= /home/tjoostrik/Downloads/rutos-ath79-trb2-sdk/staging_dir/toolchain-mips_24kc_gcc-8.4.0_musl/bin/mips-openwrt-linux-musl-

obj-m += $(BINARY).o

$(BINARY)-y	:= $(OBJECTS)


all: 
	make -C $(KERNEL) M=$(PWD) ARCH=mips CROSS_COMPILE=$(CROSS_COMPILE) modules
load:
	sudo insmod $(BINARY).ko
unload:
	sudo rmmod $(BINARY)
clean: 
	make -C $(KERNEL) M=$(PWD) clean