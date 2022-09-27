#####################################################################
# 빌드 환경 및 규칙 설정
#####################################################################
NASM64 = nasm -f elf64
GCC64 = gcc -c -m64 -ffreestanding # 독립실행이 가능하도록 ffreestanding 옵션들어감
LD64 = x86_64-linux-gnu-ld -melf_x86_64 -T ../elf_x86_64.x -nostdlib -e Main -Ttext 0x200000

OBJCOPY64 = /usr/bin/x86_64-linux-gnu-objcopy -j .text -j .data -j .rodata -j .bss -S -O binary # OBJCOPY를 이용해 섹션을 빼내고 운영체제에 의존하는 부분을 제거

OBJECTDIRECTORY = Temp
SOURCEDIRECTORY = Source


#####################################################################
# 빌드 항목 및 빌드 방법 설정 
#####################################################################

all: prepare Kernel64.bin

prepare:
	mkdir -p $(OBJECTDIRECTORY)

dep:
	@echo === Make Dependency File ===
	make -C $(OBJECTDIRECTORY) -f ../makefile InternalDependency
	@echo === Dependency Search Complete ===

ExecuteInternalBuild: dep
	make -C $(OBJECTDIRECTORY) -f ../makefile Kernel64.elf

Kernel64.bin: ExecuteInternalBuild
	$(OBJCOPY64) $(OBJECTDIRECTORY)/Kernel64.elf $@

clean:
	rm -f *.bin
	rm -f $(OBJECTDIRECTORY)/*.*

#####################################################################
# Make에 의해 다시 호출 되는 부분, Temp 디렉터리 기준으로 수행
#####################################################################

# 빌드할 어셈블리어 엔트리 포인트 소스 파일 정의, Temp 디렉터리를 기준으로 설정
ENTRYPOINTSOURCEFILE = ../$(SOURCEDIRECTORY)/EntryPoint.s
ENTRYPOINTOBJECTFILE = EntryPoint.o

# 빌드할 C 소스파일 정의, Temp 디렉터리를 기준으로 설정
CSOURCEFILES = $(wildcard ../$(SOURCEDIRECTORY)/*.c) # wildcard 함수: 디렉터리에 특정 패턴 추출
ASSEMBLYSOURCEFILES = $(wildcard ../$(SOURCEDIRECTORY)/*.asm)
COBJECTFILES = $(notdir $(patsubst %.c,%.o,$(CSOURCEFILES))) # subst: 문자열 치환, COBJECTFILES에 있는 문자열 중 Main.o를 공백으로 치환 # patsubst: 패턴치환 함수, CSOURCEFILES 변수에서 c의 확장자를 가지는 모든 문자열 확장자를 o로 교체함
ASSEMBLYOBJECTFILES = $(notdir $(patsubst %.asm,%.o,$(ASSEMBLYSOURCEFILES))) # notdir: 나열된 문자열 중에서 파일 경로를 제거한 파일명만을 출력하게 해줍니다.

# 어셈블리어 엔트리 포인트 빌드 # $@는 생성될 파일 :의 왼쪽
$(ENTRYPOINTOBJECTFILE): $(ENTRYPOINTSOURCEFILE)
	$(NASM64) -o $@ $<

# 모든 c파일을 o로 출력 정의
%.o: ../$(SOURCEDIRECTORY)/%.c  # $< Dependency의 첫번째 항목
	$(GCC64) -c $<

# .asm 파일을 .o 파일로 출력 정의
%.o: ../$(SOURCEDIRECTORY)/%.asm
	$(NASM64) -o $@ $<


# -MM 전처리기 옵션 시스템 헤더파일(stdio.h같은거) 제외하고 헤더파일 의존성 추출 
InternalDependency:
	$(GCC64) -MM $(CSOURCEFILES) > Dependency.dep

Kernel64.elf: $(ENTRYPOINTOBJECTFILE) $(COBJECTFILES) $(ASSEMBLYOBJECTFILES)
	$(LD64) -o $@ $^

# 현재 디렉토리의 파일 중, dependency 파일이 있으면 make에 포함
ifeq (Dependency.dep, $(wildcard Dependency.dep))
include Dependency.dep
endif