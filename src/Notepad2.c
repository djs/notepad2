/******************************************************************************
*
*
* Notepad2
*
* Notepad2.c
*   Main application window functionality
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
#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <richedit.h>
#include <shellapi.h>
#include <commdlg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "scintilla.h"
#include "scilexer.h"
#include "sciwrap.h"
#include "edit.h"
#include "styles.h"
#include "helpers.h"
#include "dialogs.h"
#include "notepad2.h"
#include "resource.h"



/******************************************************************************
*
* Local and global Variables for Notepad2.c
*
*/
HWND      hwndStatus;
HWND      hwndToolbar;
HWND      hwndReBar;
HWND      hwndEdit;
HWND      hwndMain;
HWND      hwndFindReplace = NULL;
HWND      hwndNextCBChain = NULL;
HWND      hDlgFindReplace = NULL;

#define NUMTOOLBITMAPS  23
#define NUMINITIALTOOLS 24

TBBUTTON  tbbMainWnd[] = { {0,IDT_FILE_NEW,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {1,IDT_FILE_OPEN,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {2,IDT_FILE_BROWSE,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {3,IDT_FILE_SAVE,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {4,IDT_EDIT_UNDO,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {5,IDT_EDIT_REDO,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {6,IDT_EDIT_CUT,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {7,IDT_EDIT_COPY,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {8,IDT_EDIT_PASTE,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {9,IDT_EDIT_FIND,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {10,IDT_EDIT_REPLACE,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {11,IDT_VIEW_WORDWRAP,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {12,IDT_VIEW_ZOOMIN,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {13,IDT_VIEW_ZOOMOUT,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {14,IDT_VIEW_SCHEME,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {15,IDT_VIEW_SCHEMECONFIG,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {16,IDT_FILE_EXIT,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {17,IDT_FILE_SAVEAS,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {18,IDT_FILE_SAVECOPY,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {19,IDT_EDIT_CLEAR,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {20,IDT_FILE_PRINT,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {21,IDT_FILE_OPENFAV,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {22,IDT_FILE_ADDTOFAV,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0} };

char      szIniFile[MAX_PATH] = "";
BOOL      bSaveSettings;
BOOL      bSaveRecentFiles;
BOOL      bSaveFindReplace;
char      tchOpenWithDir[MAX_PATH];
char      tchOpenWithDirBuf[MAX_PATH];
char      tchFavoritesDir[MAX_PATH];
char      tchFavoritesDirBuf[MAX_PATH];
char      tchDefaultDir[MAX_PATH];
char      tchDefaultExtension[64];
char      tchFileDlgFilters[5*1024];
char      tchToolbarButtons[512];
char      tchToolbarBitmap[MAX_PATH];
char      tchToolbarBitmapHot[MAX_PATH];
char      tchToolbarBitmapDisabled[MAX_PATH];
int       iPathNameFormat;
BOOL      fWordWrap;
int       iWordWrapIndent;
int       iWordWrapSymbols;
BOOL      bShowWordWrapSymbols;
BOOL      bMatchBraces;
BOOL      bAutoIndent;
BOOL      bAutoCloseTags;
BOOL      bShowIndentGuides;
BOOL      bHiliteCurrentLine;
BOOL      bTabsAsSpaces;
int       iTabWidth;
int       iIndentWidth;
BOOL      bMarkLongLines;
int       iLongLinesLimit;
int       iLongLineMode;
BOOL      bShowSelectionMargin;
BOOL      bShowLineNumbers;
BOOL      bViewWhiteSpace;
BOOL      bViewEOLs;
int       iDefaultEncoding;
BOOL      bSkipUnicodeDetection;
int       iDefaultEOLMode;
BOOL      bFixLineEndings;
int       iPrintHeader;
int       iPrintFooter;
int       iPrintColor;
int       iPrintZoom;
RECT      pagesetupMargin;
BOOL      bSaveBeforeRunningTools;
BOOL      bEnableFileWatching;
int       iFileCheckInverval;
int       iEscFunction;
BOOL      bAlwaysOnTop;
BOOL      bMinimizeToTray;
BOOL      bTransparentMode;
BOOL      bTransparentModeAvailable;
BOOL      bShowToolbar;
BOOL      bShowStatusbar;

typedef struct _wi
{
  int x;
  int y;
  int cx;
  int cy;
  int max;
} WININFO;

WININFO wi;
BOOL    bStickyWinPos;

BOOL    bIsAppThemed;
int     cyReBar;
int     cyReBarFrame;

int     cxFileMRUDlg;
int     cyFileMRUDlg;
int     cxOpenWithDlg;
int     cyOpenWithDlg;
int     cxFavoritesDlg;
int     cyFavoritesDlg;

LPSTR      lpFileArg = NULL;
LPMRULIST  pFileMRU;
LPMRULIST  mruFind;
LPMRULIST  mruReplace;

char      szCurFile[MAX_PATH+40];
BOOL      bModified;
BOOL      bReadOnly = FALSE;
int       iCodePage;
int       iInitialCP;
int       iEOLMode;

int       iDefaultCodePage;
int       iDefaultCharSet;

int       iInitialLine;
int       iInitialColumn;

int       iInitialLexer;

BOOL      bLastCopyFromMe = FALSE;
UINT      uidsAppTitle = IDS_APPTITLE;

HANDLE    hChangeHandle = NULL;
BOOL      bRunningWatch = FALSE;
WIN32_FIND_DATA fdCurFile;

UINT      msgTaskbarCreated = 0;

HMODULE   hModUxTheme = NULL;

EDITFINDREPLACE efrData = { "", "", "", "", 0, 0, 0, 0, 0, 0, NULL };
UINT cpLastFind = 0;

int iEncodings[5] = {
  NCP_DEFAULT,
  NCP_UNICODE|NCP_UNICODE_BOM,
  NCP_UNICODE|NCP_UNICODE_REVERSE|NCP_UNICODE_BOM,
  NCP_UTF8,
  NCP_UTF8|NCP_UTF8_SIGN
};

int iLineEndings[3] = {
  SC_EOL_CRLF,
  SC_EOL_LF,
  SC_EOL_CR
};

struct {
  int flags;
  int location;
} WrapSymbols[4] = { { 1, 1 },
                     { 2, 2 },
                     { 3, 3 },
                     { 3, 0 } };

WCHAR wchPrefixSelection[256] = L"";
WCHAR wchAppendSelection[256] = L"";

WCHAR wchPrefixLines[256] = L"";
WCHAR wchAppendLines[256] = L"";

LPMALLOC  g_lpMalloc;
HINSTANCE g_hInstance;



//=============================================================================
//
// Flags
//
int flagNoReuseWindow      = 0;
int flagSingleFileInstance = 0;
int flagStartAsTrayIcon    = 0;
int flagNoFadeHidden       = 0;
int flagSimpleIndentGuides = 0;
int flagPosParam           = 0;
int flagDefaultPos         = 0;
int flagNewFromClipboard   = 0;
int flagPasteBoard         = 0;
int flagJumpTo             = 0;
int flagLexerSpecified     = 0;
int flagDisplayHelp        = 0;



//=============================================================================
//
//  WinMain()
//
//
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInst,
                   LPSTR lpCmdLine,int nCmdShow)
{

  MSG msg;
  HWND hwnd;
  HACCEL hAccel;
  INITCOMMONCONTROLSEX icex;
  //HMODULE hSciLexer;
  extern char szIniFile[MAX_PATH];

  // Set global variable g_hInstance
  g_hInstance = hInstance;

  // check if running at least on Windows 2000
  if (!IsWindows2korLater()) {
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER|
        FORMAT_MESSAGE_FROM_SYSTEM|
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        ERROR_OLD_WIN_VERSION,
        MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), // Default language
        (LPSTR)&lpMsgBuf,
        0,
        NULL);
    MessageBox(NULL,(LPCSTR)lpMsgBuf,"Notepad2",MB_OK|MB_ICONEXCLAMATION);
    LocalFree(lpMsgBuf);
    return(0);
  }

  // Command Line, Ini File and Flags
  ParseCommandLine(lpCmdLine);
  FindIniFile();
  LoadFlags();

  // Command Line Help Dialog
  if (flagDisplayHelp)
  {
    DisplayCmdLineHelp();
    return(0);
  }

  // Try to activate another window
  if (ActivatePrevInst())
    return(0);

  // Init OLE and Common Controls
  OleInitialize(NULL);
  SHGetMalloc(&g_lpMalloc);

  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC  = ICC_WIN95_CLASSES|ICC_COOL_CLASSES|ICC_BAR_CLASSES|ICC_USEREX_CLASSES;
  InitCommonControlsEx(&icex);

  SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);

  msgTaskbarCreated = RegisterWindowMessage("TaskbarCreated");

  hModUxTheme = LoadLibrary("uxtheme.dll");

  SciWrap_RegisterClasses(hInstance);

  // Load Settings
  LoadSettings();

  if (!InitApplication(hInstance))
    return FALSE;

  if (!(hwnd = InitInstance(hInstance,lpCmdLine,nCmdShow)))
    return FALSE;

  hAccel = LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_MAINWND));

  while (GetMessageW(&msg,NULL,0,0))
  {
    if (!IsWindow(hDlgFindReplace) || !IsDialogMessage(hDlgFindReplace,&msg))
    {
      if (!TranslateAccelerator(hwnd,hAccel,&msg))
      {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
      }
    }
  }

  // Save Settings is done elsewhere

  SciWrap_ReleaseResources();

  if (hModUxTheme)
    FreeLibrary(hModUxTheme);

  g_lpMalloc->lpVtbl->Release(g_lpMalloc);
  OleUninitialize();

  return(msg.wParam);

  hPrevInst;

}


//=============================================================================
//
//  InitApplication()
//
//
BOOL InitApplication(HINSTANCE hInstance)
{

  WNDCLASS   wc;

  wc.style         = CS_BYTEALIGNWINDOW | CS_DBLCLKS;
  wc.lpfnWndProc   = (WNDPROC)MainWndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = hInstance;
  wc.hIcon         = LoadIcon(hInstance,MAKEINTRESOURCE(IDR_MAINWND));
  wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_3DFACE+1);
  wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MAINWND);
  wc.lpszClassName = (!flagPasteBoard) ? WC_NOTEPAD2 : WC_NOTEPAD2PASTEBOARD;

  return RegisterClass(&wc);

}


//=============================================================================
//
//  InitInstance()
//
//
HWND InitInstance(HINSTANCE hInstance,LPSTR pszCmdLine,int nCmdShow)
{

  RECT rc = { wi.x, wi.y, wi.x+wi.cx, wi.y+wi.cy };
  RECT rc2;
  MONITORINFO mi;

  HMONITOR hMonitor = MonitorFromRect(&rc,MONITOR_DEFAULTTONEAREST);
  mi.cbSize = sizeof(mi);
  GetMonitorInfo(hMonitor,&mi);

  if (flagDefaultPos) {
    wi.x = wi.y = wi.cx = wi.cy = CW_USEDEFAULT;
    wi.max = 0;
  }

  else if (wi.x == CW_USEDEFAULT || wi.y == CW_USEDEFAULT ||
      wi.cx == CW_USEDEFAULT || wi.cy == CW_USEDEFAULT) {

    // default window position
    SystemParametersInfo(SPI_GETWORKAREA,0,&rc,0);
    wi.y = rc.top + 16;
    wi.cy = rc.bottom - rc.top - 32;
    wi.cx = min(rc.right - rc.left - 32,wi.cy);
    wi.x = rc.right - wi.cx - 16;
  }

  else {

    // fit window into working area of current monitor
    wi.x += (mi.rcWork.left - mi.rcMonitor.left);
    wi.y += (mi.rcWork.top - mi.rcMonitor.top);
    if (wi.x < mi.rcWork.left)
      wi.x = mi.rcWork.left;
    if (wi.y < mi.rcWork.top)
      wi.y = mi.rcWork.top;
    if (wi.x + wi.cx > mi.rcWork.right) {
      wi.x -= (wi.x + wi.cx - mi.rcWork.right);
      if (wi.x < mi.rcWork.left)
        wi.x = mi.rcWork.left;
      if (wi.x + wi.cx > mi.rcWork.right)
        wi.cx = mi.rcWork.right - wi.x;
    }
    if (wi.y + wi.cy > mi.rcWork.bottom) {
      wi.y -= (wi.y + wi.cy - mi.rcWork.bottom);
      if (wi.y < mi.rcWork.top)
        wi.y = mi.rcWork.top;
      if (wi.y + wi.cy > mi.rcWork.bottom)
        wi.cy = mi.rcWork.bottom - wi.y;
    }
    SetRect(&rc,wi.x,wi.y,wi.x+wi.cx,wi.y+wi.cy);
    if (!IntersectRect(&rc2,&rc,&mi.rcWork)) {
      wi.y = mi.rcWork.top + 16;
      wi.cy = mi.rcWork.bottom - mi.rcWork.top - 32;
      wi.cx = min(mi.rcWork.right - mi.rcWork.left - 32,wi.cy);
      wi.x = mi.rcWork.right - wi.cx - 16;
    }
  }

  hwndMain = CreateWindowEx(
               0,
               (!flagPasteBoard) ? WC_NOTEPAD2 : WC_NOTEPAD2PASTEBOARD,
               "Notepad2",
               WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
               wi.x,
               wi.y,
               wi.cx,
               wi.cy,
               NULL,
               NULL,
               hInstance,
               NULL);

  if (wi.max)
    nCmdShow = SW_SHOWMAXIMIZED;

  if (bAlwaysOnTop)
    SetWindowPos(hwndMain,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

  if (bTransparentMode)
    SetWindowTransparentMode(hwndMain,TRUE);

  // Current file information -- moved in front of ShowWindow()
  FileLoad(TRUE,TRUE,FALSE,FALSE,"");

  if (!flagStartAsTrayIcon) {
    ShowWindow(hwndMain,nCmdShow);
    UpdateWindow(hwndMain);
  }
  else {
    ShowWindow(hwndMain,SW_HIDE);    // trick ShowWindow()
    ShowNotifyIcon(hwndMain,TRUE);
  }

  // Pathname parameter
  if (lpFileArg /*&& !flagNewFromClipboard*/)
  {
    // Open from Directory
    if (PathIsDirectory(lpFileArg))
    {
      char tchFile[MAX_PATH];
      if (OpenFileDlg(hwndMain,tchFile,COUNTOF(tchFile),lpFileArg))
        FileLoad(FALSE,FALSE,FALSE,FALSE,tchFile);
    }
    else {
      if (FileLoad(FALSE,FALSE,FALSE,FALSE,lpFileArg)) {
        if (flagJumpTo) // Jump to position
          EditJumpTo(hwndEdit,iInitialLine,iInitialColumn);
      }
    }
    GlobalFree(lpFileArg);
  }

  // Check for /c [if no file is specified] -- even if a file is specified
  /*else */if (flagNewFromClipboard) {
    if (SendMessage(hwndEdit,SCI_CANPASTE,0,0)) {
      BOOL bAutoIndent2 = bAutoIndent;
      bAutoIndent = 0;
      EditJumpTo(hwndEdit,-1,0);
      SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
      if (SendMessage(hwndEdit,SCI_GETLENGTH,0,0) > 0)
        SendMessage(hwndEdit,SCI_NEWLINE,0,0);
      SendMessage(hwndEdit,SCI_PASTE,0,0);
      SendMessage(hwndEdit,SCI_NEWLINE,0,0);
      SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
      bAutoIndent = bAutoIndent2;
    }
  }

  // Check for Paste Board option -- after loading files
  if (flagPasteBoard) {
    bLastCopyFromMe = TRUE;
    hwndNextCBChain = SetClipboardViewer(hwndMain);
    uidsAppTitle = IDS_APPTITLE_PASTEBOARD;
    SetWindowTitle(hwndMain,uidsAppTitle,IDS_UNTITLED,szCurFile,
      iPathNameFormat,bModified || iCodePage != iInitialCP,
      IDS_READONLY,bReadOnly);
    bLastCopyFromMe = FALSE;
  }

  // check if a lexer was specified from the command line
  if (flagLexerSpecified) {
    if (iInitialLexer >=0 && iInitialLexer < NUMLEXERS)
      Style_SetLexerFromID(hwndEdit,iInitialLexer);
  }

  // If start as tray icon, set current filename as tooltip
  if (flagStartAsTrayIcon)
    SetNotifyIconTitle(hwndMain);

  // Toolbar
  UpdateToolbar();

  // Statusbar
  UpdateStatusbar();

  return(hwndMain);

}


//=============================================================================
//
//  MainWndProc()
//
//  Messages are distributed to the MsgXXX-handlers
//
//
LRESULT CALLBACK MainWndProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{
  static BOOL bHideIcon = FALSE;

  switch(umsg)
  {

    // Quickly handle painting and sizing messages, found in ScintillaWin.cxx
    // Cool idea, don't know if this has any effect... ;-)
    case WM_MOVE:
    case WM_MOUSEACTIVATE:
    case WM_NCHITTEST:
    case WM_NCCALCSIZE:
    case WM_NCPAINT:
    case WM_PAINT:
    case WM_ERASEBKGND:
    case WM_NCMOUSEMOVE:
    case WM_NCLBUTTONDOWN:
    case WM_WINDOWPOSCHANGING:
    case WM_WINDOWPOSCHANGED:
      return DefWindowProc(hwnd,umsg,wParam,lParam);

    case WM_CREATE:
      return MsgCreate(hwnd,wParam,lParam);


    case WM_DESTROY:
      {
        WINDOWPLACEMENT wndpl;

        // Terminate file watching
        InstallFileWatching(NULL);

        // GetWindowPlacement
        wndpl.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(hwnd,&wndpl);

        wi.x = wndpl.rcNormalPosition.left;
        wi.y = wndpl.rcNormalPosition.top;
        wi.cx = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
        wi.cy = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
        wi.max = (IsZoomed(hwnd) || (wndpl.flags & WPF_RESTORETOMAXIMIZED));

        DragAcceptFiles(hwnd,FALSE);

        // Restore clipboard chain...
        if (flagPasteBoard)
          ChangeClipboardChain(hwnd,hwndNextCBChain);

        // Destroy find / replace dialog
        if (IsWindow(hDlgFindReplace))
          DestroyWindow(hDlgFindReplace);

        // call SaveSettings() when hwndToolbar is still valid
        SaveSettings(FALSE);

        // Cleanup unwanted MRU's
        if (!bSaveRecentFiles)
          MRU_Empty(pFileMRU);
        MRU_Save(pFileMRU);
        MRU_Destroy(pFileMRU);

        if (!bSaveFindReplace) {
          MRU_Empty(mruFind);
          MRU_Empty(mruReplace);
        }
        MRU_Save(mruFind);
        MRU_Destroy(mruFind);
        MRU_Save(mruReplace);
        MRU_Destroy(mruReplace);

        // Remove tray icon if necessary
        ShowNotifyIcon(hwnd,FALSE);

        PostQuitMessage(0);
      }
      break;


    case WM_CLOSE:
      if (FileSave(FALSE,TRUE,FALSE,FALSE))
        DestroyWindow(hwnd);
      break;


    case WM_QUERYENDSESSION:
      if (FileSave(FALSE,TRUE,FALSE,FALSE))
        return TRUE;
      else
        return FALSE;


    // Reinitialize theme-dependent values and resize windows
    case 0x031A /*WM_THEMECHANGED*/:
      MsgThemeChanged(hwnd,wParam,lParam);
      break;


    // update Scintilla colors
    case WM_SYSCOLORCHANGE:
      {
        extern PEDITLEXER pLexCurrent;
        Style_SetLexer(hwndEdit,pLexCurrent);
        return DefWindowProc(hwnd,umsg,wParam,lParam);
      }


    //case WM_TIMER:
    //  break;


    case WM_SIZE:
      MsgSize(hwnd,wParam,lParam);
      break;


    case WM_SETFOCUS:
      SetFocus(hwndEdit);

      //if (bPendingChangeNotify)
      //  PostMessage(hwnd,WM_CHANGENOTIFY,0,0);
      break;


    case WM_DROPFILES:
      {
        char szBuf[MAX_PATH+40];
        HDROP hDrop = (HDROP)wParam;

        // Reset Change Notify
        //bPendingChangeNotify = FALSE;

        if (IsIconic(hwnd))
          ShowWindow(hwnd,SW_RESTORE);

        //SetForegroundWindow(hwnd);

        DragQueryFile(hDrop,0,szBuf,COUNTOF(szBuf));
        FileLoad(FALSE,FALSE,FALSE,FALSE,szBuf);

        if (DragQueryFile(hDrop,(UINT)(-1),NULL,0) > 1)
          MsgBox(MBINFO,IDS_ERR_DROP);

        DragFinish(hDrop);
      }
      break;


    case WM_COPYDATA:
      {
        PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lParam;

        // Reset Change Notify
        //bPendingChangeNotify = FALSE;

        if (pcds->dwData == DATA_NOTEPAD2_FILEARG)
        {
          LPSTR lpsz = LocalAlloc(LPTR,pcds->cbData);
          CopyMemory(lpsz,pcds->lpData,pcds->cbData);
          FileLoad(FALSE,FALSE,FALSE,FALSE,lpsz);
          LocalFree(lpsz);
        }
      }
      return TRUE;


    case WM_CONTEXTMENU:
    {
      HMENU hmenu;
      int imenu = 0;
      POINT pt;
      int nID = GetDlgCtrlID((HWND)wParam);

      if ((nID != IDC_EDIT) && (nID != IDC_STATUSBAR) &&
          (nID != IDC_REBAR) && (nID != IDC_TOOLBAR))
        return DefWindowProc(hwnd,umsg,wParam,lParam);

      hmenu = LoadMenu(g_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU));
      //SetMenuDefaultItem(GetSubMenu(hmenu,1),0,FALSE);

      pt.x = (int)(short)LOWORD(lParam);
      pt.y = (int)(short)HIWORD(lParam);

      switch(nID)
      {
        case IDC_EDIT:
          {
            int iSelStart = SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
            int iSelEnd   = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0);

            if (iSelStart == iSelEnd && pt.x != -1 && pt.y != -1)
            {
              int iNewPos;
              POINT ptc = { pt.x, pt.y };
              ScreenToClient(hwndEdit,&ptc);
              iNewPos = SendMessage(hwndEdit,SCI_POSITIONFROMPOINT,(WPARAM)ptc.x,(LPARAM)ptc.y);
              SendMessage(hwndEdit,SCI_GOTOPOS,(WPARAM)iNewPos,0);
            }

            if (pt.x == -1 && pt.y == -1)
            {
              int iCurrentPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
              pt.x = SendMessage(hwndEdit,SCI_POINTXFROMPOSITION,0,(LPARAM)iCurrentPos);
              pt.y = SendMessage(hwndEdit,SCI_POINTYFROMPOSITION,0,(LPARAM)iCurrentPos);
              ClientToScreen(hwndEdit,&pt);
            }
            imenu = 0;
          }
          break;

        case IDC_TOOLBAR:
        case IDC_STATUSBAR:
        case IDC_REBAR:
          if (pt.x == -1 && pt.y == -1)
            GetCursorPos(&pt);
          imenu = 1;
          break;
      }

      TrackPopupMenuEx(GetSubMenu(hmenu,imenu),
        TPM_LEFTBUTTON | TPM_RIGHTBUTTON,pt.x+1,pt.y+1,hwnd,NULL);

      DestroyMenu(hmenu);
    }
    break;


    case WM_INITMENU:
      MsgInitMenu(hwnd,wParam,lParam);
      break;


    case WM_NOTIFY:
      return MsgNotify(hwnd,wParam,lParam);


    case WM_COMMAND:
      return MsgCommand(hwnd,wParam,lParam);


    case WM_SYSCOMMAND:
      switch (wParam)
      {
        case SC_MINIMIZE:
          ShowOwnedPopups(hwnd,FALSE);
          if (bMinimizeToTray) {
            MinimizeWndToTray(hwnd);
            ShowNotifyIcon(hwnd,TRUE);
            SetNotifyIconTitle(hwnd);
            return(0);
          }
          else
            return DefWindowProc(hwnd,umsg,wParam,lParam);

        case SC_RESTORE: {
          LRESULT lrv = DefWindowProc(hwnd,umsg,wParam,lParam);
          ShowOwnedPopups(hwnd,TRUE);
          return(lrv);
        }
      }
      return DefWindowProc(hwnd,umsg,wParam,lParam);


      case WM_CHANGENOTIFY:
          SetForegroundWindow(hwnd);
          if (PathFileExists(szCurFile)) {

            if (MsgBox(MBYESNO,IDS_FILECHANGENOTIFY) == IDYES)
              FileLoad(TRUE,FALSE,TRUE,FALSE,szCurFile);
          }
          else {

            if (MsgBox(MBYESNO,IDS_FILECHANGENOTIFY2) == IDYES)
              FileSave(TRUE,FALSE,FALSE,FALSE);
          }

          if (!bRunningWatch)
            InstallFileWatching(szCurFile);
        break;


    //// This message is posted before Notepad2 reactivates itself
    //case WM_CHANGENOTIFYCLEAR:
    //  bPendingChangeNotify = FALSE;
    //  break;


    case WM_DRAWCLIPBOARD:
      if (!bLastCopyFromMe) {
        if (SendMessage(hwndEdit,SCI_CANPASTE,0,0)) {
          BOOL bAutoIndent2 = bAutoIndent;
          bAutoIndent = 0;
          EditJumpTo(hwndEdit,-1,0);
          SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
          if (SendMessage(hwndEdit,SCI_GETLENGTH,0,0) > 0)
            SendMessage(hwndEdit,SCI_NEWLINE,0,0);
          SendMessage(hwndEdit,SCI_PASTE,0,0);
          SendMessage(hwndEdit,SCI_NEWLINE,0,0);
          SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
          bAutoIndent = bAutoIndent2;
        }
      }
      else
        bLastCopyFromMe = FALSE;
      if (hwndNextCBChain)
        SendMessage(hwndNextCBChain,WM_DRAWCLIPBOARD,wParam,lParam);
      break;


    case WM_CHANGECBCHAIN:
      if ((HWND)wParam == hwndNextCBChain)
        hwndNextCBChain = (HWND)lParam;
      if (hwndNextCBChain)
        SendMessage(hwndNextCBChain,WM_CHANGECBCHAIN,lParam,wParam);
      break;


    case WM_TRAYMESSAGE:
      switch(lParam)
      {
        case WM_RBUTTONUP: {

          HMENU hMenu = LoadMenu(g_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU));
          HMENU hMenuPopup = GetSubMenu(hMenu,2);

          POINT pt;
          int iCmd;

          SetForegroundWindow(hwnd);

          GetCursorPos(&pt);
          SetMenuDefaultItem(hMenuPopup,IDM_TRAY_RESTORE,FALSE);
          iCmd = TrackPopupMenu(hMenuPopup,
                   TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
                   pt.x,pt.y,0,hwnd,NULL);

          PostMessage(hwnd,WM_NULL,0,0);

          DestroyMenu(hMenu);

          if (iCmd == IDM_TRAY_RESTORE) {
            RestoreWndFromTray(hwnd);
            ShowOwnedPopups(hwnd,TRUE);
            ShowNotifyIcon(hwnd,FALSE);
            bHideIcon = FALSE;
          }

          else if (iCmd == IDM_TRAY_EXIT) {
              //ShowNotifyIcon(hwnd,FALSE);
              SendMessage(hwnd,WM_CLOSE,0,0);
            }
          }

          return TRUE;

        case WM_LBUTTONDBLCLK:
          RestoreWndFromTray(hwnd);
          ShowOwnedPopups(hwnd,TRUE);
          bHideIcon = TRUE;
          return TRUE;

        case WM_LBUTTONUP:
          if(bHideIcon) {
            ShowNotifyIcon(hwnd,FALSE);
            bHideIcon = FALSE;
          }
            return TRUE;
      }
      break;


    default:

      if (umsg == msgTaskbarCreated) {
        if (!IsWindowVisible(hwnd))
          ShowNotifyIcon(hwnd,TRUE);
          SetNotifyIconTitle(hwnd);
        return(0);
      }

      return DefWindowProc(hwnd,umsg,wParam,lParam);

  }

  return(0);

}


//=============================================================================
//
//  MsgCreate() - Handles WM_CREATE
//
//
LRESULT MsgCreate(HWND hwnd,WPARAM wParam,LPARAM lParam)
{

  HINSTANCE hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

  // Setup edit control
  hwndEdit = EditCreate(hwnd);

  // Word wrap
  SendMessage(hwndEdit,SCI_SETWRAPMODE,(fWordWrap)?SC_WRAP_WORD:SC_WRAP_NONE,0);
  SendMessage(hwndEdit,SCI_SETWRAPSTARTINDENT,iWordWrapIndent,0);
  if (bShowWordWrapSymbols) {
    SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGSLOCATION,WrapSymbols[iWordWrapSymbols].location,0);
    SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGS,WrapSymbols[iWordWrapSymbols].flags,0);
  }
  else {
    SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGS,0,0);
  }

  // Indent Guides
  Style_SetIndentGuides(hwndEdit,bShowIndentGuides);

  // Tabs
  SendMessage(hwndEdit,SCI_SETUSETABS,!bTabsAsSpaces,0);
  SendMessage(hwndEdit,SCI_SETTABWIDTH,iTabWidth,0);
  SendMessage(hwndEdit,SCI_SETINDENT,iIndentWidth,0);

  // Long Lines
  if (bMarkLongLines)
    SendMessage(hwndEdit,SCI_SETEDGEMODE,(iLongLineMode == EDGE_LINE)?EDGE_LINE:EDGE_BACKGROUND,0);
  else
    SendMessage(hwndEdit,SCI_SETEDGEMODE,EDGE_NONE,0);
  SendMessage(hwndEdit,SCI_SETEDGECOLUMN,iLongLinesLimit,0);

  // Margins
  SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,2,0);
  SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,1,(bShowSelectionMargin)?16:0);
  UpdateLineNumberWidth();
  //SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,0,
  //  (bShowLineNumbers)?SendMessage(hwndEdit,SCI_TEXTWIDTH,STYLE_LINENUMBER,(LPARAM)"_999999_"):0);

  // Nonprinting characters
  SendMessage(hwndEdit,SCI_SETVIEWWS,(bViewWhiteSpace)?SCWS_VISIBLEALWAYS:SCWS_INVISIBLE,0);
  SendMessage(hwndEdit,SCI_SETVIEWEOL,bViewEOLs,0);

  if (PrivateIsAppThemed()) {
    bIsAppThemed = TRUE;
    SetWindowLong(hwndEdit,GWL_EXSTYLE,GetWindowLong(hwndEdit,GWL_EXSTYLE) & ~WS_EX_CLIENTEDGE);
    SetWindowPos(hwndEdit,NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
  }
  else {
    bIsAppThemed = FALSE;
  }

  // Create Toolbar and Statusbar
  CreateBars(hwnd,hInstance);

  // Window Initialization

  CreateWindow(
    WC_STATIC,
    NULL,
    WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
    0,0,10,10,
    hwnd,
    (HMENU)IDC_FILENAME,
    hInstance,
    NULL);

  SetDlgItemText(hwnd,IDC_FILENAME,szCurFile);

  // Menu
  //SetMenuDefaultItem(GetSubMenu(GetMenu(hwnd),0),0);

  // Drag & Drop
  DragAcceptFiles(hwnd,TRUE);

  // File MRU
  pFileMRU = MRU_Create("Recent Files",MRU_NOCASE,32);
  MRU_Load(pFileMRU);

  mruFind = MRU_Create("Recent Find",(/*IsWindowsNT()*/1) ? MRU_UTF8 : 0,16);
  MRU_Load(mruFind);

  mruReplace = MRU_Create("Recent Replace",(/*IsWindowsNT()*/1) ? MRU_UTF8 : 0,16);
  MRU_Load(mruReplace);

  return(0);

}


//=============================================================================
//
//  CreateBars() - Create Toolbar and Statusbar
//
//
void CreateBars(HWND hwnd,HINSTANCE hInstance)
{
  RECT rc;

  REBARINFO rbi;
  REBARBANDINFO rbBand;

  BITMAP bmp;
  HBITMAP hbmp;
  HIMAGELIST himl;
  char szTmp[MAX_PATH];
  BOOL bExternalBitmap = TRUE;

  DWORD dwToolbarStyle = WS_TOOLBAR;
  DWORD dwStatusbarStyle = WS_CHILD | WS_CLIPSIBLINGS;
  DWORD dwReBarStyle = WS_REBAR;

  BOOL bIsAppThemed = PrivateIsAppThemed();

  int i,n;
  char tchDesc[256];
  char tchIndex[256];

  char *pIniSection = NULL;
  int   cIniSection = 0;

  if (bShowToolbar)
    dwReBarStyle |= WS_VISIBLE;

  hwndToolbar = CreateWindowEx(0,TOOLBARCLASSNAME,NULL,dwToolbarStyle,
                               0,0,0,0,hwnd,(HMENU)IDC_TOOLBAR,hInstance,NULL);

  SendMessage(hwndToolbar,TB_BUTTONSTRUCTSIZE,(WPARAM)sizeof(TBBUTTON),0);

  // Add normal Toolbar Bitmap
  hbmp = NULL;
  if (lstrlen(tchToolbarBitmap))
  {
    if (!SearchPath(NULL,tchToolbarBitmap,NULL,COUNTOF(szTmp),szTmp,NULL))
      lstrcpy(szTmp,tchToolbarBitmap);
    hbmp = LoadImage(NULL,szTmp,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE);
  }
  if (!hbmp) {
    hbmp = LoadImage(hInstance,MAKEINTRESOURCE(IDR_MAINWND),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
    bExternalBitmap = FALSE;
  }
  GetObject(hbmp,sizeof(BITMAP),&bmp);
  himl = ImageList_Create(bmp.bmWidth/NUMTOOLBITMAPS,bmp.bmHeight,ILC_COLOR32|ILC_MASK,0,0);
  ImageList_AddMasked(himl,hbmp,CLR_DEFAULT);
  DeleteObject(hbmp);
  SendMessage(hwndToolbar,TB_SETIMAGELIST,0,(LPARAM)himl);

  // Optionally add hot Toolbar Bitmap
  hbmp = NULL;
  if (lstrlen(tchToolbarBitmapHot))
  {
    if (!SearchPath(NULL,tchToolbarBitmapHot,NULL,COUNTOF(szTmp),szTmp,NULL))
      lstrcpy(szTmp,tchToolbarBitmapHot);
    if (hbmp = LoadImage(NULL,szTmp,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE))
    {
      GetObject(hbmp,sizeof(BITMAP),&bmp);
      himl = ImageList_Create(bmp.bmWidth/NUMTOOLBITMAPS,bmp.bmHeight,ILC_COLOR32|ILC_MASK,0,0);
      ImageList_AddMasked(himl,hbmp,CLR_DEFAULT);
      DeleteObject(hbmp);
      SendMessage(hwndToolbar,TB_SETHOTIMAGELIST,0,(LPARAM)himl);
    }
  }

  // Optionally add disabled Toolbar Bitmap
  hbmp = NULL;
  if (lstrlen(tchToolbarBitmapDisabled))
  {
    if (!SearchPath(NULL,tchToolbarBitmapDisabled,NULL,COUNTOF(szTmp),szTmp,NULL))
      lstrcpy(szTmp,tchToolbarBitmapDisabled);
    if (hbmp = LoadImage(NULL,szTmp,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE))
    {
      GetObject(hbmp,sizeof(BITMAP),&bmp);
      himl = ImageList_Create(bmp.bmWidth/NUMTOOLBITMAPS,bmp.bmHeight,ILC_COLOR32|ILC_MASK,0,0);
      ImageList_AddMasked(himl,hbmp,CLR_DEFAULT);
      DeleteObject(hbmp);
      SendMessage(hwndToolbar,TB_SETDISABLEDIMAGELIST,0,(LPARAM)himl);
    }
  }

  // Load toolbar labels
  pIniSection = LocalAlloc(LPTR,32*1024);
  cIniSection = LocalSize(pIniSection);
  LoadIniSection("Toolbar Labels",pIniSection,cIniSection);
  n = 1;
  for (i = 0; i < COUNTOF(tbbMainWnd); i++) {

    if (tbbMainWnd[i].fsStyle == TBSTYLE_SEP)
      continue;

    wsprintf(tchIndex,"%02i",n++);

    if (IniSectionGetString(pIniSection,tchIndex,"",tchDesc,COUNTOF(tchDesc))) {
      tbbMainWnd[i].iString = SendMessage(hwndToolbar,TB_ADDSTRING,0,(LPARAM)tchDesc);
      tbbMainWnd[i].fsStyle |= BTNS_AUTOSIZE | BTNS_SHOWTEXT;
    }

    else
      tbbMainWnd[i].fsStyle &= ~(BTNS_AUTOSIZE | BTNS_SHOWTEXT);
  }
  LocalFree(pIniSection);

  SendMessage(hwndToolbar,TB_SETEXTENDEDSTYLE,0,
    SendMessage(hwndToolbar,TB_GETEXTENDEDSTYLE,0,0) | TBSTYLE_EX_MIXEDBUTTONS);

  SendMessage(hwndToolbar,TB_ADDBUTTONS,NUMINITIALTOOLS,(LPARAM)tbbMainWnd);
  if (Toolbar_SetButtons(hwndToolbar,IDT_FILE_NEW,tchToolbarButtons,tbbMainWnd,COUNTOF(tbbMainWnd)) == 0)
    SendMessage(hwndToolbar,TB_ADDBUTTONS,NUMINITIALTOOLS,(LPARAM)tbbMainWnd);
  SendMessage(hwndToolbar,TB_GETITEMRECT,0,(LPARAM)&rc);
  //SendMessage(hwndToolbar,TB_SETINDENT,2,0);

  if (bShowStatusbar)
    dwStatusbarStyle |= WS_VISIBLE;

  hwndStatus = CreateStatusWindow(dwStatusbarStyle,NULL,hwnd,IDC_STATUSBAR);

  // Create ReBar and add Toolbar
  hwndReBar = CreateWindowEx(WS_EX_TOOLWINDOW,REBARCLASSNAME,NULL,dwReBarStyle,
                             0,0,0,0,hwnd,(HMENU)IDC_REBAR,hInstance,NULL);

  rbi.cbSize = sizeof(REBARINFO);
  rbi.fMask  = 0;
  rbi.himl   = (HIMAGELIST)NULL;
  SendMessage(hwndReBar,RB_SETBARINFO,0,(LPARAM)&rbi);

  rbBand.cbSize  = sizeof(REBARBANDINFO);
  rbBand.fMask   = /*RBBIM_COLORS | RBBIM_TEXT | RBBIM_BACKGROUND | */
                   RBBIM_STYLE | RBBIM_CHILD | RBBIM_CHILDSIZE /*| RBBIM_SIZE*/;
  rbBand.fStyle  = /*RBBS_CHILDEDGE |*//* RBBS_BREAK |*/ RBBS_FIXEDSIZE /*| RBBS_GRIPPERALWAYS*/;
  if (bIsAppThemed)
    rbBand.fStyle |= RBBS_CHILDEDGE;
  rbBand.hbmBack = NULL;
  rbBand.lpText     = "Toolbar";
  rbBand.hwndChild  = hwndToolbar;
  rbBand.cxMinChild = (rc.right - rc.left) * COUNTOF(tbbMainWnd);
  rbBand.cyMinChild = (rc.bottom - rc.top) + 2 * rc.top;
  rbBand.cx         = 0;
  SendMessage(hwndReBar,RB_INSERTBAND,(WPARAM)-1,(LPARAM)&rbBand);

  SetWindowPos(hwndReBar,NULL,0,0,0,0,SWP_NOZORDER);
  GetWindowRect(hwndReBar,&rc);
  cyReBar = rc.bottom - rc.top;

  cyReBarFrame = bIsAppThemed ? 0 : 2;
}


//=============================================================================
//
//  MsgThemeChanged() - Handle WM_THEMECHANGED
//
//
void MsgThemeChanged(HWND hwnd,WPARAM wParam,LPARAM lParam)
{
  RECT rc;
  HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE);

  // reinitialize edit frame

  if (PrivateIsAppThemed()) {
    bIsAppThemed = TRUE;
    SetWindowLong(hwndEdit,GWL_EXSTYLE,GetWindowLong(hwndEdit,GWL_EXSTYLE) & ~WS_EX_CLIENTEDGE);
    SetWindowPos(hwndEdit,NULL,0,0,0,0,SWP_NOZORDER|SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE);
  }

  else {
    bIsAppThemed = FALSE;
    SetWindowLong(hwndEdit,GWL_EXSTYLE,WS_EX_CLIENTEDGE|GetWindowLong(hwndEdit,GWL_EXSTYLE));
    SetWindowPos(hwndEdit,NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
  }

  // recreate toolbar and statusbar
  Toolbar_GetButtons(hwndToolbar,IDT_FILE_NEW,tchToolbarButtons,COUNTOF(tchToolbarButtons));

  DestroyWindow(hwndToolbar);
  DestroyWindow(hwndReBar);
  DestroyWindow(hwndStatus);
  CreateBars(hwnd,hInstance);
  UpdateToolbar();

  GetClientRect(hwnd,&rc);
  SendMessage(hwnd,WM_SIZE,SIZE_RESTORED,MAKELONG(rc.right,rc.bottom));
  UpdateStatusbar();
}


//=============================================================================
//
//  MsgSize() - Handles WM_SIZE
//
//
void MsgSize(HWND hwnd,WPARAM wParam,LPARAM lParam)
{

  RECT rc;
  int x,y,cx,cy;

  // Statusbar
  int aWidth[6];

  if (wParam == SIZE_MINIMIZED)
    return;

  x = 0;
  y = 0;

  cx = LOWORD(lParam);
  cy = HIWORD(lParam);

  if (bShowToolbar)
  {
/*  SendMessage(hwndToolbar,WM_SIZE,0,0);
    GetWindowRect(hwndToolbar,&rc);
    y = (rc.bottom - rc.top);
    cy -= (rc.bottom - rc.top);*/

    //SendMessage(hwndToolbar,TB_GETITEMRECT,0,(LPARAM)&rc);
    SetWindowPos(hwndReBar,NULL,0,0,LOWORD(lParam),cyReBar,SWP_NOZORDER);
    // the ReBar automatically sets the correct height
    // calling SetWindowPos() with the height of one toolbar button
    // causes the control not to temporarily use the whole client area
    // and prevents flickering

    //GetWindowRect(hwndReBar,&rc);
    y = cyReBar + cyReBarFrame;    // define
    cy -= cyReBar + cyReBarFrame;  // border
  }

  if (bShowStatusbar)
  {
    SendMessage(hwndStatus,WM_SIZE,0,0);
    GetWindowRect(hwndStatus,&rc);
    cy -= (rc.bottom - rc.top);
  }

  SetWindowPos(hwndEdit,NULL,x,y,cx,cy,SWP_NOZORDER | SWP_NOACTIVATE);

  // Statusbar width
  aWidth[0] = max(120,min(cx/3,StatusCalcPaneWidth(hwndStatus,"Ln 9'999'999 : 9'999'999   Col 9'999'999 : 999   Sel 9'999'999")));
  aWidth[1] = aWidth[0] + StatusCalcPaneWidth(hwndStatus,"9'999'999 Bytes");
  aWidth[2] = aWidth[1] + StatusCalcPaneWidth(hwndStatus,"Unicode BE BOM");
  aWidth[3] = aWidth[2] + StatusCalcPaneWidth(hwndStatus,"CR+LF");
  aWidth[4] = aWidth[3] + StatusCalcPaneWidth(hwndStatus,"OVR");
  aWidth[5] = -1;

  SendMessage(hwndStatus,SB_SETPARTS,COUNTOF(aWidth),(LPARAM)aWidth);

  //UpdateStatusbar();

}


//=============================================================================
//
//  MsgInitMenu() - Handles WM_INITMENU
//
//
void MsgInitMenu(HWND hwnd,WPARAM wParam,LPARAM lParam)
{

  int i,i2;
  HMENU hmenu = (HMENU)wParam;

  i = lstrlen(szCurFile);
  EnableCmd(hmenu,IDM_FILE_REVERT,i);
  EnableCmd(hmenu,IDM_FILE_LAUNCH,i);
  EnableCmd(hmenu,IDM_FILE_PROPERTIES,i);
  EnableCmd(hmenu,IDM_FILE_CREATELINK,i);
  EnableCmd(hmenu,IDM_FILE_ADDTOFAV,i);

  EnableCmd(hmenu,IDM_FILE_READONLY,i);
  CheckCmd(hmenu,IDM_FILE_READONLY,bReadOnly);

  //EnableCmd(hmenu,IDM_ENCODING_UNICODEREV,!bReadOnly);
  //EnableCmd(hmenu,IDM_ENCODING_UNICODE,!bReadOnly);
  //EnableCmd(hmenu,IDM_ENCODING_UTF8SIGN,!bReadOnly);
  //EnableCmd(hmenu,IDM_ENCODING_UTF8,!bReadOnly);
  //EnableCmd(hmenu,IDM_ENCODING_ANSI,!bReadOnly);
  //EnableCmd(hmenu,IDM_LINEENDINGS_CRLF,!bReadOnly);
  //EnableCmd(hmenu,IDM_LINEENDINGS_LF,!bReadOnly);
  //EnableCmd(hmenu,IDM_LINEENDINGS_CR,!bReadOnly);

  if (iCodePage & NCP_UNICODE_REVERSE)
    i = IDM_ENCODING_UNICODEREV;
  else if (iCodePage & NCP_UNICODE)
    i = IDM_ENCODING_UNICODE;
  else if (iCodePage & NCP_UTF8_SIGN)
    i = IDM_ENCODING_UTF8SIGN;
  else if (iCodePage & NCP_UTF8)
    i = IDM_ENCODING_UTF8;
  else
    i = IDM_ENCODING_ANSI;
  CheckMenuRadioItem(hmenu,IDM_ENCODING_ANSI,IDM_ENCODING_UTF8SIGN,i,MF_BYCOMMAND);

  if (iEOLMode == SC_EOL_CRLF)
    i = IDM_LINEENDINGS_CRLF;
  else if (iEOLMode == SC_EOL_LF)
    i = IDM_LINEENDINGS_LF;
  else
    i = IDM_LINEENDINGS_CR;
  CheckMenuRadioItem(hmenu,IDM_LINEENDINGS_CRLF,IDM_LINEENDINGS_CR,i,MF_BYCOMMAND);

  EnableCmd(hmenu,IDM_FILE_RECENT,(MRU_Enum(pFileMRU,0,NULL,0) > 0));

  EnableCmd(hmenu,IDM_EDIT_UNDO,SendMessage(hwndEdit,SCI_CANUNDO,0,0) /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_REDO,SendMessage(hwndEdit,SCI_CANREDO,0,0) /*&& !bReadOnly*/);

  i  = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) - SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
  i2 = SendMessage(hwndEdit,SCI_CANPASTE,0,0);

  EnableCmd(hmenu,IDM_EDIT_CUT,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_COPY,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_COPYALL,SendMessage(hwndEdit,SCI_GETLENGTH,0,0) /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_PASTE,i2 /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_SWAP,i || i2 /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_CLEAR,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_CLEARALL,SendMessage(hwndEdit,SCI_GETLENGTH,0,0) /*&& !bReadOnly*/);

  OpenClipboard(hwnd);
  EnableCmd(hmenu,IDM_EDIT_CLEARCLIPBOARD,CountClipboardFormats());
  CloseClipboard();

  //EnableCmd(hmenu,IDM_EDIT_MOVELINEUP,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_MOVELINEDOWN,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_DUPLICATELINE,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_CUTLINE,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_COPYLINE,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_DELETELINE,!bReadOnly);

  //EnableCmd(hmenu,IDM_EDIT_INDENT,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_UNINDENT,!bReadOnly);

  //EnableCmd(hmenu,IDM_EDIT_STRIP1STCHAR,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_TRIMLINES,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_REMOVEBLANKLINES,!bReadOnly);

  EnableCmd(hmenu,IDM_EDIT_COLUMNWRAP,i /*&& IsWindowsNT()*/);
  EnableCmd(hmenu,IDM_EDIT_SPLITLINES,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_JOINLINES,i /*&& !bReadOnly*/);

  EnableCmd(hmenu,IDM_EDIT_CONVERTUPPERCASE,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_CONVERTLOWERCASE,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_INVERTCASE,i /*&& !bReadOnly*/ /*&& IsWindowsNT()*/);
  EnableCmd(hmenu,IDM_EDIT_TITLECASE,i /*&& !bReadOnly*/ /*&& IsWindowsNT()*/);
  EnableCmd(hmenu,IDM_EDIT_SENTENCECASE,i /*&& !bReadOnly*/ /*&& IsWindowsNT()*/);

  EnableCmd(hmenu,IDM_EDIT_CONVERTTABS,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_CONVERTSPACES,i /*&& !bReadOnly*/);

  i2 = SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0);
  EnableCmd(hmenu,IDM_EDIT_CONVERTOEM,i && (i2 == iDefaultCodePage || i2 == 0) /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_CONVERTANSI,i && (i2 == iDefaultCodePage || i2 == 0) /*&& !bReadOnly*/);

  i = SendMessage(hwndEdit,SCI_GETLEXER,0,0);
  EnableCmd(hmenu,IDM_EDIT_LINECOMMENT,
    !(i == SCLEX_NULL || i == SCLEX_CSS || i == SCLEX_DIFF));
  EnableCmd(hmenu,IDM_EDIT_STREAMCOMMENT,
    !(i == SCLEX_NULL || i == SCLEX_VBSCRIPT || i == SCLEX_MAKEFILE || i == SCLEX_VB || i == SCLEX_ASM ||
      i == SCLEX_SQL || i == SCLEX_PERL || i == SCLEX_PYTHON || i == SCLEX_PROPERTIES ||i == SCLEX_CONF ||
      i == SCLEX_BATCH || i == SCLEX_DIFF));

  //EnableCmd(hmenu,IDM_EDIT_INSERT_SHORTDATE,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_INSERT_LONGDATE,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_INSERT_FILENAME,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_INSERT_PATHNAME,!bReadOnly);

  i = SendMessage(hwndEdit,SCI_GETLENGTH,0,0);
  EnableCmd(hmenu,IDM_EDIT_FIND,i);
  EnableCmd(hmenu,IDM_EDIT_SAVEFIND,i);
  EnableCmd(hmenu,IDM_EDIT_FINDNEXT,i);
  EnableCmd(hmenu,IDM_EDIT_FINDPREV,i && lstrlen(efrData.szFind));
  EnableCmd(hmenu,IDM_EDIT_SELTONEXT,i);
  EnableCmd(hmenu,IDM_EDIT_SELTOPREV,i && lstrlen(efrData.szFind));
  EnableCmd(hmenu,IDM_EDIT_REPLACE,i /*&& !bReadOnly*/);

  CheckCmd(hmenu,IDM_VIEW_USE2NDDEFAULT,Style_GetUse2ndDefault(hwndEdit));

  CheckCmd(hmenu,IDM_VIEW_WORDWRAP,fWordWrap);
  CheckCmd(hmenu,IDM_VIEW_LONGLINEMARKER,bMarkLongLines);
  CheckCmd(hmenu,IDM_VIEW_TABSASSPACES,bTabsAsSpaces);
  CheckCmd(hmenu,IDM_VIEW_SHOWINDENTGUIDES,bShowIndentGuides);
  CheckCmd(hmenu,IDM_VIEW_AUTOINDENTTEXT,bAutoIndent);
  CheckCmd(hmenu,IDM_VIEW_LINENUMBERS,bShowLineNumbers);
  CheckCmd(hmenu,IDM_VIEW_MARGIN,bShowSelectionMargin);
  CheckCmd(hmenu,IDM_VIEW_SHOWWHITESPACE,bViewWhiteSpace);
  CheckCmd(hmenu,IDM_VIEW_SHOWEOLS,bViewEOLs);
  CheckCmd(hmenu,IDM_VIEW_WORDWRAPSYMBOLS,bShowWordWrapSymbols);
  CheckCmd(hmenu,IDM_VIEW_MATCHBRACES,bMatchBraces);
  CheckCmd(hmenu,IDM_VIEW_TOOLBAR,bShowToolbar);
  EnableCmd(hmenu,IDM_VIEW_CUSTOMIZETB,bShowToolbar);
  CheckCmd(hmenu,IDM_VIEW_STATUSBAR,bShowStatusbar);

  i = SendMessage(hwndEdit,SCI_GETLEXER,0,0);
  //EnableCmd(hmenu,IDM_VIEW_AUTOCLOSETAGS,(i == SCLEX_HTML || i == SCLEX_XML));
  CheckCmd(hmenu,IDM_VIEW_AUTOCLOSETAGS,bAutoCloseTags /*&& (i == SCLEX_HTML || i == SCLEX_XML)*/);
  CheckCmd(hmenu,IDM_VIEW_HILITECURRENTLINE,bHiliteCurrentLine);

  i = IniGetInt("Settings2","ReuseWindow",0);
  CheckCmd(hmenu,IDM_VIEW_REUSEWINDOW,i);
  i = IniGetInt("Settings2","SingleFileInstance",0);
  CheckCmd(hmenu,IDM_VIEW_SINGLEFILEINSTANCE,i);
  bStickyWinPos = IniGetInt("Settings2","StickyWindowPosition",0);
  CheckCmd(hmenu,IDM_VIEW_STICKYWINPOS,bStickyWinPos);
  CheckCmd(hmenu,IDM_VIEW_ALWAYSONTOP,bAlwaysOnTop);
  CheckCmd(hmenu,IDM_VIEW_MINTOTRAY,bMinimizeToTray);
  CheckCmd(hmenu,IDM_VIEW_TRANSPARENT,bTransparentMode && bTransparentModeAvailable);
  EnableCmd(hmenu,IDM_VIEW_TRANSPARENT,bTransparentModeAvailable);

  CheckCmd(hmenu,IDM_VIEW_NOSAVERECENT,bSaveRecentFiles);
  CheckCmd(hmenu,IDM_VIEW_NOSAVEFINDREPL,bSaveFindReplace);
  CheckCmd(hmenu,IDM_VIEW_SAVEBEFORERUNNINGTOOLS,bSaveBeforeRunningTools);
  CheckCmd(hmenu,IDM_VIEW_FILECHANGENOTIFY,bEnableFileWatching);

  if (iPathNameFormat == 0)
    i = IDM_VIEW_SHOWFILENAMEONLY;
  else if (iPathNameFormat == 1)
    i = IDM_VIEW_SHOWFILENAMEFIRST;
  else
    i = IDM_VIEW_SHOWFULLPATH;
  CheckMenuRadioItem(hmenu,IDM_VIEW_SHOWFILENAMEONLY,IDM_VIEW_SHOWFULLPATH,i,MF_BYCOMMAND);

  if (iEscFunction == 1)
    i = IDM_VIEW_ESCMINIMIZE;
  else if (iEscFunction == 2)
    i = IDM_VIEW_ESCEXIT;
  else
    i = IDM_VIEW_NOESNFUNC;
  CheckMenuRadioItem(hmenu,IDM_VIEW_ESCMINIMIZE,IDM_VIEW_NOESNFUNC,i,MF_BYCOMMAND);

  i = lstrlen(szIniFile);
  CheckCmd(hmenu,IDM_VIEW_SAVESETTINGS,bSaveSettings && i);

  EnableCmd(hmenu,IDM_VIEW_REUSEWINDOW,i);
  EnableCmd(hmenu,IDM_VIEW_STICKYWINPOS,i);
  EnableCmd(hmenu,IDM_VIEW_SINGLEFILEINSTANCE,i);
  EnableCmd(hmenu,IDM_VIEW_NOSAVERECENT,i);
  EnableCmd(hmenu,IDM_VIEW_NOSAVEFINDREPL,i);
  EnableCmd(hmenu,IDM_VIEW_SAVESETTINGS,i);
  EnableCmd(hmenu,IDM_VIEW_SAVESETTINGSNOW,i);

}


//=============================================================================
//
//  MsgCommand() - Handles WM_COMMAND
//
//
LRESULT MsgCommand(HWND hwnd,WPARAM wParam,LPARAM lParam)
{

  switch(LOWORD(wParam))
  {

    case IDM_FILE_NEW:
      FileLoad(FALSE,TRUE,FALSE,FALSE,"");
      break;


    case IDM_FILE_OPEN:
      FileLoad(FALSE,FALSE,FALSE,FALSE,"");
      break;


    case IDM_FILE_REVERT:
      {
        if (lstrlen(szCurFile)) {

          char tchCurFile2[MAX_PATH];

          int iCurPos     = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
          int iAnchorPos  = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
          int iCurTopLine = SendMessage(hwndEdit,SCI_GETFIRSTVISIBLELINE,0,0);
          int iXOffset    = SendMessage(hwndEdit,SCI_GETXOFFSET,0,0);

          if ((bModified || iCodePage != iInitialCP) && MsgBox(MBOKCANCEL,IDS_ASK_REVERT) != IDOK)
            return(0);

          lstrcpy(tchCurFile2,szCurFile);

          if (FileLoad(TRUE,FALSE,TRUE,FALSE,tchCurFile2))
          {
            if (SendMessage(hwndEdit,SCI_GETLENGTH,0,0) >= 4) {
              char tch[5] = "";
              SendMessage(hwndEdit,SCI_GETTEXT,5,(LPARAM)tch);
              if (lstrcmp(tch,".LOG") != 0) {
                int iTopLineAfterLoad;
                SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
                iTopLineAfterLoad = SendMessage(hwndEdit,SCI_GETFIRSTVISIBLELINE,0,0);
                SendMessage(hwndEdit,SCI_LINESCROLL,0,(LPARAM)(iCurTopLine - iTopLineAfterLoad));
                SendMessage(hwndEdit,SCI_SETXOFFSET,(WPARAM)iXOffset,0);
              }
            }
          }
        }
      }
      break;


    case IDM_FILE_SAVE:
      FileSave(TRUE,FALSE,FALSE,FALSE);
      break;


    case IDM_FILE_SAVEAS:
      FileSave(TRUE,FALSE,TRUE,FALSE);
      break;


    case IDM_FILE_SAVECOPY:
      FileSave(TRUE,FALSE,TRUE,TRUE);
      break;


    case IDM_FILE_READONLY:
      //bReadOnly = (bReadOnly) ? FALSE : TRUE;
      //SendMessage(hwndEdit,SCI_SETREADONLY,bReadOnly,0);
      //UpdateToolbar();
      //UpdateStatusbar();
      if (lstrlen(szCurFile))
      {
        BOOL bSuccess = FALSE;
        DWORD dwFileAttributes = GetFileAttributes(szCurFile);
        if (dwFileAttributes != INVALID_FILE_ATTRIBUTES) {
          if (bReadOnly)
            dwFileAttributes = (dwFileAttributes & ~FILE_ATTRIBUTE_READONLY);
          else
            dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
          if (!SetFileAttributes(szCurFile,dwFileAttributes))
            MsgBox(MBWARN,IDS_READONLY_MODIFY,szCurFile);
        }
        else
          MsgBox(MBWARN,IDS_READONLY_MODIFY,szCurFile);

        dwFileAttributes = GetFileAttributes(szCurFile);
        if (dwFileAttributes != INVALID_FILE_ATTRIBUTES)
          bReadOnly = (dwFileAttributes & FILE_ATTRIBUTE_READONLY);

        SetWindowTitle(hwnd,uidsAppTitle,IDS_UNTITLED,szCurFile,
          iPathNameFormat,bModified || iCodePage != iInitialCP,
          IDS_READONLY,bReadOnly);
      }
      break;


    case IDM_FILE_BROWSE:
      {
        SHELLEXECUTEINFO sei;
        char tchParam[MAX_PATH+4];
        char tchExeFile[MAX_PATH+4];

        lstrcpy(tchParam,szCurFile);
        PathQuoteSpaces(tchParam);

        if (!IniGetString("Settings2","filebrowser.exe","",tchExeFile,COUNTOF(tchExeFile))) {
          if (!SearchPath(NULL,"metapath.exe",NULL,COUNTOF(tchExeFile),tchExeFile,NULL)) {
            lstrcpy(tchExeFile,"metapath.exe");
          }
        }

        ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));

        sei.cbSize = sizeof(SHELLEXECUTEINFO);
        sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
        sei.hwnd = hwnd;
        sei.lpVerb = NULL;
        sei.lpFile = tchExeFile;
        sei.lpParameters = tchParam;
        sei.lpDirectory = NULL;
        sei.nShow = SW_SHOWNORMAL;

        ShellExecuteEx(&sei);

        if ((int)sei.hInstApp < 32)
          MsgBox(MBWARN,IDS_ERR_BROWSE);
      }
      break;


    case IDM_FILE_NEWWINDOW:
    case IDM_FILE_NEWWINDOW2:
      {
        char szModuleName[MAX_PATH];
        char szFileName[MAX_PATH];
        char szParameters[2*MAX_PATH+64];

        MONITORINFO mi;
        HMONITOR hMonitor;
        WINDOWPLACEMENT wndpl;
        int x,y,cx,cy,imax;
        char tch[64];

        if (bSaveBeforeRunningTools && !FileSave(FALSE,TRUE,FALSE,FALSE))
          break;

        GetModuleFileName(NULL,szModuleName,COUNTOF(szModuleName));

        lstrcpy(szParameters,"-f");
        if (lstrlen(szIniFile)) {
          lstrcat(szParameters," \"");
          lstrcat(szParameters,szIniFile);
          lstrcat(szParameters,"\"");
        }
        else
          lstrcat(szParameters,"0");

        lstrcat(szParameters," -n");

        wndpl.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(hwnd,&wndpl);

        hMonitor = MonitorFromRect(&wndpl.rcNormalPosition,MONITOR_DEFAULTTONEAREST);
        mi.cbSize = sizeof(mi);
        GetMonitorInfo(hMonitor,&mi);

        // offset new window position +10/+10
        x = wndpl.rcNormalPosition.left + 10;
        y = wndpl.rcNormalPosition.top  + 10;
        cx = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
        cy = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;

        // check if window fits monitor
        if ((x + cx) > mi.rcWork.right || (y + cy) > mi.rcWork.bottom) {
          x = mi.rcMonitor.left;
          y = mi.rcMonitor.top;
        }

        imax = IsZoomed(hwnd);

        wsprintf(tch," -p %i,%i,%i,%i,%i",x,y,cx,cy,imax);
        lstrcat(szParameters,tch);

        if (LOWORD(wParam) != IDM_FILE_NEWWINDOW2 && lstrlen(szCurFile)) {
          lstrcpy(szFileName,szCurFile);
          PathQuoteSpaces(szFileName);
          lstrcat(szParameters," ");
          lstrcat(szParameters,szFileName);
        }

        ShellExecute(hwnd,NULL,szModuleName,szParameters,NULL,SW_SHOWNORMAL);
      }
      break;


    case IDM_FILE_LAUNCH:
      {
        SHELLEXECUTEINFO sei;

        if (!lstrlen(szCurFile))
          break;

        if (bSaveBeforeRunningTools && !FileSave(FALSE,TRUE,FALSE,FALSE))
          break;

        ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));

        sei.cbSize = sizeof(SHELLEXECUTEINFO);
        sei.fMask = SEE_MASK_NOCLOSEPROCESS;
        sei.hwnd = hwnd;
        sei.lpVerb = NULL;
        sei.lpFile = szCurFile;
        sei.lpParameters = NULL;
        sei.lpDirectory = NULL;
        sei.nShow = SW_SHOWNORMAL;

        ShellExecuteEx(&sei);
      }
      break;


    case IDM_FILE_RUN:
      {
        char tchCmdLine[MAX_PATH+4];

        if (bSaveBeforeRunningTools && !FileSave(FALSE,TRUE,FALSE,FALSE))
          break;

        lstrcpy(tchCmdLine,szCurFile);
        PathQuoteSpaces(tchCmdLine);

        RunDlg(hwnd,tchCmdLine);
      }
      break;


    case IDM_FILE_OPENWITH:
      if (bSaveBeforeRunningTools && !FileSave(FALSE,TRUE,FALSE,FALSE))
        break;
      OpenWithDlg(hwnd,szCurFile);
      break;


    case IDM_FILE_PAGESETUP:
      EditPrintSetup(hwndEdit);
      break;

    case IDM_FILE_PRINT:
      {
        SHFILEINFO shfi;
        char *pszTitle;
        char tchUntitled[32];
        char tchPageFmt[32];

        if (lstrlen(szCurFile)) {
          SHGetFileInfo2(szCurFile,0,&shfi,sizeof(SHFILEINFO),SHGFI_DISPLAYNAME);
          pszTitle = shfi.szDisplayName;
        }
        else {
          GetString(IDS_UNTITLED,tchUntitled,COUNTOF(tchUntitled));
          pszTitle = tchUntitled;
        }

        GetString(IDS_PRINT_PAGENUM,tchPageFmt,COUNTOF(tchPageFmt));

        if (!EditPrint(hwndEdit,pszTitle,tchPageFmt))
          MsgBox(MBWARN,IDS_PRINT_ERROR,pszTitle);
      }
      break;


    case IDM_FILE_PROPERTIES:
      {
        SHELLEXECUTEINFO sei;

        if (lstrlen(szCurFile) == 0)
          break;

        ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));

        sei.cbSize = sizeof(SHELLEXECUTEINFO);
        sei.fMask = SEE_MASK_INVOKEIDLIST | SEE_MASK_NOCLOSEPROCESS;
        sei.hwnd = hwnd;
        sei.lpVerb = "properties";
        sei.lpFile = szCurFile;
        sei.nShow = SW_SHOWNORMAL;

        ShellExecuteEx(&sei);
      }
      break;

    case IDM_FILE_CREATELINK:
      {
        if (!lstrlen(szCurFile))
          break;

        if (!PathCreateDeskLnk(szCurFile))
          MsgBox(MBWARN,IDS_ERR_CREATELINK);
      }
      break;


    case IDM_FILE_OPENFAV:
      if (FileSave(FALSE,TRUE,FALSE,FALSE)) {

        char tchSelItem[MAX_PATH];

        if (FavoritesDlg(hwnd,tchSelItem))
        {
          if (PathIsLnkToDirectory(tchSelItem,NULL,0))
            PathGetLnkPath(tchSelItem,tchSelItem,COUNTOF(tchSelItem));

          if (PathIsDirectory(tchSelItem))
          {
            char tchFile[MAX_PATH];

            if (OpenFileDlg(hwndMain,tchFile,COUNTOF(tchFile),tchSelItem))
              FileLoad(TRUE,FALSE,FALSE,FALSE,tchFile);
          }
          else
            FileLoad(TRUE,FALSE,FALSE,FALSE,tchSelItem);
          }
        }
      break;


    case IDM_FILE_ADDTOFAV:
      if (lstrlen(szCurFile)) {
        SHFILEINFO shfi;
        SHGetFileInfo2(szCurFile,0,&shfi,sizeof(SHFILEINFO),SHGFI_DISPLAYNAME);
        AddToFavDlg(hwnd,shfi.szDisplayName,szCurFile);
      }
      break;


    case IDM_FILE_MANAGEFAV:
      {
        SHELLEXECUTEINFO sei;
        ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));

        sei.cbSize = sizeof(SHELLEXECUTEINFO);
        sei.fMask = SEE_MASK_NOCLOSEPROCESS;
        sei.hwnd = hwnd;
        sei.lpVerb = NULL;
        sei.lpFile = tchFavoritesDir;
        sei.lpParameters = NULL;
        sei.lpDirectory = NULL;
        sei.nShow = SW_SHOWNORMAL;

        // Run favorites directory
        ShellExecuteEx(&sei);
      }
      break;


    case IDM_FILE_RECENT:
      if (MRU_Enum(pFileMRU,0,NULL,0) > 0) {
        if (FileSave(FALSE,TRUE,FALSE,FALSE)) {
          char tchFile[MAX_PATH];
          if (FileMRUDlg(hwnd,tchFile))
            FileLoad(TRUE,FALSE,FALSE,FALSE,tchFile);
          }
        }
      break;


    case IDM_FILE_EXIT:
      SendMessage(hwnd,WM_CLOSE,0,0);
      break;


    case IDM_ENCODING_ANSI:
    case IDM_ENCODING_UNICODE:
    case IDM_ENCODING_UNICODEREV:
    case IDM_ENCODING_UTF8:
    case IDM_ENCODING_UTF8SIGN:
      {
        int iNewCodePage = iEncodings[LOWORD(wParam)-IDM_ENCODING_ANSI];
        if (SendMessage(hwndEdit,SCI_GETLENGTH,0,0) == 0) {
          if (iCodePage != 0 && iNewCodePage != 0 || InfoBox(MBYESNO,IDS_ASK_ENCODING2,"MsgConv2") == IDYES) {
            EditConvertText(hwndEdit,
              (iCodePage == NCP_DEFAULT) ? iDefaultCodePage : SC_CP_UTF8,
              (iNewCodePage == NCP_DEFAULT) ? iDefaultCodePage : SC_CP_UTF8,
              (lstrlen(szCurFile) == 0));
            iCodePage = iNewCodePage;
            iInitialCP = iNewCodePage;
            UpdateToolbar();
            UpdateStatusbar();
            SetWindowTitle(hwnd,uidsAppTitle,IDS_UNTITLED,szCurFile,
              iPathNameFormat,bModified || iCodePage != iInitialCP,
              IDS_READONLY,bReadOnly);
          }
        }
        else if (iCodePage != iNewCodePage)
        {
          if (iCodePage != 0 && iNewCodePage != 0 || InfoBox(MBYESNO,IDS_ASK_ENCODING,"MsgConv1") == IDYES) {
            BeginWaitCursor();
            EditConvertText(hwndEdit,
              (iCodePage == NCP_DEFAULT) ? iDefaultCodePage : SC_CP_UTF8,
              (iNewCodePage == NCP_DEFAULT) ? iDefaultCodePage : SC_CP_UTF8,
              FALSE);
            iInitialCP = iCodePage;
            iCodePage = iNewCodePage;
            EndWaitCursor();
          }
          UpdateToolbar();
          UpdateStatusbar();
          SetWindowTitle(hwnd,uidsAppTitle,IDS_UNTITLED,szCurFile,
            iPathNameFormat,bModified || iCodePage != iInitialCP,
            IDS_READONLY,bReadOnly);
        }
      }
      break;


    case IDM_ENCODING_SETDEFAULT:
      SelectEncodingDlg(hwnd,&iDefaultEncoding);
      break;


    case IDM_LINEENDINGS_CRLF:
    case IDM_LINEENDINGS_LF:
    case IDM_LINEENDINGS_CR:
      {
        int iNewEOLMode = iLineEndings[LOWORD(wParam)-IDM_LINEENDINGS_CRLF];

        iEOLMode = iNewEOLMode;
        SendMessage(hwndEdit,SCI_SETEOLMODE,iEOLMode,0);
        SendMessage(hwndEdit,SCI_CONVERTEOLS,iEOLMode,0);
        UpdateToolbar();
        UpdateStatusbar();
        SetWindowTitle(hwnd,uidsAppTitle,IDS_UNTITLED,szCurFile,
          iPathNameFormat,bModified || iCodePage != iInitialCP,
          IDS_READONLY,bReadOnly);
      }
      break;


    case IDM_LINEENDINGS_SETDEFAULT:
      SelectLineEndingDlg(hwnd,&iDefaultEOLMode);
      break;


    case IDM_EDIT_UNDO:
      SendMessage(hwndEdit,SCI_UNDO,0,0);
      break;


    case IDM_EDIT_REDO:
      SendMessage(hwndEdit,SCI_REDO,0,0);
      break;


    case IDM_EDIT_CUT:
      if (flagPasteBoard)
        bLastCopyFromMe = TRUE;
      SendMessage(hwndEdit,SCI_CUT,0,0);
      break;


    case IDM_EDIT_COPY:
      if (flagPasteBoard)
        bLastCopyFromMe = TRUE;
      SendMessage(hwndEdit,SCI_COPY,0,0);
      UpdateToolbar();
      break;


    case IDM_EDIT_COPYALL:
      if (flagPasteBoard)
        bLastCopyFromMe = TRUE;
      SendMessage(hwndEdit,SCI_COPYRANGE,0,SendMessage(hwndEdit,SCI_GETLENGTH,0,0));
      UpdateToolbar();
      break;


    case IDM_EDIT_PASTE:
      SendMessage(hwndEdit,SCI_PASTE,0,0);
      break;


    case IDM_EDIT_SWAP:
      if (SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
          SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0) == 0) {
        int iNewPos = -1;
        int iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        SendMessage(hwndEdit,SCI_PASTE,0,0);
        iNewPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        SendMessage(hwndEdit,SCI_SETSEL,iPos,iNewPos);
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_CLEARCLIPBOARD,1),0);
      }
      else {
        int iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        int iAnchor = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
        LPSTR pClip = EditGetClipboardText(hwndEdit);
        if (flagPasteBoard)
          bLastCopyFromMe = TRUE;
        SendMessage(hwndEdit,SCI_CUT,0,0);
        SendMessage(hwndEdit,SCI_REPLACESEL,(WPARAM)0,(LPARAM)pClip);
        if (iPos > iAnchor)
          SendMessage(hwndEdit,SCI_SETSEL,iAnchor,iAnchor + lstrlen(pClip));
        else
          SendMessage(hwndEdit,SCI_SETSEL,iPos + lstrlen(pClip),iPos);
        LocalFree(pClip);
      }
      break;


    case IDM_EDIT_CLEAR:
      SendMessage(hwndEdit,SCI_CLEAR,0,0);
      break;


    case IDM_EDIT_CLEARALL:
      SendMessage(hwndEdit,SCI_CLEARALL,0,0);
      break;


    case IDM_EDIT_CLEARCLIPBOARD:
      if (OpenClipboard(hwnd)) {
        if (CountClipboardFormats() > 0) {
          EmptyClipboard();
          UpdateToolbar();
          UpdateStatusbar();
        }
        CloseClipboard();
      }
      break;

    case IDM_EDIT_SELECTALL:
      SendMessage(hwndEdit,SCI_SELECTALL,0,0);
      //SendMessage(hwndEdit,SCI_SETSEL,0,(LPARAM)-1);
      break;


    case IDM_EDIT_SELECTWORD:
      {
        int iSel =
          SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
          SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);

        int iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);

        if (iSel == 0) {

          int iWordStart = SendMessage(hwndEdit,SCI_WORDSTARTPOSITION,iPos,TRUE);
          int iWordEnd   = SendMessage(hwndEdit,SCI_WORDENDPOSITION,iPos,TRUE);

          if (iWordStart == iWordEnd) // we are in whitespace salad...
          {
            iWordStart = SendMessage(hwndEdit,SCI_WORDENDPOSITION,iPos,FALSE);
            iWordEnd   = SendMessage(hwndEdit,SCI_WORDENDPOSITION,iWordStart,TRUE);
            if (iWordStart != iWordEnd) {
              //if (SCLEX_HTML == SendMessage(hwndEdit,SCI_GETLEXER,0,0) &&
              //    SCE_HPHP_VARIABLE == SendMessage(hwndEdit,SCI_GETSTYLEAT,(WPARAM)iWordStart,0) &&
              //    '$' == (char)SendMessage(hwndEdit,SCI_GETCHARAT,(WPARAM)iWordStart-1,0))
              //  iWordStart--;
              SendMessage(hwndEdit,SCI_SETSEL,iWordStart,iWordEnd);
            }
          }
          else {
            //if (SCLEX_HTML == SendMessage(hwndEdit,SCI_GETLEXER,0,0) &&
            //    SCE_HPHP_VARIABLE == SendMessage(hwndEdit,SCI_GETSTYLEAT,(WPARAM)iWordStart,0) &&
            //    '$' == (char)SendMessage(hwndEdit,SCI_GETCHARAT,(WPARAM)iWordStart-1,0))
            //  iWordStart--;
            SendMessage(hwndEdit,SCI_SETSEL,iWordStart,iWordEnd);
          }

          iSel =
            SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
            SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);

          if (iSel == 0) {
            int iLine = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,iPos,0);
            int iLineStart = SendMessage(hwndEdit,SCI_GETLINEINDENTPOSITION,iLine,0);
            int iLineEnd   = SendMessage(hwndEdit,SCI_GETLINEENDPOSITION,iLine,0);
            SendMessage(hwndEdit,SCI_SETSEL,iLineStart,iLineEnd);
          }
        }
        else {
          int iLine = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,iPos,0);
          int iLineStart = SendMessage(hwndEdit,SCI_GETLINEINDENTPOSITION,iLine,0);
          int iLineEnd   = SendMessage(hwndEdit,SCI_GETLINEENDPOSITION,iLine,0);
          SendMessage(hwndEdit,SCI_SETSEL,iLineStart,iLineEnd);
        }
      }
      break;


    case IDM_EDIT_MOVELINEUP:
      {
        int iCurPos  = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        int iCurLine = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,(WPARAM)iCurPos,0);
        int iLinePos = iCurPos - SendMessage(hwndEdit,SCI_POSITIONFROMLINE,(WPARAM)iCurLine,0);
        if (iCurLine > 0) {
          SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
          SendMessage(hwndEdit,SCI_LINETRANSPOSE,0,0);
          //SendMessage(hwndEdit,SCI_GOTOLINE,(WPARAM)iCurLine-1,0);
          SendMessage(hwndEdit,SCI_GOTOPOS,(WPARAM)SendMessage(hwndEdit,SCI_POSITIONFROMLINE,(WPARAM)iCurLine-1,0)+iLinePos,0);
          SendMessage(hwndEdit,SCI_CHOOSECARETX,0,0);
          SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
        }
      }
      break;


    case IDM_EDIT_MOVELINEDOWN:
      {
        int iCurPos  = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        int iCurLine = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,(WPARAM)iCurPos,0);
        int iLinePos = iCurPos - SendMessage(hwndEdit,SCI_POSITIONFROMLINE,(WPARAM)iCurLine,0);
        if (iCurLine < SendMessage(hwndEdit,SCI_GETLINECOUNT,0,0) - 1) {
          SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
          SendMessage(hwndEdit,SCI_GOTOLINE,(WPARAM)iCurLine+1,0);
          SendMessage(hwndEdit,SCI_LINETRANSPOSE,0,0);
          SendMessage(hwndEdit,SCI_GOTOPOS,(WPARAM)SendMessage(hwndEdit,SCI_POSITIONFROMLINE,(WPARAM)iCurLine+1,0)+iLinePos,0);
          SendMessage(hwndEdit,SCI_CHOOSECARETX,0,0);
          SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
        }
      }
      break;


    case IDM_EDIT_DUPLICATELINE:
      SendMessage(hwndEdit,SCI_LINEDUPLICATE,0,0);
      break;


    case IDM_EDIT_CUTLINE:
      if (flagPasteBoard)
        bLastCopyFromMe = TRUE;
      SendMessage(hwndEdit,SCI_LINECUT,0,0);
      break;


    case IDM_EDIT_COPYLINE:
      if (flagPasteBoard)
        bLastCopyFromMe = TRUE;
      SendMessage(hwndEdit,SCI_LINECOPY,0,0);
      UpdateToolbar();
      break;


    case IDM_EDIT_DELETELINE:
      SendMessage(hwndEdit,SCI_LINEDELETE,0,0);
      break;


    case IDM_EDIT_DELETELINELEFT:
      SendMessage(hwndEdit,SCI_DELLINELEFT,0,0);
      break;


    case IDM_EDIT_DELETELINERIGHT:
      SendMessage(hwndEdit,SCI_DELLINERIGHT,0,0);
      break;


    case IDM_EDIT_INDENT:
      {
        int iLineSelStart = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,
          SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0),0);
        int iLineSelEnd   = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,
          SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0),0);

      if (iLineSelStart == iLineSelEnd) {
        SendMessage(hwndEdit,SCI_VCHOME,0,0);
        SendMessage(hwndEdit,SCI_TAB,0,0);
        }

      else
        SendMessage(hwndEdit,SCI_TAB,0,0);
      }
      break;


    case IDM_EDIT_UNINDENT:
      {
        int iLineSelStart = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,
          SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0),0);
        int iLineSelEnd   = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,
          SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0),0);

      if (iLineSelStart == iLineSelEnd) {
        SendMessage(hwndEdit,SCI_VCHOME,0,0);
        SendMessage(hwndEdit,SCI_BACKTAB,0,0);
        }

      else
        SendMessage(hwndEdit,SCI_BACKTAB,0,0);
      }
      break;


    case IDM_EDIT_ENCLOSESELECTION:
      if (EditEncloseSelectionDlg(hwnd,wchPrefixSelection,wchAppendSelection)) {
        SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
        EditEncloseSelection(hwndEdit,wchPrefixSelection,wchAppendSelection);
        SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      }
      break;


    case IDM_EDIT_SELECTIONDUPLICATE:
      SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
      SendMessage(hwndEdit,SCI_SELECTIONDUPLICATE,0,0);
      SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
      break;


    case IDM_EDIT_STRIP1STCHAR:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditStripFirstCharacter(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_TRIMLINES:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditStripTrailingBlanks(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_COMPRESSWS:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditCompressSpaces(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_REMOVEBLANKLINES:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditRemoveBlankLines(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_MODIFYLINES:
      if (EditModifyLinesDlg(hwnd,wchPrefixLines,wchAppendLines)) {
        SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
        EditModifyLines(hwndEdit,wchPrefixLines,wchAppendLines);
        SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      }
      break;


    case IDM_EDIT_COLUMNWRAP:
      {
        static int iWrapCol;

        if (iWrapCol == 0)
          iWrapCol = iLongLinesLimit;

        if (ColumnWrapDlg(hwnd,IDD_COLUMNWRAP,&iWrapCol))
        {
          iWrapCol = max(min(iWrapCol,512),1);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditWrapToColumn(hwndEdit,iWrapCol);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
        }
      }
      break;


    case IDM_EDIT_SPLITLINES:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      SendMessage(hwndEdit,SCI_TARGETFROMSELECTION,0,0);
      SendMessage(hwndEdit,SCI_LINESSPLIT,0,0);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_JOINLINES:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      SendMessage(hwndEdit,SCI_TARGETFROMSELECTION,0,0);
      SendMessage(hwndEdit,SCI_LINESJOIN,0,0);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_CONVERTUPPERCASE:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditMakeUppercase(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_CONVERTLOWERCASE:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditMakeLowercase(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_INVERTCASE:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditInvertCase(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_TITLECASE:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditTitleCase(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_SENTENCECASE:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditSentenceCase(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_CONVERTTABS:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditTabsToSpaces(hwndEdit,iTabWidth);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_CONVERTSPACES:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditSpacesToTabs(hwndEdit,iTabWidth);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_CONVERTANSI:
      {
        int iCurPos    = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        int iAnchorPos = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
        if (iCurPos != iAnchorPos)
        {
          if (SC_SEL_RECTANGLE != SendMessage(hwndEdit,SCI_GETSELECTIONMODE,0,0))
          {
            int iSelCount = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
                              SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
            char *pszOEM = GlobalAlloc(GPTR,iSelCount+2);
            char *pszANSI = GlobalAlloc(GPTR,iSelCount+2);
            SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
            SendMessage(hwndEdit,SCI_GETSELTEXT,0,(LPARAM)pszOEM);
            OemToCharBuff(pszOEM,pszANSI,GlobalSize(pszANSI));
            SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
            SendMessage(hwndEdit,SCI_CLEAR,0,0);
            SendMessage(hwndEdit,SCI_ADDTEXT,(WPARAM)iSelCount,(LPARAM)pszANSI);
            SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
            SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
            GlobalFree(pszOEM);
            GlobalFree(pszANSI);
            SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          }
          else
            MsgBox(MBINFO,IDS_SELRECT);
        }
      }
      break;


    case IDM_EDIT_CONVERTOEM:
      {
        int iCurPos    = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        int iAnchorPos = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
        if (iCurPos != iAnchorPos)
        {
          if (SC_SEL_RECTANGLE != SendMessage(hwndEdit,SCI_GETSELECTIONMODE,0,0))
          {
            int iSelCount = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
                              SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
            char *pszANSI = GlobalAlloc(GPTR,iSelCount+2);
            char *pszOEM = GlobalAlloc(GPTR,iSelCount+2);
            SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
            SendMessage(hwndEdit,SCI_GETSELTEXT,0,(LPARAM)pszANSI);
            CharToOemBuff(pszANSI,pszOEM,GlobalSize(pszOEM));
            SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
            SendMessage(hwndEdit,SCI_CLEAR,0,0);
            SendMessage(hwndEdit,SCI_ADDTEXT,(WPARAM)iSelCount,(LPARAM)pszOEM);
            SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
            SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
            GlobalFree(pszANSI);
            GlobalFree(pszOEM);
            SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          }
          else
            MsgBox(MBINFO,IDS_SELRECT);
        }
      }
      break;


    case IDM_EDIT_INSERT_TAG:
      {
        WCHAR wszOpen[256] = L"";
        WCHAR wszClose[256] = L"";
        if (EditInsertTagDlg(hwnd,wszOpen,wszClose))
          EditEncloseSelection(hwndEdit,wszOpen,wszClose);
      }
      break;


    case IDM_EDIT_INSERT_SHORTDATE:
    case IDM_EDIT_INSERT_LONGDATE:
      {
        char tchDate[128];
        char tchTime[128];
        char tchDateTime[256];
        char tchTemplate[256];
        SYSTEMTIME st;

        GetLocalTime(&st);

        if (IniGetString("Settings2",
              (LOWORD(wParam) == IDM_EDIT_INSERT_SHORTDATE) ? "DateTimeShort" : "DateTimeLong",
              "",tchTemplate,COUNTOF(tchTemplate))) {
          struct tm sst;
          sst.tm_isdst       = -1;
          sst.tm_sec         = (int)st.wSecond;
          sst.tm_min         = (int)st.wMinute;
          sst.tm_hour        = (int)st.wHour;
          sst.tm_mday        = (int)st.wDay;
          sst.tm_mon         = (int)st.wMonth - 1;
          sst.tm_year        = (int)st.wYear - 1900;
          sst.tm_wday        = (int)st.wDayOfWeek;
          mktime(&sst);
          strftime(tchDateTime,COUNTOF(tchDateTime),tchTemplate,&sst);
        }
        else {
          GetDateFormat(LOCALE_USER_DEFAULT,(
            LOWORD(wParam) == IDM_EDIT_INSERT_SHORTDATE) ? DATE_SHORTDATE : DATE_LONGDATE,
            &st,NULL,tchDate,COUNTOF(tchDate));
          GetTimeFormat(LOCALE_USER_DEFAULT,TIME_NOSECONDS,&st,NULL,tchTime,COUNTOF(tchTime));

          wsprintf(tchDateTime,"%s %s",tchTime,tchDate);
        }

        if (SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0) == SC_CP_UTF8 /*&& IsWindowsNT()*/)
        {
          WCHAR wszBuf[128];
          char  mszBuf[128*3];

          MultiByteToWideChar(CP_ACP,0,tchDateTime,-1,wszBuf,COUNTOF(wszBuf));
          WideCharToMultiByte(CP_UTF8,0,wszBuf,-1,mszBuf,COUNTOF(mszBuf),NULL,NULL);

          SendMessage(hwndEdit,SCI_REPLACESEL,0,(LPARAM)mszBuf);
        }
        else {
          SendMessage(hwndEdit,SCI_REPLACESEL,0,(LPARAM)tchDateTime);
        }
      }
      break;


    case IDM_EDIT_INSERT_FILENAME:
    case IDM_EDIT_INSERT_PATHNAME:
      {
        SHFILEINFO shfi;
        char *pszInsert;
        char tchUntitled[32];

        if (lstrlen(szCurFile)) {
          if (LOWORD(wParam) == IDM_EDIT_INSERT_FILENAME) {
            SHGetFileInfo2(szCurFile,0,&shfi,sizeof(SHFILEINFO),SHGFI_DISPLAYNAME);
            pszInsert = shfi.szDisplayName;
          }
          else
            pszInsert = szCurFile;
        }

        else {
          GetString(IDS_UNTITLED,tchUntitled,COUNTOF(tchUntitled));
          pszInsert = tchUntitled;
        }

        if (SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0) == SC_CP_UTF8 /*&& IsWindowsNT()*/)
        {
          WCHAR wszBuf[MAX_PATH];
          char  mszBuf[MAX_PATH*3];

          MultiByteToWideChar(CP_ACP,0,pszInsert,-1,wszBuf,COUNTOF(wszBuf));
          WideCharToMultiByte(CP_UTF8,0,wszBuf,-1,mszBuf,COUNTOF(mszBuf),NULL,NULL);

          SendMessage(hwndEdit,SCI_REPLACESEL,0,(LPARAM)mszBuf);
        }
        else {
          SendMessage(hwndEdit,SCI_REPLACESEL,0,(LPARAM)pszInsert);
        }
      }
      break;


    case IDM_EDIT_LINECOMMENT:
      switch (SendMessage(hwndEdit,SCI_GETLEXER,0,0)) {
        case SCLEX_NULL:
        case SCLEX_CSS:
        case SCLEX_DIFF:
          break;
        case SCLEX_HTML:
        case SCLEX_XML:
        case SCLEX_CPP:
        case SCLEX_PASCAL:
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditToggleLineComments(hwndEdit,L"//",FALSE);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          break;
        case SCLEX_VBSCRIPT:
        case SCLEX_VB:
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditToggleLineComments(hwndEdit,L"'",FALSE);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          break;
        case SCLEX_MAKEFILE:
        case SCLEX_PERL:
        case SCLEX_PYTHON:
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditToggleLineComments(hwndEdit,L"#",TRUE);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          break;
        case SCLEX_ASM:
        case SCLEX_PROPERTIES:
        case SCLEX_CONF:
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditToggleLineComments(hwndEdit,L";",TRUE);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          break;
        case SCLEX_SQL:
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditToggleLineComments(hwndEdit,L"--",TRUE);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          break;
        case SCLEX_BATCH:
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditToggleLineComments(hwndEdit,L"rem ",TRUE);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          break;
      }
      break;


    case IDM_EDIT_STREAMCOMMENT:
      switch (SendMessage(hwndEdit,SCI_GETLEXER,0,0)) {
        case SCLEX_NULL:
        case SCLEX_VBSCRIPT:
        case SCLEX_MAKEFILE:
        case SCLEX_VB:
        case SCLEX_ASM:
        case SCLEX_SQL:
        case SCLEX_PERL:
        case SCLEX_PYTHON:
        case SCLEX_PROPERTIES:
        case SCLEX_CONF:
        case SCLEX_BATCH:
        case SCLEX_DIFF:
          break;
        case SCLEX_HTML:
        case SCLEX_XML:
        case SCLEX_CSS:
        case SCLEX_CPP:
          EditEncloseSelection(hwndEdit,L"/*",L"*/");
          break;
        case SCLEX_PASCAL:
          EditEncloseSelection(hwndEdit,L"{",L"}");
      }
      break;


    case IDM_EDIT_FINDMATCHINGBRACE:
      {
        int iBrace2 = -1;
        int iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        char c = (char)SendMessage(hwndEdit,SCI_GETCHARAT,iPos,0);
        if (StrChr("()[]{}",c))
          iBrace2 = SendMessage(hwndEdit,SCI_BRACEMATCH,iPos,0);
        // Try one before
        else {
          iPos = SendMessage(hwndEdit,SCI_POSITIONBEFORE,iPos,0);
          c = (char)SendMessage(hwndEdit,SCI_GETCHARAT,iPos,0);
          if (StrChr("()[]{}",c))
            iBrace2 = SendMessage(hwndEdit,SCI_BRACEMATCH,iPos,0);
        }
        if (iBrace2 != -1)
          SendMessage(hwndEdit,SCI_GOTOPOS,(WPARAM)iBrace2,0);
      }
      break;


    case IDM_EDIT_SELTOMATCHINGBRACE:
      {
        int iBrace2 = -1;
        int iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        char c = (char)SendMessage(hwndEdit,SCI_GETCHARAT,iPos,0);
        if (StrChr("()[]{}",c))
          iBrace2 = SendMessage(hwndEdit,SCI_BRACEMATCH,iPos,0);
        // Try one before
        else {
          iPos = SendMessage(hwndEdit,SCI_POSITIONBEFORE,iPos,0);
          c = (char)SendMessage(hwndEdit,SCI_GETCHARAT,iPos,0);
          if (StrChr("()[]{}",c))
            iBrace2 = SendMessage(hwndEdit,SCI_BRACEMATCH,iPos,0);
        }
        if (iBrace2 != -1) {
          if (iBrace2 > iPos)
            SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iPos,(LPARAM)iBrace2+1);
          else
            SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iPos+1,(LPARAM)iBrace2);
        }
      }
      break;


    case IDM_EDIT_FIND:
      if (!IsWindow(hDlgFindReplace))
        hDlgFindReplace = EditFindReplaceDlg(hwndEdit,&efrData,FALSE);
      else {
        if (GetDlgItem(hDlgFindReplace,IDC_REPLACE)) {
          DestroyWindow(hDlgFindReplace);
          hDlgFindReplace = EditFindReplaceDlg(hwndEdit,&efrData,FALSE);
        }
        else
          SetForegroundWindow(hDlgFindReplace);
      }
      break;


    case IDM_EDIT_FINDNEXT:
    case IDM_EDIT_FINDPREV:
    case IDM_EDIT_SELTONEXT:
    case IDM_EDIT_SELTOPREV:
      if (!lstrlen(efrData.szFind))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_FIND,1),0);
      else {
        UINT cp = SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0);
        if (cpLastFind != cp) {
          if (cp != SC_CP_UTF8) {
            WCHAR wszFind[256];
            MultiByteToWideChar(CP_UTF8,0,efrData.szFindUTF8,-1,wszFind,COUNTOF(wszFind));
            WideCharToMultiByte(cp,0,wszFind,-1,efrData.szFind,COUNTOF(efrData.szFind),NULL,NULL);
          }
          else
            lstrcpy(efrData.szFind,efrData.szFindUTF8);
        }
        cpLastFind = cp;
        switch (LOWORD(wParam)) {

          case IDM_EDIT_FINDNEXT:
            EditFindNext(hwndEdit,&efrData,FALSE);
            break;

          case IDM_EDIT_FINDPREV:
            EditFindPrev(hwndEdit,&efrData,FALSE);
            break;

          case IDM_EDIT_SELTONEXT:
            EditFindNext(hwndEdit,&efrData,TRUE);
            break;

          case IDM_EDIT_SELTOPREV:
            EditFindPrev(hwndEdit,&efrData,TRUE);
            break;
        }
      }
      break;


    case IDM_EDIT_REPLACE:
      if (!IsWindow(hDlgFindReplace))
        hDlgFindReplace = EditFindReplaceDlg(hwndEdit,&efrData,TRUE);
      else {
        if (!GetDlgItem(hDlgFindReplace,IDC_REPLACE)) {
          DestroyWindow(hDlgFindReplace);
          hDlgFindReplace = EditFindReplaceDlg(hwndEdit,&efrData,TRUE);
        }
        else
          SetForegroundWindow(hDlgFindReplace);
      }
      break;


    case IDM_EDIT_GOTOLINE:
      EditLinenumDlg(hwndEdit);
      break;


    case IDM_VIEW_SCHEME:
      Style_SelectLexerDlg(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case IDM_VIEW_USE2NDDEFAULT:
      Style_ToggleUse2ndDefault(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case IDM_VIEW_SCHEMECONFIG:
      Style_ConfigDlg(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case IDM_VIEW_FONT:
      Style_SetDefaultFont(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case IDM_VIEW_WORDWRAP:
      fWordWrap = (fWordWrap) ? FALSE : TRUE;
      SendMessage(hwndEdit,SCI_SETWRAPMODE,(fWordWrap)?SC_WRAP_WORD:SC_WRAP_NONE,0);
      UpdateToolbar();
      break;


    case IDM_VIEW_WORDWRAPSETTINGS:
      if (WordWrapSettingsDlg(hwnd,IDD_WORDWRAP,&iWordWrapIndent))
      {
        iWordWrapIndent = max(min(iWordWrapIndent,1024),0);
        SendMessage(hwndEdit,SCI_SETWRAPSTARTINDENT,iWordWrapIndent,0);

        if (bShowWordWrapSymbols) {
          SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGSLOCATION,WrapSymbols[iWordWrapSymbols].location,0);
          SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGS,WrapSymbols[iWordWrapSymbols].flags,0);
        }
        else {
          SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGS,0,0);
        }
      }
      break;


    case IDM_VIEW_LONGLINEMARKER:
      bMarkLongLines = (bMarkLongLines) ? FALSE: TRUE;
      if (bMarkLongLines) {
        SendMessage(hwndEdit,SCI_SETEDGEMODE,(iLongLineMode == EDGE_LINE)?EDGE_LINE:EDGE_BACKGROUND,0);
        Style_SetLongLineColors(hwndEdit);
      }
      else
        SendMessage(hwndEdit,SCI_SETEDGEMODE,EDGE_NONE,0);
      UpdateStatusbar();
      break;


    case IDM_VIEW_LONGLINESETTINGS:
      if (LongLineSettingsDlg(hwnd,IDD_LONGLINES,&iLongLinesLimit))
      {
        bMarkLongLines = TRUE;
        SendMessage(hwndEdit,SCI_SETEDGEMODE,(iLongLineMode == EDGE_LINE)?EDGE_LINE:EDGE_BACKGROUND,0);
        Style_SetLongLineColors(hwndEdit);
        iLongLinesLimit = max(min(iLongLinesLimit,999),0);
        SendMessage(hwndEdit,SCI_SETEDGECOLUMN,iLongLinesLimit,0);
        UpdateStatusbar();
      }
      break;


    case IDM_VIEW_TABSASSPACES:
      bTabsAsSpaces = (bTabsAsSpaces) ? FALSE : TRUE;
      SendMessage(hwndEdit,SCI_SETUSETABS,!bTabsAsSpaces,0);
      break;


    case IDM_VIEW_TABSETTINGS:
      if (TabSettingsDlg(hwnd,IDD_TABSETTINGS,NULL))
      {
        SendMessage(hwndEdit,SCI_SETUSETABS,!bTabsAsSpaces,0);
        iTabWidth = max(min(iTabWidth,24),1);
        iIndentWidth = max(min(iIndentWidth,24),0);
        SendMessage(hwndEdit,SCI_SETTABWIDTH,iTabWidth,0);
        SendMessage(hwndEdit,SCI_SETINDENT,iIndentWidth,0);
      }
      break;


    case IDM_VIEW_SHOWINDENTGUIDES:
      bShowIndentGuides = (bShowIndentGuides) ? FALSE : TRUE;
      Style_SetIndentGuides(hwndEdit,bShowIndentGuides);
      break;


    case IDM_VIEW_AUTOINDENTTEXT:
      bAutoIndent = (bAutoIndent) ? FALSE : TRUE;
      break;


    case IDM_VIEW_LINENUMBERS:
      bShowLineNumbers = (bShowLineNumbers) ? FALSE : TRUE;
      UpdateLineNumberWidth();
      //SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,0,
      //  (bShowLineNumbers)?SendMessage(hwndEdit,SCI_TEXTWIDTH,STYLE_LINENUMBER,(LPARAM)"_999999_"):0);
      break;


    case IDM_VIEW_MARGIN:
      bShowSelectionMargin = (bShowSelectionMargin) ? FALSE : TRUE;
      SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,1,(bShowSelectionMargin)?16:0);
      break;


    case IDM_VIEW_SHOWWHITESPACE:
      bViewWhiteSpace = (bViewWhiteSpace) ? FALSE : TRUE;
      SendMessage(hwndEdit,SCI_SETVIEWWS,(bViewWhiteSpace)?SCWS_VISIBLEALWAYS:SCWS_INVISIBLE,0);
      break;


    case IDM_VIEW_SHOWEOLS:
      bViewEOLs = (bViewEOLs) ? FALSE : TRUE;
      SendMessage(hwndEdit,SCI_SETVIEWEOL,bViewEOLs,0);
      break;


    case IDM_VIEW_WORDWRAPSYMBOLS:
      bShowWordWrapSymbols = (bShowWordWrapSymbols) ? 0 : 1;
      if (bShowWordWrapSymbols) {
        SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGSLOCATION,WrapSymbols[iWordWrapSymbols].location,0);
        SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGS,WrapSymbols[iWordWrapSymbols].flags,0);
      }
      else {
        SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGS,0,0);
      }
      break;


    case IDM_VIEW_MATCHBRACES:
      bMatchBraces = (bMatchBraces) ? FALSE : TRUE;
      if (!bMatchBraces)
        SendMessage(hwndEdit,SCI_BRACEHIGHLIGHT,(WPARAM)-1,(LPARAM)-1);
      break;


    case IDM_VIEW_AUTOCLOSETAGS:
      bAutoCloseTags = (bAutoCloseTags) ? FALSE : TRUE;
      break;


    case IDM_VIEW_HILITECURRENTLINE:
      bHiliteCurrentLine = (bHiliteCurrentLine) ? FALSE : TRUE;
      Style_SetCurrentLineBackground(hwndEdit);
      break;


    case IDM_VIEW_ZOOMIN:
      SendMessage(hwndEdit,SCI_ZOOMIN,0,0);
      //UpdateLineNumberWidth();
      break;


    case IDM_VIEW_ZOOMOUT:
      SendMessage(hwndEdit,SCI_ZOOMOUT,0,0);
      //UpdateLineNumberWidth();
      break;


    case IDM_VIEW_RESETZOOM:
      SendMessage(hwndEdit,SCI_SETZOOM,0,0);
      //UpdateLineNumberWidth();
      break;


    case IDM_VIEW_TOOLBAR:
      if (bShowToolbar) {
        bShowToolbar = 0;
        ShowWindow(hwndReBar,SW_HIDE);
      }
      else {
        bShowToolbar = 1;
        UpdateToolbar();
        ShowWindow(hwndReBar,SW_SHOW);
      }
      SendWMSize(hwnd);
      break;


    case IDM_VIEW_CUSTOMIZETB:
      SendMessage(hwndToolbar,TB_CUSTOMIZE,0,0);
      break;


    case IDM_VIEW_STATUSBAR:
      if (bShowStatusbar) {
        bShowStatusbar = 0;
        ShowWindow(hwndStatus,SW_HIDE);
      }
      else {
        bShowStatusbar = 1;
        UpdateStatusbar();
        ShowWindow(hwndStatus,SW_SHOW);
      }
      SendWMSize(hwnd);
      break;


    case IDM_VIEW_STICKYWINPOS:
      bStickyWinPos = IniGetInt("Settings2","StickyWindowPosition",bStickyWinPos);
      if (!bStickyWinPos) {
        WINDOWPLACEMENT wndpl;
        char tchPosX[32], tchPosY[32], tchSizeX[32], tchSizeY[32], tchMaximized[32];

        int ResX = GetSystemMetrics(SM_CXSCREEN);
        int ResY = GetSystemMetrics(SM_CYSCREEN);

        // GetWindowPlacement
        wndpl.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(hwndMain,&wndpl);

        wi.x = wndpl.rcNormalPosition.left;
        wi.y = wndpl.rcNormalPosition.top;
        wi.cx = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
        wi.cy = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
        wi.max = (IsZoomed(hwndMain) || (wndpl.flags & WPF_RESTORETOMAXIMIZED));

        wsprintf(tchPosX,"%ix%i PosX",ResX,ResY);
        wsprintf(tchPosY,"%ix%i PosY",ResX,ResY);
        wsprintf(tchSizeX,"%ix%i SizeX",ResX,ResY);
        wsprintf(tchSizeY,"%ix%i SizeY",ResX,ResY);
        wsprintf(tchMaximized,"%ix%i Maximized",ResX,ResY);

        bStickyWinPos = 1;
        IniSetInt("Settings2","StickyWindowPosition",1);

        IniSetInt("Window",tchPosX,wi.x);
        IniSetInt("Window",tchPosY,wi.y);
        IniSetInt("Window",tchSizeX,wi.cx);
        IniSetInt("Window",tchSizeY,wi.cy);
        IniSetInt("Window",tchMaximized,wi.max);

        InfoBox(0,IDS_STICKYWINPOS,"MsgStickyWinPos");
      }
      else {
        bStickyWinPos = 0;
        IniSetInt("Settings2","StickyWindowPosition",0);
      }
      break;


    case IDM_VIEW_REUSEWINDOW:
      if (IniGetInt("Settings2","ReuseWindow",0))
        IniSetInt("Settings2","ReuseWindow",0);
      else
        IniSetInt("Settings2","ReuseWindow",1);
      break;


    case IDM_VIEW_SINGLEFILEINSTANCE:
      if (IniGetInt("Settings2","SingleFileInstance",0))
        IniSetInt("Settings2","SingleFileInstance",0);
      else
        IniSetInt("Settings2","SingleFileInstance",1);
      break;


    case IDM_VIEW_ALWAYSONTOP:
      if (bAlwaysOnTop) {
        bAlwaysOnTop = 0;
        SetWindowPos(hwnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
      }
      else {
        bAlwaysOnTop = 1;
        SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
      }
      break;


    case IDM_VIEW_MINTOTRAY:
      bMinimizeToTray =(bMinimizeToTray) ? FALSE : TRUE;
      break;


    case IDM_VIEW_TRANSPARENT:
      bTransparentMode =(bTransparentMode) ? FALSE : TRUE;
      SetWindowTransparentMode(hwnd,bTransparentMode);
      break;


    case IDM_VIEW_SHOWFILENAMEONLY:
      iPathNameFormat = 0;
      SetWindowTitle(hwnd,uidsAppTitle,IDS_UNTITLED,szCurFile,
        iPathNameFormat,bModified || iCodePage != iInitialCP,
        IDS_READONLY,bReadOnly);
      break;


    case IDM_VIEW_SHOWFILENAMEFIRST:
      iPathNameFormat = 1;
      SetWindowTitle(hwnd,uidsAppTitle,IDS_UNTITLED,szCurFile,
        iPathNameFormat,bModified || iCodePage != iInitialCP,
        IDS_READONLY,bReadOnly);
      break;


    case IDM_VIEW_SHOWFULLPATH:
      iPathNameFormat = 2;
      SetWindowTitle(hwnd,uidsAppTitle,IDS_UNTITLED,szCurFile,
        iPathNameFormat,bModified || iCodePage != iInitialCP,
        IDS_READONLY,bReadOnly);
      break;


    case IDM_VIEW_NOSAVERECENT:
      bSaveRecentFiles = (bSaveRecentFiles) ? FALSE : TRUE;
      break;


    case IDM_VIEW_NOSAVEFINDREPL:
      bSaveFindReplace = (bSaveFindReplace) ? FALSE : TRUE;
      break;


    case IDM_VIEW_SAVEBEFORERUNNINGTOOLS:
      bSaveBeforeRunningTools = (bSaveBeforeRunningTools) ? FALSE : TRUE;
      break;


    case IDM_VIEW_FILECHANGENOTIFY:
      bEnableFileWatching = (bEnableFileWatching) ? FALSE : TRUE;
      InstallFileWatching(szCurFile);
      break;


    case IDM_VIEW_ESCMINIMIZE:
      iEscFunction = 1;
      break;


    case IDM_VIEW_ESCEXIT:
      iEscFunction = 2;
      break;


    case IDM_VIEW_NOESNFUNC:
      iEscFunction = 0;
      break;


    case IDM_VIEW_SAVESETTINGS:
      bSaveSettings = (bSaveSettings) ? FALSE : TRUE;
      break;


    case IDM_VIEW_SAVESETTINGSNOW:
      BeginWaitCursor();
      StatusSetTextID(hwndStatus,STATUS_HELP,IDS_SAVINGSETTINGS);
      StatusSetSimple(hwndStatus,TRUE);
      InvalidateRect(hwndStatus,NULL,TRUE);
      UpdateWindow(hwndStatus);
      SaveSettings(TRUE);
      StatusSetSimple(hwndStatus,FALSE);
      EndWaitCursor();
      MsgBox(MBINFO,IDS_SAVEDSETTINGS);
      break;


    case IDM_HELP_ABOUT:
      DialogBox(g_hInstance,MAKEINTRESOURCE(IDD_ABOUT),
        hwnd,AboutDlgProc);
      break;


    case CMD_ESCAPE:
      if (iEscFunction == 1)
        SendMessage(hwnd,WM_SYSCOMMAND,SC_MINIMIZE,0);
      else if (iEscFunction == 2)
        SendMessage(hwnd,WM_CLOSE,0,0);
      break;


    case CMD_SHIFTESC:
      if (FileSave(TRUE,FALSE,FALSE,FALSE))
        SendMessage(hwnd,WM_CLOSE,0,0);
      break;


    // Newline with toggled auto indent setting
    case CMD_CTRLENTER:
      bAutoIndent = (bAutoIndent) ? 0 : 1;
      SendMessage(hwndEdit,SCI_NEWLINE,0,0);
      bAutoIndent = (bAutoIndent) ? 0 : 1;
      break;


    case CMD_CTRLBACK:
      {
        int iPos        = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        int iAnchor     = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
        int iLine       = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,(WPARAM)iPos,0);
        int iStartPos   = SendMessage(hwndEdit,SCI_POSITIONFROMLINE,(WPARAM)iLine,0);
        int iEndPos     = SendMessage(hwndEdit,SCI_GETLINEENDPOSITION,(WPARAM)iLine,0);
        int iIndentPos  = SendMessage(hwndEdit,SCI_GETLINEINDENTPOSITION,(WPARAM)iLine,0);

        if (iPos != iAnchor)
          SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iPos,(LPARAM)iPos);
        else {
          if (iIndentPos != iEndPos)
            SendMessage(hwndEdit,SCI_DELWORDLEFT,0,0);
          else if (iStartPos != iEndPos)
            SendMessage(hwndEdit,SCI_DELLINELEFT,0,0);
          else // iStartPos == iEndPos == iIndentPos
            SendMessage(hwndEdit,SCI_DELETEBACK,0,0);
        }
      }
      break;


    case CMD_CTRLDEL:
      {
        int iPos        = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        int iAnchor     = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
        int iLine       = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,(WPARAM)iPos,0);
        int iStartPos   = SendMessage(hwndEdit,SCI_POSITIONFROMLINE,(WPARAM)iLine,0);
        int iEndPos     = SendMessage(hwndEdit,SCI_GETLINEENDPOSITION,(WPARAM)iLine,0);

        if (iPos != iAnchor)
          SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iPos,(LPARAM)iPos);
        else {
          if (iStartPos != iEndPos)
            SendMessage(hwndEdit,SCI_DELWORDRIGHT,0,0);
          else // iStartPos == iEndPos
            SendMessage(hwndEdit,SCI_LINEDELETE,0,0);
        }
      }
      break;


    case CMD_CTRLTAB:
      SendMessage(hwnd,SCI_SETTABINDENTS,FALSE,0);
      SendMessage(hwndEdit,SCI_SETUSETABS,TRUE,0);
      SendMessage(hwndEdit,SCI_TAB,0,0);
      SendMessage(hwndEdit,SCI_SETUSETABS,!bTabsAsSpaces,0);
      SendMessage(hwnd,SCI_SETTABINDENTS,TRUE,0);
      break;


    case CMD_F8:
      {
        char tchCurFile2[MAX_PATH];
        if (lstrlen(szCurFile)) {
          lstrcpy(tchCurFile2,szCurFile);
          FileLoad(FALSE,FALSE,TRUE,TRUE,tchCurFile2);
        }
      }
      break;


    case CMD_CTRLF8:
      {
        char tchCurFile2[MAX_PATH];
        if (lstrlen(szCurFile)) {
          bSkipUnicodeDetection = (bSkipUnicodeDetection) ? 0 : 1;
          lstrcpy(tchCurFile2,szCurFile);
          FileLoad(FALSE,FALSE,TRUE,FALSE,tchCurFile2);
          bSkipUnicodeDetection = (bSkipUnicodeDetection) ? 0 : 1;
        }
      }
      break;


    case CMD_SHIFTF8:
      {
        char tchCurFile2[MAX_PATH];
        if (lstrlen(szCurFile)) {
          int iDefaultEncoding2 = iDefaultEncoding;
          if (iEncodings[iDefaultEncoding] & NCP_UTF8)
            iDefaultEncoding = 0;
          else
            iDefaultEncoding = 3;
          lstrcpy(tchCurFile2,szCurFile);
          FileLoad(FALSE,FALSE,TRUE,FALSE,tchCurFile2);
          iDefaultEncoding = iDefaultEncoding2;
        }
      }
      break;


    case CMD_LEXDEFAULT:
      Style_SetDefaultLexer(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case CMD_LEXHTML:
      Style_SetHTMLLexer(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case CMD_LEXXML:
      Style_SetXMLLexer(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case CMD_WEBACTION1:
    case CMD_WEBACTION2:
      {
        BOOL  bCmdEnabled = FALSE;
        LPSTR lpszTemplateName;
        char  szCmdTemplate[256];
        LPSTR lpszSelection;
        DWORD cchSelection;
        LPSTR lpsz;
        LPSTR lpszCommand;
        LPSTR lpszArgs;
        SHELLEXECUTEINFO sei;

        lpszTemplateName = (LOWORD(wParam) == CMD_WEBACTION1) ? "WebTemplate1" : "WebTemplate2";

        bCmdEnabled = IniGetString("Settings2",lpszTemplateName,"",szCmdTemplate,COUNTOF(szCmdTemplate));

        if (bCmdEnabled) {

          cchSelection = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
                          SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);

          if (cchSelection > 0 && cchSelection < 256)
          {
            lpszSelection = GlobalAlloc(GPTR,cchSelection+2);
            SendMessage(hwndEdit,SCI_GETSELTEXT,0,(LPARAM)lpszSelection);

            // Check lpszSelection and truncate bad chars
            lpsz = strchr(lpszSelection,13);
            if (lpsz) *lpsz = '\0';

            lpsz = strchr(lpszSelection,10);
            if (lpsz) *lpsz = '\0';

            lpsz = strchr(lpszSelection,9);
            if (lpsz) *lpsz = '\0';

            if (lstrlen(lpszSelection)) {

              lpszCommand = GlobalAlloc(GPTR,GlobalSize(lpszSelection)+COUNTOF(szCmdTemplate)+MAX_PATH+32);
              wsprintf(lpszCommand,szCmdTemplate,lpszSelection);
              ExpandEnvironmentStringsEx(lpszCommand,GlobalSize(lpszCommand));

              lpszArgs = GlobalAlloc(GPTR,GlobalSize(lpszCommand));
              ExtractFirstArgument(lpszCommand,lpszCommand,lpszArgs);

              ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));

              sei.cbSize = sizeof(SHELLEXECUTEINFO);
              sei.fMask = SEE_MASK_NOCLOSEPROCESS;
              sei.hwnd = NULL;
              sei.lpVerb = NULL;
              sei.lpFile = lpszCommand;
              sei.lpParameters = lpszArgs;
              sei.lpDirectory = NULL;
              sei.nShow = SW_SHOWNORMAL;

              ShellExecuteEx(&sei);

              GlobalFree(lpszSelection);
              GlobalFree(lpszCommand);
            }
          }
        }
      }
      break;


    case CMD_FINDNEXTSEL:
    case CMD_FINDPREVSEL:
    case IDM_EDIT_SAVEFIND:
      {
        int cchSelection = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
                             SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);

        if (cchSelection == 0)
        {
          SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_SELECTWORD,1),0);
          cchSelection = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
                           SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
        }

        if (cchSelection > 0 && cchSelection < 255)
        {
          char  mszSelection[256];
          char  *lpsz;

          SendMessage(hwndEdit,SCI_GETSELTEXT,0,(LPARAM)mszSelection);
          mszSelection[cchSelection] = 0; // zero terminate

          // Check lpszSelection and truncate newlines
          lpsz = strchr(mszSelection,'\n');
          if (lpsz) *lpsz = '\0';

          lpsz = strchr(mszSelection,'\r');
          if (lpsz) *lpsz = '\0';

          cpLastFind = SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0);
          lstrcpy(efrData.szFind,mszSelection);

          if (cpLastFind != SC_CP_UTF8)
          {
            WCHAR wszBuf[256];

            MultiByteToWideChar(cpLastFind,0,mszSelection,-1,wszBuf,COUNTOF(wszBuf));
            WideCharToMultiByte(CP_UTF8,0,wszBuf,-1,efrData.szFindUTF8,COUNTOF(efrData.szFindUTF8),NULL,NULL);
          }
          else
            lstrcpy(efrData.szFindUTF8,mszSelection);

          efrData.fuFlags &= (~SCFIND_REGEXP);
          efrData.bTransformBS = FALSE;

          switch (LOWORD(wParam)) {

            case IDM_EDIT_SAVEFIND:
              break;

            case CMD_FINDNEXTSEL:
              EditFindNext(hwndEdit,&efrData,FALSE);
              break;

            case CMD_FINDPREVSEL:
              EditFindPrev(hwndEdit,&efrData,FALSE);
              break;
          }
        }
      }
      break;


    case CMD_INCLINELIMIT:
    case CMD_DECLINELIMIT:
      if (!bMarkLongLines)
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_LONGLINEMARKER,1),0);
      else {
        if (LOWORD(wParam) == CMD_INCLINELIMIT)
          iLongLinesLimit++;
        else
          iLongLinesLimit--;
        iLongLinesLimit = max(min(iLongLinesLimit,999),0);
        SendMessage(hwndEdit,SCI_SETEDGECOLUMN,iLongLinesLimit,0);
        UpdateStatusbar();
      }
      break;


    case CMD_STRINGIFY:
      EditEncloseSelection(hwndEdit,L"'",L"'");
      break;


    case CMD_STRINGIFY2:
      EditEncloseSelection(hwndEdit,L"\"",L"\"");
      break;


    case CMD_EMBRACE:
      EditEncloseSelection(hwndEdit,L"(",L")");
      break;


    case CMD_EMBRACE2:
      EditEncloseSelection(hwndEdit,L"[",L"]");
      break;


    case IDT_FILE_NEW:
      if (IsCmdEnabled(hwnd,IDM_FILE_NEW))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_NEW,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_OPEN:
      if (IsCmdEnabled(hwnd,IDM_FILE_OPEN))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_OPEN,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_BROWSE:
      if (IsCmdEnabled(hwnd,IDM_FILE_BROWSE))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_BROWSE,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_SAVE:
      if (IsCmdEnabled(hwnd,IDM_FILE_SAVE))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_SAVE,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_UNDO:
      if (IsCmdEnabled(hwnd,IDM_EDIT_UNDO))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_UNDO,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_REDO:
      if (IsCmdEnabled(hwnd,IDM_EDIT_REDO))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_REDO,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_CUT:
      if (IsCmdEnabled(hwnd,IDM_EDIT_CUT))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_CUT,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_COPY:
      if (IsCmdEnabled(hwnd,IDM_EDIT_COPY))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_COPY,1),0);
      else
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_COPYALL,1),0);
      break;


    case IDT_EDIT_PASTE:
      if (IsCmdEnabled(hwnd,IDM_EDIT_PASTE))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_PASTE,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_FIND:
      if (IsCmdEnabled(hwnd,IDM_EDIT_FIND))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_FIND,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_REPLACE:
      if (IsCmdEnabled(hwnd,IDM_EDIT_REPLACE))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_REPLACE,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_VIEW_WORDWRAP:
      if (IsCmdEnabled(hwnd,IDM_VIEW_WORDWRAP))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_WORDWRAP,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_VIEW_ZOOMIN:
      if (IsCmdEnabled(hwnd,IDM_VIEW_ZOOMIN))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_ZOOMIN,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_VIEW_ZOOMOUT:
      if (IsCmdEnabled(hwnd,IDM_VIEW_ZOOMOUT))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_ZOOMOUT,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_VIEW_SCHEME:
      if (IsCmdEnabled(hwnd,IDM_VIEW_SCHEME))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_SCHEME,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_VIEW_SCHEMECONFIG:
      if (IsCmdEnabled(hwnd,IDM_VIEW_SCHEMECONFIG))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_SCHEMECONFIG,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_EXIT:
      SendMessage(hwnd,WM_CLOSE,0,0);
      break;


    case IDT_FILE_SAVEAS:
      if (IsCmdEnabled(hwnd,IDM_FILE_SAVEAS))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_SAVEAS,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_SAVECOPY:
      if (IsCmdEnabled(hwnd,IDM_FILE_SAVECOPY))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_SAVECOPY,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_CLEAR:
      if (IsCmdEnabled(hwnd,IDM_EDIT_CLEAR))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_CLEAR,1),0);
      else
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_CLEARALL,1),0);
      break;


    case IDT_FILE_PRINT:
      if (IsCmdEnabled(hwnd,IDM_FILE_PRINT))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_PRINT,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_OPENFAV:
      if (IsCmdEnabled(hwnd,IDM_FILE_OPENFAV))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_OPENFAV,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_ADDTOFAV:
      if (IsCmdEnabled(hwnd,IDM_FILE_ADDTOFAV))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_ADDTOFAV,1),0);
      else
        MessageBeep(0);
      break;

  }

  return(0);

}


