#!/bin/sh
export STAGING_DIR=/home/tjoostrik/Downloads/rutos-ath79-trb2-sdk/staging_dir
export TOOLCHAIN=${STAGING_DIR}/toolchain-mips_24kc_gcc-8.4.0_musl
export CC=${TOOLCHAIN}/bin/mips-openwrt-linux-musl-gcc

# Build
make

# Send to router
sshpass -p "100Teltonika!" scp ./serial root@178.229.39.1:/root
