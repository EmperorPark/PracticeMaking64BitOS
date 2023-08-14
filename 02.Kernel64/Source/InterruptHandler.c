#include "InterruptHandler.h"
#include "PIC.h"

// 공통으로 사용하는 예외 핸들러
void kCommonExceptionHandler( int iVectorNumber, QWORD qwErrorCode )
{
    char vcBuffer[ 3 ] = { 0, };
    
    // 인터럽트 벡터를 화면 오른쪽 위 2자리 정수로 출력
    vcBuffer[ 0 ] = '0' + iVectorNumber / 10;
    vcBuffer[ 1 ] = '0' + iVectorNumber % 10;
    
    kPrintString( 0, 0, "====================================================" );
    kPrintString( 0, 1, "                 Exception Occur~!!!!               " );
    kPrintString( 0, 2, "                    Vector:                         " );
    kPrintString( 27, 2, vcBuffer );
    kPrintString( 0, 3, "====================================================" );
    
    while ( 1 ) ;
}

// 공통으로 사용하는 인터럽트 핸들러
void kCommonInterruptHandler( int iVectorNumber )
{
    char vcBuffer[] = "[INT:  , ]"; // 화면에 IRQ 번호를 출력하기 위한 버퍼
    static int g_iCommonInterruptCount = 0;
    
    //=========================================================================
    // 인터럽트가 발생했음을 알리려고 메시지를 출력하는 부분
    // 인터럽트 벡터를 화면 오른쪽 위 2자리 정수로 출력
    vcBuffer[ 5 ] = '0' + iVectorNumber / 10; // 인터럽트 벡터의 10의 자리를 문자로 변환
    vcBuffer[ 6 ] = '0' + iVectorNumber % 10; // 인터럽트 벡터의 1의 자리를 문자로 변환
    // 발생한 횟수 출력
    vcBuffer[ 8 ] = '0' + g_iCommonInterruptCount;
    g_iCommonInterruptCount = ( g_iCommonInterruptCount + 1 ) % 10;
    kPrintString( 70, 0, vcBuffer );
    //=========================================================================

    // EOI 전송
    kSendEOIToPIC( iVectorNumber - PIC_IRQSTARTVECTOR ); // 인터럽트의 벡터는 32번부터 시작하므로, 32를 빼서 IRQ 번호를 구한 후 EOI 전송
}

// 키보드 인터럽트의 핸들러
void kKeyboardHandler( int iVectorNumber )
{
    char vcBuffer[] = "[INT:  , ]";
    static int g_iKeyboardInterruptCount = 0;

    //=========================================================================
    // 인터럽트가 발생했음을 알리려고 메시지를 출력하는 부분
    // 인터럽트 벡터를 화면 왼쪽 위에 2자리 정수로 출력
    vcBuffer[ 5 ] = '0' + iVectorNumber / 10; // 인터럽트 벡터의 10의 자리를 문자로 변환
    vcBuffer[ 6 ] = '0' + iVectorNumber % 10; // 인터럽트 벡터의 1의 자리를 문자로 변환
    // 발생한 횟수 출력
    vcBuffer[ 8 ] = '0' + g_iKeyboardInterruptCount;
    g_iKeyboardInterruptCount = ( g_iKeyboardInterruptCount + 1 ) % 10;
    kPrintString( 0, 0, vcBuffer );
    //=========================================================================

    // EOI 전송
    kSendEOIToPIC( iVectorNumber - PIC_IRQSTARTVECTOR );
}