//=============================================================================
//
//  MsgNotify() - Handles WM_NOTIFY
//
//
LRESULT MsgNotify(HWND hwnd,WPARAM wParam,LPARAM lParam)
{

  LPNMHDR pnmh = (LPNMHDR)lParam;
  struct SCNotification* scn = (struct SCNotification*)lParam;

  switch(pnmh->idFrom)
  {

    case IDC_EDIT:

      switch(pnmh->code)
      {
        case SCN_UPDATEUI:
          UpdateToolbar();
          UpdateStatusbar();

          // Brace Match
          if (bMatchBraces)
          {
            int iPos;
            char c;

            int iEndStyled = SendMessage(hwndEdit,SCI_GETENDSTYLED,0,0);
            if (iEndStyled < SendMessage(hwndEdit,SCI_GETLENGTH,0,0)) {
              int iLine = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,iEndStyled,0);
              int iEndStyled = SendMessage(hwndEdit,SCI_POSITIONFROMLINE,iLine,0);
              SendMessage(hwndEdit,SCI_COLOURISE,iEndStyled,-1);
            }

            iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
            c = (char)SendMessage(hwndEdit,SCI_GETCHARAT,iPos,0);
            if (StrChr("()[]{}",c)) {
              int iBrace2 = SendMessage(hwndEdit,SCI_BRACEMATCH,iPos,0);
              if (iBrace2 != -1) {
                int col1 = SendMessage(hwndEdit,SCI_GETCOLUMN,iPos,0);
                int col2 = SendMessage(hwndEdit,SCI_GETCOLUMN,iBrace2,0);
                SendMessage(hwndEdit,SCI_BRACEHIGHLIGHT,iPos,iBrace2);
                SendMessage(hwndEdit,SCI_SETHIGHLIGHTGUIDE,min(col1,col2),0);
              }
              else {
                SendMessage(hwndEdit,SCI_BRACEBADLIGHT,iPos,0);
                SendMessage(hwndEdit,SCI_SETHIGHLIGHTGUIDE,0,0);
              }
            }
            // Try one before
            else
            {
              iPos = SendMessage(hwndEdit,SCI_POSITIONBEFORE,iPos,0);
              c = (char)SendMessage(hwndEdit,SCI_GETCHARAT,iPos,0);
              if (StrChr("()[]{}",c)) {
                int iBrace2 = SendMessage(hwndEdit,SCI_BRACEMATCH,iPos,0);
                if (iBrace2 != -1) {
                  int col1 = SendMessage(hwndEdit,SCI_GETCOLUMN,iPos,0);
                  int col2 = SendMessage(hwndEdit,SCI_GETCOLUMN,iBrace2,0);
                  SendMessage(hwndEdit,SCI_BRACEHIGHLIGHT,iPos,iBrace2);
                  SendMessage(hwndEdit,SCI_SETHIGHLIGHTGUIDE,min(col1,col2),0);
                }
                else {
                  SendMessage(hwndEdit,SCI_BRACEBADLIGHT,iPos,0);
                  SendMessage(hwndEdit,SCI_SETHIGHLIGHTGUIDE,0,0);
                }
              }
              else {
                SendMessage(hwndEdit,SCI_BRACEHIGHLIGHT,(WPARAM)-1,(LPARAM)-1);
                SendMessage(hwndEdit,SCI_SETHIGHLIGHTGUIDE,0,0);
              }
            }
          }
          break;

        case SCN_CHARADDED:
          // Auto indent
          if (bAutoIndent && (scn->ch == '\x0D' || scn->ch == '\x0A'))
          {
            // in CRLF mode handle LF only...
            if ((SC_EOL_CRLF == iEOLMode && scn->ch != '\x0A') || SC_EOL_CRLF != iEOLMode)
            {
              char *pLineBuf;
              char *pPos;
              //int  iIndentLen;

              int iCurPos    = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
              //int iAnchorPos = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
              int iCurLine = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,(WPARAM)iCurPos,0);
              int iLineLength = SendMessage(hwndEdit,SCI_LINELENGTH,iCurLine,0);
              //int iIndentBefore = SendMessage(hwndEdit,SCI_GETLINEINDENTATION,(WPARAM)iCurLine-1,0);

              if (iCurLine > 0/* && iLineLength <= 2*/)
              {
                int iPrevLineLength = SendMessage(hwndEdit,SCI_LINELENGTH,iCurLine-1,0);
                if (pLineBuf = GlobalAlloc(GPTR,iPrevLineLength+1))
                {
                  SendMessage(hwndEdit,SCI_GETLINE,iCurLine-1,(LPARAM)pLineBuf);
                  *(pLineBuf+iPrevLineLength) = '\0';
                  for (pPos = pLineBuf; *pPos; pPos++) {
                    if (*pPos != ' ' && *pPos != '\t')
                      *pPos = '\0';
                  }
                  if (*pLineBuf) {
                    //int iPrevLineStartPos;
                    //int iPrevLineEndPos;
                    //int iPrevLineIndentPos;

                    SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
                    SendMessage(hwndEdit,SCI_ADDTEXT,lstrlen(pLineBuf),(LPARAM)pLineBuf);
                    SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);

                    //iPrevLineStartPos  = SendMessage(hwndEdit,SCI_POSITIONFROMLINE,(WPARAM)iCurLine-1,0);
                    //iPrevLineEndPos    = SendMessage(hwndEdit,SCI_GETLINEENDPOSITION,(WPARAM)iCurLine-1,0);
                    //iPrevLineIndentPos = SendMessage(hwndEdit,SCI_GETLINEINDENTPOSITION,(WPARAM)iCurLine-1,0);

                    //if (iPrevLineEndPos == iPrevLineIndentPos) {
                    //  SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
                    //  SendMessage(hwndEdit,SCI_SETTARGETSTART,(WPARAM)iPrevLineStartPos,0);
                    //  SendMessage(hwndEdit,SCI_SETTARGETEND,(WPARAM)iPrevLineEndPos,0);
                    //  SendMessage(hwndEdit,SCI_REPLACETARGET,0,(LPARAM)"");
                    //  SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
                    //}
                  }
                  GlobalFree(pLineBuf);
                  //int iIndent = SendMessage(hwndEdit,SCI_GETLINEINDENTATION,(WPARAM)iCurLine,0);
                  //SendMessage(hwndEdit,SCI_SETLINEINDENTATION,(WPARAM)iCurLine,(LPARAM)iIndentBefore);
                  //iIndentLen = /*- iIndent +*/ SendMessage(hwndEdit,SCI_GETLINEINDENTATION,(WPARAM)iCurLine,0);
                  //if (iIndentLen > 0)
                  //  SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iAnchorPos+iIndentLen,(LPARAM)iCurPos+iIndentLen);
                }
              }
            }
          }
          // Auto close tags
          else if (bAutoCloseTags && scn->ch == '>')
          {
            int iLexer = SendMessage(hwndEdit,SCI_GETLEXER,0,0);
            if (/*iLexer == SCLEX_HTML || iLexer == SCLEX_XML*/ 1)
            {
              char tchBuf[512];
              char tchIns[516] = "</";
              int  cchIns = 2;
              int  iCurPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
              int  iHelper = iCurPos - (COUNTOF(tchBuf) - 1);
              int  iStartPos = max(0,iHelper);
              int  iSize = iCurPos - iStartPos;

              if (iSize >= 3) {

                struct TextRange tr = { { iStartPos, iCurPos }, tchBuf };
                SendMessage(hwndEdit,SCI_GETTEXTRANGE,0,(LPARAM)&tr);

                if (tchBuf[iSize - 2] != '/') {

                  const char* pBegin = &tchBuf[0];
                  const char* pCur = &tchBuf[iSize - 2];

                  while (pCur > pBegin && *pCur != '<' && *pCur != '>')
                    --pCur;

                  if (*pCur == '<') {
                    pCur++;
                    while (StrChr(":_-.", *pCur) || IsCharAlphaNumeric(*pCur)) {
                      tchIns[cchIns++] = *pCur;
                      pCur++;
                    }
                  }

                  tchIns[cchIns++] = '>';
                  tchIns[cchIns] = 0;

                  if (cchIns > 3 &&
                      lstrcmpi(tchIns,"</base>") &&
                      lstrcmpi(tchIns,"</bgsound>") &&
                      lstrcmpi(tchIns,"</br>") &&
                      lstrcmpi(tchIns,"</embed>") &&
                      lstrcmpi(tchIns,"</hr>") &&
                      lstrcmpi(tchIns,"</img>") &&
                      lstrcmpi(tchIns,"</input>") &&
                      lstrcmpi(tchIns,"</link>") &&
                      lstrcmpi(tchIns,"</meta>"))
                  {
                    SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
                    SendMessage(hwndEdit,SCI_REPLACESEL,0,(LPARAM)tchIns);
                    SendMessage(hwndEdit,SCI_SETSEL,iCurPos,iCurPos);
                    SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
                  }
                }
              }
            }
          }
          break;

        case SCN_MODIFIED:
        case SCN_ZOOM:
          UpdateLineNumberWidth();
          break;

        case SCN_SAVEPOINTREACHED:
          bModified = FALSE;
          SetWindowTitle(hwnd,uidsAppTitle,IDS_UNTITLED,szCurFile,
            iPathNameFormat,bModified || iCodePage != iInitialCP,
            IDS_READONLY,bReadOnly);
          break;

        case SCN_SAVEPOINTLEFT:
          bModified = TRUE;
          SetWindowTitle(hwnd,uidsAppTitle,IDS_UNTITLED,szCurFile,
            iPathNameFormat,bModified || iCodePage != iInitialCP,
            IDS_READONLY,bReadOnly);
          break;
      }
      break;


    case IDC_TOOLBAR:

      switch(pnmh->code)
      {

        case TBN_ENDADJUST:
          UpdateToolbar();
          break;

        case TBN_QUERYDELETE:
        case TBN_QUERYINSERT:
          return TRUE;

        case TBN_GETBUTTONINFO:
          {
            if (((LPTBNOTIFY)lParam)->iItem < COUNTOF(tbbMainWnd))
            {
              char tch[256];
              GetString(tbbMainWnd[((LPTBNOTIFY)lParam)->iItem].idCommand,tch,COUNTOF(tch));
              lstrcpyn(((LPTBNOTIFY)lParam)->pszText,/*strchr(tch,'\n')+1*/tch,((LPTBNOTIFY)lParam)->cchText);
              CopyMemory(&((LPTBNOTIFY)lParam)->tbButton,&tbbMainWnd[((LPTBNOTIFY)lParam)->iItem],sizeof(TBBUTTON));
              return TRUE;
            }
          }
          return FALSE;

        case TBN_RESET:
          {
            int i; int c = SendMessage(hwndToolbar,TB_BUTTONCOUNT,0,0);
            for (i = 0; i < c; i++)
              SendMessage(hwndToolbar,TB_DELETEBUTTON,0,0);
            SendMessage(hwndToolbar,TB_ADDBUTTONS,NUMINITIALTOOLS,(LPARAM)tbbMainWnd);
            return(0);
          }

      }
      break;


    case IDC_STATUSBAR:

      switch(pnmh->code)
      {

        case NM_DBLCLK:
          {
            int i;
            LPNMMOUSE pnmm = (LPNMMOUSE)lParam;

            switch (pnmm->dwItemSpec)
            {
              case STATUS_CODEPAGE:
                if (iCodePage & NCP_UNICODE_REVERSE)
                  i = IDM_ENCODING_UNICODEREV;
                else if (iCodePage & NCP_UNICODE)
                  i = IDM_ENCODING_UNICODE;
                else if (iCodePage & NCP_UTF8_SIGN)
                  i = IDM_ENCODING_UTF8SIGN;
                else if (iCodePage & NCP_UTF8)
                  i = IDM_ENCODING_UTF8;
                else
                  i = IDM_ENCODING_ANSI;
                i++;
                if (i > IDM_ENCODING_UTF8SIGN)
                  i = IDM_ENCODING_ANSI;
                SendMessage(hwnd,WM_COMMAND,MAKELONG(i,1),0);
                return TRUE;

              case STATUS_EOLMODE:
                if (iEOLMode == SC_EOL_CRLF)
                  i = IDM_LINEENDINGS_CRLF;
                else if (iEOLMode == SC_EOL_LF)
                  i = IDM_LINEENDINGS_LF;
                else
                  i = IDM_LINEENDINGS_CR;
                i++;
                if (i > IDM_LINEENDINGS_CR)
                  i = IDM_LINEENDINGS_CRLF;
                SendMessage(hwnd,WM_COMMAND,MAKELONG(i,1),0);
                return TRUE;

              case STATUS_LEXER:
                SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_SCHEME,1),0);
                return TRUE;

              case STATUS_OVRMODE:
                SendMessage(hwndEdit,SCI_EDITTOGGLEOVERTYPE,0,0);
                return TRUE;

              default:
                return FALSE;
            }
          }
          break;

      }
      break;


    default:

      switch(pnmh->code)
      {

        case TTN_NEEDTEXT:
          {
            char tch[256];

            if (((LPTOOLTIPTEXT)lParam)->uFlags == TTF_IDISHWND)
            {
/*            if (fTitleToolTip)
              {
                PathCompactPathEx(((LPTOOLTIPTEXT)lParam)->szText,szCurDir,
                                  COUNTOF(((LPTOOLTIPTEXT)lParam)->szText),0);
              }*/
            }

            else
            {
              GetString(pnmh->idFrom,tch,COUNTOF(tch));
              lstrcpyn(((LPTOOLTIPTEXT)lParam)->szText,/*strchr(tch,'\n')+1*/tch,80);
            }
          }
          break;

      }
      break;

  }


  return(0);

}


