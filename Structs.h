#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <vcl.h>

struct SProcess
{
    int         PID;
    int         Threads;
    int         ParentPID;
    int         Priority;
    AnsiString  ExeFile;
    int         Memory;
};

#endif
