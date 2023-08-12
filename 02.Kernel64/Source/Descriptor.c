#include "Descriptor.h"
#include "Utility.h"


//====================================
// GDT 및 TSS
//====================================

// GDT 테이블을 초기화
void kInitalizeGDTTableAndTSS( void ) {
    GDTR* pstGDTR;
    GDTENTRY8* pstEntry;
    TSSSEGMENT* pstTSS;
    int i;

    // GDTR 설정
    pstGDTR = ( GDTR* ) GDTR_STARTADDRESS; // 1MB 어드레스에서 264KB를 페이지 테이블 영역으로 사용하고 있으므로 그 이후 영역을 GDTR, GDT테이블, TSS 세그먼트 영역으로 사용
    pstEntry = ( GDTENTRY8* ) ( GDTR_STARTADDRESS + sizeof( GDTR ) );
    pstGDTR->wLimit = GDT_TABLESIZE - 1;
    pstGDTR->qwBaseAddress = ( QWORD ) pstEntry;
    // TSS 영역 설정
    pstTSS = ( TSSSEGMENT* ) ( ( QWORD ) pstEntry + GDT_TABLESIZE );

    // NULL, 64비트 Code/Data, TSS를 위해 총 4개의 세그먼트를 생성한다.
    // kSetGDTEntry8: 8바이트 크기의 세그먼트 디스크립터/코드와 데이터를 설정하는 함수
    kSetGDTEntry8( &( pstEntry[ 0 ] ), 0, 0, 0, 0, 0 );
    kSetGDTEntry8( &( pstEntry[ 1 ] ), 0, 0xFFFFF, GDT_FLAGS_UPPER_CODE, GDT_FLAGS_LOWER_KERNELCODE, GDT_TYPE_CODE );
    kSetGDTEntry8( &( pstEntry[ 2 ] ), 0, 0xFFFFF, GDT_FLAGS_UPPER_DATA, GDT_FLAGS_LOWER_KERNELDATA, GDT_TYPE_DATA );
    // kSetGDTEntry16: 16바이트 크기의 태스크 세그먼트 디스크립터(TSS)를 설정하는 함수
    // TSS 세그먼트 영역은 GDT 디스크립터 테이블 다음에 위치하므로, GDTR 자료구조의 시작 어드레스인 0x142000에서 8바이트의 GDT 엔트리 3개와 16바이트 GDT 엔트리 1개를 더한 어드레스를 사용
    kSetGDTEntry16( ( GDTENTRY16* ) &( pstEntry[ 3 ] ), (QWORD) pstTSS, sizeof( TSSSEGMENT ) - 1, GDT_FLAGS_UPPER_TSS, GDT_FLAGS_LOWER_TSS, GDT_TYPE_TSS );

    // TSS 초기화 GDT 이하 영역을 사용함
    kInitializeTSSSegment( pstTSS );
}

// 8바이트 크기의 GDT 엔트리에 값을 설정
//      코드와 데이터 세그먼트의 디스크립터를 설정 하는데 사용
void kSetGDTEntry8( GDTENTRY8* pstEntry, DWORD dwBaseAddress, DWORD dwLimit, BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType ) {
    pstEntry->wLowerLimit = dwLimit & 0xFFFF;
    pstEntry->wLowerBaseAddress = dwBaseAddress & 0xFFFF;
    pstEntry->bUpperBaseAddress1 = ( dwBaseAddress >> 16 ) & 0xFF;
    pstEntry->bTypeAndLowerFlag = bLowerFlags | bType;
    pstEntry->bUpperLimitAndUpperFlag = ( (dwLimit >> 16 ) & 0xFF) | bUpperFlags;
    pstEntry->bUpperBaseAddress2 = ( dwBaseAddress >> 24 ) & 0xFF;
}

// 16바이트 크기의 GDT엔트리에 값을 설정
//      TSS 세그먼트 디스크립터를 설정하는데 사용
void kSetGDTEntry16( GDTENTRY16* pstEntry, QWORD qwBaseAddress, DWORD dwLimit, BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType ) {
    pstEntry->wLowerLimit = dwLimit & 0xFFFF;
    pstEntry->wLowerBaseAddress = qwBaseAddress & 0xFFFF;
    pstEntry->bMiddleBaseAddress1 = ( qwBaseAddress >> 16 ) & 0xFF;
    pstEntry->bTypeAndLowerFlag = bLowerFlags | bType;
    pstEntry->bUpperLimitAndUpperFlag = ( ( dwLimit >> 16 ) & 0xFF ) | bLowerFlags;
    pstEntry->bMiddleBaseAddress2 = ( qwBaseAddress >> 24 ) & 0xFF;
    pstEntry->dwUpperBaseAddress = qwBaseAddress >> 32;
    pstEntry->dwReserved = 0;
}


// TSS 세그먼트의 정보를 초기화
void kInitializeTSSSegment( TSSSEGMENT* pstTSS ) {
    kMemSet( pstTSS, 0, sizeof( TSSSEGMENT ) );
    pstTSS->qwIST[ 0 ] = 0x800000;
    pstTSS->wIOMapBaseAddress = 0xFFFF;
}


//====================================
// IDT
//====================================
// IDT 테이블을 초기화
// IDT 테이블을 초기화
void kInitializeIDTTables( void ) {
    IDTR* pstIDTR;
    IDTENTRY* pstEntry;
    int i;

    // IDTR의 시작 어드레스
    pstIDTR = ( IDTR* ) IDTR_STARTADDRESS;
    // IDT 테이블 정보 생성
    pstEntry = ( IDTENTRY* ) ( IDTR_STARTADDRESS + sizeof( IDTR ) );
    pstIDTR->qwBaseAddress = ( QWORD ) pstEntry;
    pstIDTR->wLimit = IDT_TABLESIZE - 1;

    // 0~99까지 벡터를 모두 DummyHandler로 연결
    for( i = 0 ; i < IDT_MAXENTRYCOUNT ; i++ ) {
        kSetIDTEntry( &( pstEntry[ i ] ), kDummyHandler, 0x08, IDT_FLAGS_IST1, IDT_FLAGS_KERNAL, IDT_TYPE_INTERRUPT );
    }
}

// IDT 게이트 디스크립터에 값을 설정
void kSetIDTEntry( IDTENTRY* pstEntry, void* pvHandler, WORD wSelector, BYTE bIST, BYTE bFlags, BYTE bType ) {
    pstEntry->wLowerBaseAddress = ( QWORD ) pvHandler & 0xFFFF;
    pstEntry->wSegmentSelector = wSelector;
    pstEntry->bIST = bIST & 0x3;
    pstEntry->bTypeAndFlags = bType | bFlags;
    pstEntry->wMiddleBaseAddress = ( ( QWORD ) pvHandler >> 16 ) & 0xFFFF;
    pstEntry->dwUpperBaseAddress = ( QWORD ) pvHandler >> 32;
    pstEntry->dwReserved = 0;
}

// 임시 예외 또는 인터럽트 핸들러
void kDummyHandler( void ) {
    kPrintString( 0, 0, "=====================================================" );
    kPrintString( 0, 1, "        Dummy Interrupt Handler Called~!!!           " );
    kPrintString( 0, 2, "                  Interrupt Occur~!!!                " );
    kPrintString( 0, 3, "=====================================================" );

    while (1) ;
}
