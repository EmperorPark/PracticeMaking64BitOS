#include "InterruptHandler.h"
#include "PIC.h"
#include "Keyboard.h"
#include "Console.h"
#include "Utility.h"
#include "Task.h"
#include "Descriptor.h"

// 공통으로 사용하는 예외 핸들러
void kCommonExceptionHandler( int iVectorNumber, QWORD qwErrorCode )
{
    char vcBuffer[ 3 ] = { 0, };
    
    // 인터럽트 벡터를 화면 오른쪽 위 2자리 정수로 출력
    vcBuffer[ 0 ] = '0' + iVectorNumber / 10;
    vcBuffer[ 1 ] = '0' + iVectorNumber % 10;
    
    kPrintStringXY( 0, 0, "====================================================" );
    kPrintStringXY( 0, 1, "                 Exception Occur~!!!!               " );
    kPrintStringXY( 0, 2, "                    Vector:                         " );
    kPrintStringXY( 27, 2, vcBuffer );
    kPrintStringXY( 0, 3, "====================================================" );
    
    while( 1 ) ;
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
    kPrintStringXY( 70, 0, vcBuffer );
    //=========================================================================

    // EOI 전송
    kSendEOIToPIC( iVectorNumber - PIC_IRQSTARTVECTOR ); // 인터럽트의 벡터는 32번부터 시작하므로, 32를 빼서 IRQ 번호를 구한 후 EOI 전송
}

// 키보드 인터럽트의 핸들러
void kKeyboardHandler( int iVectorNumber )
{
    char vcBuffer[] = "[INT:  , ]";
    static int g_iKeyboardInterruptCount = 0;
    BYTE bTemp;

    //========================================================================================================
    // 인터럽트가 발생했음을 알리고 메시지를 출력하는 부분
    // 인터럽트 벡터를 화면 왼쪽 위에 2자리 정수를 출력
    vcBuffer[ 5 ] = '0' + iVectorNumber / 10;
    vcBuffer[ 6 ] = '0' + iVectorNumber % 10;
    // 발생한 횟수 출력
    vcBuffer[ 8 ] = '0' + g_iKeyboardInterruptCount;
    g_iKeyboardInterruptCount = ( g_iKeyboardInterruptCount + 1 ) % 10;
    kPrintStringXY( 0, 0, vcBuffer );
    //========================================================================================================

    // 키보드 컨트롤러에서 데이터를 읽어서 ASCII로 변환하여 큐에 삽입
    if( kIsOutputBufferFull() == TRUE ) // 출력 버퍼(포트 0x60)에 데이터가 수신되었는지 확인하는 함수
    {
        bTemp = kGetKeyboardScanCode(); // 키보드 컨트롤러의 출력 버퍼(0x60)에서 스캔 코드를 읽는 함수
        kConvertScanCodeAndPutQueue( bTemp );
    }

    // EOI 전송
    kSendEOIToPIC( iVectorNumber - PIC_IRQSTARTVECTOR ); // PIC 컨트롤러의 인터럽트 벡터가 시작하는 번호, 32
}

/**
 *  타이머 인터럽트의 핸들러
 */
void kTimerHandler( int iVectorNumber )
{
    char vcBuffer[] = "[INT:  , ]";
    static int g_iTimerInterruptCount = 0;

    //=========================================================================
    // 인터럽트가 발생했음을 알리려고 메시지를 출력하는 부분
    // 인터럽트 벡터를 화면 오른쪽 위에 두 자리 정수로 출력
    vcBuffer[ 5 ] = '0' + iVectorNumber / 10;
    vcBuffer[ 6 ] = '0' + iVectorNumber % 10;
    // 발생한 횟수 출력
    vcBuffer[ 8 ] = '0' + g_iTimerInterruptCount;
    g_iTimerInterruptCount = ( g_iTimerInterruptCount + 1 ) % 10;
    kPrintStringXY( 70, 0, vcBuffer );
    //=========================================================================

    // EOI 전송
    kSendEOIToPIC( iVectorNumber - PIC_IRQSTARTVECTOR ); // PIT 컨트롤러에 EOI를 전송하여 인터럽트 처리가 완료되었음을 알림

    // 타이머 발생 횟수를 증가
    g_qwTickCount++;

    // 태스크가 사용한 프로세서의 시간을 줄임
    kDecreaseProcessorTime(); // 태스크가 사용한 프로세서 시간을 1 감소시키는 함수
    // 프로세서가 사용할 수 있는 시간을 다 썻다면 태스크 전환 수행
    if( kIsProcessorTimeExpired() == TRUE ) // 태스크가 사용할 수 있는 프로세서 시간이 다 되었는지 여부를 반환하는 함수
    {
        kScheduleInInterrupt(); // IST의 콘텍스트 영역을 덮어써서 태스크 전환을 수행하는 함수
    }
}