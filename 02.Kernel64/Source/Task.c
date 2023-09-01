#include "Task.h"
#include "Descriptor.h"
#include "Utility.h"

void kSetUpTask( TCB* pstTCB, QWORD qwID, QWORD qwFlags, QWORD qwEntryPointAddress, void* pvStackAddress, QWORD qwStackSize )
{
    // 콘텍스트 초기화
    kMemSet( pstTCB->stContext.vqReigister, 0, sizeof( pstTCB->stContext.vqReigister ) );
    
    // 스택에 관련된 RSP, RBP 레지스터 설정
    pstTCB->stContext.vqReigister[ TASK_RSPOFFSET ] = ( QWORD ) pvStackAddress + qwStackSize; // 태스크가 생성 되었을 때 스택은 비어있으므로, 스택의 Top을 스택 영역의 끝으로 옮김
    pstTCB->stContext.vqReigister[ TASK_RBPOFFSET ] = ( QWORD ) pvStackAddress + qwStackSize;

    // 세그먼트 셀렉터 설정
    pstTCB->stContext.vqReigister[ TASK_CSOFFSET ] = GDT_KERNELCODESEGMENT; // 태스크가 커널모드에서 실행되므로 커널 코드 세그먼터 디스크립터를 설정
    pstTCB->stContext.vqReigister[ TASK_DSOFFSET ] = GDT_KERNELDATASEGMENT;
    pstTCB->stContext.vqReigister[ TASK_ESOFFSET ] = GDT_KERNELDATASEGMENT;
    pstTCB->stContext.vqReigister[ TASK_FSOFFSET ] = GDT_KERNELDATASEGMENT;
    pstTCB->stContext.vqReigister[ TASK_GSOFFSET ] = GDT_KERNELDATASEGMENT;
    pstTCB->stContext.vqReigister[ TASK_SSOFFSET ] = GDT_KERNELDATASEGMENT;

    // RIP 레지스터와 인터럽트 플래그 설정
    pstTCB->stContext.vqReigister[ TASK_RIPOFFSET ] = qwEntryPointAddress;

    // RFLAGS 레지스터의 IF 비트(비트 9)를 1로 설정하여 인터럽트 활성화
    pstTCB->stContext.vqReigister[ TASK_RFLAGSOFFSET ] |= 0x0200; // 인터럽트를 활성화 하기 위해 RFLAGS 레지스터의 IF 비트(비트 9)를 1로 설정
    
    // ID 및 스택, 그리고 플래그 저장
    pstTCB->qwID = qwID;
    pstTCB->pvStackAddress = pvStackAddress;
    pstTCB->qwStackSize = qwStackSize;
    pstTCB->qwFlags = qwFlags;
}