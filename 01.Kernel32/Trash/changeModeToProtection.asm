; 커널 코드 세그먼트를 0x00을 기준으로 하는 것으로 교체하고 EIP의 값을 0x00을 기준으로 재설정
; CS 세그먼트 셀렉터: EIP
jmp dword 0x08: (PROTECTIONMODE - $$ + 0x10000 )    ; 커널코드 세그먼트가 0x00을 기준으로 하는 반면 실제코드는 0x10000을 기준으로 실행되고 있으므로 
                                                    ;오프셋에서 0x10000을 더해서 세그먼트 교체 후에도 같은 선형 주소를 가리키게 함

[BITS 32]

PROTECTIONMODE:
    mov ax, 0x10        ; 보호모드 커널용 데이터 세그먼트 디스크립터를 AX레지스터에 저장
    mov ds, ax          ; ds 세그먼트 셀렉터에 설정
    mov es, ax          ; es 세그먼트 셀렉터에 설정
    mov fs, ax          ; fs 세그먼트 셀렉터에 설정
    mov gs, ax          ; gs 세그먼트 셀렉터에 설정

    ; 스택을 0x00000000 - 0x0000FFFF 영역에 64KB 크기로 생성
    mov ss, ax          ; SS 세그먼트 셀렉터에 설정
    mov esp, 0xFFFE     ; ESP 레지스터 어드레스를 0xFFFE로 설정
    mov ebp, 0xFFFE     ; ESP 레지스터 어드레스를 0xFFFE로 설정

; 메시지를 출력하는 함수
;   스택에 x 좌표, y 좌표, 문자열
PRINTMESSAGE:
    push ebp            ; 베이스 포인터 레지스터(bp)를 스택에 삽입
    mov ebp, esp        ; 베이스 포인터 레지스터(bp)에 스택 포인터 레지스터(SP)의 값을 설정
    push esi            ; 함수에서 임시로 사용하는 레지스터로 함수의 마지막 부분에서 스택에 삽입된 값을 꺼내 원래 값으로 복원
    push edi
    push eax
    push ecx
    push edx

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; X, Y의 좌표로 비디오 메모리의 어드레스를 계산함
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Y 좌표를 이용해서 먼저 라인 어드레스를 구함
    mov eax, dword [ebp + 12]   ; 파라미터 2(화면 좌표 Y)를 EAX 레지스터에 설정
    mov esi, 160                ; 한 라인의 바이트 수(2 * 80 컬럼)를 ESI 레지스터에 설정
    mul esi                     ; EAX 레지스터와 ESI레지스터를 곱하여 화면 Y 어드레스 계산
    mov edi, eax                ; 계산된 화면 Y 어드레스를 EDI 레지스터에 설정

    ; X 좌표를 이용해서 2를 곱한 후 최종 어드레스를 구함
    mov eax, dword [ebp + 8]    ; 파라미터 1(화면 좌표 X)를 EAX 레지스터에 설정
    mov esi, 2                  ; 한 문자를 나타내는 바이트 수(2)를 ESI 레지스터에 설정
    mul esi                     ; EAX 레지스터와 ESI레지스터를 곱하여 화면 X 어드레스를 계산
    add edi, eax                ; 화면 Y 어드레스와 X 어드레스를 더해서 실제 비디오 메모리 어드레스를 계산

    ; 출력할 문자열의 어드레스
    mov esi, dword [ ebp + 16 ] ; 파라미터 3(출력할 문자열 어드레스)

.MESSAGLOOP:                    ; 메시지를 출력하는 루프
    mov cl, byte [esi]          ; ESI 레지스터가 가리키는 문자열 위치에서 한 문자를 CL레지스터에 복사
                                ; CL 레지스터는 ECX레지스터의 하위 1byte(문자 하나는 1byte)
    cmp cl, 0                   ; 복사된 문자와 0을 비교
    je .MESSAGEEND              ; 복사한 문자의 값이 0이면 문자열이 종료되었음을 의미하므로 .MESSAGEEND로 이동하여 문자 출력 종료
    
    mov byte [edi + 0xB8000], cl    ; 0이 아니라면 비디오 메모리 어드레스 0xB8000 + EDI에 문자를 출력
                                    ; 보호모드에서는 32bit offset을 사용할 수 있으므로 리얼 모드처럼 별도의 세그먼트 셀렉터를 사용하지 않고 바로 접근 가능
    
    add esi, 1                  ; ESI 레지스터에 1을 더하여 다음 문자열로 이동
    add edi, 2                  ; EDI 레지스터에 2를 더하여 비디오 메모리의 다음 문자 위치로 이동(비디오 메모리는 바탕, 속성 쌍으로 구성되므로 문자만 출력하려면 2를 더해야 함)

    jmp .MESSAGLOOP             ; 메시지 출력 루프로 이동하여 다음 문자열을 출력

.MESSAGEEND:
    pop edx     ; 함수에서 사용이 끝난 EDX 레지스터 부터 EBP 레지스터 까지를 스택에 삽입된 값을 이용해서 복원
    pop ecx     ; 스택은 가장 마지막에 들어간 데이터가 가장 먼저 나오는 자료구조 이므로 삽입의 역순으로 제거해야 함
    pop eax
    pop edi
    pop esi
    pop ebp     ; 베이스 포인터 레지스터(BP) 복원
    ret         ; 함수를 호출한 다음 코드의 위치로 복귀




