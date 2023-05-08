all:
	cd build; emmake make -j8
	cp build/beshell.js ../../../beconsole.cloud/public/vm/last
	cp build/beshell.wasm ../../../beconsole.cloud/public/vm/last

cmake:
	cd build; emcmake cmake ..

fs: 
	mkdir -p tmp
	rm -rf tmp/*
	cp -rf ../../filesystem/root/* tmp
	rm -rf tmp/home/become/* 
	rm -rf tmp/mnt/*
	find tmp -name "*.js.bin" -exec rm {} \;
	cp -rf src.fs/* tmp/
	rm -rf build/fs.zip
	cd tmp; zip -r ../build/fs.zip .
	cp build/fs.zip ../../../beconsole.cloud/public/vm/last


help:
	@echo ". ../emsdk/emsdk_env.fish"
	@echo "make"
	@echo "make cmake"


	