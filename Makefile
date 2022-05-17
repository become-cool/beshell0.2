
all:
	./build.sh
	
fs:
	./pc/build/beshell --root ./filesystem/root --script ./pc/compile-fs.js

	node filesystem/packfs.js
	bin/mklittlefs -c filesystem/tmp/root -s 524288 filesystem/img/fs-root.img -d 5
	bin/mklittlefs -c filesystem/tmp/home -s 262144 filesystem/img/fs-home.img -d 5
	cp filesystem/img/fs-root.img ../beconsole.next/platform/nw.js/assets/firmware/beshell-0.2.0/flash/fs-root.img
	cp filesystem/img/fs-home.img ../beconsole.next/platform/nw.js/assets/firmware/beshell-0.2.0/flash/fs-home.img
	
	python2 /mnt/d/lib/esp-idf/components/partition_table/gen_esp32part.py filesystem/partitions-4MB.csv filesystem/img/partitions-4MB.bin
	python2 /mnt/d/lib/esp-idf/components/partition_table/gen_esp32part.py filesystem/partitions-16MB.csv filesystem/img/partitions-16MB.bin
	cp filesystem/img/partitions-4MB.bin ../beconsole.next/platform/nw.js/assets/firmware/beshell-0.2.0/flash/partitions-4MB.bin
	cp filesystem/img/partitions-16MB.bin ../beconsole.next/platform/nw.js/assets/firmware/beshell-0.2.0/flash/partitions-16MB.bin
