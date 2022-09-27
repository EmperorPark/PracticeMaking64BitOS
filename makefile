all: BootLoader Kernel32 Kernel64 Disk.img Utility

BootLoader:
	@echo
	@echo ============== Build Boot Loader ==============
	@echo

	make -C 00.BootLoader

	@echo
	@echo ============== Build Complete ==============
	@echo

Kernel32:
	@echo
	@echo ============== Build 32bit Kernel ==============
	@echo

	make -C 01.Kernel32

	@echo
	@echo ============== Build Complete ==============
	@echo

Kernel64:
	@echo
	@echo ============== Build 64bit Kernel ==============
	@echo

	make -C 02.Kernel64

	@echo
	@echo ============== Build Complete ==============
	@echo

# $^: Dependency에 나열된 모든 파일
Disk.img: 00.BootLoader/BootLoader.bin 01.Kernel32/Kernel32.bin 02.Kernel64/Kernel64.bin

	@echo
	@echo ============== Disk Image Build Start ==============
	@echo

	./ImageMaker $^

	@echo
	@echo ============== All Build Complete ==============
	@echo

Utility:
	@echo
	@echo ============== Utility Build Start ==============
	@echo
	
	make -C 04.Utility

	@echo
	@echo ============== Utility Build Complete ==============
	@echo


clean:
	make -C 00.BootLoader clean
	make -C 01.Kernel32 clean
	make -C 02.Kernel64 clean
	make -C 04.Utility clean
	rm -f Disk.img