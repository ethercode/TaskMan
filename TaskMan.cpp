//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("untMain.cpp", frmMain);
//---------------------------------------------------------------------------
extern AnsiString   szCommandLine;

WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, int)
{
    try
    {
        Application->Initialize();
        Application->Title = "";
        szCommandLine = cmdLine;
        Application->CreateForm(__classid(TfrmMain), &frmMain);

        Application->Run();
    }
    catch (Exception& exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception& exception)
        {
            Application->ShowException(&exception);
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