//=============================================================================
//
//  LoadSettings()
//
//
void LoadSettings()
{
  char *pIniSection = LocalAlloc(LPTR,32*1024);
  int   cIniSection = LocalSize(pIniSection);

  LoadIniSection("Settings",pIniSection,cIniSection);

  bSaveSettings =
    IniSectionGetInt(pIniSection,"SaveSettings",1);
  if (bSaveSettings) bSaveSettings = 1;

  bSaveRecentFiles =
    IniSectionGetInt(pIniSection,"SaveRecentFiles",0);
  if (bSaveRecentFiles) bSaveRecentFiles = 1;

  bSaveFindReplace =
    IniSectionGetInt(pIniSection,"SaveFindReplace",0);
  if (bSaveFindReplace) bSaveFindReplace = 1;

  efrData.bFindClose = IniSectionGetInt(pIniSection,"CloseFind",0);
  if (efrData.bFindClose) efrData.bReplaceClose = TRUE;

  efrData.bReplaceClose = IniSectionGetInt(pIniSection,"CloseReplace",0);
  if (efrData.bReplaceClose) efrData.bReplaceClose = TRUE;

  efrData.bNoFindWrap = IniSectionGetInt(pIniSection,"NoFindWrap",0);
  if (efrData.bNoFindWrap) efrData.bNoFindWrap = TRUE;

  if (!IniSectionGetString(pIniSection,"OpenWithDir","",
        tchOpenWithDirBuf,COUNTOF(tchOpenWithDirBuf)))
    SHGetSpecialFolderPath(NULL,tchOpenWithDirBuf,CSIDL_DESKTOP,TRUE);

  ExpandEnvironmentStrings(tchOpenWithDirBuf,tchOpenWithDir,COUNTOF(tchOpenWithDir));
  if (PathIsRelative(tchOpenWithDir)) {
    char tchModule[MAX_PATH];
    GetModuleFileName(NULL,tchModule,COUNTOF(tchModule));
    PathRemoveFileSpec(tchModule);
    PathAppend(tchModule,tchOpenWithDir);
    PathCanonicalize(tchOpenWithDir,tchModule);
  }

  if (!IniSectionGetString(pIniSection,"Favorites","",
        tchFavoritesDirBuf,COUNTOF(tchFavoritesDirBuf)))
    SHGetFolderPath(NULL,CSIDL_PERSONAL,NULL,SHGFP_TYPE_CURRENT,tchFavoritesDirBuf);

  ExpandEnvironmentStrings(tchFavoritesDirBuf,tchFavoritesDir,COUNTOF(tchFavoritesDir));
  if (PathIsRelative(tchFavoritesDir)) {
    char tchModule[MAX_PATH];
    GetModuleFileName(NULL,tchModule,COUNTOF(tchModule));
    PathRemoveFileSpec(tchModule);
    PathAppend(tchModule,tchFavoritesDir);
    PathCanonicalize(tchFavoritesDir,tchModule);
  }

  iPathNameFormat = IniSectionGetInt(pIniSection,"PathNameFormat",0);
  iPathNameFormat = max(min(iPathNameFormat,2),0);

  fWordWrap = IniSectionGetInt(pIniSection,"WordWrap",0);
  if (fWordWrap) fWordWrap = 1;

  iWordWrapIndent = IniSectionGetInt(pIniSection,"WordWrapIndent",0);
  iWordWrapIndent = max(min(iWordWrapIndent,1024),0);

  iWordWrapSymbols = IniSectionGetInt(pIniSection,"WordWrapSymbols",0);
  iWordWrapSymbols = max(min(iWordWrapSymbols,3),0);

  bShowWordWrapSymbols = IniSectionGetInt(pIniSection,"ShowWordWrapSymbols",0);
  if (bShowWordWrapSymbols) bShowWordWrapSymbols = 1;

  bMatchBraces = IniSectionGetInt(pIniSection,"MatchBraces",1);
  if (bMatchBraces) bMatchBraces = 1;

  bAutoCloseTags = IniSectionGetInt(pIniSection,"AutoCloseTags",0);
  if (bAutoCloseTags) bAutoCloseTags = 1;

  bHiliteCurrentLine = IniSectionGetInt(pIniSection,"HighlightCurrentLine",0);
  if (bHiliteCurrentLine) bHiliteCurrentLine = 1;

  bAutoIndent = IniSectionGetInt(pIniSection,"AutoIndent",1);
  if (bAutoIndent) bAutoIndent = 1;

  bShowIndentGuides = IniSectionGetInt(pIniSection,"ShowIndentGuides",0);
  if (bShowIndentGuides) bShowIndentGuides = 1;

  bTabsAsSpaces = IniSectionGetInt(pIniSection,"TabsAsSpaces",1);
  if (bTabsAsSpaces) bTabsAsSpaces = 1;

  iTabWidth = IniSectionGetInt(pIniSection,"TabWidth",2);
  iTabWidth = max(min(iTabWidth,24),1);

  iIndentWidth = IniSectionGetInt(pIniSection,"IndentWidth",0);
  iIndentWidth = max(min(iIndentWidth,24),0);

  bMarkLongLines = IniSectionGetInt(pIniSection,"MarkLongLines",0);
  if (bMarkLongLines) bMarkLongLines = 1;

  iLongLinesLimit = IniSectionGetInt(pIniSection,"LongLinesLimit",72);
  iLongLinesLimit = max(min(iLongLinesLimit,999),0);

  iLongLineMode = IniSectionGetInt(pIniSection,"LongLineMode",EDGE_LINE);
  iLongLineMode = max(min(iLongLineMode,EDGE_BACKGROUND),EDGE_LINE);

  bShowSelectionMargin = IniSectionGetInt(pIniSection,"ShowSelectionMargin",0);
  if (bShowSelectionMargin) bShowSelectionMargin = 1;

  bShowLineNumbers = IniSectionGetInt(pIniSection,"ShowLineNumbers",1);
  if (bShowLineNumbers) bShowLineNumbers = 1;

  bViewWhiteSpace = IniSectionGetInt(pIniSection,"ViewWhiteSpace",0);
  if (bViewWhiteSpace) bViewWhiteSpace = 1;

  bViewEOLs = IniSectionGetInt(pIniSection,"ViewEOLs",0);
  if (bViewEOLs) bViewEOLs = 1;

  iDefaultEncoding = IniSectionGetInt(pIniSection,"DefaultEncoding",0);
  iDefaultEncoding = max(min(iDefaultEncoding,4),0);

  bSkipUnicodeDetection = IniSectionGetInt(pIniSection,"SkipUnicodeDetection",0);
  if (bSkipUnicodeDetection) bSkipUnicodeDetection = 1;

  iDefaultEOLMode = IniSectionGetInt(pIniSection,"DefaultEOLMode",0);
  iDefaultEOLMode = max(min(iDefaultEOLMode,2),0);

  bFixLineEndings = IniSectionGetInt(pIniSection,"FixLineEndings",1);
  if (bFixLineEndings) bFixLineEndings = 1;

  iPrintHeader = IniSectionGetInt(pIniSection,"PrintHeader",1);
  iPrintHeader = max(min(iPrintHeader,3),0);

  iPrintFooter = IniSectionGetInt(pIniSection,"PrintFooter",0);
  iPrintFooter = max(min(iPrintFooter,1),0);

  iPrintColor = IniSectionGetInt(pIniSection,"PrintColorMode",3);
  iPrintColor = max(min(iPrintColor,4),0);

  iPrintZoom = IniSectionGetInt(pIniSection,"PrintZoom",10)-10;
  iPrintZoom = max(min(iPrintZoom,20),-10);

  pagesetupMargin.left = IniSectionGetInt(pIniSection,"PrintMarginLeft",-1);
  pagesetupMargin.left = max(pagesetupMargin.left,-1);

  pagesetupMargin.top = IniSectionGetInt(pIniSection,"PrintMarginTop",-1);
  pagesetupMargin.top = max(pagesetupMargin.top,-1);

  pagesetupMargin.right = IniSectionGetInt(pIniSection,"PrintMarginRight",-1);
  pagesetupMargin.right = max(pagesetupMargin.right,-1);

  pagesetupMargin.bottom = IniSectionGetInt(pIniSection,"PrintMarginBottom",-1);
  pagesetupMargin.bottom = max(pagesetupMargin.bottom,-1);

  bSaveBeforeRunningTools = IniSectionGetInt(pIniSection,"SaveBeforeRunningTools",0);
  if (bSaveBeforeRunningTools) bSaveBeforeRunningTools = 1;

  bEnableFileWatching = IniSectionGetInt(pIniSection,"EnableFileWatching",0);
  if (bEnableFileWatching) bEnableFileWatching = 1;

  iEscFunction = IniSectionGetInt(pIniSection,"EscFunction",0);
  iEscFunction = max(min(iEscFunction,2),0);

  bAlwaysOnTop = IniSectionGetInt(pIniSection,"AlwaysOnTop",0);
  if (bAlwaysOnTop) bAlwaysOnTop = 1;

  bMinimizeToTray = IniSectionGetInt(pIniSection,"MinimizeToTray",0);
  if (bMinimizeToTray) bMinimizeToTray = 1;

  bTransparentMode = IniSectionGetInt(pIniSection,"TransparentMode",0);
  if (bTransparentMode) bTransparentMode = 1;

  // Check if SetLayeredWindowAttributes() is available
  bTransparentModeAvailable =
    (GetProcAddress(GetModuleHandle("User32"),"SetLayeredWindowAttributes") != NULL);

  IniSectionGetString(pIniSection,"ToolbarButtons","",
    tchToolbarButtons,COUNTOF(tchToolbarButtons));

  bShowToolbar = IniSectionGetInt(pIniSection,"ShowToolbar",1);
  if (bShowToolbar) bShowToolbar = 1;

  bShowStatusbar = IniSectionGetInt(pIniSection,"ShowStatusbar",1);
  if (bShowStatusbar) bShowStatusbar = 1;

  cxFileMRUDlg = IniSectionGetInt(pIniSection,"FileMRUDlgSizeX",412);
  cxFileMRUDlg = max(cxFileMRUDlg,0);

  cyFileMRUDlg = IniSectionGetInt(pIniSection,"FileMRUDlgSizeY",376);
  cyFileMRUDlg = max(cyFileMRUDlg,0);

  cxOpenWithDlg = IniSectionGetInt(pIniSection,"OpenWithDlgSizeX",384);
  cxOpenWithDlg = max(cxOpenWithDlg,0);

  cyOpenWithDlg = IniSectionGetInt(pIniSection,"OpenWithDlgSizeY",386);
  cyOpenWithDlg = max(cyOpenWithDlg,0);

  cxFavoritesDlg = IniSectionGetInt(pIniSection,"FavoritesDlgSizeX",334);
  cxFavoritesDlg = max(cxFavoritesDlg,0);

  cyFavoritesDlg = IniSectionGetInt(pIniSection,"FavoritesDlgSizeY",316);
  cyFavoritesDlg = max(cyFavoritesDlg,0);

  LoadIniSection("Settings2",pIniSection,cIniSection);

  bStickyWinPos = IniSectionGetInt(pIniSection,"StickyWindowPosition",0);
  if (bStickyWinPos) bStickyWinPos = 1;

  IniSectionGetString(pIniSection,"DefaultExtension","txt",
    tchDefaultExtension,COUNTOF(tchDefaultExtension));

  IniSectionGetString(pIniSection,"DefaultDirectory","",
    tchDefaultDir,COUNTOF(tchDefaultDir));

  ZeroMemory(tchFileDlgFilters,COUNTOF(tchFileDlgFilters));
  IniSectionGetString(pIniSection,"FileDlgFilters","",
    tchFileDlgFilters,COUNTOF(tchFileDlgFilters));

  iFileCheckInverval = IniSectionGetInt(pIniSection,"FileCheckInverval",2000);

  LoadIniSection("Toolbar Images",pIniSection,cIniSection);
  IniSectionGetString(pIniSection,"BitmapDefault","",
    tchToolbarBitmap,COUNTOF(tchToolbarBitmap));
  IniSectionGetString(pIniSection,"BitmapHot","",
    tchToolbarBitmapHot,COUNTOF(tchToolbarBitmap));
  IniSectionGetString(pIniSection,"BitmapDisabled","",
    tchToolbarBitmapDisabled,COUNTOF(tchToolbarBitmap));

  if (!flagPosParam /*|| bStickyWinPos*/) { // ignore window position if /p was specified

    char tchPosX[32], tchPosY[32], tchSizeX[32], tchSizeY[32], tchMaximized[32];

    int ResX = GetSystemMetrics(SM_CXSCREEN);
    int ResY = GetSystemMetrics(SM_CYSCREEN);

    wsprintf(tchPosX,"%ix%i PosX",ResX,ResY);
    wsprintf(tchPosY,"%ix%i PosY",ResX,ResY);
    wsprintf(tchSizeX,"%ix%i SizeX",ResX,ResY);
    wsprintf(tchSizeY,"%ix%i SizeY",ResX,ResY);
    wsprintf(tchMaximized,"%ix%i Maximized",ResX,ResY);

    LoadIniSection("Window",pIniSection,cIniSection);

    wi.x = IniSectionGetInt(pIniSection,tchPosX,CW_USEDEFAULT);
    wi.y = IniSectionGetInt(pIniSection,tchPosY,CW_USEDEFAULT);
    wi.cx = IniSectionGetInt(pIniSection,tchSizeX,CW_USEDEFAULT);
    wi.cy = IniSectionGetInt(pIniSection,tchSizeY,CW_USEDEFAULT);
    wi.max = IniSectionGetInt(pIniSection,tchMaximized,0);
    if (wi.max) wi.max = 1;
  }

  LocalFree(pIniSection);

  iDefaultCodePage = 0; {
    int acp = GetACP();
    if (acp == 932 || acp == 936 || acp == 949 || acp == 950 || acp == 1361)
      iDefaultCodePage = acp;
  }

  {
    CHARSETINFO ci;
    if (TranslateCharsetInfo((DWORD*)iDefaultCodePage,&ci,TCI_SRCCODEPAGE))
      iDefaultCharSet = ci.ciCharset;
    else
      iDefaultCharSet = 0;
  }

  // Scintilla Styles
  Style_Load();

}


