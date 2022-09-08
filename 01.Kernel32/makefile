#####################################################################
# 빌드 환경 및 규칙 설정
#####################################################################
NASM32 = nasm
GCC32 = gcc -c -m32 -ffreestanding # 독립실행이 가능하도록 ffreestanding 옵션들어감
LD32 = x86_64-linux-gnu-ld -melf_i386 -T ../elf_i386.x -nostdlib -e Main -Ttext 0x10200
OBJCOPY32 = /usr/bin/x86_64-linux-gnu-objcopy -j .text -j .data -j .rodata -j .bss -S -O binary # OBJCOPY를 이용해 섹션을 빼내고 운영체제에 의존하는 부분을 제거

OBJECTDIRECTORY = Temp
SOURCEDIRECTORY = Source


#####################################################################
# 빌드 항목 및 빌드 방법 설정 
#####################################################################

all: prepare Kernel32.bin

prepare:
	mkdir -p $(OBJECTDIRECTORY)

# $@는 생성될 파일, 현상황에는 $(OBJECTDIRECTORY)/EntryPoint.bin
$(OBJECTDIRECTORY)/EntryPoint.bin: $(SOURCEDIRECTORY)/EntryPoint.s
	$(NASM32) -o $@ $<

dep:
	@echo === Make Dependency File ===
	make -C $(OBJECTDIRECTORY) -f ../makefile InternalDependency
	@echo === Dependency Search Complete ===

ExecuteInternalBulid: dep
	make -C $(OBJECTDIRECTORY) -f ../makefile Kernel32.elf

$(OBJECTDIRECTORY)/Kernel32.elf.bin: ExecuteInternalBulid
	$(OBJCOPY32) $(OBJECTDIRECTORY)/Kernel32.elf $@

Kernel32.bin: $(OBJECTDIRECTORY)/EntryPoint.bin $(OBJECTDIRECTORY)/Kernel32.elf.bin
	cat $^ > $@

clean:
	rm -f *.bin
	rm -f $(OBJECTDIRECTORY)/*.*

#####################################################################
# Make에 의해 다시 호출 되는 부분, Temp 디렉터리 기준으로 수행
#####################################################################

CENTRYPOINTOBJECTFILE = Main.o
CSOURCEFILES = $(wildcard ../$(SOURCEDIRECTORY)/*.c) # wildcard 함수: 디렉터리에 특정 패턴 추출
ASSEMBLYSOURCEFILES = $(wildcard ../$(SOURCEDIRECTORY)/*.asm)
COBJECTFILES = $(subst Main.o, ,$(notdir $(patsubst %.c,%.o,$(CSOURCEFILES)))) # subst: 문자열 치환, COBJECTFILES에 있는 문자열 중 Main.o를 공백으로 치환 # patsubst: 패턴치환 함수, CSOURCEFILES 변수에서 c의 확장자를 가지는 모든 문자열 확장자를 o로 교체함
ASSEMBLYOBJECTFILES = $(notdir $(patsubst %.asm,%.o,$(ASSEMBLYSOURCEFILES)))


# 모든 c파일을 o로 출력 정의
%.o : ../$(SOURCEDIRECTORY)/%.c  # $< Dependency의 첫번째 항목
	$(GCC32) -c $<

# .asm 파일을 .o 파일로 출력 정의
%.o: ../$(SOURCEDIRECTORY)/%.asm
	$(NASM32) -f elf32 -o $@ $<


# -MM 전처리기 옵션 시스템 헤더파일(stdio.h같은거) 제외하고 헤더파일 의존성 추출 
InternalDependency:
	$(GCC32) -MM $(CSOURCEFILES) > Dependency.dep

Kernel32.elf: $(CENTRYPOINTOBJECTFILE) $(COBJECTFILES) $(ASSEMBLYOBJECTFILES)
	$(LD32) -o $@ $^


ifeq (Dependency.dep, $(wildcard Dependency.dep))
include Dependency.dep
endif