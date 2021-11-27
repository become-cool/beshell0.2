all:
	../bin/mklittlefs -c root -s 524288 img/fs-root.img -d 2
	../bin/mklittlefs -c home -s 131072 img/fs-home.img -d 2
	cp img/fs-root.img ../../beconsole/build/src/assets/firmware/beshell-0.2.0/flash/fs-root.img
	cp img/fs-home.img ../../beconsole/build/src/assets/firmware/beshell-0.2.0/flash/fs-home.img
	
	python2 /mnt/d/lib/esp-idf/components/partition_table/gen_esp32part.py partitions-4MB.csv img/partitions-4MB.bin
	python2 /mnt/d/lib/esp-idf/components/partition_table/gen_esp32part.py partitions-16MB.csv img/partitions-16MB.bin
	cp img/partitions-4MB.bin ../../beconsole/build/src/assets/firmware/beshell-0.2.0/flash/partitions-4MB.bin
	cp img/partitions-16MB.bin ../../beconsole/build/src/assets/firmware/beshell-0.2.0/flash/partitions-16MB.bin