//=============================================================================
//
//  SaveSettings()
//
//
void SaveSettings(BOOL bSaveSettingsNow)
{
  char *pIniSection = NULL;
  int   cIniSection = 0;

  if (lstrlen(szIniFile) == 0)
    return;

  if (!bSaveSettings && !bSaveSettingsNow) {
    IniSetInt("Settings","SaveSettings",bSaveSettings);
    return;
  }

  pIniSection = LocalAlloc(LPTR,32*1024);
  cIniSection = LocalSize(pIniSection);

  IniSectionSetInt(pIniSection,"SaveSettings",bSaveSettings);
  IniSectionSetInt(pIniSection,"SaveRecentFiles",bSaveRecentFiles);
  IniSectionSetInt(pIniSection,"SaveFindReplace",bSaveFindReplace);
  IniSectionSetInt(pIniSection,"CloseFind",efrData.bFindClose);
  IniSectionSetInt(pIniSection,"CloseReplace",efrData.bReplaceClose);
  IniSectionSetInt(pIniSection,"NoFindWrap",efrData.bNoFindWrap);
  IniSectionSetString(pIniSection,"OpenWithDir",tchOpenWithDirBuf);
  IniSectionSetString(pIniSection,"Favorites",tchFavoritesDirBuf);
  IniSectionSetInt(pIniSection,"PathNameFormat",iPathNameFormat);
  IniSectionSetInt(pIniSection,"WordWrap",fWordWrap);
  IniSectionSetInt(pIniSection,"WordWrapIndent",iWordWrapIndent);
  IniSectionSetInt(pIniSection,"WordWrapSymbols",iWordWrapSymbols);
  IniSectionSetInt(pIniSection,"ShowWordWrapSymbols",bShowWordWrapSymbols);
  IniSectionSetInt(pIniSection,"MatchBraces",bMatchBraces);
  IniSectionSetInt(pIniSection,"AutoCloseTags",bAutoCloseTags);
  IniSectionSetInt(pIniSection,"HighlightCurrentLine",bHiliteCurrentLine);
  IniSectionSetInt(pIniSection,"AutoIndent",bAutoIndent);
  IniSectionSetInt(pIniSection,"ShowIndentGuides",bShowIndentGuides);
  IniSectionSetInt(pIniSection,"TabsAsSpaces",bTabsAsSpaces);
  IniSectionSetInt(pIniSection,"TabWidth",iTabWidth);
  IniSectionSetInt(pIniSection,"IndentWidth",iIndentWidth);
  IniSectionSetInt(pIniSection,"MarkLongLines",bMarkLongLines);
  IniSectionSetInt(pIniSection,"LongLinesLimit",iLongLinesLimit);
  IniSectionSetInt(pIniSection,"LongLineMode",iLongLineMode);
  IniSectionSetInt(pIniSection,"ShowSelectionMargin",bShowSelectionMargin);
  IniSectionSetInt(pIniSection,"ShowLineNumbers",bShowLineNumbers);
  IniSectionSetInt(pIniSection,"ViewWhiteSpace",bViewWhiteSpace);
  IniSectionSetInt(pIniSection,"ViewEOLs",bViewEOLs);
  IniSectionSetInt(pIniSection,"DefaultEncoding",iDefaultEncoding);
  IniSectionSetInt(pIniSection,"SkipUnicodeDetection",bSkipUnicodeDetection);
  IniSectionSetInt(pIniSection,"DefaultEOLMode",iDefaultEOLMode);
  IniSectionSetInt(pIniSection,"FixLineEndings",bFixLineEndings);
  IniSectionSetInt(pIniSection,"PrintHeader",iPrintHeader);
  IniSectionSetInt(pIniSection,"PrintFooter",iPrintFooter);
  IniSectionSetInt(pIniSection,"PrintColorMode",iPrintColor);
  IniSectionSetInt(pIniSection,"PrintZoom",iPrintZoom+10);
  IniSectionSetInt(pIniSection,"PrintMarginLeft",pagesetupMargin.left);
  IniSectionSetInt(pIniSection,"PrintMarginTop",pagesetupMargin.top);
  IniSectionSetInt(pIniSection,"PrintMarginRight",pagesetupMargin.right);
  IniSectionSetInt(pIniSection,"PrintMarginBottom",pagesetupMargin.bottom);
  IniSectionSetInt(pIniSection,"SaveBeforeRunningTools",bSaveBeforeRunningTools);
  IniSectionSetInt(pIniSection,"EnableFileWatching",bEnableFileWatching);
  IniSectionSetInt(pIniSection,"EscFunction",iEscFunction);
  IniSectionSetInt(pIniSection,"AlwaysOnTop",bAlwaysOnTop);
  IniSectionSetInt(pIniSection,"MinimizeToTray",bMinimizeToTray);
  IniSectionSetInt(pIniSection,"TransparentMode",bTransparentMode);
  Toolbar_GetButtons(hwndToolbar,IDT_FILE_NEW,tchToolbarButtons,COUNTOF(tchToolbarButtons));
  IniSectionSetString(pIniSection,"ToolbarButtons",tchToolbarButtons);
  IniSectionSetInt(pIniSection,"ShowToolbar",bShowToolbar);
  IniSectionSetInt(pIniSection,"ShowStatusbar",bShowStatusbar);
  IniSectionSetInt(pIniSection,"FileMRUDlgSizeX",cxFileMRUDlg);
  IniSectionSetInt(pIniSection,"FileMRUDlgSizeY",cyFileMRUDlg);
  IniSectionSetInt(pIniSection,"OpenWithDlgSizeX",cxOpenWithDlg);
  IniSectionSetInt(pIniSection,"OpenWithDlgSizeY",cyOpenWithDlg);
  IniSectionSetInt(pIniSection,"FavoritesDlgSizeX",cxFavoritesDlg);
  IniSectionSetInt(pIniSection,"FavoritesDlgSizeY",cyFavoritesDlg);

  SaveIniSection("Settings",pIniSection);
  LocalFree(pIniSection);

  /*
    SaveSettingsNow(): query Window Dimensions
  */

  if (bSaveSettingsNow)
  {
    WINDOWPLACEMENT wndpl;

    // GetWindowPlacement
    wndpl.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hwndMain,&wndpl);

    wi.x = wndpl.rcNormalPosition.left;
    wi.y = wndpl.rcNormalPosition.top;
    wi.cx = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
    wi.cy = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
    wi.max = (IsZoomed(hwndMain) || (wndpl.flags & WPF_RESTORETOMAXIMIZED));
  }

  if (!IniGetInt("Settings2","StickyWindowPosition",0)) {

    char tchPosX[32], tchPosY[32], tchSizeX[32], tchSizeY[32], tchMaximized[32];

    int ResX = GetSystemMetrics(SM_CXSCREEN);
    int ResY = GetSystemMetrics(SM_CYSCREEN);

    wsprintf(tchPosX,"%ix%i PosX",ResX,ResY);
    wsprintf(tchPosY,"%ix%i PosY",ResX,ResY);
    wsprintf(tchSizeX,"%ix%i SizeX",ResX,ResY);
    wsprintf(tchSizeY,"%ix%i SizeY",ResX,ResY);
    wsprintf(tchMaximized,"%ix%i Maximized",ResX,ResY);

    IniSetInt("Window",tchPosX,wi.x);
    IniSetInt("Window",tchPosY,wi.y);
    IniSetInt("Window",tchSizeX,wi.cx);
    IniSetInt("Window",tchSizeY,wi.cy);
    IniSetInt("Window",tchMaximized,wi.max);
  }

  // Scintilla Styles
  Style_Save();

}


