
TARGET:=$(shell node filesystem/target.js)

all: mkonly dist-beshell



mkonly:
	idf.py build
	@echo ''
	@echo ''
	
ifeq ($(TARGET),esp32s3)
	cp build/beshell.bin filesystem/img/beshell-s3.bin
	ls -lh filesystem/img/beshell-s3.bin
else ifeq ($(TARGET),esp32)
	cp build/beshell.bin filesystem/img/beshell.bin
	ls -lh filesystem/img/beshell.bin
else
	@echo unknow target: ${TARGET}
endif


clear-jsbin:
	find filesystem -name "*.js.bin" | xargs rm -f

compile:
	# 编译js
	./pc/build/beshell --root ./filesystem/root --script ./pc/compile-fs.js

tree-shaking:
	node filesystem/tree-shaking.js

mkfs-root:
	node filesystem/rawfs-pack-dir.js

mkfs-home:
	bin/mklittlefs -c filesystem/tmp/home -s 204800 filesystem/img/fs-home.img -d 5

mkfs-home-with-roms:
	bin/mklittlefs -c filesystem/home-with-roms -s 3145728 filesystem/img/fs-home.img -d 5

dist:
	node filesystem/dispense-to-beconsole.js all
dist-beshell:
	node filesystem/dispense-to-beconsole.js beshell
dist-fs:
	node filesystem/dispense-to-beconsole.js fs
dist-root:
	node filesystem/dispense-to-beconsole.js fs-root
dist-home:
	node filesystem/dispense-to-beconsole.js fs-home
new-version:
	mkdir 

partition:
	node filesystem/mk-partitions.js
	python3 /mnt/d/lib/esp-idf-v4.4/components/partition_table/gen_esp32part.py filesystem/partitions-4MB.csv filesystem/img/partitions-4MB.bin
	python3 /mnt/d/lib/esp-idf-v4.4/components/partition_table/gen_esp32part.py filesystem/partitions-16MB.csv filesystem/img/partitions-16MB.bin


pack-fs: tree-shaking mkfs-root mkfs-home partition dist-fs
	ls -lh filesystem/img/
pack-all: tree-shaking mkfs-root mkfs-home partition dist
	ls -lh filesystem/img/

# 编译js, 打包 / 和 /home 分区，并制作 img 文件
fs: compile pack-fs

# 使用 js 源文件打包 / 和 /home 分区，并制作 img 文件
fs-src: clear-jsbin pack-fs

# 编译js, 打包 / 分区，并制作 img 文件
fs-root: compile tree-shaking mkfs-root dist-root

# 编译js, 打包 / 分区，并制作 img 文件
fs-root-src: clear-jsbin tree-shaking mkfs-root dist-root

# 编译js, 打包 /home 分区，并制作 img 文件
fs-home: compile tree-shaking mkfs-home partition dist-home

# 编译 telweb (vue3) ，并打包制作 / 分区 img文件
telweb-build:
	cd ../beconsole.telweb/source; npm run build
	cp ../beconsole.telweb/source/dist/index.html filesystem/root/lib/local/telweb/
	gzip -f filesystem/root/lib/local/telweb/index.html
	ls -lh filesystem/root/lib/local/telweb


telweb-pack: tree-shaking mkfs-root

telweb: telweb-build fs-root


help:
	@echo "make"
	@echo "make mkonly		# idf.py build"
	@echo "make dist		# dispense full firmware package to BeConsole dir"
	@echo "make dist-beshell	# dispense beshell firmware img file to BeConsole dir"
	@echo "make dist-fs		# dispense fs img files('/root' and '/home') to BeConsole dir"
	@echo "make dist-root		# dispense fs /root img file to BeConsole dir"
	@echo "make dist-home		# dispense fs /home img file to BeConsole dir"

	@echo "make compile		# compile all .js file to .bin"
	@echo "make tree-shaking	# 将 filesystem/root 和 filesystem/home 目录整体拷贝到 filesystem/tmp 目录下，剔除不需要打包的文件和子目录，为制作镜像做准备"
	@echo "make mkfs-root		# 用 filesystem/tmp/root 内的文件制作 root 分区镜像 (rawfs) , 镜像文件保存到 filesystem/img/fs-root.bin"
	@echo "make mkfs-home		# 用 filesystem/tmp/home 内的文件制作 home 分区镜像 (littlefs) , 镜像文件保存到 filesystem/img/fs-home.bin"
	@echo "make partition		# 生产分区表 csv 文件, 并进进一步生产 分区表二进制文件"
	@echo "make pack-fs		# tree-shaking + mkfs-root + mkfs-home + partition + dist-fs"
	@echo "make pack-all		# tree-shaking + mkfs-root + mkfs-home + partition + dist"
	@echo "make fs			# compile + pack-all"
	@echo "make fs-src		# clear-jsbin + pack-all"
	@echo "make fs-root		# compile tree-shaking mkfs-root dist-root"
	@echo "make fs-root-src	# clear-jsbin tree-shaking mkfs-root dist-root"
	@echo "make telweb-build"
	@echo "make telweb-pack"
	@echo "make telweb		# telweb-build + telweb-pack"

?: help

# cd filesystem/tmp/root; tar -cvf ../root.tar  *