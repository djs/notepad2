/******************************************************************************
*
*
* Notepad2
*
* Notepad2.h
*   Global definitions and declarations
*
* See Readme.txt for more information about this source code.
* Please send me your comments to this work.
*
* See License.txt for details about distribution and modification.
*
*                                              (c) Florian Balmer 1996-2008
*                                                  florian.balmer@gmail.com
*                                               http://www.flos-freeware.ch
*
*
******************************************************************************/



//==== Main Window ============================================================
#define WC_NOTEPAD2 "Notepad2"
#define WC_NOTEPAD2PASTEBOARD "Notepad2PasteBoard"


//==== Data Type for WM_COPYDATA ==============================================
#define DATA_NOTEPAD2_FILEARG 0xDE70


//==== Toolbar Style ==========================================================
#define WS_TOOLBAR (WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | \
                    TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_ALTDRAG | \
                    TBSTYLE_LIST | CCS_NODIVIDER | CCS_NOPARENTALIGN | \
                    CCS_ADJUSTABLE)


//==== ReBar Style ============================================================
#define WS_REBAR (WS_CHILD | WS_CLIPCHILDREN | WS_BORDER | RBS_VARHEIGHT | \
                  RBS_BANDBORDERS | CCS_NODIVIDER | CCS_NOPARENTALIGN)


//==== Ids ====================================================================
#define IDC_STATUSBAR    0xFB00
#define IDC_TOOLBAR      0xFB01
#define IDC_REBAR        0xFB02
#define IDC_EDIT         0xFB03
#define IDC_EDITFRAME    0xFB04
#define IDC_FILENAME     0xFB05


//==== Statusbar ==============================================================
#define STATUS_DOCPOS    0
#define STATUS_DOCSIZE   1
#define STATUS_CODEPAGE  2
#define STATUS_EOLMODE   3
#define STATUS_OVRMODE   4
#define STATUS_LEXER     5
#define STATUS_HELP    255


//==== Change Notifications ===================================================
#define ID_WATCHTIMER 0xA000
#define WM_CHANGENOTIFY WM_USER+1
#define WM_CHANGENOTIFYCLEAR WM_USER+2


//==== Callback Message from System Tray ======================================
#define WM_TRAYMESSAGE WM_USER


//==== Function Declarations ==================================================
BOOL InitApplication(HINSTANCE);
HWND InitInstance(HINSTANCE,LPSTR,int);
BOOL ActivatePrevInst();
void ShowNotifyIcon(HWND,BOOL);
void SetNotifyIconTitle(HWND);
void InstallFileWatching(LPCSTR);
void CALLBACK WatchTimerProc(HWND,UINT,UINT_PTR,DWORD);


void LoadSettings();
void SaveSettings(BOOL);
void ParseCommandLine(LPSTR);
void LoadFlags();
int  FindIniFile();


void UpdateStatusbar();
void UpdateToolbar();
void UpdateLineNumberWidth();


BOOL FileIO(BOOL,LPCSTR,BOOL,int*,int*,BOOL*,BOOL*,BOOL);
BOOL FileLoad(BOOL,BOOL,BOOL,BOOL,LPCSTR);
BOOL FileSave(BOOL,BOOL,BOOL,BOOL);
BOOL OpenFileDlg(HWND,LPSTR,int,LPCSTR);
BOOL SaveFileDlg(HWND,LPSTR,int);


LRESULT CALLBACK MainWndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT MsgCreate(HWND,WPARAM,LPARAM);
void    CreateBars(HWND,HINSTANCE);
void    MsgThemeChanged(HWND,WPARAM,LPARAM);
void    MsgSize(HWND,WPARAM,LPARAM);
void    MsgInitMenu(HWND,WPARAM,LPARAM);
LRESULT MsgCommand(HWND,WPARAM,LPARAM);
LRESULT MsgNotify(HWND,WPARAM,LPARAM);



///   End of Textview.h   \\\
