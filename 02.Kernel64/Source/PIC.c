#include "PIC.h"
#include "AssemblyUtility.h"

// 마스터 PIC 컨트롤러와 슬레이브 PIC 컨트롤러의 초기화 코드
void kInitializePIC( void )
{
    // 마스터 PIC 컨트롤러를 초기화
    // ICW1(포트 0x20), IC4 비트(비트 0) = 1
    kOutPortByte( PIC_MASTER_PORT1, 0x11 ); // LTIM비트=0(엣지트리거방식), SNGL비트=0(캐스케이드(마스터-슬레이브)), IC4비트=1(IWC 커맨드 사용)

    // ICW2(포트 0x21), 인터럽트 벡터(0x20)
    kOutPortByte( PIC_MASTER_PORT2, PIC_IRQSTARTVECTOR ); // 인터럽트 벡터를 0x20(32)부터 차례로 할당 PIC 컨트롤러 0번 ~ 7번 핀의 인터럽트는 프로세서에 벡터 0x20 ~ 0x27로 전달됨

    // ICW3(포트 0x21), 슬레이브 PIC 컨트롤러가 연결 위치(비트로 표현)
    // 마스터 PIC컨트롤러의 2번핀에 연결되어 있으므로, 0x04(비트 2)로 설정
    kOutPortByte( PIC_MASTER_PORT2, 0x04 ); // 슬레이브 PIC 컨트롤러가 마스터 PIC 컨트롤러의 2번 핀에 연결됨(0b00000100)

    // ICW4(포트 0x21), uPM 비트(비트 0) = 1
    kOutPortByte( PIC_MASTER_PORT2, 0x01 ); // uPM 비트 = 1(8086/88 모드)


    // 슬레이브 PIC 컨트롤러를 초기화
    // ICW1(포트 0xA0), IC4 비트(비트 0) = 1
    kOutPortByte( PIC_SLAVE_PORT1, 0x11 ); // LTIM비트=0(엣지트리거방식), SNGL비트=0(캐스케이드(마스터-슬레이브)), IC4비트=1(IWC 커맨드 사용)

    // ICW2(포트 0xA1), 인터럽트 벡터(0x20 + 8)
    kOutPortByte( PIC_SLAVE_PORT2, PIC_IRQSTARTVECTOR + 8 ); // 인터럽트 벡터를 0x28(40)부터 차례로 할당

    // ICW3(포트 0xA1), 마스터 PIC 컨트롤러에 연결된 위치(정수로 표현)
    // 마스터 PIC 컨트롤러의 2번 핀에 연결되어 있으므로 0x02로 설정
    kOutPortByte( PIC_SLAVE_PORT2, 0x02 ); // 슬레이브 PIC 컨트롤러가 마스터 PIC 컨트롤러의 2번 핀에 연결됨

    // ICW4(포트 0xA1), uPM 비트(비트 0) = 1
    kOutPortByte( PIC_SLAVE_PORT2, 0x01 ); // uPM 비트=1
}

// OCW1 커맨드를 이용하여 인터럽트를 선택하는 코드 
// 인터럽트를 마스크하여 해당 인터럽트가 발생하지 않게 처리
void kMaskPICInterrupt( WORD wIRQBitmask )
{
    // 마스터 PIC 컨트롤러에 IMR 설정
    // OCW1(포트 0x21), IRQ 0 ~ IRQ 7
    kOutPortByte( PIC_MASTER_PORT2, (BYTE) wIRQBitmask ); // 하위 8비트(IRQ 0 ~ IRQ 7)는 마스터 PIC 컨트롤러에 연결되어 있으므로 OCW1 커맨드에 하위 8비트만 전송

    // 슬레이브 PIC 컨트롤러에 IMR 설정
    // OCW1(포트 0xA1), IRQ 8 ~ IRQ 15
    kOutPortByte( PIC_SLAVE_PORT2, (BYTE) ( wIRQBitmask >> 8 ) ); // 상위 8비트(IRQ 8 ~ IRQ 15)는 슬레이브 PIC 컨트롤러에 연결되어 있으므로 OCW1 커맨드에 상위 8비트만 전송
}

// OCW2 커맨드를 이용하여 EOI를 전송하는 코드
// 인터럽트 처리가 완료 되었음을 전송(EOI)
//      마스터 PIC 컨트롤러의 경우, 마스터 PIC 컨트롤러에만 EOI 전송
//      슬레이브 PIC 컨트롤러의 경우, 마스터 및 슬레이브 PIC 컨트롤러에 모두 전송
void kSendEOIToPIC( int iIRQNumber )
{
    // 마스터 PIC 컨트롤러에 EOI 전송
    // OCW2(포트 0x20), EOI 비트(비트 5) = 1
    kOutPortByte( PIC_MASTER_PORT1, 0x20 ); // OCW 2 커맨드의 EOI 비트만 1로 설정하여 대상을 지정하지 않는 EOI 커맨드(Non-specific EOI Command)

    // 슬레이브 PIC 컨트롤러의 인터럽트인 경우 슬레이브 PIC 컨트롤러에도 EOI 전송
    if( iIRQNumber >= 8 )
    {
        // OCW2(포트 0xA0), EOI 비트(비트 5) = 1
        kOutPortByte( PIC_SLAVE_PORT1, 0x20 ); // OCW 2 커맨드의 EOI 비트만 1로 설정하여 대상을 지정하지 않는 EOI 커맨드(Non-specific EOI Command)
    }
}
