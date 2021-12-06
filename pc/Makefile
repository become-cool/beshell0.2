all:
	ninja -C build

config:
	cd build; cmake -G Ninja .. ; cd -

debug:
	gdbgui build/beshell
