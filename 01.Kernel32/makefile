all: Kernel32.bin

# $<: Dependency의 첫번째 파일
Kernel32.bin: Source/EntryPoint.s
	nasm -o Kernel32.bin $<

clean: rm -f Kernel32.bin