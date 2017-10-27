#include <windows.h>
#include <iostream>
#include "conio.h"
using namespace std;

int main()
{
   char NameS[80];
   char pipeName[80];
   HANDLE hNamedPipe;
   
   cout << "Enter a name of the server: ";
   cin >> NameS; 
   wsprintf (pipeName,"\\\\%s\\pipe\\demo_pipe",NameS);
   
   hNamedPipe = CreateFile(pipeName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
   
   if (hNamedPipe == INVALID_HANDLE_VALUE)
    {
       cerr << "Connection with the named pipe failed." << endl
       << "The last error code: " << GetLastError() << endl;
       cout << "Press any key to exit.";
       cin.get();
       return 0;
    }

   char buf[80] ;
   char nData[80];
   
   cout<<"Connected to demo_pipe " << hNamedPipe << endl;
   
   for (;;)
    {
		cout<<"Type the message for server..."<<endl;
	  char c;
      for(int j=0;j<80;j++)
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
	  cout<<endl<<"The server has sent the following message: "<<endl<<nData<<endl;
    }
   CloseHandle(hNamedPipe);
   cin.get();
   return 0;
}