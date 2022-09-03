mov eax, 0x4000003B ; PG(Paging)=0, CD(CacheDisable)=1, NW(Not Write-through)=0, AM(Alignment Mask)=0, 
                    ; WP(Write Protect)=0, NE(Numeric Error 1: Processor Internel Interrupt, 0: Externel Interrupt)=1, ET(Extension Type)=1, TS(Task Switched, 1: available FPU opertaion, 0: FPU opertaion error)=1, 
                    ; EM(0: bulit-in FPU, 1: not bulit-in FPU)=0, MP(Monitor Coprocessor, wait or fwait operation TS filed ref)=1, PE(Protection Enable)=1
mov cr0, eax        ; CR0 Control register에 위에서 저장한 플래그를 설정하여 보호모드로 전환