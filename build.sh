#!/bin/sh
export STAGING_DIR=/home/tjoostrik/Downloads/rutos-ath79-trb2-sdk/staging_dir
export TOOLCHAIN=${STAGING_DIR}/toolchain-mips_24kc_gcc-8.4.0_musl
export CC=${TOOLCHAIN}/bin/mips-openwrt-linux-musl-gcc

# Build !
#${CC} /home/tjoostrik/Downloads/modbus/src/modbus.c -o modbus
${CC} -I./include main.c -L./lib -lmodbus -o main

sshpass -p "100Teltonika!" scp ./main root@178.229.97.8:/root