//=============================================================================
//
//  ParseCommandLine()
//
//
void ParseCommandLine(LPSTR lpCmdLine)
{

  LPSTR lp1,lp2,lp3;
  BOOL bContinue = TRUE;

  if (lstrlen(lpCmdLine) == 0)
    return;

  // Good old console can also send args separated by Tabs
  StrTab2Space(lpCmdLine);

  lp1 = LocalAlloc(LPTR,lstrlen(lpCmdLine) + 1);
  lp2 = LocalAlloc(LPTR,lstrlen(lpCmdLine) + 1);
  lp3 = LocalAlloc(LPTR,lstrlen(lpCmdLine) + 1);

  lstrcpy(lp3,lpCmdLine);

  while (bContinue && ExtractFirstArgument(lp3,lp1,lp2))
  {

    // options
    if ((*lp1 == '/') || (*lp1 == '-'))
    {

      switch (*CharUpper(lp1+1))
      {

        case 'N':
          flagNoReuseWindow = 1;
          flagSingleFileInstance = 0;
          break;

        case 'F':
          if (*(lp1+2) == '0' || *CharUpper(lp1+2) == 'O')
            lstrcpy(szIniFile,"*?");
          else if (ExtractFirstArgument(lp2,lp1,lp2)) {
            StrCpyN(szIniFile,lp1,COUNTOF(szIniFile));
            TrimString(szIniFile);
            PathUnquoteSpaces(szIniFile);
          }
          break;

        case 'I':
          flagStartAsTrayIcon = 1;
          break;

        case 'P':
          if (*(lp1+2) == '0' || *CharUpper(lp1+2) == 'O') {
            flagPosParam = 1;
            flagDefaultPos = 1;
          }
          else if (ExtractFirstArgument(lp2,lp1,lp2)) {
            int itok =
              sscanf(lp1,"%i,%i,%i,%i,%i",&wi.x,&wi.y,&wi.cx,&wi.cy,&wi.max);
            if (itok == 4 || itok == 5) { // scan successful
              flagPosParam = 1;
              if (wi.cx < 1) wi.cx = CW_USEDEFAULT;
              if (wi.cy < 1) wi.cy = CW_USEDEFAULT;
              if (wi.max) wi.max = 1;
              if (itok == 4) wi.max = 0;
            }
          }
          break;

        case 'C':
          flagNewFromClipboard = 1;
          break;

        case 'B':
          flagPasteBoard = 1;
          break;

        case 'G':
          if (ExtractFirstArgument(lp2,lp1,lp2)) {
            int itok =
              sscanf(lp1,"%i,%i",&iInitialLine,&iInitialColumn);
            if (itok == 1 || itok == 2) { // scan successful
              flagJumpTo = 1;
            }
          }
          break;

        case 'S':
          if (ExtractFirstArgument(lp2,lp1,lp2)) {
            int itok =
              sscanf(lp1,"%i",&iInitialLexer);
            if (itok == 1) { // scan successful
              flagLexerSpecified = 1;
            }
          }
          break;

        case 'T':
          iInitialLexer = 0;
          flagLexerSpecified = 1;
          break;

        case 'H':
          iInitialLexer = 1;
          flagLexerSpecified = 1;
          break;

        case 'X':
          iInitialLexer = 2;
          flagLexerSpecified = 1;
          break;

        case '?':
          flagDisplayHelp = 1;
          break;

        default:
          break;

      }

    }

    // pathname
    else
    {
      if (lpFileArg)
        GlobalFree(lpFileArg);

      lpFileArg = GlobalAlloc(GPTR,MAX_PATH+2); // changed for ActivatePrevInst() needs
      lstrcpy(lpFileArg,lp3);

      TrimString(lpFileArg);
      PathUnquoteSpaces(lpFileArg);

      // only one quote, adjust blanks ...
      if (*lpFileArg == '\"') {
        *lpFileArg = ' ';
        TrimString(lpFileArg);
      }

      bContinue = FALSE;
    }

    // Continue with next argument
    if (bContinue)
      lstrcpy(lp3,lp2);

  }

  LocalFree(lp1);
  LocalFree(lp2);
  LocalFree(lp3);

}


