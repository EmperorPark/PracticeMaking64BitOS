#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "AssemblyUtility.h"

// 함수 선언
// 문자열 출력함수
void kPrintString( int iX, int iY, const char* pcString );

// 아래 함수는 C언어 커널의 시작 부분임
//      반드시 다른 함수들 보다 가장 앞쪽에 존재해야 함
void Main(void) {

    char vcTemp[ 2 ] = { 0, };
    BYTE bFlags;
    BYTE bTemp;
    int i = 0;

    kPrintString( 0, 10, "Switch To IA-32e Mode Success~!!" );
    kPrintString( 0, 11, "IA-32e C Language Kernel Start..............[Pass]" );
    kPrintString( 0, 12, "GDT Initialize And Switch For IA-32e Mode...[    ]");
    kInitalizeGDTTableAndTSS();
    kLoadGDTR( GDTR_STARTADDRESS );
    kPrintString( 45, 12, "Pass" );

    kPrintString( 0, 13, "TSS Segment Load............................[    ]");
    kLoadTR( GDT_TSSSEGMENT );
    kPrintString( 45, 13, "Pass" );

    kPrintString( 0, 14, "IDT Initialize..............................[    ]");
    kInitializeIDTTables();
    kLoadIDTR( IDTR_STARTADDRESS );
    kPrintString( 45, 14, "Pass" );

    kPrintString( 0, 15, "Keyboard Actiavate..........................[    ]" );

    // 키보드를 활성화
    if( kActivateKeyboard() == TRUE ) {
        kPrintString( 45, 15, "Pass" );
        kChangeKeyboardLED( FALSE, FALSE, FALSE );
    } else {
        kPrintString( 45, 15, "Fail" );
        while( 1 ) ;
    }

    while ( 1 ) {
        // 출력 버퍼(포트 0x60)가 차 있으면 스캔 코드를 읽을 수 있음
        if( kIsOutputBufferFull() == TRUE ) { // 출력 버퍼(0x60)의 상태를 확인하여 데이터가 차 있을 경우 TRUE, 비어 있을 경우 FALSE를 반환하는 함숮
            bTemp = kGetKeyboardScanCode(); // 출력 버퍼 포트(0x60)에서 스캔 코드 값을 읽어 반환하는 함수
            // 스캔 코드를 ASCII 코드로 변환하는 함수를 호출 하여 ASCII 코드와 눌림 또는 떨어짐 정보를 반환
            if( kConvertScanCodeToASCIICode( bTemp, &(vcTemp[ 0 ]), &bFlags ) == TRUE ) {
                // 키가 눌러졌으면 키의 ASCII 코드 값을 화면에 출력
                if( bFlags & KEY_FLAGS_DOWN ) {
                    kPrintString( i++, 16, vcTemp );
                    // 0이 입력되면 변수를 0으로 나누어 Divide Error 예외(벡터 0번)를 발생시킴
                    if( vcTemp[ 0 ] == '0' ) {
                        // 아래 코드를 수행하면 Divide Error 예외가 발생하여 커널의 임시 핸들러가 수행됨
                        bTemp = bTemp / 0;
                    }
                }
            }
        }
    } 
}

// 문자열을 X, Y 위치에 출력
void kPrintString( int iX, int iY, const char* pcString ) {
    CHARACTER* pstScreen = ( CHARACTER* ) 0xB8000;
    int i;

    // X, Y 좌표를 이용해서 문자열을 출력할 어드레스를 계산
    pstScreen += ( iY * 80 ) + iX;

    // NULL이 나올 때까지 문자열 출력
    for(i = 0; pcString[i] != 0; i++) {
        pstScreen[i].bCharactor = pcString[i];
    }
}
