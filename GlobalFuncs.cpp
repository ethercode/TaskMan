//---------------------------------------------------------------------------

#pragma hdrstop
#include <vcl.h>
#include "untMain.h"
#include "GlobalFuncs.h"
#include <tlhelp32.h>
#include <list>

using namespace std;
//---------------------------------------------------------------------------

/*
    This function splits up a string around a certain character.
    e.g. Split("Hello, World!",',', &string1, &string2) will put
    "Hello" in string1 and " World!" in string2.
*/
void Split(string Source, char Splitter, string* Dest1, string* Dest2)
{
    const char* Source_cstr = Source.c_str();  //get the char* of the source string
    int         a;  //temp var
    int         b           = Source.size();  //get the size of the source string
    int         Found       = -1;  //default found state as not yet found
    for (a = 0; a < b; a++)  //go through the whole string
    {
        if (Source_cstr[a] == Splitter)  //if splitter character found
        {
            Found = a;  //set the found position
            break;  //break out of the for loop
        }
    }
    if (Found == -1)  //if not found
    {
        if (Dest1 != 0)
        {
            *Dest1 = Source;
        }  //copy source into Dest1
        if (Dest2 != 0)
        {
            *Dest2 = "";
        }  //and leave Dest2 blank
        return;  //exit
    }
    else  //if it was found
    {
        if (Dest1 != 0)
        {
            *Dest1 = Source.substr(0, Found);
        }  //put the first half in Dest1
        if (Dest2 != 0)
        {
            *Dest2 = Source.substr(Found + 1, b - Found);
        }  //and the last half in Dest2
        return;  //exit
    }
}

void GetProcessList(list<PROCESSENTRY32>* sl)
{
    HANDLE          hSnapshot;
    PROCESSENTRY32  pe32    =
    {
        0
    };

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == (HANDLE) - 1)
    {
        return;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32))
    {
        do
        {
            sl->push_back(pe32);
        }
        while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
}

#pragma package(smart_init)