//=============================================================================
//
//  LoadFlags()
//
//
void LoadFlags()
{
  char *pIniSection = LocalAlloc(LPTR,32*1024);
  int   cIniSection = LocalSize(pIniSection);

  LoadIniSection("Settings2",pIniSection,cIniSection);

  if (!flagNoReuseWindow) {

    if (!IniSectionGetInt(pIniSection,"ReuseWindow",0))
      flagNoReuseWindow = 1;

    if (IniSectionGetInt(pIniSection,"SingleFileInstance",0))
      flagSingleFileInstance = 1;
  }

  if (IniSectionGetInt(pIniSection,"NoFadeHidden",0))
    flagNoFadeHidden = 1;

  if (IniSectionGetInt(pIniSection,"SimpleIndentGuides",0))
    flagSimpleIndentGuides = 1;

  LocalFree(pIniSection);
}


//=============================================================================
//
//  FindIniFile()
//
//
int FindIniFile() {

  char tchModule[MAX_PATH], tch1[MAX_PATH], tch2[MAX_PATH];

  GetModuleFileName(NULL,tchModule,COUNTOF(tchModule));

  if (lstrlen(szIniFile)) {
    if (lstrcmpi(szIniFile,"*?") == 0) {
      lstrcpy(szIniFile,"");
      return(0);
    }
    else {
      ExpandEnvironmentStrings(szIniFile,tch1,COUNTOF(tch1));
      if (PathIsRelative(tch1)) {
        lstrcpy(tch2,tchModule);
        lstrcpy(PathFindFileName(tch2),tch1);
        if (PathFileExists(tch2))
          lstrcpy(szIniFile,tch2);
        else {
          if (SearchPath(NULL,tch1,NULL,COUNTOF(tch2),tch2,NULL))
            lstrcpy(szIniFile,tch2);
          else
            lstrcpy(szIniFile,tch2);
        }
      }
      else {
        if (PathIsDirectory(tch1)) {
          PathAppend(tch1,PathFindFileName(tchModule));
          lstrcpy(PathFindFileName(tch1),"Notepad2.ini");
          lstrcpy(szIniFile,tch1);
        }
        else
          lstrcpy(szIniFile,tch1);
      }
    }
    return(1);
  }

  lstrcpy(tch1,tchModule);
  PathRenameExtension(tch1,".ini");
  if (PathFileExists(tch1)) {
    if (GetPrivateProfileString("Notepad2","Notepad2.ini","",tch2,COUNTOF(tch2),tch1)) {
      ExpandEnvironmentStringsEx(tch2,COUNTOF(tch2));
      if (PathIsRelative(tch2)) {
        lstrcpy(szIniFile,tchModule);
        lstrcpy(PathFindFileName(szIniFile),tch2);
      }
      else
        lstrcpy(szIniFile,tch2);
    }
    else
      lstrcpy(szIniFile,tch1);
    return(1);
  }

  lstrcpy(tch1,tchModule);
  lstrcpy(PathFindFileName(tch1),"Notepad2.ini");
  if (PathFileExists(tch1)) {
    if (GetPrivateProfileString("Notepad2","Notepad2.ini","",tch2,COUNTOF(tch2),tch1)) {
      ExpandEnvironmentStringsEx(tch2,COUNTOF(tch2));
      if (PathIsRelative(tch2)) {
        lstrcpy(szIniFile,tchModule);
        lstrcpy(PathFindFileName(szIniFile),tch2);
      }
      else
        lstrcpy(szIniFile,tch2);
    }
    else
      lstrcpy(szIniFile,tch1);
    return(1);
  }

  lstrcpy(tch2,PathFindFileName(tchModule));
  PathRenameExtension(tch2,".ini");
  if (SearchPath(NULL,tch2,NULL,COUNTOF(tch1),tch1,NULL)) {
    if (GetPrivateProfileString("Notepad2","Notepad2.ini","",tch2,COUNTOF(tch2),tch1)) {
      ExpandEnvironmentStringsEx(tch2,COUNTOF(tch2));
      if (PathIsRelative(tch2)) {
        lstrcpy(szIniFile,tchModule);
        lstrcpy(PathFindFileName(szIniFile),tch2);
      }
      else
        lstrcpy(szIniFile,tch2);
    }
    else
      lstrcpy(szIniFile,tch1);
    return(1);
  }

  if (SearchPath(NULL,"Notepad2.ini",NULL,COUNTOF(tch1),tch1,NULL)) {
    if (GetPrivateProfileString("Notepad2","Notepad2.ini","",tch2,COUNTOF(tch2),tch1)) {
      ExpandEnvironmentStringsEx(tch2,COUNTOF(tch2));
      if (PathIsRelative(tch2)) {
        lstrcpy(szIniFile,tchModule);
        lstrcpy(PathFindFileName(szIniFile),tch2);
      }
      else
        lstrcpy(szIniFile,tch2);
    }
    else
      lstrcpy(szIniFile,tch1);

    return(1);
  }

  lstrcpy(szIniFile,tchModule);
  PathRenameExtension(szIniFile,".ini");

  return(1);
}


