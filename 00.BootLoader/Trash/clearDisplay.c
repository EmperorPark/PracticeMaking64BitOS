int main(){
    int i = 0;
    int j = 0;
    char* pcVideoMemory = (char*) 0xB8000;
    char* pcMessage = "MINT64 OS Boot Loader Start~!!";
    char cTemp;

    while(1)
    {
        cTemp = pcMessage[i];
        if(cTemp == 0)
        {
            break;
        }
        
        pcVideoMemory[j] = cTemp;
        pcVideoMemory[i + 1] = 0x0A;
        i += 1;
        j += 2;
    }
    return 0;

}
