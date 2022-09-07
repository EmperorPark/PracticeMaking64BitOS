all: BootLoader Kernel32 Disk.img

BootLoader:
	@echo
	@echo ============== Bulid Boot Loader ==============
	@echo

	make -C 00.BootLoader

	@echo
	@echo ============== Bulid Complete ==============
	@echo

Kernel32:
	@echo
	@echo ============== Bulid 32bit Kernel ==============
	@echo

	make -C 01.Kernel32

	@echo
	@echo ============== Bulid Complete ==============
	@echo

# $^: Dependency에 나열된 모든 파일
Disk.img: 00.BootLoader/BootLoader.bin 01.Kernel32/Kernel32.bin

	@echo
	@echo ============== Disk Image Bulid Start ==============
	@echo

	./ImageMaker $^

	@echo
	@echo ============== All Bulid Complete ==============
	@echo

clean:
	make -C 00.BootLoader clean
	make -C 01.Kernel32 clean
	rm -f Disk.img