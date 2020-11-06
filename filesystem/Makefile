all:
	 ../bin/mklittlefs -c source -s 2621440 img/fs-4MB.img -d 2
	 ../bin/mklittlefs -c source -s 15204352 img/fs-16MB.img -d 2
	
	python2 ../esp-idf/components/partition_table/gen_esp32part.py partitions-4MB.csv img/partitions-4MB.bin
	python2 ../esp-idf/components/partition_table/gen_esp32part.py partitions-16MB.csv img/partitions-16MB.bin

	cp img/fs-4MB.img ../../beconsole/source/public/assets/firmware/beshell-1.1.0/flash/fs-4MB.img
	cp img/fs-16MB.img ../../beconsole/source/public/assets/firmware/beshell-1.1.0/flash/fs-16MB.img
	cp img/partitions-4MB.bin ../../beconsole/source/public/assets/firmware/beshell-1.1.0/flash/partitions-4MB.bin
	cp img/partitions-16MB.bin ../../beconsole/source/public/assets/firmware/beshell-1.1.0/flash/partitions-16MB.bin



