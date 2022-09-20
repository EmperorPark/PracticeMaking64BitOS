#ifndef __PAGE_H__
#define __PAGE_H__

#include "Types.h"


// 매크로
#define PAGE_FLAGS_P    0x00000001 // Present(유효)
#define PAGE_FLAGS_RW   0x00000002 // Read/Write(0: Read, 1:Read/Write)
#define PAGE_FLAGS_US   0x00000004 // User/Supervisor(Flag Set -> User Level)
#define PAGE_FLAGS_PWT  0x00000008 // Page Level Write-through(0: Write-Back, 1: Write-through)
#define PAGE_FLAGS_PCD  0x00000010 // Page Level Cache Disable(0: Cache Enable, 1: Cache Disable)
#define PAGE_FLAGS_A    0x00000020 // Accessed(watched and setted by Processor)
#define PAGE_FLAGS_D    0x00000040 // Dirty(watched and setted by Processor)
#define PAGE_FLAGS_PS   0x00000080 // Page Size(0: 4KB, 1:CR4 PAE bit 0 -> 4MB / CR4 PAE bit 1 -> 2MB)
#define PAGE_FLAGS_G    0x00000100 // Global(Don't replace this page from the table cache(TLB: Translation Lookaside Buffer) even if the value changes)
#define PAGE_FLAGS_PAT  0x00001000 // Page Attribute Table Index
// 상위 32비트 용 속성 필드
#define PAGE_FLAGS_EXB  0x80000000 // Excute Disable 비트
// 기타
#define PAGE_FLAGS_DEFAULT  ( PAGE_FLAGS_P | PAGE_FLAGS_RW ) // 실제 속성을 나타내는 필드는 아니지만 편의를 위해 정의
#define PAGE_TABLESIZE  0x1000
#define PAGE_MAXENTRYCOUNT  512
#define PAGE_DEFAULTSIZE    0x200000

// 구조체
#pragma pack( push, 1 )

/// @brief 페이지 엔트리에 대한 자료 구조
typedef struct pageTableEntryStruct
{
    // PML4T와 PDPTE의 경우
    // 1bit P,RW, US, PWT, PCD, A, D, PS, G, 3bits Avail, 1bit PAT, 8bits Reserved, 20 bits Base Address
    // PDE의 경우
    // 1bit P, RW, US, PWT, PCD, A, D, 1, G, 3bits Avail, 1bit PAT, 8bits Avail, 11bits Base Address
    DWORD dwAttributeAndLowerBaseAddress; // 8Bytes 크기의 페이지 엔트리 중에 하위 4Bytes를 의미, 기준 주소의 하위 필드와 G 비트, PAT 비트, D비트, A 비트, PCB 비트, U/S 비트, R/W 비트 P 비트 등을 포함
    // 8bits Upper BaseAddress, 12bits Reserved, 11bit Avail, 1bit EXB
    DWORD dwUpperBaseAddressAndEXB;       // 8Bytes 크기의 페이지 엔트리 중에 상위 4Bytes를 의미, 기준주소의 상위 필드와 EXB 비트 등을 포함
} PML4TENTRY, PDPTENTRY, PDENTRY, PTENTRY;

#pragma pack( pop )

// 함수
// 페이지 엔트리에 데이터를 설정하는 함수
void kSetPageEntryData( PTENTRY* pstEntry, DWORD dwUpperBaseAddress, DWORD dwLowerBaseAddress, // dwUpperBaseAddress, dwLowerBaseAddress: 32bit변수로는 64bit Address를 표현 할 수 없으므로 상위 32bit와 하위 32bit 어드레스를 나타내는 변수를 사용
DWORD dwLowerFlags, DWORD dwUpperFlags );

void kInitalizePageTables( void );

#endif /*__PAGE_H__*/
