python2 /mnt/d/lib/esp-idf/components/esptool_py/esptool/esptool.py --chip esp32 merge_bin --fill-flash-size 4MB -o beshell-image.bin --flash_size 4MB \
0x1000 raw/bootloader.bin \
0x8000 raw/partitions.bin \
0x10000 raw/beshell.bin \
0x190000  raw/fs-root.img \
0x1D0000 raw/fs-home.img
