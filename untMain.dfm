object frmMain: TfrmMain
  Left = 409
  Top = 194
  Width = 548
  Height = 478
  BorderIcons = [biSystemMenu]
  Caption = 'antivoid'#39's Task Manager'
  Color = clBtnFace
  Constraints.MaxWidth = 548
  Constraints.MinHeight = 368
  Constraints.MinWidth = 548
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  DesignSize = (
    540
    444)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 272
    Top = 423
    Width = 142
    Height = 13
    Cursor = crHandPoint
    Alignment = taRightJustify
    Anchors = [akRight, akBottom]
    AutoSize = False
    Caption = 'http://www.antivoid.net'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = [fsUnderline]
    ParentFont = False
    OnClick = Label1Click
  end
  object Label9: TLabel
    Left = 249
    Top = 423
    Width = 24
    Height = 13
    Anchors = [akLeft, akBottom]
    Caption = 'Visit'
  end
  object GroupBox2: TGroupBox
    Left = 274
    Top = 8
    Width = 260
    Height = 407
    Anchors = [akLeft, akTop, akBottom]
    Caption = 'Process Lockdown'
    TabOrder = 1
    DesignSize = (
      260
      407)
    object Label4: TLabel
      Left = 8
      Top = 16
      Width = 110
      Height = 13
      Caption = 'Allowed Processes:'
      PopupMenu = PopupMenu1
    end
    object btnBan: TButton
      Left = 8
      Top = 319
      Width = 105
      Height = 25
      Anchors = [akLeft, akBottom]
      Caption = 'Ban <<'
      TabOrder = 2
      OnClick = btnBanClick
    end
    object chkActivate: TCheckBox
      Left = 9
      Top = 382
      Width = 136
      Height = 17
      Anchors = [akRight, akBottom]
      Caption = 'Enable lockdown'
      TabOrder = 6
      OnClick = chkActivateClick
    end
    object lbxAllowed: TListBox
      Left = 8
      Top = 40
      Width = 244
      Height = 243
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelInner = bvNone
      BevelKind = bkFlat
      BorderStyle = bsNone
      ItemHeight = 13
      MultiSelect = True
      Sorted = True
      TabOrder = 0
      OnKeyDown = lbxAllowedKeyDown
    end
    object Button1: TButton
      Left = 7
      Top = 351
      Width = 106
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = 'Add...'
      TabOrder = 4
      OnClick = Button1Click
    end
    object Button3: TButton
      Left = 128
      Top = 319
      Width = 123
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = '>> Sync'
      TabOrder = 3
      OnClick = Button3Click
    end
    object Button4: TButton
      Left = 128
      Top = 351
      Width = 123
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = '>> Sync && Lock'
      TabOrder = 5
      OnClick = Button4Click
    end
    object Button5: TButton
      Left = 7
      Top = 288
      Width = 244
      Height = 25
      Anchors = [akLeft, akBottom]
      Caption = 'Default recommended minimum'
      TabOrder = 1
      OnClick = Button5Click
    end
  end
  object GroupBox3: TGroupBox
    Left = 6
    Top = 8
    Width = 261
    Height = 407
    Anchors = [akLeft, akTop, akBottom]
    Caption = 'Process List'
    TabOrder = 0
    DesignSize = (
      261
      407)
    object Label2: TLabel
      Left = 8
      Top = 17
      Width = 94
      Height = 13
      Caption = 'Total processes:'
    end
    object lblTotalProcesses: TLabel
      Left = 106
      Top = 17
      Width = 7
      Height = 13
      Caption = '0'
    end
    object Label3: TLabel
      Left = 9
      Top = 288
      Width = 71
      Height = 13
      Alignment = taRightJustify
      Anchors = [akLeft, akBottom]
      AutoSize = False
      Caption = 'Exe name:'
    end
    object Label5: TLabel
      Left = 9
      Top = 304
      Width = 71
      Height = 13
      Alignment = taRightJustify
      Anchors = [akLeft, akBottom]
      AutoSize = False
      Caption = 'PID:'
    end
    object Label6: TLabel
      Left = 9
      Top = 320
      Width = 71
      Height = 13
      Alignment = taRightJustify
      Anchors = [akLeft, akBottom]
      AutoSize = False
      Caption = 'Parent PID:'
    end
    object Label7: TLabel
      Left = 9
      Top = 336
      Width = 71
      Height = 13
      Alignment = taRightJustify
      Anchors = [akLeft, akBottom]
      AutoSize = False
      Caption = 'Threads'
    end
    object Label8: TLabel
      Left = 9
      Top = 352
      Width = 71
      Height = 13
      Alignment = taRightJustify
      Anchors = [akLeft, akBottom]
      AutoSize = False
      Caption = 'Priority:'
    end
    object lblExeName: TLabel
      Left = 83
      Top = 288
      Width = 6
      Height = 13
      Anchors = [akLeft, akBottom]
      Caption = '?'
    end
    object lblPID: TLabel
      Left = 83
      Top = 304
      Width = 6
      Height = 13
      Anchors = [akLeft, akBottom]
      Caption = '?'
    end
    object lblPPID: TLabel
      Left = 83
      Top = 320
      Width = 6
      Height = 13
      Anchors = [akLeft, akBottom]
      Caption = '?'
    end
    object lblThreads: TLabel
      Left = 83
      Top = 336
      Width = 6
      Height = 13
      Anchors = [akLeft, akBottom]
      Caption = '?'
    end
    object lblPriority: TLabel
      Left = 83
      Top = 352
      Width = 6
      Height = 13
      Anchors = [akLeft, akBottom]
      Caption = '?'
    end
    object lbxProcessList: TListBox
      Left = 9
      Top = 40
      Width = 244
      Height = 243
      Anchors = [akLeft, akTop, akBottom]
      BevelInner = bvNone
      BevelKind = bkFlat
      BevelOuter = bvRaised
      BorderStyle = bsNone
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Courier New'
      Font.Style = []
      ItemHeight = 14
      MultiSelect = True
      ParentFont = False
      PopupMenu = popProcess
      Sorted = True
      TabOrder = 0
      OnClick = lbxProcessListClick
      OnDblClick = lbxProcessListDblClick
      OnKeyDown = lbxProcessListKeyDown
    end
    object btnEndProcess: TButton
      Left = 8
      Top = 374
      Width = 105
      Height = 25
      Anchors = [akLeft, akBottom]
      Caption = 'End Process'
      TabOrder = 1
      OnClick = btnEndProcessClick
    end
    object btnAllow: TButton
      Left = 178
      Top = 375
      Width = 75
      Height = 25
      Anchors = [akLeft, akBottom]
      Caption = 'Allow >>'
      TabOrder = 2
      OnClick = btnAllowClick
    end
  end
  object Button2: TButton
    Left = 459
    Top = 419
    Width = 75
    Height = 22
    Anchors = [akRight, akBottom]
    Caption = 'About'
    TabOrder = 3
    OnClick = Button2Click
  end
  object btnUnload: TButton
    Left = 8
    Top = 419
    Width = 121
    Height = 22
    Anchors = [akLeft, akBottom]
    Caption = 'Unload and Exit'
    TabOrder = 2
    OnClick = btnUnloadClick
  end
  object Timer1: TTimer
    Interval = 250
    OnTimer = Timer1Timer
    Left = 56
    Top = 56
  end
  object Timer2: TTimer
    Interval = 100
    OnTimer = Timer2Timer
    Left = 288
    Top = 56
  end
  object TrayIcon1: TTrayIcon
    Visible = True
    PopupMenu = PopupMenu1
    Hide = True
    RestoreOn = imDoubleClick
    PopupMenuOn = imRightClickUp
    OnRestore = TrayIcon1Restore
    Left = 24
    Top = 56
  end
  object PopupMenu1: TPopupMenu
    Left = 88
    Top = 56
    object Show1: TMenuItem
      Caption = 'Show'
      OnClick = Show1Click
    end
    object SyncLockdown1: TMenuItem
      Caption = 'Sync && Lockdown'
      OnClick = SyncLockdown1Click
    end
    object EnableProcessLockdown1: TMenuItem
      Caption = 'Enable Process Lockdown'
      OnClick = EnableProcessLockdown1Click
    end
    object Quit1: TMenuItem
      Caption = '-'
    end
    object Quit2: TMenuItem
      Caption = 'Quit'
      OnClick = Quit2Click
    end
  end
  object ApplicationEvents1: TApplicationEvents
    OnMessage = ApplicationEvents1Message
    Left = 120
    Top = 56
  end
  object popProcess: TPopupMenu
    Left = 152
    Top = 56
    object Priority1: TMenuItem
      Caption = 'Priority'
      object Idle1: TMenuItem
        Caption = 'Idle'
        OnClick = Idle1Click
      end
      object Normal1: TMenuItem
        Caption = 'Normal'
        OnClick = Normal1Click
      end
      object High1: TMenuItem
        Caption = 'High'
        OnClick = High1Click
      end
      object Realtime1: TMenuItem
        Caption = 'Realtime'
        OnClick = Realtime1Click
      end
    end
    object GiveDebugPrivilege1: TMenuItem
      Caption = 'Give Debug Privilege'
      OnClick = GiveDebugPrivilege1Click
    end
  end
end
