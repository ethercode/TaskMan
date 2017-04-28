//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "untMain.h"
#include <tlhelp32.h>
#include <list>
#include <stdio.h>
#include "Structs.h"
#include "GlobalFuncs.h"

using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "trayicon"
#pragma resource "*.dfm"
TfrmMain*   frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner):
    TForm(Owner)
{
}
//---------------------------------------------------------------------------

AnsiString  szVersion               = "TaskMan 1.3   ";

AnsiString  szCommandLine;


bool        fQueueSelectAllProcs    = false;
bool        fQueueSelectAllAllowed  = false;
bool        fQueueAddAllProcs       = false;
bool        fMayClose               = false;
bool        fQueueEnableLockdown    = false;
bool        fQueueExit              = false;

HANDLE      hProcess;
HANDLE      hToken;

BOOL SetPrivilege(HANDLE hToken, // token handle
    LPCTSTR Privilege, // Privilege to enable/disable
    BOOL bEnablePrivilege   // TRUE to enable.  FALSE to disable
)
{
    TOKEN_PRIVILEGES    tp;
    LUID                luid;
    TOKEN_PRIVILEGES    tpPrevious;
    DWORD               cbPrevious  = sizeof(TOKEN_PRIVILEGES);

    if (!LookupPrivilegeValue(NULL, Privilege, &luid))
    {
        return FALSE;
    }

    //
    // first pass.  get current privilege setting
    //
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = 0;

    AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &tpPrevious, &cbPrevious);

    if (GetLastError() != ERROR_SUCCESS)
    {
        return FALSE;
    }

    //
    // second pass.  set privilege based on previous setting

    //
    tpPrevious.PrivilegeCount = 1;
    tpPrevious.Privileges[0].Luid = luid;

    if (bEnablePrivilege)
    {
        tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
    }
    else
    {
        tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED & tpPrevious.Privileges[0].Attributes);
    }

    AdjustTokenPrivileges(hToken, FALSE, &tpPrevious, cbPrevious, NULL, NULL);

    if (GetLastError() != ERROR_SUCCESS)
    {
        return FALSE;
    }

    return TRUE;
}

void About()
{
    Application->MessageBox("Welcome to Antivoid's Task Manager.\n"
                            "\n"
                            "This application will give you complete control of any process.\n"
                            "If you don't want a process running, you can kill it, without regard or question.\n"
                            "WARNING: This program can terminate ANYTHING, even winlogon. It does not prompt\n"
                            "you, it just does it. Ending kernel processes (like csrss) WILL cause your pc to panic\n"
                            "and reboot.\n"
                            "\n"
                            "The process lockdown feature keeps only a certain list of exe's running, and kill the\n"
                            "rest of them. Be very careful with this! You can sync the allow list to the list of\n"
                            "current processes in the main window or on the popup menu in the system tray.\n", "About", MB_OK);
}