//=============================================================================
//
//  UpdateToolbar()
//
//
#define EnableTool(id,b) SendMessage(hwndToolbar,TB_ENABLEBUTTON,id, \
                           MAKELONG(((b) ? 1 : 0), 0))

#define CheckTool(id,b)  SendMessage(hwndToolbar,TB_CHECKBUTTON,id, \
                           MAKELONG(b,0))

void UpdateToolbar()
{

  int i;

  if (!bShowToolbar)
    return;

  EnableTool(IDT_FILE_ADDTOFAV,lstrlen(szCurFile));

  EnableTool(IDT_EDIT_UNDO,SendMessage(hwndEdit,SCI_CANUNDO,0,0) /*&& !bReadOnly*/);
  EnableTool(IDT_EDIT_REDO,SendMessage(hwndEdit,SCI_CANREDO,0,0) /*&& !bReadOnly*/);

  i = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) - SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
  EnableTool(IDT_EDIT_CUT,i /*&& !bReadOnly*/);
  EnableTool(IDT_EDIT_COPY,SendMessage(hwndEdit,SCI_GETLENGTH,0,0));
  EnableTool(IDT_EDIT_PASTE,SendMessage(hwndEdit,SCI_CANPASTE,0,0) /*&& !bReadOnly*/);

  i = SendMessage(hwndEdit,SCI_GETLENGTH,0,0);
  EnableTool(IDT_EDIT_FIND,i);
  //EnableTool(IDT_EDIT_FINDNEXT,i);
  //EnableTool(IDT_EDIT_FINDPREV,i && lstrlen(efrData.szFind));
  EnableTool(IDT_EDIT_REPLACE,i /*&& !bReadOnly*/);
  EnableTool(IDT_EDIT_CLEAR,i /*&& !bReadOnly*/);

  CheckTool(IDT_VIEW_WORDWRAP,fWordWrap);

}


//=============================================================================
//
//  UpdateStatusbar()
//
//
void UpdateStatusbar()
{

  int iPos;
  int iLn;
  int iLines;
  int iCol;
  int iSel;
  char tchLn[32];
  char tchLines[32];
  char tchCol[32];
  char tchCols[32];
  char tchSel[32];
  char tchDocPos[256];

  int iBytes;
  char tchBytes[64];
  char tchDocSize[256];

  char tchCodePage[32];
  char tchEOLMode[32];

  char tchOvrMode[32];

  if (!bShowStatusbar)
    return;

  iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);

  iLn = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,iPos,0) + 1;
  wsprintf(tchLn,"%i",iLn);
  FormatNumberStr(tchLn);

  iLines = SendMessage(hwndEdit,SCI_GETLINECOUNT,0,0);
  wsprintf(tchLines,"%i",iLines);
  FormatNumberStr(tchLines);

  iCol = SendMessage(hwndEdit,SCI_GETCOLUMN,iPos,0) + 1;
  wsprintf(tchCol,"%i",iCol);
  FormatNumberStr(tchCol);

  if (bMarkLongLines) {
    wsprintf(tchCols,"%i",iLongLinesLimit);
    FormatNumberStr(tchCols);
  }

  if (SC_SEL_RECTANGLE != SendMessage(hwndEdit,SCI_GETSELECTIONMODE,0,0))
  {
    iSel = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) - SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
    wsprintf(tchSel,"%i",iSel);
    FormatNumberStr(tchSel);
  }
  else
    lstrcpy(tchSel,"--");

  if (!bMarkLongLines)
    FormatString(tchDocPos,COUNTOF(tchDocPos),IDS_DOCPOS,tchLn,tchLines,tchCol,tchSel);
  else
    FormatString(tchDocPos,COUNTOF(tchDocPos),IDS_DOCPOS2,tchLn,tchLines,tchCol,tchCols,tchSel);

  iBytes = SendMessage(hwndEdit,SCI_GETLENGTH,0,0);
  FormatBytes(tchBytes,COUNTOF(tchBytes),iBytes);

  FormatString(tchDocSize,COUNTOF(tchDocSize),IDS_DOCSIZE,tchBytes);

  if (iCodePage & NCP_UNICODE)
  {
    lstrcpy(tchCodePage,"Unicode");
    if (iCodePage & NCP_UNICODE_REVERSE)
      lstrcat(tchCodePage," BE");
    if (iCodePage & NCP_UNICODE_BOM)
      lstrcat(tchCodePage," BOM");
  }
  else if (iCodePage & NCP_UTF8)
  {
    lstrcpy(tchCodePage,"UTF-8");
    if (iCodePage & NCP_UTF8_SIGN)
      lstrcat(tchCodePage," Signature");
  }
  else
    lstrcpy(tchCodePage,"ANSI");

  if (iEOLMode == SC_EOL_CR)
    lstrcpy(tchEOLMode,"CR");
  else if (iEOLMode == SC_EOL_LF)
    lstrcpy(tchEOLMode,"LF");
  else
    lstrcpy(tchEOLMode,"CR+LF");

  if (SendMessage(hwndEdit,SCI_GETOVERTYPE,0,0))
    lstrcpy(tchOvrMode,"OVR");
  else
    lstrcpy(tchOvrMode,"INS");

  StatusSetText(hwndStatus,STATUS_DOCPOS,tchDocPos);
  StatusSetText(hwndStatus,STATUS_DOCSIZE,tchDocSize);
  StatusSetText(hwndStatus,STATUS_CODEPAGE,tchCodePage);
  StatusSetText(hwndStatus,STATUS_EOLMODE,tchEOLMode);
  StatusSetText(hwndStatus,STATUS_OVRMODE,tchOvrMode);
  StatusSetText(hwndStatus,STATUS_LEXER,Style_GetCurrentLexerName());

  //InvalidateRect(hwndStatus,NULL,TRUE);

}


//=============================================================================
//
//  UpdateLineNumberWidth()
//
//
void UpdateLineNumberWidth()
{
  char tchLines[32];
  int  iLineMarginWidthNow;
  int  iLineMarginWidthFit;

  if (bShowLineNumbers) {

    wsprintf(tchLines,"_%i_",SendMessage(hwndEdit,SCI_GETLINECOUNT,0,0));

    iLineMarginWidthNow = SendMessage(hwndEdit,SCI_GETMARGINWIDTHN,0,0);
    iLineMarginWidthFit = SendMessage(hwndEdit,SCI_TEXTWIDTH,STYLE_LINENUMBER,(LPARAM)tchLines);

    if (iLineMarginWidthNow != iLineMarginWidthFit) {
      //SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,0,0);
      SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,0,iLineMarginWidthFit);
    }
  }

  else
    SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,0,0);
}


//=============================================================================
//
//  FileIO()
//
//
BOOL FileIO(BOOL fLoad,LPCSTR psz,BOOL bNoEncDetect,int *icp, int *ieol,
            BOOL *pbUnicodeErr,BOOL *pbFileTooBig,BOOL bSaveCopy)
{
  SHFILEINFO shfi;
  char tch[MAX_PATH+40];
  BOOL fSuccess;
  DWORD dwFileAttributes;

  BeginWaitCursor();

  SHGetFileInfo2(psz,0,&shfi,sizeof(SHFILEINFO),SHGFI_DISPLAYNAME);
  FormatString(tch,COUNTOF(tch),(fLoad) ? IDS_LOADFILE : IDS_SAVEFILE,shfi.szDisplayName);

  StatusSetText(hwndStatus,STATUS_HELP,tch);
  StatusSetSimple(hwndStatus,TRUE);

  InvalidateRect(hwndStatus,NULL,TRUE);
  UpdateWindow(hwndStatus);

  if (fLoad)
    fSuccess = EditLoadFile(hwndEdit,psz,bNoEncDetect,icp,ieol,pbUnicodeErr,pbFileTooBig);
  else
    fSuccess = EditSaveFile(hwndEdit,psz,*icp,bSaveCopy);

  dwFileAttributes = GetFileAttributes(psz);
  bReadOnly = (dwFileAttributes != INVALID_FILE_ATTRIBUTES && dwFileAttributes & FILE_ATTRIBUTE_READONLY);

  StatusSetSimple(hwndStatus,FALSE);

  EndWaitCursor();

  return(fSuccess);
}


