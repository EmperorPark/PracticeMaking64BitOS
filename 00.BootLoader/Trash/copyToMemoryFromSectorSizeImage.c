#include <stdio.h>

void HandleDiskError();
int BIOSReadOneSector(int iSectorNumber, int iHeadNumber, int iTrackNumber, char* pcTargetAddress);

int main(int argc, char* argv[])
{
    int iTotalSectorCount = 1024;
    int iSectorNumber = 2;
    int iHeadNumber = 0;
    int iTrackNumber = 0;
    // 실제 이미지를 복사할 어드레스(물리 주소)
    char* pcTargetAddress = (char*) 0x10000;

    while (1)
    {
        //전체 섹터 수를 하나씩 감소시키면서 0이 될 때까지 섹터를 복사
        if(iTotalSectorCount == 0) break;
        iTotalSectorCount = iTotalSectorCount - 1;

        // 1섹터를 읽어들여서 메모리 어드레스에 복사
        if(BIOSReadOneSector(iSectorNumber, iHeadNumber, iTrackNumber, pcTargetAddress) != 0) HandleDiskError(); // 바이오스 섹터읽기 기능 등 추상화영역에 직접접근 할수 없어 선언한 임의 함수들임

        // 1 섹터는 512(0x200) Bytes이므로, 복사한 섹터 수 만큼 어드레스 증가
        pcTargetAddress = pcTargetAddress + 0x200;

        //섹터 -> 헤드 -> 트랙 순으로 번호 증가
        iSectorNumber = iSectorNumber +1;
        if(iSectorNumber < 19) continue;

        iHeadNumber = iHeadNumber ^ 0x01; /// head번호는 0과 1이 반복되므로 이를 편리하게 처리하기 위한 xor연산 iHeadNumber = (iHeadNumber = 0x00) ? 0x01: 0x00과 같은 의미
        iSectorNumber = 1;

        if(iHeadNumber != 0) continue;

        iTrackNumber = iTrackNumber + 1;
    }

    return 0;
    
}

void HandleDiskError(){
    printf("DISK Error~!!");
    while(1) ;
}

int BIOSReadOneSector(int iSectorNumber, int iHeadNumber, int iTrackNumber, char* pcTargetAddress)
{
    return 0;
}