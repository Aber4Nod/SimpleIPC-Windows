#include <windows.h>
#include <iostream>
#include "conio.h"
using namespace std;

int main()
{
   HANDLE hNamedPipe;
   char NameC;

   hNamedPipe = CreateNamedPipe("\\\\.\\pipe\\demo_pipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT | PIPE_READMODE_MESSAGE, 1, 0, 0, INFINITE, NULL);
   if (hNamedPipe == INVALID_HANDLE_VALUE)
    {
       cerr << "Create named pipe failed." << endl
       << "The last error code: " << GetLastError() << endl;
       cout << "Press any key to exit.";
       cin.get();
       return 0;
    }
   cout << "The server is waiting for connection with a client by demo_pipe" << hNamedPipe << endl;
   
   if(!ConnectNamedPipe(hNamedPipe, NULL))
     {
       cerr << "The connection failed." << endl
       << "The last error code: " << GetLastError() << endl;
       CloseHandle(hNamedPipe);
       cout << "Press any key to exit.";
       cin.get();
       return 0;
     }
   
   char buf[80];
   char nData[80];
   
   cout << "Client has connected" << endl;

   for (;;)
    {
       DWORD dwBytesRead;
       if (!ReadFile(hNamedPipe, nData, sizeof(nData), &dwBytesRead, NULL))
        {
          cerr << "Read file failed." << endl
          << "The last error code: " << GetLastError() << endl;
          CloseHandle(hNamedPipe);
          cout << "Press any key to exit.";
          cin.get();
          return 0;
        }
	   cout<<endl<<"The client has sent the following message: "<<endl<<nData<<endl;
       cout<<"Type the message for client..."<<endl;
	   char c;
	   for(int j=0;;j++)
	    {
		  c=getch();
		  if(c==13)
	     	{
              buf[j]='\0';		
		      break;
		    }
		  else 
		    {
			   cout<<c;
			   buf[j]=c;
		    }
	    }
	   cout<<endl;

       DWORD dwBytesWritten;
       if (!WriteFile(hNamedPipe, buf, sizeof(buf), &dwBytesWritten, NULL))
         {
            cerr << "Writing to the named pipe failed: " << endl
            << "The last error code: " << GetLastError() << endl;
            CloseHandle(hNamedPipe);
            cout << "Press any key to exit.";
            cin.get();
            return 0;
         }
   }

   CloseHandle(hNamedPipe);
   cin.get();
   return 0;
}