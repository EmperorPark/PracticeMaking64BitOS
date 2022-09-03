CODEDESCRIPTOR:
    dw 0xFFFF   ; Limit [15:0]
    dw 0x0000   ; Base [15:0]
    db 0x00     ; Base [23:16]
    db 0x9A     ; P(Present)=0b1, DPL(Descriptor Privilege Level)=0b00, S(Discripter Type 1:Segement Des, 0:System Des)=0b1(Code Segment), Type=0x0A(Execute/Read)
    db 0xCF     ; G(Granularity 1:*4KB, 0:*1)=0b1, D/B(Default Operation Size 1:32bit, 0: 16bit)=0b1, L(use in IA-32e, 1: IA-32e, 0: 32bit legacy compatibility)=0b0, AVL=0b0, Limit[19:16]
    db 0x00     ; Base [31:24]

DATADESCRIPTOR:
    dw 0xFFFF   ; Limit [15:0]
    dw 0x0000   ; Base [15:0]
    db 0x00     ; Base [23:16]
    db 0x92     ; P(Present)=0b1, DPL(Descriptor Privilege Level)=0b00, S(Discripter Type 1:Segement Des, 0:System Des)=0b1(Data Segment), Type=0x02(Read/Write)
    db 0xCF     ; G(Granularity 1:*4KB, 0:*1)=0b1, D/B(Default Operation Size 1:32bit, 0: 16bit)=0b1, L(use in IA-32e, 1: IA-32e, 0: 32bit legacy compatibility)=0b0, AVL=0b0, Limit[19:16]
    db 0x00     ; Base [31:24]