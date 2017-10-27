#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;
int main()
{
   char lpszComLine[150];    
   STARTUPINFO si;
   PROCESS_INFORMATION pi;
   
   HANDLE hWritePipe,hReadPipe,hInheritReadPipe,hInheritWritePipe;
   HANDLE hEnableReadC=CreateEvent(NULL,FALSE,FALSE,NULL);
   HANDLE hEnableReadS=OpenEvent(EVENT_ALL_ACCESS, FALSE, NULL);
   HANDLE hEnableWriteC=CreateEvent(NULL,FALSE,FALSE,NULL);
   HANDLE hEnableWriteS=OpenEvent(EVENT_ALL_ACCESS, FALSE, NULL);

   if(!CreatePipe(&hReadPipe, &hWritePipe, NULL, 0)) 
    {
      _cputs("Create pipe failed.\n");
      _cputs("Press any key to finish.\n");
      _getch();
      return GetLastError();
    }
   if(!DuplicateHandle(GetCurrentProcess(), hWritePipe, GetCurrentProcess(), &hInheritWritePipe, 0, TRUE,DUPLICATE_SAME_ACCESS )) 
    {
      _cputs("Press any key to finish.\n");
      _getch();
      return GetLastError();
    }
   if(!DuplicateHandle(GetCurrentProcess(), hReadPipe, GetCurrentProcess(), &hInheritReadPipe, 0, TRUE, DUPLICATE_SAME_ACCESS )) 
    {
      _cputs("Press any key to finish.\n");
      _getch();
      return GetLastError();
    }
   
   ZeroMemory(&si, sizeof(STARTUPINFO));
   si.cb = sizeof(STARTUPINFO);

   wsprintf(lpszComLine,"C:\\Users\\MikhnenkoKV\\Documents\\Visual Studio 2008\\Projects\\32321\\Debug\\32321.exe %d %d",(int)hInheritWritePipe,(int)hInheritReadPipe);
   _cprintf("Start client process [%s].\n",lpszComLine);

   if (!CreateProcess(NULL, lpszComLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi ))
    {
      _cputs("Create process failed.\n");
      _cputs("Press any key to finish.\n");
      _getch();
      return GetLastError();
    }

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  CloseHandle(hInheritWritePipe);
  CloseHandle(hInheritReadPipe);
  int j=0;
   for(;;)
    {
      for (int i = 0; i < 10; i++)
        {
          int nData;
          DWORD dwBytesRead;
          DWORD dwBytesWritten;

		  ResetEvent(hEnableReadS);
		  WaitForSingleObject(hEnableReadS, INFINITE);
		  SetEvent(hEnableWriteC);		  
		  
            if (!ReadFile(hReadPipe,&nData,sizeof(nData),&dwBytesRead,NULL))
             {
               _cputs("Read from the pipe failed.\n"); 
               _cputs("Press any key to finish.\n");
               _getch();
               return GetLastError();
             }
          _cprintf("The number %d is read from the pipe.\n", nData);
		  
		  ResetEvent(hEnableWriteS);
          SetEvent(hEnableReadC);
		  WaitForSingleObject(hEnableWriteS, INFINITE);
          
		  j=i+11; 
		    if (!WriteFile(hWritePipe,&j,sizeof(j),&dwBytesWritten,NULL))
              {
                _cputs("Write to file failed.\n");
                _cputs("Press any key to finish.\n");
                _getch(); 
                return GetLastError();
              }
            _cprintf("The number %d is written to the pipe.\n", j);
	    }
	}

 CloseHandle(hEnableReadC); 
 CloseHandle(hEnableReadS);
 CloseHandle(hEnableWriteC); 
 CloseHandle(hEnableWriteS);
 CloseHandle(hReadPipe);
 CloseHandle(hWritePipe);
 

 _cputs("Press any key to exit.\n");
 _getch();
 return 0;
}
