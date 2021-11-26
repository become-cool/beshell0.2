idf.py build || exit 1

echo ''
echo ''
ls -lh build/beshell.bin

echo ''
echo ===============================
echo copy to beconsole dist dir ...
cp build/beshell.bin ../beconsole/build/src/assets/firmware/beshell-1.1.9/flash/beshell.bin

echo ''
echo ''
date
echo ''

# echo ===============================
# python D:\lib\esp-idf\components\esptool_py\esptool\esptool.py --chip esp32 merge_bin --fill-flash-size 4MB -o qemu/beshell-image.bin --flash_size 4MB 0x1000 qemu/raw/bootloader.bin 0x8000 qemu/raw/partitions.bin 0x10000 build/beshell.bin 0x190000  qemu/raw/fs-root.img 0x1D0000 qemu/raw/fs-home.img
