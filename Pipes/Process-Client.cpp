#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
  using namespace std;

  DWORD FilterFunc(int EC)
  {
	  if (EC==5) cout<<"You might enter the arguments of command line"<<endl;
	  cout<<"Enter any key to continue"<<endl;
      _getch();
	  return EXCEPTION_EXECUTE_HANDLER;
  }


int main(int argc, char *argv[])
{
   HANDLE hWritePipe,hReadPipe;
   HANDLE hEnableReadC,hEnableReadS,hEnableWriteC,hEnableWriteS; 
   
   hEnableWriteS = CreateEvent(NULL,FALSE,FALSE,NULL);
   hEnableReadS = CreateEvent(NULL,FALSE,FALSE,NULL);
   hEnableReadC = OpenEvent(EVENT_ALL_ACCESS, FALSE, NULL); 
   hEnableWriteC = OpenEvent(EVENT_ALL_ACCESS, FALSE, NULL); 

   __try 
     {  
	  __try
	  {
	   if((!argv[1])||(!argv[2])) RaiseException (0x00000005,0,0,NULL);
		    
	   hWritePipe = (HANDLE)atoi(argv[1]);
       hReadPipe = (HANDLE)atoi(argv[2]);
      
       _cprintf("Press any key to start communication by pipe %d %d.\n",hWritePipe,hReadPipe);
       for(;;)
         {
            for (int i = 0; i < 10; i++)
               {
                  DWORD dwBytesWritten;
	              int nData;
                  DWORD dwBytesRead;
                  ResetEvent(hEnableWriteC);
				  SetEvent(hEnableReadS);
                  WaitForSingleObject(hEnableWriteC, INFINITE);
				 
                    if (!WriteFile(hWritePipe,&i,sizeof(i),&dwBytesWritten,NULL))
                       {
                         _cputs("Write to file failed.\n");
                         _cputs("Press any key to finish.\n");
                         _getch();
                         return GetLastError();
                       }
                    _cprintf("The number %d is written to the pipe.\n", i);
					
					ResetEvent(hEnableReadC);
					WaitForSingleObject(hEnableReadC, INFINITE); 
                    SetEvent(hEnableWriteS);

					if(!ReadFile(hReadPipe,&nData,sizeof(nData),&dwBytesRead,NULL))
                       { 
                         _cputs("Read from the pipe failed.\n"); 
                         _cputs("Press any key to finish.\n");
                         _getch();
                         return GetLastError();
                       }
                    _cprintf("The number %d is read from the pipe.\n", nData);    
					Sleep(500);
			   }
	     }
	  }
	  __finally
	   {
		   CloseHandle(hEnableWriteC);
		   CloseHandle(hEnableWriteS);
	       CloseHandle(hEnableReadC);
		   CloseHandle(hEnableReadS);
	   }
     }

    __except (FilterFunc(GetExceptionCode()))
      {
          return 0;
      } 
CloseHandle(hWritePipe);
CloseHandle(hReadPipe);
_getch();
return 0;
}


