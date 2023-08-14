; GDTR 자료구조 정의
GDTR:
    dw GDTEND - GDT -1          ; 아래에 위치하는 GDT 테이블의 전체 크기
    dd ( GDT - $$ + 0x10000 )   ; 아래에 위치하는 GDT 테이블의 시작 어드레스

; GDT 테이블 정의
GDT:
    ; 널 디스크립터. 반드시 0으로 초기화 해야야 함
    NULLDescriptor:
        dw 0x0000
        dw 0x0000
        db 0x00
        db 0x00
        db 0x00
        db 0x00


        ;;; 생략 ;;;
    DATADESCRIPTOR:
        dw 0xFFFF   ; Limit [15:0]
        dw 0x0000   ; Base [15:0]
        db 0x00     ; Base [23:16]
        db 0x92     ; P(Present)=0b1, DPL(Descriptor Privilege Level)=0b00, S(Discripter Type 1:Segment Des, 0:System Des)=0b1(Data Segment), Type=0x02(Read/Write)
        db 0xCF     ; G(Granularity 1:*4KB, 0:*1)=0b1, D/B(Default Operation Size 1:32bit, 0: 16bit)=0b1, L(use in IA-32e, 1: IA-32e, 0: 32bit legacy compatibility)=0b0, AVL=0b0, Limit[19:16]
        db 0x00     ; Base [31:24]


lgdt [ GDTR ]   ; GDTR 자료구조를 프로세서에 설정하여 GDT 테이블을 로드