void TfrmMain::RefreshProcessList()
{
    while (!ProcessList.empty())
    {
        SProcess*   Process = ProcessList.front();
        delete Process;
        ProcessList.pop_front();
    }
    list<PROCESSENTRY32>    List;
    GetProcessList(&List);
    list<PROCESSENTRY32>::iterator  ListIterator    = List.begin();
    int                             ListTotal       = List.size();
    int                             k;
    for (k = 0; k < ListTotal; ++k)
    {
        SProcess*       ProcessToAdd    = new SProcess;
        PROCESSENTRY32  Process         = *ListIterator;
        ProcessToAdd->PID = Process.th32ProcessID;
        ProcessToAdd->Threads = Process.cntThreads;
        ProcessToAdd->ParentPID = Process.th32ParentProcessID;
        ProcessToAdd->Priority = Process.pcPriClassBase;
        ProcessToAdd->ExeFile = Process.szExeFile;

        ProcessList.push_back(ProcessToAdd);

        ++ListIterator;
    }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::Timer1Timer(TObject* Sender)
{
    RefreshProcessList();
    RefreshProcessListBox();
}
//---------------------------------------------------------------------------

void TfrmMain::RefreshProcessListBox()
{
    static int  OldTotalProcesses   = 0;
    int         TotalProcesses      = ProcessList.size();
    if (TotalProcesses == OldTotalProcesses)
    {
        return;
    }
    OldTotalProcesses = ProcessList.size();


    int OldPos  = lbxProcessList->ItemIndex;
    lbxProcessList->Clear();

    list<SProcess*>::iterator   it  = ProcessList.begin();
    int                         k;
    for (k = 0; k < TotalProcesses; ++k)
    {
        SProcess*   Process = *it;

        char        Buffer[1000];
        sprintf(Buffer, "%-24s %5d", Process->ExeFile.c_str(), Process->PID);

        lbxProcessList->Items->Add(Buffer);

        ++it;
    }

    lblTotalProcesses->Caption = IntToStr(TotalProcesses);
    lbxProcessList->ItemIndex = OldPos;
    lbxProcessListClick(this);
}

//---------------------------------------------------------------------------

SProcess* TfrmMain::GetProcessInfo(int PID)
{
    int                         TotalProcesses  = ProcessList.size();

    list<SProcess*>::iterator   it              = ProcessList.begin();
    int                         k;
    for (k = 0; k < TotalProcesses; ++k)
    {
        SProcess*   Process = *it;
        if (Process->PID == PID)
        {
            return Process;
        }

        ++it;
    }
    return 0;
}


//---------------------------------------------------------------------------


void __fastcall TfrmMain::lbxProcessListClick(TObject* Sender)
{
    int         PID     = GetListPID(lbxProcessList->ItemIndex);
    SProcess*   Process = GetProcessInfo(PID);
    lblExeName->Caption = Process->ExeFile;
    lblPID->Caption = IntToStr(Process->PID);
    lblPPID->Caption = IntToStr(Process->ParentPID);
    lblThreads->Caption = IntToStr(Process->Threads);
    lblPriority->Caption = IntToStr(Process->Priority);

    int SelCount    = lbxProcessList->SelCount;
    if (SelCount == 0)
    {
        btnEndProcess->Caption = "End Process";
        btnEndProcess->Enabled = false;
    }
    else if (SelCount == 1)
    {
        btnEndProcess->Caption = "End Process";
        btnEndProcess->Enabled = true;
    }
    else
    {
        btnEndProcess->Caption = "End Processes";
        btnEndProcess->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormCreate(TObject* Sender)
{
    GiveDebugPriv(GetCurrentProcessId());
    if (szCommandLine == "tray")
    {
        Application->ShowMainForm = false;
        Hide();
    }
    HWND    h   = FindWindow(NULL, szVersion.c_str());
    Application->Title = szVersion;
    if (h != NULL)
    {
        if (szCommandLine != "tray")
        {
            DWORD   dwThreadID;
            dwThreadID = GetWindowThreadProcessId(h, NULL);
            PostThreadMessage(dwThreadID, WM_HOTKEY, 100, 0);
        }
        Application->ShowMainForm = false;
        fQueueExit = true;
        return;
    }

    RefreshProcessList();
    RefreshProcessListBox();
    try
    {
        lbxAllowed->Items->LoadFromFile("processes.lst");
    }
    catch (...)
    {
        fQueueAddAllProcs = true;
        About();
    }

    Caption = szVersion;
    RegisterHotKey(Application->Handle, 100, MOD_ALT | MOD_CONTROL, VK_F12);
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::btnEndProcessClick(TObject* Sender)
{
    for (int k = 0; k < lbxProcessList->Items->Count; ++k)
    {
        if (lbxProcessList->Selected[k])
        {
            EndProcess(GetListPID(k));
        }
    }
}
//---------------------------------------------------------------------------

int TfrmMain::GetListPID(int Index)
{
    if ((Index < 0) || (Index >= lbxProcessList->Items->Count))
    {
        return 0;
    }
    AnsiString  Txt     = lbxProcessList->Items->Strings[Index];
    AnsiString  PID_Txt = Txt.SubString(Txt.Length() - 6, 7);
    int         PID     = 0;
    TryStrToInt(PID_Txt, PID);
    return PID;
}

//---------------------------------------------------------------------------

void TfrmMain::EndProcess(int PID)
{
    HANDLE  h   = OpenProcess(PROCESS_ALL_ACCESS, 0, PID);
    TerminateProcess(h, -1);
    CloseHandle(h);
}

void __fastcall TfrmMain::FormDestroy(TObject* Sender)
{
    Save();
}
//---------------------------------------------------------------------------


void TfrmMain::KillProcesses()
{
    list<SProcess*>::iterator   it;
    int                         t   = ProcessList.size();
    list<SProcess*>             ToKill;
    it = ProcessList.begin();
    for (int i = 0; i < t; ++i)
    {
        ToKill.push_back(*(it++));
    }
    for (int i = 0; i < lbxAllowed->Items->Count; ++i)
    {
        AnsiString  n   = lbxAllowed->Items->Strings[i];
        it = ProcessList.begin();
        for (int j = 0; j < t; ++j)
        {
            SProcess*   Proc    = *(it++);
            if (Proc->ExeFile.LowerCase() == n.LowerCase())
            {
                ToKill.remove(Proc);
            }
        }
    }

    t = ToKill.size();
    it = ToKill.begin();
    for (int i = 0; i < t; ++i)
    {
        SProcess*   Proc    = *(it++);
        EndProcess(Proc->PID);
    }
}
void __fastcall TfrmMain::Timer2Timer(TObject* Sender)
{
    if (chkActivate->Checked)
    {
        KillProcesses();
    }
    if (fQueueSelectAllProcs)
    {
        int tot = lbxProcessList->Items->Count;
        for (int i = 0; i < tot; ++i)
        {
            lbxProcessList->Selected[i] = true;
        }
    }
    if (fQueueSelectAllAllowed)
    {
        int tot = lbxAllowed->Items->Count;
        for (int i = 0; i < tot; ++i)
        {
            lbxAllowed->Selected[i] = true;
        }
    }
    if (fQueueAddAllProcs)
    {
        lbxAllowed->Items->Clear();
        RefreshProcessList();
        RefreshProcessListBox();
        int tot = lbxProcessList->Items->Count;
        for (int i = 0; i < tot; ++i)
        {
            AnsiString  s   = lbxProcessList->Items->Strings[i];
            AnsiString  s2  = s.SubString(0, s.Length() - 4).Trim();
            if (lbxAllowed->Items->IndexOf(s2) == -1)
            {
                lbxAllowed->Items->Add(s2);
            }
        }
        Save();
    }
    if (fQueueEnableLockdown)
    {
        chkActivate->Checked = true;
    }
    if (fQueueExit)
    {
        fMayClose = true;
        Close();
    }
    fQueueEnableLockdown = false;
    fQueueSelectAllProcs = false;
    fQueueSelectAllAllowed = false;
    fQueueAddAllProcs = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnBanClick(TObject* Sender)
{
    int tot = lbxAllowed->Items->Count;
    for (int i = tot - 1; i >= 0; --i)
    {
        if (lbxAllowed->Selected[i])
        {
            lbxAllowed->Items->Delete(i);
        }
    }
    Save();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnAllowClick(TObject* Sender)
{
    int tot = lbxProcessList->Items->Count;
    for (int i = 0; i < tot; ++i)
    {
        if (lbxProcessList->Selected[i])
        {
            AnsiString  s   = lbxProcessList->Items->Strings[i];
            AnsiString  s2  = s.SubString(0, s.Length() - 4).Trim();
            if (lbxAllowed->Items->IndexOf(s2) == -1)
            {
                lbxAllowed->Items->Add(s2);
            }
        }
    }
    Save();
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::lbxAllowedKeyDown(TObject* Sender, WORD& Key, TShiftState Shift)
{
    if (Key == VK_DELETE)
    {
        btnBan->OnClick(this);
    }
    if (Key == 'a' || Key == 'A')
    {
        if (Shift.Contains(ssCtrl))
        {
            fQueueSelectAllAllowed = true;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::lbxProcessListKeyDown(TObject* Sender, WORD& Key, TShiftState Shift)
{
    if (Key == VK_RETURN)
    {
        btnAllow->OnClick(this);
    }
    if (Key == 'a' || Key == 'A')
    {
        if (Shift.Contains(ssCtrl))
        {
            fQueueSelectAllProcs = true;
        }
    }
}
//---------------------------------------------------------------------------




void __fastcall TfrmMain::Button1Click(TObject* Sender)
{
    AnsiString  s2  = InputBox("Add process", "Enter an .exe:", "");
    if (s2 == "")
    {
        return;
    }
    if (lbxAllowed->Items->IndexOf(s2) == -1)
    {
        lbxAllowed->Items->Add(s2);
    }
    Save();
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::FormClose(TObject* Sender, TCloseAction& Action)
{
    if (fMayClose)
    {
        Action = caFree;
    }
    else
    {
        Action = caNone;
        Hide();
    }
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::TrayIcon1Restore(TObject* Sender)
{
    Show();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Quit2Click(TObject* Sender)
{
    btnUnload->OnClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Show1Click(TObject* Sender)
{
    Show();
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::chkActivateClick(TObject* Sender)
{
    EnableProcessLockdown1->Checked = chkActivate->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::EnableProcessLockdown1Click(TObject* Sender)
{
    chkActivate->Checked = !chkActivate->Checked;
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::Label1Click(TObject* Sender)
{
    ShellExecute(NULL, "open", "http://www.antivoid.net", "", "", SW_SHOW);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Button2Click(TObject* Sender)
{
    About();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Button3Click(TObject* Sender)
{
    fQueueAddAllProcs = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::SyncLockdown1Click(TObject* Sender)
{
    fQueueAddAllProcs = true;
    fQueueEnableLockdown = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Button4Click(TObject* Sender)
{
    SyncLockdown1->OnClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnUnloadClick(TObject* Sender)
{
    if (Application->MessageBox("Are you sure you want to exit?", szVersion.c_str(), MB_YESNO) == IDYES)
    {
        fQueueExit = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::lbxProcessListDblClick(TObject* Sender)
{
    btnAllow->OnClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormKeyDown(TObject* Sender, WORD& Key, TShiftState Shift)
{
    if (Key == VK_ESCAPE)
    {
        Close();
    }
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::Button5Click(TObject* Sender)
{
    lbxAllowed->Clear();

    lbxAllowed->Items->Add(ExtractFileName(Application->ExeName));

    lbxAllowed->Items->Add("csrss.exe");
    lbxAllowed->Items->Add("explorer.exe");
    lbxAllowed->Items->Add("lsass.exe");
    lbxAllowed->Items->Add("services.exe");
    lbxAllowed->Items->Add("smss.exe");
    lbxAllowed->Items->Add("svchost.exe");
    lbxAllowed->Items->Add("System");
    lbxAllowed->Items->Add("winlogon.exe");
    lbxAllowed->Items->Add("[System Process]");
    Save();
}
//---------------------------------------------------------------------------
void TfrmMain::Save()
{
    try
    {
        lbxAllowed->Items->SaveToFile("processes.lst");
    }
    catch (...)
    {
    }
}

void __fastcall TfrmMain::ApplicationEvents1Message(tagMSG& Msg, bool& Handled)
{
    if (Msg.message == WM_HOTKEY && Msg.wParam == 100)
    {
        Show();
        SetFocus();
        SetForegroundWindow(Handle);
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormHide(TObject* Sender)
{
    Application->ShowMainForm = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormShow(TObject* Sender)
{
    Application->ShowMainForm = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Idle1Click(TObject* Sender)
{
    DoPriority(IDLE_PRIORITY_CLASS);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TfrmMain::DoPriority(int iClass)
{
    int tot = lbxProcessList->Items->Count;
    for (int i = 0; i < tot; ++i)
    {
        if (lbxProcessList->Selected[i])
        {
            AnsiString  s           = lbxProcessList->Items->Strings[i];
            AnsiString  s2          = s.SubString(s.Length() - 8, 16).Trim();
            int         iPID        = StrToInt(s2);
            HANDLE      hProcess    = OpenProcess(PROCESS_ALL_ACCESS, 0, iPID);
            SetPriorityClass(hProcess, iClass);
            CloseHandle(hProcess);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Normal1Click(TObject* Sender)
{
    DoPriority(NORMAL_PRIORITY_CLASS);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::High1Click(TObject* Sender)
{
    DoPriority(HIGH_PRIORITY_CLASS);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Realtime1Click(TObject* Sender)
{
    DoPriority(REALTIME_PRIORITY_CLASS);
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::GiveDebugPrivilege1Click(TObject* Sender)
{
    int tot = lbxProcessList->Items->Count;
    for (int i = 0; i < tot; ++i)
    {
        if (lbxProcessList->Selected[i])
        {
            AnsiString  s       = lbxProcessList->Items->Strings[i];
            AnsiString  s2      = s.SubString(s.Length() - 8, 16).Trim();
            int         iPID    = StrToInt(s2);
            GiveDebugPriv(iPID);
        }
    }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TfrmMain::GiveDebugPriv(int iPID)
{
    HANDLE  hProcess    = OpenProcess(PROCESS_ALL_ACCESS, false, iPID);
    if (hProcess == NULL)
    {
        return;
    }

    OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

    SetPrivilege(hToken, SE_DEBUG_NAME, TRUE);
    SetPrivilege(hToken, "SeTcbPrivilege", TRUE);

    CloseHandle(hProcess);
}
//---------------------------------------------------------------------------

