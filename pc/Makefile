all:
	ninja -C build

config:
	cd build; cmake -G Ninja -S .. -B . ; cd -

debug:
	gdbgui build/beshell
