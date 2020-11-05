all:
	 ../bin/mkfatfs -c etc -s 131072 img/etc.img -d 2
	 ../bin/mkfatfs -c etc -s 2490368 img/home-4MB.img -d 2
	 ../bin/mkfatfs -c home -s 15073280 img/home-16MB.img -d 2
	
	python2 ../esp-idf/components/partition_table/gen_esp32part.py partitions-4MB.csv img/partitions-4MB.bin
	python2 ../esp-idf/components/partition_table/gen_esp32part.py partitions-16MB.csv img/partitions-16MB.bin

	cp img/etc.img ../../beconsole/source/public/assets/firmware/beshell-1.1.0/flash/etc.img
	cp img/home-4MB.img ../../beconsole/source/public/assets/firmware/beshell-1.1.0/flash/home-4MB.img
	cp img/home-16MB.img ../../beconsole/source/public/assets/firmware/beshell-1.1.0/flash/home-16MB.img
	cp img/partitions-4MB.bin ../../beconsole/source/public/assets/firmware/beshell-1.1.0/flash/partitions-4MB.bin
	cp img/partitions-16MB.bin ../../beconsole/source/public/assets/firmware/beshell-1.1.0/flash/partitions-16MB.bin



