//---------------------------------------------------------------------------

#ifndef GlobalFuncsH
#define GlobalFuncsH

#include <string>
#include <vcl.h>
#include <tlhelp32.h>
#include <list>
using namespace std;

void    Split(string Source, char Splitter, string* Dest1, string* Dest2);
void    GetProcessList(list<PROCESSENTRY32>* sl);

//---------------------------------------------------------------------------
#endif
