//---------------------------------------------------------------------------

#ifndef untMainH
#define untMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "Structs.h"
#include "trayicon.h"
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <AppEvnts.hpp>
#include <list>

using namespace std;
//---------------------------------------------------------------------------
class TfrmMain:public TForm
{
__published:    // IDE-managed Components
    TTimer*                 Timer1;
    TTimer*                 Timer2;
    TTrayIcon*              TrayIcon1;
    TPopupMenu*             PopupMenu1;
    TGroupBox*              GroupBox2;
    TLabel*                 Label4;
    TButton*                btnBan;
    TCheckBox*              chkActivate;
    TListBox*               lbxAllowed;
    TButton*                Button1;
    TGroupBox*              GroupBox3;
    TLabel*                 Label2;
    TLabel*                 lblTotalProcesses;
    TLabel*                 Label3;
    TLabel*                 Label5;
    TLabel*                 Label6;
    TLabel*                 Label7;
    TLabel*                 Label8;
    TLabel*                 lblExeName;
    TLabel*                 lblPID;
    TLabel*                 lblPPID;
    TLabel*                 lblThreads;
    TLabel*                 lblPriority;
    TListBox*               lbxProcessList;
    TButton*                btnEndProcess;
    TButton*                btnAllow;
    TMenuItem*              Quit1;
    TMenuItem*              Quit2;
    TMenuItem*              EnableProcessLockdown1;
    TMenuItem*              Show1;
    TLabel*                 Label1;
    TButton*                Button2;
    TButton*                Button3;
    TMenuItem*              SyncLockdown1;
    TButton*                Button4;
    TButton*                btnUnload;
    TLabel*                 Label9;
    TButton*                Button5;
    TApplicationEvents*     ApplicationEvents1;
    TPopupMenu*             popProcess;
    TMenuItem*              Priority1;
    TMenuItem*              Idle1;
    TMenuItem*              Normal1;
    TMenuItem*              High1;
    TMenuItem*              Realtime1;
    TMenuItem*              GiveDebugPrivilege1;
    void __fastcall         Timer1Timer(TObject* Sender);
    void __fastcall         lbxProcessListClick(TObject* Sender);
    void __fastcall         FormCreate(TObject* Sender);
    void __fastcall         btnEndProcessClick(TObject* Sender);
    void __fastcall         FormDestroy(TObject* Sender);
    void __fastcall         Timer2Timer(TObject* Sender);
    void __fastcall         btnBanClick(TObject* Sender);
    void __fastcall         btnAllowClick(TObject* Sender);
    void __fastcall         lbxAllowedKeyDown(TObject* Sender, WORD& Key, TShiftState Shift);
    void __fastcall         lbxProcessListKeyDown(TObject* Sender, WORD& Key, TShiftState Shift);
    void __fastcall         Button1Click(TObject* Sender);
    void __fastcall         FormClose(TObject* Sender, TCloseAction& Action);
    void __fastcall         TrayIcon1Restore(TObject* Sender);
    void __fastcall         Quit2Click(TObject* Sender);
    void __fastcall         Show1Click(TObject* Sender);
    void __fastcall         chkActivateClick(TObject* Sender);
    void __fastcall         EnableProcessLockdown1Click(TObject* Sender);
    void __fastcall         Label1Click(TObject* Sender);
    void __fastcall         Button2Click(TObject* Sender);
    void __fastcall         Button3Click(TObject* Sender);
    void __fastcall         SyncLockdown1Click(TObject* Sender);
    void __fastcall         Button4Click(TObject* Sender);
    void __fastcall         btnUnloadClick(TObject* Sender);
    void __fastcall         lbxProcessListDblClick(TObject* Sender);
    void __fastcall         FormKeyDown(TObject* Sender, WORD& Key, TShiftState Shift);
    void __fastcall         Button5Click(TObject* Sender);
    void __fastcall         ApplicationEvents1Message(tagMSG& Msg, bool& Handled);
    void __fastcall         FormHide(TObject* Sender);
    void __fastcall         FormShow(TObject* Sender);
    void __fastcall         Idle1Click(TObject* Sender);
    void __fastcall         Normal1Click(TObject* Sender);
    void __fastcall         High1Click(TObject* Sender);
    void __fastcall         Realtime1Click(TObject* Sender);
    void __fastcall         GiveDebugPrivilege1Click(TObject* Sender);
private:    // User declarations
    void                    RefreshProcessList();
    void                    RefreshProcessListBox();

    SProcess*               GetProcessInfo(int PID);
    int                     GetListPID(int Index);
    void                    EndProcess(int PID);

    list<SProcess*>         ProcessList;
public:     // User declarations
    __fastcall              TfrmMain(TComponent* Owner);
    void                    KillProcesses();
    void                    Save();
    void                    DoPriority(int iClass);
    void                    GiveDebugPriv(int iPID);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain*frmMain;
//---------------------------------------------------------------------------
#endif