//=============================================================================
//
//  FileLoad()
//
//
BOOL FileLoad(BOOL bDontSave,BOOL bNew,BOOL bReload,BOOL bNoEncDetect,LPCSTR lpszFile)
{
  char tch[MAX_PATH] = "";
  char szFileName[MAX_PATH] = "";
  BOOL fSuccess;
  BOOL bUnicodeErr = FALSE;
  BOOL bFileTooBig = FALSE;

  if (!bDontSave)
  {
    if (!FileSave(FALSE,TRUE,FALSE,FALSE))
      return FALSE;
  }

  if (bNew) {
    lstrcpy(szCurFile,"");
    SetDlgItemText(hwndMain,IDC_FILENAME,szCurFile);
    EditSetNewText(hwndEdit,"",0);
    Style_SetLexer(hwndEdit,NULL);
    UpdateLineNumberWidth();
    bModified = FALSE;
    bReadOnly = FALSE;
    iEOLMode = iLineEndings[iDefaultEOLMode];
    SendMessage(hwndEdit,SCI_SETEOLMODE,iLineEndings[iDefaultEOLMode],0);
    iCodePage = iEncodings[iDefaultEncoding];
    iInitialCP = iEncodings[iDefaultEncoding];
    SendMessage(hwndEdit,SCI_SETCODEPAGE,(iDefaultEncoding == 0) ? iDefaultCodePage : SC_CP_UTF8,0);
    EditSetNewText(hwndEdit,"",0);
    SetWindowTitle(hwndMain,uidsAppTitle,IDS_UNTITLED,szCurFile,
      iPathNameFormat,bModified || iCodePage != iInitialCP,
      IDS_READONLY,bReadOnly);

    // Terminate file watching
    InstallFileWatching(NULL);

    return TRUE;
  }

  if (!lpszFile || lstrlen(lpszFile) == 0) {
    if (!OpenFileDlg(hwndMain,tch,COUNTOF(tch),NULL))
      return FALSE;
  }

  else
    lstrcpy(tch,lpszFile);

  ExpandEnvironmentStringsEx(tch,COUNTOF(tch));

  if (PathIsRelative(tch)) {
    GetCurrentDirectory(COUNTOF(szFileName),szFileName);
    PathAppend(szFileName,tch);
    if (!PathFileExists(szFileName)) {
      if (!SearchPath(NULL,tch,NULL,COUNTOF(szFileName),szFileName,NULL)) {
        lstrcpy(szFileName,tch);
      }
    }
  }

  else
    lstrcpy(szFileName,tch);

  PathCanonicalizeEx(szFileName);
  GetLongPathNameEx(szFileName,szFileName,COUNTOF(szFileName));

  if (PathIsLnkFile(szFileName))
    PathGetLnkPath(szFileName,szFileName,COUNTOF(szFileName));

  // Ask to create a new file...
  if (!bReload && !PathFileExists(szFileName))
  {
    if (MsgBox(MBYESNO,IDS_ASK_CREATE,szFileName) == IDYES) {
      HANDLE hFile = CreateFile(szFileName,
                      GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
                      NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
      if (fSuccess = (hFile != INVALID_HANDLE_VALUE)) {
        CloseHandle(hFile);
        EditSetNewText(hwndEdit,"",0);
        Style_SetLexer(hwndEdit,NULL);
        iEOLMode = iLineEndings[iDefaultEOLMode];
        SendMessage(hwndEdit,SCI_SETEOLMODE,iLineEndings[iDefaultEOLMode],0);
        iCodePage = iEncodings[iDefaultEncoding];
        iInitialCP = iEncodings[iDefaultEncoding];
        SendMessage(hwndEdit,SCI_SETCODEPAGE,(iDefaultEncoding == 0) ? iDefaultCodePage : SC_CP_UTF8,0);
        bReadOnly = FALSE;
        EditSetNewText(hwndEdit,"",0);
      }
    }
    else
      return FALSE;
  }

  else
    fSuccess = FileIO(TRUE,szFileName,bNoEncDetect,&iCodePage,&iEOLMode,&bUnicodeErr,&bFileTooBig,FALSE);

  if (fSuccess) {
    lstrcpy(szCurFile,szFileName);
    SetDlgItemText(hwndMain,IDC_FILENAME,szCurFile);
    Style_SetLexerFromFile(hwndEdit,szCurFile);
    UpdateLineNumberWidth();
    iInitialCP = iCodePage;
    bModified = FALSE;
    //bReadOnly = FALSE;
    SendMessage(hwndEdit,SCI_SETEOLMODE,iEOLMode,0);
    MRU_Add(pFileMRU,szFileName);
    SetWindowTitle(hwndMain,uidsAppTitle,IDS_UNTITLED,szFileName,
      iPathNameFormat,bModified || iCodePage != iInitialCP,
      IDS_READONLY,bReadOnly);

    // Install watching of the current file
    InstallFileWatching(szCurFile);

    // the .LOG feature ...
    if (SendMessage(hwndEdit,SCI_GETLENGTH,0,0) >= 4) {
      char tchLog[5] = "";
      SendMessage(hwndEdit,SCI_GETTEXT,5,(LPARAM)tchLog);
      if (lstrcmp(tchLog,".LOG") == 0) {
        EditJumpTo(hwndEdit,-1,0);
        SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
        SendMessage(hwndEdit,SCI_NEWLINE,0,0);
        SendMessage(hwndMain,WM_COMMAND,MAKELONG(IDM_EDIT_INSERT_SHORTDATE,1),0);
        SendMessage(hwndEdit,SCI_NEWLINE,0,0);
        SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
        EditJumpTo(hwndEdit,-1,0);
      }
    }

    // Show warning: Unicode file loaded as ANSI
    if (bUnicodeErr)
      MsgBox(MBWARN,IDS_ERR_UNICODE);
  }

  else if (!bFileTooBig)
    MsgBox(MBWARN,IDS_ERR_LOADFILE,szFileName);

  return(fSuccess);
}


//=============================================================================
//
//  FileSave()
//
//
BOOL FileSave(BOOL bSaveAlways,BOOL bAsk,BOOL bSaveAs,BOOL bSaveCopy)
{
  char tchFile[MAX_PATH];
  BOOL fSuccess = FALSE;

  BOOL bIsEmptyNewFile = FALSE;
  if (lstrlen(szCurFile) == 0) {
    int cchText = SendMessage(hwndEdit,SCI_GETLENGTH,0,0);
    if (cchText == 0)
      bIsEmptyNewFile = TRUE;
    else if (cchText < 1023) {
      char tchText[1024];
      SendMessage(hwndEdit,SCI_GETTEXT,(WPARAM)1023,(LPARAM)tchText);
      StrTrim(tchText," \t\n\r");
      if (lstrlen(tchText) == 0)
        bIsEmptyNewFile = TRUE;
    }
  }

  if (!bSaveAlways && (!bModified && iCodePage == iInitialCP || bIsEmptyNewFile) && !bSaveAs)
    return TRUE;

  if (bAsk)
  {
    // File or "Untitled" ...
    char tch[MAX_PATH];
    if (lstrlen(szCurFile))
      lstrcpy(tch,szCurFile);
    else
      GetString(IDS_UNTITLED,tch,COUNTOF(tch));

    switch (MsgBox(MBYESNOCANCEL,IDS_ASK_SAVE,tch)) {
      case IDCANCEL:
        return FALSE;
      case IDNO:
        return TRUE;
    }
  }

  // Read only...
  if (!bSaveAs && !bSaveCopy && lstrlen(szCurFile))
  {
    DWORD dwFileAttributes = GetFileAttributes(szCurFile);
    if (dwFileAttributes != INVALID_FILE_ATTRIBUTES)
      bReadOnly = (dwFileAttributes & FILE_ATTRIBUTE_READONLY);
    if (bReadOnly) {
      SetWindowTitle(hwndMain,uidsAppTitle,IDS_UNTITLED,szCurFile,
        iPathNameFormat,bModified || iCodePage != iInitialCP,
        IDS_READONLY,bReadOnly);
      if (MsgBox(MBYESNOWARN,IDS_READONLY_SAVE,szCurFile) == IDYES)
        bSaveAs = TRUE;
      else
        return FALSE;
    }
  }

  // Save As...
  if (bSaveAs || bSaveCopy || lstrlen(szCurFile) == 0)
  {
    lstrcpy(tchFile,szCurFile);
    if (SaveFileDlg(hwndMain,tchFile,COUNTOF(tchFile)))
    {
      if (fSuccess = FileIO(FALSE,tchFile,FALSE,&iCodePage,&iEOLMode,NULL,NULL,bSaveCopy))
      {
        if (!bSaveCopy)
        {
          lstrcpy(szCurFile,tchFile);
          SetDlgItemText(hwndMain,IDC_FILENAME,szCurFile);
          Style_SetLexerFromFile(hwndEdit,szCurFile);
          UpdateStatusbar();
          UpdateLineNumberWidth();
        }
      }
    }
    else
      return FALSE;
  }

  else
    fSuccess = FileIO(FALSE,szCurFile,FALSE,&iCodePage,&iEOLMode,NULL,NULL,FALSE);

  if (fSuccess)
  {
    if (!bSaveCopy)
    {
      bModified = FALSE;
      iInitialCP = iCodePage;
      MRU_Add(pFileMRU,szCurFile);
      SetWindowTitle(hwndMain,uidsAppTitle,IDS_UNTITLED,szCurFile,
        iPathNameFormat,bModified || iCodePage != iInitialCP,
        IDS_READONLY,bReadOnly);

      // Install watching of the current file
      InstallFileWatching(szCurFile);
    }
  }

  else
  {
    if (lstrlen(szCurFile) != 0)
      lstrcpy(tchFile,szCurFile);

    SetWindowTitle(hwndMain,uidsAppTitle,IDS_UNTITLED,szCurFile,
      iPathNameFormat,bModified || iCodePage != iInitialCP,
      IDS_READONLY,bReadOnly);

    MsgBox(MBWARN,IDS_ERR_SAVEFILE,tchFile);
  }

  return(fSuccess);
}


//=============================================================================
//
//  OpenFileDlg()
//
//
BOOL OpenFileDlg(HWND hwnd,LPSTR lpstrFile,int cchFile,LPCSTR lpstrInitialDir)
{
  OPENFILENAME ofn;
  char szFile[MAX_PATH];
  char szFilter[NUMLEXERS*1024];
  char tchInitialDir[MAX_PATH];

  lstrcpy(szFile,"");
  Style_GetOpenDlgFilterStr(szFilter,COUNTOF(szFilter));

  if (!lpstrInitialDir) {
    if (lstrlen(szCurFile)) {
      lstrcpy(tchInitialDir,szCurFile);
      PathRemoveFileSpec(tchInitialDir);
    }
    else if (lstrlen(tchDefaultDir)) {
      ExpandEnvironmentStrings(tchDefaultDir,tchInitialDir,COUNTOF(tchInitialDir));
      if (PathIsRelative(tchInitialDir)) {
        char tchModule[MAX_PATH];
        GetModuleFileName(NULL,tchModule,COUNTOF(tchModule));
        PathRemoveFileSpec(tchModule);
        PathAppend(tchModule,tchInitialDir);
        PathCanonicalize(tchInitialDir,tchModule);
      }
    }
  }

  ZeroMemory(&ofn,sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = szFilter;
  ofn.lpstrFile = szFile;
  ofn.lpstrInitialDir = (lpstrInitialDir) ? lpstrInitialDir : tchInitialDir;
  ofn.nMaxFile = COUNTOF(szFile);
  ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | /* OFN_NOCHANGEDIR |*/
            /*OFN_DONTADDTORECENT*/0x02000000 | OFN_PATHMUSTEXIST |
              OFN_SHAREAWARE /*| OFN_NODEREFERENCELINKS*/;
  ofn.lpstrDefExt = (lstrlen(tchDefaultExtension)) ? tchDefaultExtension : NULL;

  if (GetOpenFileName(&ofn)) {
    lstrcpyn(lpstrFile,szFile,cchFile);
    return TRUE;
  }

  else
    return FALSE;
}


//=============================================================================
//
//  SaveFileDlg()
//
//
BOOL SaveFileDlg(HWND hwnd,LPSTR lpstrFile,int cchFile)
{
  OPENFILENAME ofn;
  char szNewFile[MAX_PATH];
  char szFilter[NUMLEXERS*1024];
  char tchInitialDir[MAX_PATH];

  lstrcpy(szNewFile,lpstrFile);
  Style_GetOpenDlgFilterStr(szFilter,COUNTOF(szFilter));

  if (lstrlen(szCurFile)) {
    lstrcpy(tchInitialDir,szCurFile);
    PathRemoveFileSpec(tchInitialDir);
  }
  else if (lstrlen(tchDefaultDir)) {
    ExpandEnvironmentStrings(tchDefaultDir,tchInitialDir,COUNTOF(tchInitialDir));
    if (PathIsRelative(tchInitialDir)) {
      char tchModule[MAX_PATH];
      GetModuleFileName(NULL,tchModule,COUNTOF(tchModule));
      PathRemoveFileSpec(tchModule);
      PathAppend(tchModule,tchInitialDir);
      PathCanonicalize(tchInitialDir,tchModule);
    }
  }

  ZeroMemory(&ofn,sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = szFilter;
  ofn.lpstrFile = szNewFile;
  ofn.lpstrInitialDir = tchInitialDir;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_HIDEREADONLY /*| OFN_NOCHANGEDIR*/ |
            /*OFN_NODEREFERENCELINKS |*/ OFN_OVERWRITEPROMPT |
            /*OFN_DONTADDTORECENT*/0x02000000 | OFN_PATHMUSTEXIST;
  ofn.lpstrDefExt = (lstrlen(tchDefaultExtension)) ? tchDefaultExtension : NULL;

  if (GetSaveFileName(&ofn)) {
    lstrcpyn(lpstrFile,szNewFile,cchFile);
    return TRUE;
  }

  else
    return FALSE;
}


/******************************************************************************
*
* ActivatePrevInst()
*
* Tries to find and activate an already open Notepad2 Window
*
*
******************************************************************************/
BOOL CALLBACK EnumWndProc(HWND hwnd,LPARAM lParam)
{
  BOOL bContinue = TRUE;
  char szClassName[64];

  if (GetClassName(hwnd,szClassName,COUNTOF(szClassName)))

    if (lstrcmpi(szClassName,WC_NOTEPAD2) == 0) {

      *(HWND*)lParam = hwnd;

      if (IsWindowEnabled(hwnd))
        bContinue = FALSE;
    }
  return(bContinue);
}

BOOL CALLBACK EnumWndProc2(HWND hwnd,LPARAM lParam)
{
  BOOL bContinue = TRUE;
  char szClassName[64];

  if (GetClassName(hwnd,szClassName,COUNTOF(szClassName)))

    if (lstrcmpi(szClassName,WC_NOTEPAD2) == 0) {

      char tchFileName[MAX_PATH] = "";

      if (IsWindowEnabled(hwnd))
        bContinue = FALSE;

      GetDlgItemText(hwnd,IDC_FILENAME,tchFileName,COUNTOF(tchFileName));
      if (lstrcmpi(tchFileName,lpFileArg) == 0)
        *(HWND*)lParam = hwnd;
      else
        bContinue = TRUE;
    }
  return(bContinue);
}

BOOL ActivatePrevInst()
{
  HWND hwnd = NULL;
  COPYDATASTRUCT cds;

  if ((flagNoReuseWindow && !flagSingleFileInstance) || flagStartAsTrayIcon || flagNewFromClipboard || flagPasteBoard)
    return(FALSE);

  if (flagSingleFileInstance && lpFileArg) {

    // Search working directory from second instance, first!
    // lpFileArg is at least MAX_PATH+2 bytes
    char tchTmp[MAX_PATH];

    ExpandEnvironmentStringsEx(lpFileArg,GlobalSize(lpFileArg));

    if (PathIsRelative(lpFileArg)) {
      GetCurrentDirectory(COUNTOF(tchTmp),tchTmp);
      PathAppend(tchTmp,lpFileArg);
      if (PathFileExists(tchTmp))
        lstrcpy(lpFileArg,tchTmp);
      else {
        if (SearchPath(NULL,lpFileArg,NULL,COUNTOF(tchTmp),tchTmp,NULL))
          lstrcpy(lpFileArg,tchTmp);
      }
    }

    else if (SearchPath(NULL,lpFileArg,NULL,COUNTOF(tchTmp),tchTmp,NULL))
      lstrcpy(lpFileArg,tchTmp);

    EnumWindows(EnumWndProc2,(LPARAM)&hwnd);

    if (hwnd != NULL)
    {
      // Enabled
      if (IsWindowEnabled(hwnd))
      {
        // Make sure the previous window won't pop up a change notification message
        //SendMessage(hwnd,WM_CHANGENOTIFYCLEAR,0,0);

        if (IsIconic(hwnd))
          ShowWindowAsync(hwnd,SW_RESTORE);

        if (!IsWindowVisible(hwnd)) {
          SendMessage(hwnd,WM_TRAYMESSAGE,0,WM_LBUTTONDBLCLK);
          SendMessage(hwnd,WM_TRAYMESSAGE,0,WM_LBUTTONUP);
        }

        SetForegroundWindow(hwnd);

        return(TRUE);
      }

      else // IsWindowEnabled()
      {
        // Ask...
        if (IDYES == MsgBox(MBYESNO,IDS_ERR_PREVWINDISABLED))
          return(FALSE);
        else
          return(TRUE);
      }
    }
  }

  if (flagNoReuseWindow)
    return(FALSE);

  hwnd = NULL;
  EnumWindows(EnumWndProc,(LPARAM)&hwnd);

  // Found a window
  if (hwnd != NULL)
  {
    // Enabled
    if (IsWindowEnabled(hwnd))
    {
      // Make sure the previous window won't pop up a change notification message
      //SendMessage(hwnd,WM_CHANGENOTIFYCLEAR,0,0);

      if (IsIconic(hwnd))
        ShowWindowAsync(hwnd,SW_RESTORE);

      if (!IsWindowVisible(hwnd)) {
        SendMessage(hwnd,WM_TRAYMESSAGE,0,WM_LBUTTONDBLCLK);
        SendMessage(hwnd,WM_TRAYMESSAGE,0,WM_LBUTTONUP);
      }

      SetForegroundWindow(hwnd);

      if (lpFileArg)
      {
        // Search working directory from second instance, first!
        // lpFileArg is at least MAX_PATH+2 bytes
        char tchTmp[MAX_PATH];

        ExpandEnvironmentStringsEx(lpFileArg,GlobalSize(lpFileArg));

        if (PathIsRelative(lpFileArg)) {
          GetCurrentDirectory(COUNTOF(tchTmp),tchTmp);
          PathAppend(tchTmp,lpFileArg);
          if (PathFileExists(tchTmp))
            lstrcpy(lpFileArg,tchTmp);
          else {
            if (SearchPath(NULL,lpFileArg,NULL,COUNTOF(tchTmp),tchTmp,NULL))
              lstrcpy(lpFileArg,tchTmp);
          }
        }

        else if (SearchPath(NULL,lpFileArg,NULL,COUNTOF(tchTmp),tchTmp,NULL))
          lstrcpy(lpFileArg,tchTmp);

        cds.dwData = DATA_NOTEPAD2_FILEARG;
        cds.cbData = GlobalSize(lpFileArg);
        cds.lpData = lpFileArg;

        // Send lpFileArg to previous instance
        SendMessage(hwnd,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);
        GlobalFree(lpFileArg);
      }
      return(TRUE);
    }
    else // IsWindowEnabled()
    {
      // Ask...
      if (IDYES == MsgBox(MBYESNO,IDS_ERR_PREVWINDISABLED))
        return(FALSE);
      else
        return(TRUE);
    }
  }
  else
    return(FALSE);
}


//=============================================================================
//
//  ShowNotifyIcon()
//
//
void ShowNotifyIcon(HWND hwnd,BOOL bAdd)
{

  static HICON hIcon;
  NOTIFYICONDATA nid;

  if (!hIcon)
    hIcon = LoadImage(g_hInstance,MAKEINTRESOURCE(IDR_MAINWND),
                      IMAGE_ICON,16,16,LR_DEFAULTCOLOR);

  ZeroMemory(&nid,sizeof(NOTIFYICONDATA));
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hwnd;
  nid.uID = 0;
  nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  nid.uCallbackMessage = WM_TRAYMESSAGE;
  nid.hIcon = hIcon;
  lstrcpy(nid.szTip,"Notepad2");

  if(bAdd)
    Shell_NotifyIcon(NIM_ADD,&nid);
  else
    Shell_NotifyIcon(NIM_DELETE,&nid);

}


//=============================================================================
//
//  SetNotifyIconTitle()
//
//
void SetNotifyIconTitle(HWND hwnd)
{

  NOTIFYICONDATA nid;
  SHFILEINFO shfi;
  char tchTitle[128];

  ZeroMemory(&nid,sizeof(NOTIFYICONDATA));
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hwnd;
  nid.uID = 0;
  nid.uFlags = NIF_TIP;

  if (lstrlen(szCurFile)) {
    SHGetFileInfo2(szCurFile,0,&shfi,sizeof(SHFILEINFO),SHGFI_DISPLAYNAME);
    PathCompactPathEx(tchTitle,shfi.szDisplayName,COUNTOF(tchTitle)-4,0);
  }
  else
    GetString(IDS_UNTITLED,tchTitle,COUNTOF(tchTitle)-4);

  if (bModified || iCodePage != iInitialCP)
    lstrcpy(nid.szTip,"* ");
  else
    lstrcpy(nid.szTip,"");
  lstrcat(nid.szTip,tchTitle);

  Shell_NotifyIcon(NIM_MODIFY,&nid);

}


//=============================================================================
//
//  InstallFileWatching()
//
//
void InstallFileWatching(LPCSTR lpszFile)
{

  char tchDirectory[MAX_PATH];
  HANDLE hFind;

  // Terminate
  if (!bEnableFileWatching || !lpszFile || lstrlen(lpszFile) == 0)
  {
    if (bRunningWatch)
    {
      FindCloseChangeNotification(hChangeHandle);
      KillTimer(NULL,ID_WATCHTIMER);
      bRunningWatch = FALSE;
      //bPendingChangeNotify = FALSE;
    }
    return;
  }

  // Install
  else
  {
    // Terminate previous watching
    if (bRunningWatch) {
      FindCloseChangeNotification(hChangeHandle);
      //bPendingChangeNotify = FALSE;
    }

    // No previous watching installed, so launch the timer first
    else
      SetTimer(NULL,ID_WATCHTIMER,iFileCheckInverval,WatchTimerProc);

    lstrcpy(tchDirectory,lpszFile);
    PathRemoveFileSpec(tchDirectory);

    // Save data of current file
    hFind = FindFirstFile(szCurFile,&fdCurFile);
    if (hFind != INVALID_HANDLE_VALUE)
      FindClose(hFind);
    else
      ZeroMemory(&fdCurFile,sizeof(WIN32_FIND_DATA));

    hChangeHandle = FindFirstChangeNotification(tchDirectory,FALSE,
      FILE_NOTIFY_CHANGE_FILE_NAME  | \
      FILE_NOTIFY_CHANGE_DIR_NAME   | \
      FILE_NOTIFY_CHANGE_ATTRIBUTES | \
      FILE_NOTIFY_CHANGE_SIZE | \
      FILE_NOTIFY_CHANGE_LAST_WRITE);

    bRunningWatch = TRUE;
    //bPendingChangeNotify = FALSE;
  }
}


//=============================================================================
//
//  WatchTimerProc()
//
//
void CALLBACK WatchTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
  // Check Change Notification Handle
  if (bRunningWatch && WAIT_OBJECT_0 == WaitForSingleObject(hChangeHandle,0))
  {
    // Check if the changes affect the current file
    WIN32_FIND_DATA fdUpdated;
    HANDLE hFind = FindFirstFile(szCurFile,&fdUpdated);
    if (INVALID_HANDLE_VALUE != hFind)
      FindClose(hFind);
    else
      // The current file has been removed
      ZeroMemory(&fdUpdated,sizeof(WIN32_FIND_DATA));

    // Check if the file has been changed
    if (CompareFileTime(&fdCurFile.ftLastWriteTime,&fdUpdated.ftLastWriteTime) != 0 ||
          fdCurFile.nFileSizeLow != fdUpdated.nFileSizeLow ||
          fdCurFile.nFileSizeHigh != fdUpdated.nFileSizeHigh)
    {
      // Shutdown current watching and give control to main window
      bRunningWatch = FALSE;
      //bPendingChangeNotify = TRUE;
      FindCloseChangeNotification(hChangeHandle);
      KillTimer(NULL,ID_WATCHTIMER);
      SendMessage(hwndMain,WM_CHANGENOTIFY,0,0);
    }

    else
      FindNextChangeNotification(hChangeHandle);
  }
}



///  End of Notepad2.c  \\\
