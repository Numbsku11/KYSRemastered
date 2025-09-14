#include <iostream>
#include <windows.h>
#include <tchar.h>


void startupConfigs(int argc, _TCHAR* argv[])
{
    SetConsoleTitle(T("KYS SILLY CAT"));    
}

int main()
{
    startupConfigs();
    
     while(1 == 1)
    {
        std::cout << "KYS ";
        Sleep (25);
    }
    return 0;
}
