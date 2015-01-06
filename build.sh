#!/bin/bash

export PATH=/home/supermap/OpenThings/ESP8266App/esp-open-sdk/xtensa-lx106-elf/bin:$PATH
echo $PATH

echo "================================="
echo "Building wixcmd for ESP8266..."
echo "================================="

echo "clean..."
make clean
make --makefile=Makefile.openthings

CWD="/firmware/esp.esp_mqtt.bin"
OUTPUTBIN=$(pwd)$CWD

FILE_00000="./firmware/0x00000.bin"
FILE_40000="./firmware/0x40000.bin"
FILE_7C000="./../../esp-open-sdk/esp_iot_sdk_v0.9.5/bin/esp_init_data_default.bin"
FILE_7E000="./../../esp-open-sdk/esp_iot_sdk_v0.9.5/bin/blank.bin"

FILE_00000_SIZE=$(stat -c%s $FILE_00000)
FILE_40000_SIZE=$(stat -c%s $FILE_40000)
FILE_7C000_SIZE=$(stat -c%s $FILE_7C000)
FILE_7E000_SIZE=$(stat -c%s $FILE_7E000)

echo "build blank flash image"
IMAGESIZE=$((0x7E000 + FILE_7E000_SIZE))
#thank igr for this tip
dd if=/dev/zero bs=1 count="$IMAGESIZE" conv=notrunc | LC_ALL=C tr "\000" "\377" > "$OUTPUTBIN"

echo "patch image with bins"
dd if="$FILE_00000" of="$OUTPUTBIN" bs=1 seek=0 count="$FILE_00000_SIZE" conv=notrunc
JUMP=$((0x40000))
dd if="$FILE_40000" of="$OUTPUTBIN" bs=1 seek="$JUMP" count="$FILE_40000_SIZE" conv=notrunc  
JUMP=$((0x7C000))
dd if="$FILE_7C000" of="$OUTPUTBIN" bs=1 seek="$JUMP" count="$FILE_7C000_SIZE" conv=notrunc  
JUMP=$((0x7E000))
dd if="$FILE_7E000" of="$OUTPUTBIN" bs=1 seek="$JUMP" count="$FILE_7E000_SIZE" conv=notrunc  
 
echo ">>build image [esp_mqtt] finished."
echo "Output To: "$OUTPUTBIN
cd ../../


