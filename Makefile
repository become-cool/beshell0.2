
all:
	./build.sh
	
clear:
	find filesystem -name "*.js.bin" | xargs rm -f

compile:
	# 编译js
	./pc/build/beshell --root ./filesystem/root --script ./pc/compile-fs.js

packfs:
	node filesystem/packfs.js

mkfs-root:
	bin/mklittlefs -c filesystem/tmp/root -s 655360 filesystem/img/fs-root.img -d 5
	cp filesystem/img/fs-root.img ../beconsole.next/platform/nw.js/assets/firmware/beshell-0.2.0/flash/fs-root.img
	cp filesystem/img/fs-root.img ../beconsole.next/platform/nw.js/tmp/assets/firmware/beshell-0.2.0/flash/fs-root.img

mkfs-home:
	bin/mklittlefs -c filesystem/tmp/home -s 131072 filesystem/img/fs-home.img -d 5
	cp filesystem/img/fs-home.img ../beconsole.next/platform/nw.js/assets/firmware/beshell-0.2.0/flash/fs-home.img
	cp filesystem/img/fs-home.img ../beconsole.next/platform/nw.js/tmp/assets/firmware/beshell-0.2.0/flash/fs-home.img

partition:
	python2 /mnt/d/lib/esp-idf/components/partition_table/gen_esp32part.py filesystem/partitions-4MB.csv filesystem/img/partitions-4MB.bin
	python2 /mnt/d/lib/esp-idf/components/partition_table/gen_esp32part.py filesystem/partitions-16MB.csv filesystem/img/partitions-16MB.bin
	cp filesystem/img/partitions-4MB.bin ../beconsole.next/platform/nw.js/assets/firmware/beshell-0.2.0/flash/partitions-4MB.bin
	cp filesystem/img/partitions-16MB.bin ../beconsole.next/platform/nw.js/assets/firmware/beshell-0.2.0/flash/partitions-16MB.bin


pack-all: packfs mkfs-root mkfs-home partition

fs: compile pack-all

telweb-build:
	cd ../beconsole.telweb/source; npm run build
	cp ../beconsole.telweb/source/dist/index.html filesystem/root/lib/local/telweb/
	gzip -f filesystem/root/lib/local/telweb/index.html
	ls -lh filesystem/root/lib/local/telweb


telweb-pack: packfs mkfs-root

telweb: telweb-build telweb-pack

