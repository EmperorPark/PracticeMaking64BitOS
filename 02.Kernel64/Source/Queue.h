#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "Types.h"

// 구조체
// 1바이트로 정렬
#pragma pack( push, 1 )

// 큐에 대한 구조체
typedef struct kQueueManagerStruct
{
    // 큐를 구성하는 데이터 하나의 크기와 최대 개수
    int iDataSize; // 큐를 구성하는 하나의 크기
    int iMaxDataCount; // 큐에 삽입할 수 있는 최대 개수

    // 큐 버퍼의 포인터와 삽입/제거 인덱스
    void* pvQueueArray; // 큐에 데이터를 저장하는 용도로 사용할 버퍼
    int iPutIndex; // 데이터를 삽입할 때 사용하는 인덱스
    int iGetIndex; // 데이터를 제거할 때 사용하는 인덱스

    // 큐에 수행된 마지막 명령이 삽입인지 저장
    BOOL bLastOperationPut; // 큐의 버퍼가 비었는지 혹은 가득 찼는지 확인하는 용도로 사용
} QUEUE;

#pragma pack( pop )

void kInitializeQueue( QUEUE* pstQueue, void* pvQueueBuffer, int iMaxDataCount, int iDataSize );
BOOL kIsQueueFull( const QUEUE* pstQueue ); // 큐가 가득 찼는지 여부를 반환
BOOL kIsQueueEmpty( const QUEUE* pstQueue ); // 큐가 비었는지 여부를 반환
BOOL kPutQueue( QUEUE* pstQueue, const void* pvData ); // 큐에 데이터를 삽입
BOOL kGetQueue( QUEUE* pstQueue, void* pvData); // 큐에 데이터를 제거

#endif /*__QUEUE_H__*/