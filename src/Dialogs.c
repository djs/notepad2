/******************************************************************************
*
*
* Notepad2
*
* Dialogs.c
*   Notepad2 dialog boxes implementation
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
#include <shellapi.h>
#include <shlwapi.h>
#include <commdlg.h>
#include <string.h>
#include "notepad2.h"
#include "scintilla.h"
#include "edit.h"
#include "helpers.h"
#include "dlapi.h"
#include "dialogs.h"
#include "resource.h"


extern HWND  hwndMain;
extern HWND  hwndEdit;
extern HINSTANCE g_hInstance;
extern LPMALLOC  g_lpMalloc;
extern BOOL bSkipUnicodeDetection;
extern BOOL bFixLineEndings;


//=============================================================================
//
//  MsgBox()
//
int MsgBox(int iType,UINT uIdMsg,...)
{

  char szText [256*2];
  char szBuf  [256*2];
  char szTitle[64];
  int iIcon = 0;
  HWND hwnd;

  if (!GetString(uIdMsg,szBuf,COUNTOF(szBuf)))
    return(0);

  wvsprintf(szText,szBuf,(LPVOID)(&uIdMsg + 1));

  GetString(IDS_APPTITLE,szTitle,COUNTOF(szTitle));

  switch (iType) {
    case MBINFO: iIcon = MB_ICONEXCLAMATION; break;
    case MBWARN: iIcon = MB_ICONEXCLAMATION; break;
    case MBYESNO: iIcon = MB_ICONEXCLAMATION | MB_YESNO; break;
    case MBYESNOCANCEL: iIcon = MB_ICONEXCLAMATION | MB_YESNOCANCEL; break;
    case MBYESNOWARN: iIcon = MB_ICONEXCLAMATION | MB_YESNO; break;
    case MBOKCANCEL: iIcon = MB_ICONEXCLAMATION | MB_OKCANCEL; break;
  }

  if (!(hwnd = GetFocus()))
    hwnd = hwndMain;

  return MessageBoxEx(hwnd,
           szText,szTitle,
           MB_SETFOREGROUND | iIcon,
           MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT));

}


//=============================================================================
//
//  DisplayCmdLineHelp()
//
void DisplayCmdLineHelp()
{
  MSGBOXPARAMS mbp;

  char szTitle[32];
  char szText[1024];

  GetString(IDS_APPTITLE,szTitle,COUNTOF(szTitle));
  GetString(IDS_CMDLINEHELP,szText,COUNTOF(szText));

  mbp.cbSize = sizeof(MSGBOXPARAMS);
  mbp.hwndOwner = NULL;
  mbp.hInstance = g_hInstance;
  mbp.lpszText = szText;
  mbp.lpszCaption = szTitle;
  mbp.dwStyle = MB_OK | MB_USERICON | MB_SETFOREGROUND;
  mbp.lpszIcon = MAKEINTRESOURCE(IDR_MAINWND);
  mbp.dwContextHelpId = 0;
  mbp.lpfnMsgBoxCallback = NULL;
  mbp.dwLanguageId = MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL);

  MessageBoxIndirect(&mbp);
}


//=============================================================================
//
//  BFFCallBack()
//
int CALLBACK BFFCallBack(HWND hwnd,UINT umsg,LPARAM lParam,LPARAM lpData)
{
  if (umsg == BFFM_INITIALIZED)
    SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);

  return(0);

  lParam;
}


//=============================================================================
//
//  GetDirectory()
//
BOOL GetDirectory(HWND hwndParent,int iTitle,LPSTR pszFolder,LPCSTR pszBase,BOOL bNewDialogStyle)
{

  BROWSEINFO bi;
  LPITEMIDLIST pidl;
  //LPMALLOC lpMalloc;
  char szTitle[256];
  char szBase[MAX_PATH];
  BOOL fOk = FALSE;

  lstrcpy(szTitle,"");
  GetString(iTitle,szTitle,COUNTOF(szTitle));

  if (!pszBase || !*pszBase)
    GetCurrentDirectory(MAX_PATH,szBase);
  else
    lstrcpy(szBase,pszBase);

  bi.hwndOwner = hwndParent;
  bi.pidlRoot = NULL;
  bi.pszDisplayName = pszFolder;
  bi.lpszTitle = szTitle;
  bi.ulFlags = BIF_RETURNONLYFSDIRS;
  if (bNewDialogStyle)
    bi.ulFlags |= BIF_NEWDIALOGSTYLE;
  bi.lpfn = &BFFCallBack;
  bi.lParam = (LPARAM)szBase;
  bi.iImage = 0;

  //if (SHGetMalloc(&lpMalloc) != NOERROR)
  //   return FALSE;

  pidl = SHBrowseForFolder(&bi);
  if (pidl)
  {
    SHGetPathFromIDList(pidl,pszFolder);

    g_lpMalloc->lpVtbl->Free(g_lpMalloc,pidl);

    fOk = TRUE;
  }
  //lpMalloc->lpVtbl->Release(lpMalloc);

  return fOk;

}


//=============================================================================
//
//  AboutDlgProc()
//
static const DWORD dwVerMajor    = 2;
static const DWORD dwVerMinor    = 1;
static const DWORD dwBuildNumber = 19;

BOOL CALLBACK AboutDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  static HFONT hFontTitle;

  static BOOL hover_email;
  static BOOL capture_email;
  static BOOL hover_webpage;
  static BOOL capture_webpage;

  static HFONT hFontNormal;
  static HFONT hFontHover;

  static HCURSOR hCursorNormal;
  static HCURSOR hCursorHover;

  switch(umsg)
  {
    case WM_INITDIALOG:
      {
        char  szVersion[64];
        LOGFONT lf;

        wsprintf(szVersion,"Notepad2 %u.%u.%0.2u",dwVerMajor,dwVerMinor,dwBuildNumber);
        SetDlgItemText(hwnd,IDC_VERSION,szVersion);

        if (hFontTitle == NULL) {
          if (NULL == (hFontTitle = (HFONT)SendDlgItemMessage(hwnd,IDC_VERSION,WM_GETFONT,0,0)))
            hFontTitle = GetStockObject(DEFAULT_GUI_FONT);
          GetObject(hFontTitle,sizeof(LOGFONT),&lf);
          lf.lfWeight = FW_BOLD;
          hFontTitle = CreateFontIndirect(&lf);
        }

        hover_email = FALSE;
        capture_email = FALSE;
        hover_webpage = FALSE;
        capture_webpage = FALSE;

        //  Font setup
        if (NULL == (hFontHover = (HFONT)SendDlgItemMessage(hwnd,IDC_EMAIL,WM_GETFONT,0,0)))
          hFontHover = GetStockObject(DEFAULT_GUI_FONT);
        GetObject(hFontHover,sizeof(LOGFONT),&lf);
        lf.lfUnderline = TRUE;
        hFontNormal = CreateFontIndirect(&lf);

        //  Cursor setup
        hCursorNormal = LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW));
        if (!(hCursorHover = LoadCursor(NULL,MAKEINTRESOURCE(IDC_HAND))))
          hCursorHover  = LoadCursor(g_hInstance,MAKEINTRESOURCE(IDC_HOVER));

        CenterDlgInParent(hwnd);
      }
      return TRUE;

    case WM_NCACTIVATE:
      if (!(BOOL)wParam) { //  we're not active, clear hover states
        hover_email = FALSE;
        capture_email = FALSE;
        hover_webpage = FALSE;
        capture_webpage = FALSE;
        InvalidateRect(GetDlgItem(hwnd,IDC_EMAIL),NULL,FALSE);
        InvalidateRect(GetDlgItem(hwnd,IDC_WEBPAGE),NULL,FALSE);
      }
      return FALSE;

    case WM_CTLCOLORSTATIC:
      {
        DWORD dwId = GetWindowLong((HWND)lParam,GWL_ID);
        HDC hdc = (HDC)wParam;

        if (dwId == IDC_VERSION) {
          SetBkMode(hdc,TRANSPARENT);
          SetTextColor(hdc,GetSysColor(COLOR_BTNTEXT));
          SelectObject(hdc,hFontTitle);
          return(LONG)GetSysColorBrush(COLOR_BTNFACE);
        }

        if (dwId == IDC_EMAIL || dwId == IDC_WEBPAGE) {

          SetBkMode(hdc,TRANSPARENT);

          if (GetSysColorBrush(26/*COLOR_HOTLIGHT*/))
            SetTextColor(hdc,GetSysColor(26/*COLOR_HOTLIGHT*/));
          else
            SetTextColor(hdc,RGB(0,0,255));

          if (dwId == IDC_EMAIL)
            SelectObject(hdc,hover_email?hFontHover:hFontNormal);
          else
            SelectObject(hdc,hover_webpage?hFontHover:hFontNormal);

          return(LONG)GetSysColorBrush(COLOR_BTNFACE);
        }
      }
      break;

    case WM_MOUSEMOVE:
      {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        HWND hwndHover = ChildWindowFromPoint(hwnd,pt);
        DWORD dwId = GetWindowLong(hwndHover,GWL_ID);

        if (GetActiveWindow() == hwnd) {

          if (wParam & MK_LBUTTON && !capture_email && !capture_webpage) {
            ;
          }

          else if (hover_email != (dwId == IDC_EMAIL) && !capture_webpage) {
            hover_email = !hover_email;
            InvalidateRect(GetDlgItem(hwnd,IDC_EMAIL),NULL,FALSE);
          }

          else if (hover_webpage != (dwId == IDC_WEBPAGE) && !capture_email) {
            hover_webpage = !hover_webpage;
            InvalidateRect(GetDlgItem(hwnd,IDC_WEBPAGE),NULL,FALSE);
          }

          SetCursor((hover_email || hover_webpage)?hCursorHover:hCursorNormal);
        }
      }
      break;

    case WM_LBUTTONDOWN:
      {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        HWND hwndHover = ChildWindowFromPoint(hwnd,pt);
        DWORD dwId = GetWindowLong(hwndHover,GWL_ID);

        if (dwId == IDC_EMAIL) {
          GetCapture();
          hover_email = TRUE;
          capture_email = TRUE;
          InvalidateRect(GetDlgItem(hwnd,IDC_EMAIL),NULL,FALSE);
        }

        else if (dwId == IDC_WEBPAGE) {
          GetCapture();
          hover_webpage = TRUE;
          capture_webpage = TRUE;
          InvalidateRect(GetDlgItem(hwnd,IDC_WEBPAGE),NULL,FALSE);
        }

        SetCursor((hover_email || hover_webpage)?hCursorHover:hCursorNormal);
      }
      break;

    case WM_LBUTTONUP:
      {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        HWND hwndHover = ChildWindowFromPoint(hwnd,pt);
        DWORD dwId = GetWindowLong(hwndHover,GWL_ID);

        if (capture_email || capture_webpage) {
            ReleaseCapture();

          if (dwId == IDC_EMAIL && capture_email) {
            ShellExecute(hwnd,"open","mailto:florian.balmer@gmail.com",NULL,NULL,SW_SHOWNORMAL);
          }

          else if (dwId == IDC_WEBPAGE && capture_webpage) {
            ShellExecute(hwnd,"open","http://www.flos-freeware.ch",NULL,NULL,SW_SHOWNORMAL);
          }

          capture_email = FALSE;
          capture_webpage = FALSE;
        }
        SetCursor((hover_email || hover_webpage)?hCursorHover:hCursorNormal);
      }
      break;

    case WM_CANCELMODE:
      if (capture_email || capture_webpage) {
        ReleaseCapture();
        hover_email = FALSE;
        capture_email = FALSE;
        hover_webpage = FALSE;
        capture_webpage = FALSE;
        SetCursor(hCursorNormal);
      }
      break;

    case WM_COMMAND:

      switch(LOWORD(wParam))
      {
        case IDOK:
        case IDCANCEL:
          EndDialog(hwnd,IDOK);
          break;
      }
      return TRUE;
  }
  return FALSE;
}


//=============================================================================
//
//  RunDlgProc()
//
BOOL CALLBACK RunDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  switch(umsg)
  {

    case WM_INITDIALOG:
      {
        MakeBitmapButton(hwnd,IDC_SEARCHEXE,g_hInstance,IDB_OPEN);

        SendDlgItemMessage(hwnd,IDC_COMMANDLINE,EM_LIMITTEXT,MAX_PATH - 1,0);
        SetDlgItemText(hwnd,IDC_COMMANDLINE,(LPCSTR)lParam);
        SHAutoComplete(GetDlgItem(hwnd,IDC_COMMANDLINE),SHACF_FILESYSTEM);

        CenterDlgInParent(hwnd);
      }
      return TRUE;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case IDC_SEARCHEXE:
          {
            char szArgs[MAX_PATH];
            char szArg2[MAX_PATH];
            char szFile[MAX_PATH * 2];
            char szFilter[256];
            OPENFILENAME ofn;
            ZeroMemory(&ofn,sizeof(OPENFILENAME));

            GetDlgItemText(hwnd,IDC_COMMANDLINE,szArgs,COUNTOF(szArgs));
            ExpandEnvironmentStringsEx(szArgs,COUNTOF(szArgs));
            ExtractFirstArgument(szArgs,szFile,szArg2);

            GetString(IDS_FILTER_EXE,szFilter,COUNTOF(szFilter));
            PrepareFilterStr(szFilter);

            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFilter = szFilter;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = COUNTOF(szFile);
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | /*OFN_DONTADDTORECENT*/0x02000000
                      | OFN_PATHMUSTEXIST | OFN_SHAREAWARE | OFN_NODEREFERENCELINKS;

            if (GetOpenFileName(&ofn)) {
              PathQuoteSpaces(szFile);
              if (lstrlen(szArg2))
              {
                lstrcat(szFile," ");
                lstrcat(szFile,szArg2);
              }
              SetDlgItemText(hwnd,IDC_COMMANDLINE,szFile);
            }

            PostMessage(hwnd,WM_NEXTDLGCTL,1,0);
          }
          break;


        case IDC_COMMANDLINE:
          {
            BOOL bEnableOK = FALSE;
            char args[MAX_PATH];

            if (GetDlgItemText(hwnd,IDC_COMMANDLINE,args,MAX_PATH))
              if (ExtractFirstArgument(args,args,NULL))
                if (lstrlen(args))
                  bEnableOK = TRUE;

            EnableWindow(GetDlgItem(hwnd,IDOK),bEnableOK);
          }
          break;


        case IDOK:
          {
            char arg1[MAX_PATH];
            char arg2[MAX_PATH];
            SHELLEXECUTEINFO sei;

            if (GetDlgItemText(hwnd,IDC_COMMANDLINE,arg1,MAX_PATH))
            {
              ExpandEnvironmentStringsEx(arg1,COUNTOF(arg1));
              ExtractFirstArgument(arg1,arg1,arg2);

              ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));

              sei.cbSize = sizeof(SHELLEXECUTEINFO);
              sei.fMask = SEE_MASK_NOCLOSEPROCESS;
              sei.hwnd = hwnd;
              sei.lpVerb = NULL;
              sei.lpFile = arg1;
              sei.lpParameters = arg2;
              sei.lpDirectory = NULL;
              sei.nShow = SW_SHOWNORMAL;

              if (ShellExecuteEx(&sei))
                EndDialog(hwnd,IDOK);

              else
                PostMessage(hwnd,WM_NEXTDLGCTL,
                  (WPARAM)(GetDlgItem(hwnd,IDC_COMMANDLINE)),1);
            }
          }
          break;


        case IDCANCEL:
          EndDialog(hwnd,IDCANCEL);
          break;

      }

      return TRUE;

  }

  return FALSE;

}


//=============================================================================
//
//  RunDlg()
//
void RunDlg(HWND hwnd,LPCSTR lpstrDefault)
{

  DialogBoxParam(g_hInstance,MAKEINTRESOURCE(IDD_RUN),
    hwnd,RunDlgProc,(LPARAM)lpstrDefault);

}


//=============================================================================
//
//  OpenWithDlgProc()
//
extern char tchOpenWithDir[MAX_PATH];
extern char tchOpenWithDirBuf[MAX_PATH];
extern int  flagNoFadeHidden;

extern int cxOpenWithDlg;
extern int cyOpenWithDlg;

BOOL CALLBACK OpenWithDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  static int cxClient;
  static int cyClient;
  static int mmiPtMaxY;
  static int mmiPtMinX;

  switch(umsg)
  {

    case WM_INITDIALOG:
      {
        RECT rc;
        char tch[MAX_PATH];
        int cGrip;
        LVCOLUMN lvc = { LVCF_FMT|LVCF_TEXT, LVCFMT_LEFT, 0, "", -1, 0, 0, 0 };

        GetClientRect(hwnd,&rc);
        cxClient = rc.right - rc.left;
        cyClient = rc.bottom - rc.top;

        GetWindowRect(hwnd,&rc);
        mmiPtMaxY = rc.bottom-rc.top;
        mmiPtMinX = rc.right-rc.left;

        if (cxOpenWithDlg < (rc.right-rc.left))
          cxOpenWithDlg = rc.right-rc.left;
        if (cyOpenWithDlg < (rc.bottom-rc.top))
          cyOpenWithDlg = rc.bottom-rc.top;
        SetWindowPos(hwnd,NULL,rc.left,rc.top,cxOpenWithDlg,cyOpenWithDlg,SWP_NOZORDER);

        SetWindowLong(hwnd,GWL_STYLE,GetWindowLong(hwnd,GWL_STYLE)|WS_THICKFRAME);
        SetWindowPos(hwnd,NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

        GetMenuString(GetSystemMenu(GetParent(hwnd),FALSE),SC_SIZE,tch,COUNTOF(tch),MF_BYCOMMAND);
        InsertMenu(GetSystemMenu(hwnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_STRING|MF_ENABLED,SC_SIZE,tch);
        InsertMenu(GetSystemMenu(hwnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_SEPARATOR,0,NULL);

        SetWindowLong(GetDlgItem(hwnd,IDC_RESIZEGRIP3),GWL_STYLE,
          GetWindowLong(GetDlgItem(hwnd,IDC_RESIZEGRIP3),GWL_STYLE)|SBS_SIZEGRIP|WS_CLIPSIBLINGS);

        cGrip = GetSystemMetrics(SM_CXHTHUMB);
        SetWindowPos(GetDlgItem(hwnd,IDC_RESIZEGRIP3),NULL,cxClient-cGrip,
                     cyClient-cGrip,cGrip,cGrip,SWP_NOZORDER);

        SetWindowLong(hwnd,DWL_USER,lParam);

        ListView_SetExtendedListViewStyle(GetDlgItem(hwnd,IDC_OPENWITHDIR),/*LVS_EX_FULLROWSELECT|*//*LVS_EX_DOUBLEBUFFER*/0x00010000|LVS_EX_LABELTIP);
        ListView_InsertColumn(GetDlgItem(hwnd,IDC_OPENWITHDIR),0,&lvc);
        DirList_Init(GetDlgItem(hwnd,IDC_OPENWITHDIR),NULL);
        DirList_Fill(GetDlgItem(hwnd,IDC_OPENWITHDIR),tchOpenWithDir,DL_ALLOBJECTS,NULL,FALSE,flagNoFadeHidden,DS_NAME,FALSE);
        DirList_StartIconThread(GetDlgItem(hwnd,IDC_OPENWITHDIR));
        ListView_SetItemState(GetDlgItem(hwnd,IDC_OPENWITHDIR),0,LVIS_FOCUSED,LVIS_FOCUSED);

        MakeBitmapButton(hwnd,IDC_GETOPENWITHDIR,g_hInstance,IDB_OPEN);

        CenterDlgInParent(hwnd);
      }
      return TRUE;


    case WM_DESTROY:
      {
        RECT rc;

        DirList_Destroy(GetDlgItem(hwnd,IDC_OPENWITHDIR));

        GetWindowRect(hwnd,&rc);
        cxOpenWithDlg = rc.right-rc.left;
        cyOpenWithDlg = rc.bottom-rc.top;
      }
      return FALSE;


    case WM_SIZE:
      {
        RECT rc;

        int dxClient = LOWORD(lParam) - cxClient;
        int dyClient = HIWORD(lParam) - cyClient;
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);

        GetWindowRect(GetDlgItem(hwnd,IDC_RESIZEGRIP3),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_RESIZEGRIP3),NULL,rc.left+dxClient,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDC_RESIZEGRIP3),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDOK),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDOK),NULL,rc.left+dxClient,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDOK),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDCANCEL),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDCANCEL),NULL,rc.left+dxClient,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDCANCEL),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDC_OPENWITHDIR),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_OPENWITHDIR),NULL,0,0,rc.right-rc.left+dxClient,rc.bottom-rc.top+dyClient,SWP_NOZORDER|SWP_NOMOVE);
        InvalidateRect(GetDlgItem(hwnd,IDC_OPENWITHDIR),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDC_GETOPENWITHDIR),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_GETOPENWITHDIR),NULL,rc.left,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        ListView_SetColumnWidth(GetDlgItem(hwnd,IDC_OPENWITHDIR),0,LVSCW_AUTOSIZE_USEHEADER);
        InvalidateRect(GetDlgItem(hwnd,IDC_OPENWITHDIR),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDC_OPENWITHDESCR),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_OPENWITHDESCR),NULL,rc.left,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDC_OPENWITHDESCR),NULL,TRUE);
      }
      return TRUE;


    case WM_GETMINMAXINFO:
      {
        LPMINMAXINFO lpmmi = (LPMINMAXINFO)lParam;
        lpmmi->ptMinTrackSize.x = mmiPtMinX;
        lpmmi->ptMinTrackSize.y = mmiPtMaxY;
        //lpmmi->ptMaxTrackSize.y = mmiPtMaxY;
      }
      return TRUE;


    case WM_NOTIFY:
      {
        LPNMHDR pnmh = (LPNMHDR)lParam;

        if (pnmh->idFrom == IDC_OPENWITHDIR)
        {
          switch(pnmh->code)
          {
            case LVN_GETDISPINFO:
              DirList_GetDispInfo(GetDlgItem(hwnd,IDC_OPENWITHDIR),lParam,flagNoFadeHidden);
              break;

            case LVN_DELETEITEM:
              DirList_DeleteItem(GetDlgItem(hwnd,IDC_OPENWITHDIR),lParam);
              break;

            case LVN_ITEMCHANGED: {
                NM_LISTVIEW *pnmlv = (NM_LISTVIEW*)lParam;
                EnableWindow(GetDlgItem(hwnd,IDOK),(pnmlv->uNewState & LVIS_SELECTED));
              }
              break;

            case NM_DBLCLK:
              if (ListView_GetSelectedCount(GetDlgItem(hwnd,IDC_OPENWITHDIR)))
                SendMessage(hwnd,WM_COMMAND,MAKELONG(IDOK,1),0);
              break;
          }
        }
      }
      return TRUE;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case IDC_GETOPENWITHDIR:
          {
            if (GetDirectory(hwnd,IDS_OPENWITH,tchOpenWithDir,tchOpenWithDir,TRUE))
            {
              lstrcpy(tchOpenWithDirBuf,tchOpenWithDir);
              DirList_Fill(GetDlgItem(hwnd,IDC_OPENWITHDIR),tchOpenWithDir,DL_ALLOBJECTS,NULL,FALSE,flagNoFadeHidden,DS_NAME,FALSE);
              DirList_StartIconThread(GetDlgItem(hwnd,IDC_OPENWITHDIR));
              ListView_EnsureVisible(GetDlgItem(hwnd,IDC_OPENWITHDIR),0,FALSE);
              ListView_SetItemState(GetDlgItem(hwnd,IDC_OPENWITHDIR),0,LVIS_FOCUSED,LVIS_FOCUSED);
            }
            PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_OPENWITHDIR)),1);
          }
          break;


        case IDOK: {
            LPDLITEM lpdli = (LPDLITEM)GetWindowLong(hwnd,DWL_USER);
            lpdli->mask = DLI_FILENAME | DLI_TYPE;
            lpdli->ntype = DLE_NONE;
            DirList_GetItem(GetDlgItem(hwnd,IDC_OPENWITHDIR),(-1),lpdli);

            if (lpdli->ntype != DLE_NONE)
              EndDialog(hwnd,IDOK);
            else
              MessageBeep(0);
          }
          break;


        case IDCANCEL:
          EndDialog(hwnd,IDCANCEL);
          break;

      }

      return TRUE;

  }

  return FALSE;

}


//=============================================================================
//
//  OpenWithDlg()
//
BOOL OpenWithDlg(HWND hwnd,LPCSTR lpstrFile)
{

  DLITEM dliOpenWith;
  dliOpenWith.mask = DLI_FILENAME;

  if (IDOK == DialogBoxParam(g_hInstance,MAKEINTRESOURCE(IDD_OPENWITH),
                             hwnd,OpenWithDlgProc,(LPARAM)&dliOpenWith))
  {
    SHELLEXECUTEINFO sei;
    char szParam[MAX_PATH];

    ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));
    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = hwnd;
    sei.lpVerb = NULL;
    sei.lpFile = dliOpenWith.szFileName;
    sei.lpParameters = szParam;
    sei.lpDirectory = NULL;
    sei.nShow = SW_SHOWNORMAL;

    // resolve links and get short path name
    if (!(PathIsLnkFile(lpstrFile) && PathGetLnkPath(lpstrFile,szParam,COUNTOF(szParam))))
      lstrcpy(szParam,lpstrFile);
    GetShortPathName(szParam,szParam,sizeof(char)*COUNTOF(szParam));
    //PathQuoteSpaces(szParam);

    ShellExecuteEx(&sei);

    return(TRUE);
  }

  return(FALSE);

}


//=============================================================================
//
//  FavoritesDlgProc()
//
extern char tchFavoritesDir[MAX_PATH];
extern char tchFavoritesDirBuf[MAX_PATH];
//extern int  flagNoFadeHidden;

extern int cxFavoritesDlg;
extern int cyFavoritesDlg;

BOOL CALLBACK FavoritesDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  static int cxClient;
  static int cyClient;
  static int mmiPtMaxY;
  static int mmiPtMinX;

  switch(umsg)
  {

    case WM_INITDIALOG:
      {
        RECT rc;
        char tch[MAX_PATH];
        int cGrip;
        LVCOLUMN lvc = { LVCF_FMT|LVCF_TEXT, LVCFMT_LEFT, 0, "", -1, 0, 0, 0 };

        GetClientRect(hwnd,&rc);
        cxClient = rc.right - rc.left;
        cyClient = rc.bottom - rc.top;

        GetWindowRect(hwnd,&rc);
        mmiPtMaxY = rc.bottom-rc.top;
        mmiPtMinX = rc.right-rc.left;

        if (cxFavoritesDlg < (rc.right-rc.left))
          cxFavoritesDlg = rc.right-rc.left;
        if (cyFavoritesDlg < (rc.bottom-rc.top))
          cyFavoritesDlg = rc.bottom-rc.top;
        SetWindowPos(hwnd,NULL,rc.left,rc.top,cxFavoritesDlg,cyFavoritesDlg,SWP_NOZORDER);

        SetWindowLong(hwnd,GWL_STYLE,GetWindowLong(hwnd,GWL_STYLE)|WS_THICKFRAME);
        SetWindowPos(hwnd,NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

        GetMenuString(GetSystemMenu(GetParent(hwnd),FALSE),SC_SIZE,tch,COUNTOF(tch),MF_BYCOMMAND);
        InsertMenu(GetSystemMenu(hwnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_STRING|MF_ENABLED,SC_SIZE,tch);
        InsertMenu(GetSystemMenu(hwnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_SEPARATOR,0,NULL);

        SetWindowLong(GetDlgItem(hwnd,IDC_RESIZEGRIP3),GWL_STYLE,
          GetWindowLong(GetDlgItem(hwnd,IDC_RESIZEGRIP3),GWL_STYLE)|SBS_SIZEGRIP|WS_CLIPSIBLINGS);

        cGrip = GetSystemMetrics(SM_CXHTHUMB);
        SetWindowPos(GetDlgItem(hwnd,IDC_RESIZEGRIP3),NULL,cxClient-cGrip,
                     cyClient-cGrip,cGrip,cGrip,SWP_NOZORDER);

        SetWindowLong(hwnd,DWL_USER,lParam);

        ListView_SetExtendedListViewStyle(GetDlgItem(hwnd,IDC_FAVORITESDIR),/*LVS_EX_FULLROWSELECT|*//*LVS_EX_DOUBLEBUFFER*/0x00010000|LVS_EX_LABELTIP);
        ListView_InsertColumn(GetDlgItem(hwnd,IDC_FAVORITESDIR),0,&lvc);
        DirList_Init(GetDlgItem(hwnd,IDC_FAVORITESDIR),NULL);
        DirList_Fill(GetDlgItem(hwnd,IDC_FAVORITESDIR),tchFavoritesDir,DL_ALLOBJECTS,NULL,FALSE,flagNoFadeHidden,DS_NAME,FALSE);
        DirList_StartIconThread(GetDlgItem(hwnd,IDC_FAVORITESDIR));
        ListView_SetItemState(GetDlgItem(hwnd,IDC_FAVORITESDIR),0,LVIS_FOCUSED,LVIS_FOCUSED);

        MakeBitmapButton(hwnd,IDC_GETFAVORITESDIR,g_hInstance,IDB_OPEN);

        CenterDlgInParent(hwnd);
      }
      return TRUE;


    case WM_DESTROY:
      {
        RECT rc;

        DirList_Destroy(GetDlgItem(hwnd,IDC_FAVORITESDIR));

        GetWindowRect(hwnd,&rc);
        cxFavoritesDlg = rc.right-rc.left;
        cyFavoritesDlg = rc.bottom-rc.top;
      }
      return FALSE;


    case WM_SIZE:
      {
        RECT rc;

        int dxClient = LOWORD(lParam) - cxClient;
        int dyClient = HIWORD(lParam) - cyClient;
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);

        GetWindowRect(GetDlgItem(hwnd,IDC_RESIZEGRIP3),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_RESIZEGRIP3),NULL,rc.left+dxClient,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDC_RESIZEGRIP3),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDOK),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDOK),NULL,rc.left+dxClient,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDOK),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDCANCEL),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDCANCEL),NULL,rc.left+dxClient,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDCANCEL),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDC_FAVORITESDIR),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_FAVORITESDIR),NULL,0,0,rc.right-rc.left+dxClient,rc.bottom-rc.top+dyClient,SWP_NOZORDER|SWP_NOMOVE);
        ListView_SetColumnWidth(GetDlgItem(hwnd,IDC_FAVORITESDIR),0,LVSCW_AUTOSIZE_USEHEADER);
        InvalidateRect(GetDlgItem(hwnd,IDC_FAVORITESDIR),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDC_GETFAVORITESDIR),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_GETFAVORITESDIR),NULL,rc.left,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDC_GETFAVORITESDIR),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDC_FAVORITESDESCR),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_FAVORITESDESCR),NULL,rc.left,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDC_FAVORITESDESCR),NULL,TRUE);
      }
      return TRUE;


    case WM_GETMINMAXINFO:
      {
        LPMINMAXINFO lpmmi = (LPMINMAXINFO)lParam;
        lpmmi->ptMinTrackSize.x = mmiPtMinX;
        lpmmi->ptMinTrackSize.y = mmiPtMaxY;
        //lpmmi->ptMaxTrackSize.y = mmiPtMaxY;
      }
      return TRUE;


    case WM_NOTIFY:
      {
        LPNMHDR pnmh = (LPNMHDR)lParam;

        if (pnmh->idFrom == IDC_FAVORITESDIR)
        {
          switch(pnmh->code)
          {
            case LVN_GETDISPINFO:
              DirList_GetDispInfo(GetDlgItem(hwnd,IDC_OPENWITHDIR),lParam,flagNoFadeHidden);
              break;

            case LVN_DELETEITEM:
              DirList_DeleteItem(GetDlgItem(hwnd,IDC_FAVORITESDIR),lParam);
              break;

            case LVN_ITEMCHANGED: {
                NM_LISTVIEW *pnmlv = (NM_LISTVIEW*)lParam;
                EnableWindow(GetDlgItem(hwnd,IDOK),(pnmlv->uNewState & LVIS_SELECTED));
              }
              break;

            case NM_DBLCLK:
              if (ListView_GetSelectedCount(GetDlgItem(hwnd,IDC_FAVORITESDIR)))
                SendMessage(hwnd,WM_COMMAND,MAKELONG(IDOK,1),0);
              break;
          }
        }
      }
      return TRUE;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case IDC_GETFAVORITESDIR:
          {
            if (GetDirectory(hwnd,IDS_FAVORITES,tchFavoritesDir,tchFavoritesDir,TRUE))
            {
              lstrcpy(tchFavoritesDirBuf,tchFavoritesDir);
              DirList_Fill(GetDlgItem(hwnd,IDC_FAVORITESDIR),tchFavoritesDir,DL_ALLOBJECTS,NULL,FALSE,flagNoFadeHidden,DS_NAME,FALSE);
              DirList_StartIconThread(GetDlgItem(hwnd,IDC_FAVORITESDIR));
              ListView_EnsureVisible(GetDlgItem(hwnd,IDC_FAVORITESDIR),0,FALSE);
              ListView_SetItemState(GetDlgItem(hwnd,IDC_FAVORITESDIR),0,LVIS_FOCUSED,LVIS_FOCUSED);
            }
            PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_FAVORITESDIR)),1);
          }
          break;


        case IDOK: {
            LPDLITEM lpdli = (LPDLITEM)GetWindowLong(hwnd,DWL_USER);
            lpdli->mask = DLI_FILENAME | DLI_TYPE;
            lpdli->ntype = DLE_NONE;
            DirList_GetItem(GetDlgItem(hwnd,IDC_FAVORITESDIR),(-1),lpdli);

            if (lpdli->ntype != DLE_NONE)
              EndDialog(hwnd,IDOK);
            else
              MessageBeep(0);
          }
          break;


        case IDCANCEL:
          EndDialog(hwnd,IDCANCEL);
          break;

      }

      return TRUE;

  }

  return FALSE;

}


//=============================================================================
//
//  FavoritesDlg()
//
BOOL FavoritesDlg(HWND hwnd,LPSTR lpstrFile)
{

  DLITEM dliFavorite;
  dliFavorite.mask = DLI_FILENAME;

  if (IDOK == DialogBoxParam(g_hInstance,MAKEINTRESOURCE(IDD_FAVORITES),
                             hwnd,FavoritesDlgProc,(LPARAM)&dliFavorite))
  {
    lstrcpyn(lpstrFile,dliFavorite.szFileName,MAX_PATH);
    return(TRUE);
  }

  return(FALSE);

}


//=============================================================================
//
//  AddToFavDlgProc()
//
//  Controls: 100 Edit
//
BOOL CALLBACK AddToFavDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  switch(umsg)
  {

    char *pszName;

    case WM_INITDIALOG:
      pszName = (LPSTR)lParam;
      SetWindowLong(hwnd,DWL_USER,(LONG)pszName);

      SendDlgItemMessage(hwnd,100,EM_LIMITTEXT,MAX_PATH-1,0);
      SetDlgItemText(hwnd,100,pszName);

      CenterDlgInParent(hwnd);
      return TRUE;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case 100:
            EnableWindow(GetDlgItem(hwnd,IDOK),
              GetWindowTextLength(GetDlgItem(hwnd,100)));
          break;


        case IDOK:
          pszName = (LPSTR)GetWindowLong(hwnd,DWL_USER);
          GetDlgItemText(hwnd,100,pszName,
            MAX_PATH-1);
          EndDialog(hwnd,IDOK);
          break;


        case IDCANCEL:
          EndDialog(hwnd,IDCANCEL);
          break;

      }

      return TRUE;

  }

  return FALSE;

}


//=============================================================================
//
//  AddToFavDlg()
//
BOOL AddToFavDlg(HWND hwnd,LPCSTR lpszName,LPCSTR lpszTarget)
{

  int iResult;

  char pszName[MAX_PATH];
  lstrcpy(pszName,lpszName);

  iResult = DialogBoxParam(
              g_hInstance,
              MAKEINTRESOURCE(IDD_ADDTOFAV),
              hwnd,
              AddToFavDlgProc,(LPARAM)pszName);

  if (iResult == IDOK)
  {
    if (!PathCreateFavLnk(pszName,lpszTarget,tchFavoritesDir)) {
      MsgBox(MBWARN,IDS_FAV_FAILURE);
      return FALSE;
    }

    else {
      MsgBox(MBINFO,IDS_FAV_SUCCESS);
      return TRUE;
    }
  }

  else
    return FALSE;

}


//=============================================================================
//
//  FileMRUDlgProc()
//
//
extern LPMRULIST pFileMRU;
extern BOOL bSaveRecentFiles;
extern int  cxFileMRUDlg;
extern int  cyFileMRUDlg;
extern int  flagNoFadeHidden;

typedef struct tagIconThreadInfo
{
  HWND hwnd;                 // HWND of ListView Control
  HANDLE hExitThread;        // Flag is set when Icon Thread should terminate
  HANDLE hTerminatedThread;  // Flag is set when Icon Thread has terminated

} ICONTHREADINFO, *LPICONTHREADINFO;

DWORD WINAPI FileMRUIconThread(LPVOID lpParam) {

  HWND hwnd;
  LPICONTHREADINFO lpit;
  LV_ITEM lvi;
  char tch[MAX_PATH];
  SHFILEINFO shfi;
  DWORD dwFlags = SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_ATTRIBUTES | SHGFI_ATTR_SPECIFIED;
  DWORD dwAttr  = 0;
  int iItem = 0;
  int iMaxItem;

  lpit = (LPICONTHREADINFO)lpParam;
  ResetEvent(lpit->hTerminatedThread);

  hwnd = lpit->hwnd;
  iMaxItem = ListView_GetItemCount(hwnd);

  CoInitialize(NULL);

  ZeroMemory(&lvi,sizeof(LV_ITEM));

  while (iItem < iMaxItem && WaitForSingleObject(lpit->hExitThread,0) != WAIT_OBJECT_0) {

    lvi.mask = LVIF_TEXT;
    lvi.pszText = tch;
    lvi.cchTextMax = COUNTOF(tch);
    lvi.iItem = iItem;
    if (ListView_GetItem(hwnd,&lvi)) {

      if (!PathFileExists(tch)) {
        dwFlags |= SHGFI_USEFILEATTRIBUTES;
        dwAttr = FILE_ATTRIBUTE_NORMAL;
        shfi.dwAttributes = 0;
        SHGetFileInfo(PathFindFileName(tch),dwAttr,&shfi,sizeof(SHFILEINFO),dwFlags);
      }

      else {
        shfi.dwAttributes = SFGAO_LINK | SFGAO_SHARE;
        SHGetFileInfo(tch,dwAttr,&shfi,sizeof(SHFILEINFO),dwFlags);
      }

      lvi.mask = LVIF_IMAGE;
      lvi.iImage = shfi.iIcon;
      lvi.stateMask = 0;
      lvi.state = 0;

      if (shfi.dwAttributes & SFGAO_LINK) {
        lvi.mask |= LVIF_STATE;
        lvi.stateMask |= LVIS_OVERLAYMASK;
        lvi.state |= INDEXTOOVERLAYMASK(2);
      }

      if (shfi.dwAttributes & SFGAO_SHARE) {
        lvi.mask |= LVIF_STATE;
        lvi.stateMask |= LVIS_OVERLAYMASK;
        lvi.state |= INDEXTOOVERLAYMASK(1);
      }

      dwAttr = GetFileAttributes(tch);

      if (!flagNoFadeHidden &&
          dwAttr != INVALID_FILE_ATTRIBUTES &&
          dwAttr & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM)) {
        lvi.mask |= LVIF_STATE;
        lvi.stateMask |= LVIS_CUT;
        lvi.state |= LVIS_CUT;
      }

      lvi.iSubItem = 0;
      ListView_SetItem(hwnd,&lvi);
    }
    iItem++;
  }

  CoUninitialize();

  SetEvent(lpit->hTerminatedThread);
  ExitThread(0);
  return(0);
}

BOOL CALLBACK FileMRUDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  static int cxClient;
  static int cyClient;
  static int mmiPtMaxY;
  static int mmiPtMinX;

  switch(umsg)
  {

    case WM_INITDIALOG:
      {
        RECT rc;
        char tch[MAX_PATH];
        int cGrip;
        SHFILEINFO shfi;
        LVCOLUMN lvc = { LVCF_FMT|LVCF_TEXT, LVCFMT_LEFT, 0, "", -1, 0, 0, 0 };

        LPICONTHREADINFO lpit = (LPVOID)GlobalAlloc(GPTR,sizeof(ICONTHREADINFO));
        SetProp(hwnd,"it",(HANDLE)lpit);
        lpit->hwnd = GetDlgItem(hwnd,IDC_FILEMRU);
        lpit->hExitThread = CreateEvent(NULL,TRUE,FALSE,NULL);
        lpit->hTerminatedThread = CreateEvent(NULL,TRUE,TRUE,NULL);

        SetWindowLong(hwnd,DWL_USER,lParam);

        GetClientRect(hwnd,&rc);
        cxClient = rc.right - rc.left;
        cyClient = rc.bottom - rc.top;

        GetWindowRect(hwnd,&rc);
        mmiPtMaxY = rc.bottom-rc.top;
        mmiPtMinX = rc.right-rc.left;

        if (cxFileMRUDlg < (rc.right-rc.left))
          cxFileMRUDlg = rc.right-rc.left;
        if (cyFileMRUDlg < (rc.bottom-rc.top))
          cyFileMRUDlg = rc.bottom-rc.top;
        SetWindowPos(hwnd,NULL,0,0,cxFileMRUDlg,cyFileMRUDlg,SWP_NOZORDER|SWP_NOMOVE);

        SetWindowLong(hwnd,GWL_STYLE,GetWindowLong(hwnd,GWL_STYLE)|WS_THICKFRAME);
        SetWindowPos(hwnd,NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

        GetMenuString(GetSystemMenu(GetParent(hwnd),FALSE),SC_SIZE,tch,COUNTOF(tch),MF_BYCOMMAND);
        InsertMenu(GetSystemMenu(hwnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_STRING|MF_ENABLED,SC_SIZE,tch);
        InsertMenu(GetSystemMenu(hwnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_SEPARATOR,0,NULL);

        SetWindowLong(GetDlgItem(hwnd,IDC_RESIZEGRIP),GWL_STYLE,
          GetWindowLong(GetDlgItem(hwnd,IDC_RESIZEGRIP),GWL_STYLE)|SBS_SIZEGRIP|WS_CLIPSIBLINGS);

        cGrip = GetSystemMetrics(SM_CXHTHUMB);
        SetWindowPos(GetDlgItem(hwnd,IDC_RESIZEGRIP),NULL,cxClient-cGrip,
                     cyClient-cGrip,cGrip,cGrip,SWP_NOZORDER);

        ListView_SetImageList(GetDlgItem(hwnd,IDC_FILEMRU),
          (HIMAGELIST)SHGetFileInfo("C:\\",0,&shfi,sizeof(SHFILEINFO),SHGFI_SMALLICON | SHGFI_SYSICONINDEX),
          LVSIL_SMALL);

        ListView_SetImageList(GetDlgItem(hwnd,IDC_FILEMRU),
          (HIMAGELIST)SHGetFileInfo("C:\\",0,&shfi,sizeof(SHFILEINFO),SHGFI_LARGEICON | SHGFI_SYSICONINDEX),
          LVSIL_NORMAL);

        ListView_SetExtendedListViewStyle(GetDlgItem(hwnd,IDC_FILEMRU),/*LVS_EX_FULLROWSELECT|*//*LVS_EX_DOUBLEBUFFER*/0x00010000|LVS_EX_LABELTIP);
        ListView_InsertColumn(GetDlgItem(hwnd,IDC_FILEMRU),0,&lvc);

        // Update view
        SendMessage(hwnd,WM_COMMAND,MAKELONG(0x00A0,1),0);

        if (bSaveRecentFiles)
          CheckDlgButton(hwnd,IDC_SAVEMRU,BST_CHECKED);

        CenterDlgInParent(hwnd);
      }
      return TRUE;


    case WM_DESTROY:
      {
        RECT rc;

        LPICONTHREADINFO lpit = (LPVOID)GetProp(hwnd,"it");
        SetEvent(lpit->hExitThread);
        while (WaitForSingleObject(lpit->hTerminatedThread,0) != WAIT_OBJECT_0) {
          MSG msg;
          if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
          }
        }
        CloseHandle(lpit->hExitThread);
        CloseHandle(lpit->hTerminatedThread);
        RemoveProp(hwnd,"it");
        GlobalFree(lpit);

        GetWindowRect(hwnd,&rc);
        cxFileMRUDlg = rc.right-rc.left;
        cyFileMRUDlg = rc.bottom-rc.top;

        bSaveRecentFiles = (IsDlgButtonChecked(hwnd,IDC_SAVEMRU)) ? 1 : 0;
      }
      return FALSE;


    case WM_SIZE:
      {
        RECT rc;

        int dxClient = LOWORD(lParam) - cxClient;
        int dyClient = HIWORD(lParam) - cyClient;
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);

        GetWindowRect(GetDlgItem(hwnd,IDC_RESIZEGRIP),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_RESIZEGRIP),NULL,rc.left+dxClient,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDC_RESIZEGRIP),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDOK),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDOK),NULL,rc.left+dxClient,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDOK),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDCANCEL),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDCANCEL),NULL,rc.left+dxClient,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDCANCEL),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDC_FILEMRU),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_FILEMRU),NULL,0,0,rc.right-rc.left+dxClient,rc.bottom-rc.top+dyClient,SWP_NOZORDER|SWP_NOMOVE);
        ListView_SetColumnWidth(GetDlgItem(hwnd,IDC_FILEMRU),0,LVSCW_AUTOSIZE_USEHEADER);
        InvalidateRect(GetDlgItem(hwnd,IDC_FILEMRU),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDC_SAVEMRU),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_SAVEMRU),NULL,rc.left,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDC_SAVEMRU),NULL,TRUE);
      }
      return TRUE;


    case WM_GETMINMAXINFO:
      {
        LPMINMAXINFO lpmmi = (LPMINMAXINFO)lParam;
        lpmmi->ptMinTrackSize.x = mmiPtMinX;
        lpmmi->ptMinTrackSize.y = mmiPtMaxY;
        //lpmmi->ptMaxTrackSize.y = mmiPtMaxY;
      }
      return TRUE;


    case WM_NOTIFY: {
      if (((LPNMHDR)(lParam))->idFrom == IDC_FILEMRU) {

      switch (((LPNMHDR)(lParam))->code) {

        case NM_DBLCLK:
          SendMessage(hwnd,WM_COMMAND,MAKELONG(IDOK,1),0);
          break;


        case LVN_GETDISPINFO: {
            /*
            LV_DISPINFO *lpdi = (LPVOID)lParam;

            if (lpdi->item.mask & LVIF_IMAGE) {

              char tch[MAX_PATH];
              LV_ITEM lvi;
              SHFILEINFO shfi;
              DWORD dwFlags = SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_ATTRIBUTES | SHGFI_ATTR_SPECIFIED;
              DWORD dwAttr  = 0;

              ZeroMemory(&lvi,sizeof(LV_ITEM));

              lvi.mask = LVIF_TEXT;
              lvi.pszText = tch;
              lvi.cchTextMax = COUNTOF(tch);
              lvi.iItem = lpdi->item.iItem;

              ListView_GetItem(GetDlgItem(hwnd,IDC_FILEMRU),&lvi);

              if (!PathFileExists(tch)) {
                dwFlags |= SHGFI_USEFILEATTRIBUTES;
                dwAttr = FILE_ATTRIBUTE_NORMAL;
                shfi.dwAttributes = 0;
                SHGetFileInfo(PathFindFileName(tch),dwAttr,&shfi,sizeof(SHFILEINFO),dwFlags);
              }

              else {
                shfi.dwAttributes = SFGAO_LINK | SFGAO_SHARE;
                SHGetFileInfo(tch,dwAttr,&shfi,sizeof(SHFILEINFO),dwFlags);
              }

              lpdi->item.iImage = shfi.iIcon;
              lpdi->item.mask |= LVIF_DI_SETITEM;

              lpdi->item.stateMask = 0;
              lpdi->item.state = 0;

              if (shfi.dwAttributes & SFGAO_LINK) {
                lpdi->item.mask |= LVIF_STATE;
                lpdi->item.stateMask |= LVIS_OVERLAYMASK;
                lpdi->item.state |= INDEXTOOVERLAYMASK(2);
              }

              if (shfi.dwAttributes & SFGAO_SHARE) {
                lpdi->item.mask |= LVIF_STATE;
                lpdi->item.stateMask |= LVIS_OVERLAYMASK;
                lpdi->item.state |= INDEXTOOVERLAYMASK(1);
              }

              dwAttr = GetFileAttributes(tch);

              if (!flagNoFadeHidden &&
                  dwAttr != INVALID_FILE_ATTRIBUTES &&
                  dwAttr & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM)) {
                lpdi->item.mask |= LVIF_STATE;
                lpdi->item.stateMask |= LVIS_CUT;
                lpdi->item.state |= LVIS_CUT;
              }
            }
            */
          }
          break;


        case LVN_ITEMCHANGED:
        case LVN_DELETEITEM:
            EnableWindow(GetDlgItem(hwnd,IDOK),ListView_GetSelectedCount(GetDlgItem(hwnd,IDC_FILEMRU)));
            break;
          }
        }
      }

      return TRUE;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case 0x00A0:
          {
            int i;
            char tch[MAX_PATH];
            LV_ITEM lvi;
            SHFILEINFO shfi;

            DWORD dwtid;
            LPICONTHREADINFO lpit = (LPVOID)GetProp(hwnd,"it");

            SetEvent(lpit->hExitThread);
            while (WaitForSingleObject(lpit->hTerminatedThread,0) != WAIT_OBJECT_0) {
              MSG msg;
              if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
              }
            }
            ResetEvent(lpit->hExitThread);
            SetEvent(lpit->hTerminatedThread);

            ListView_DeleteAllItems(GetDlgItem(hwnd,IDC_FILEMRU));

            ZeroMemory(&lvi,sizeof(LV_ITEM));
            lvi.mask = LVIF_TEXT | LVIF_IMAGE;

            SHGetFileInfo("Icon",FILE_ATTRIBUTE_NORMAL,&shfi,sizeof(SHFILEINFO),
              SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON | SHGFI_SYSICONINDEX);
            lvi.iImage = shfi.iIcon;

            for (i = 0; i < MRU_Enum(pFileMRU,0,NULL,0); i++) {
              MRU_Enum(pFileMRU,i,tch,COUNTOF(tch));
              //  SendDlgItemMessage(hwnd,IDC_FILEMRU,LB_ADDSTRING,0,(LPARAM)tch); }
              //  SendDlgItemMessage(hwnd,IDC_FILEMRU,LB_SETCARETINDEX,0,FALSE);
              lvi.iItem = i;
              lvi.pszText = tch;
              ListView_InsertItem(GetDlgItem(hwnd,IDC_FILEMRU),&lvi);
            }

            ListView_SetItemState(GetDlgItem(hwnd,IDC_FILEMRU),0,LVIS_FOCUSED,LVIS_FOCUSED);
            ListView_SetColumnWidth(GetDlgItem(hwnd,IDC_FILEMRU),0,LVSCW_AUTOSIZE_USEHEADER);

            CreateThread(NULL,0,FileMRUIconThread,(LPVOID)lpit,0,&dwtid);
          }
          break;

        case IDC_FILEMRU:
          break;


        case IDOK:
          {
            char tch[MAX_PATH];
            //int  iItem;

            //if ((iItem = SendDlgItemMessage(hwnd,IDC_FILEMRU,LB_GETCURSEL,0,0)) != LB_ERR)
            if (ListView_GetSelectedCount(GetDlgItem(hwnd,IDC_FILEMRU)))
            {
              //SendDlgItemMessage(hwnd,IDC_FILEMRU,LB_GETTEXT,(WPARAM)iItem,(LPARAM)tch);
              LV_ITEM lvi;
              ZeroMemory(&lvi,sizeof(LV_ITEM));

              lvi.mask = LVIF_TEXT;
              lvi.pszText = tch;
              lvi.cchTextMax = COUNTOF(tch);
              lvi.iItem = ListView_GetNextItem(GetDlgItem(hwnd,IDC_FILEMRU),-1,LVNI_ALL | LVNI_SELECTED);

              ListView_GetItem(GetDlgItem(hwnd,IDC_FILEMRU),&lvi);

              PathUnquoteSpaces(tch);

              if (!PathFileExists(tch)) {

                // Ask...
                if (IDYES == MsgBox(MBYESNO,IDS_ERR_MRUDLG)) {

                    MRU_Delete(pFileMRU,lvi.iItem);

                    //SendDlgItemMessage(hwnd,IDC_FILEMRU,LB_DELETESTRING,(WPARAM)iItem,0);
                    //ListView_DeleteItem(GetDlgItem(hwnd,IDC_FILEMRU),lvi.iItem);
                    // must use IDM_VIEW_REFRESH, index might change...
                    SendMessage(hwnd,WM_COMMAND,MAKELONG(0x00A0,1),0);

                    //EnableWindow(GetDlgItem(hwnd,IDOK),
                    //  (LB_ERR != SendDlgItemMessage(hwnd,IDC_GOTO,LB_GETCURSEL,0,0)));

                    EnableWindow(GetDlgItem(hwnd,IDOK),
                      ListView_GetSelectedCount(GetDlgItem(hwnd,IDC_FILEMRU)));
                }
              }

              else {
                lstrcpy((LPSTR)GetWindowLong(hwnd,DWL_USER),tch);
                EndDialog(hwnd,IDOK);
              }
            }
          }
          break;


        case IDCANCEL:
          EndDialog(hwnd,IDCANCEL);
          break;

      }

      return TRUE;

  }

  return FALSE;

}


//=============================================================================
//
//  FileMRUDlg()
//
//
BOOL FileMRUDlg(HWND hwnd,LPSTR lpstrFile)
{

  if (IDOK == DialogBoxParam(g_hInstance,MAKEINTRESOURCE(IDD_FILEMRU),
                hwnd,FileMRUDlgProc,(LPARAM)lpstrFile))
    return TRUE;
  else
    return FALSE;

}


//=============================================================================
//
//  ColumnWrapDlgProc()
//
//  Controls: 100 Edit
//
BOOL CALLBACK ColumnWrapDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  static int *piNumber;

  switch(umsg)
  {

    case WM_INITDIALOG:
      {

        piNumber = (int*)lParam;

        SetDlgItemInt(hwnd,100,*piNumber,FALSE);
        SendDlgItemMessage(hwnd,100,EM_LIMITTEXT,15,0);

        CenterDlgInParent(hwnd);

      }
      return TRUE;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case IDOK: {

          BOOL fTranslated;

          int iNewNumber = GetDlgItemInt(hwnd,100,&fTranslated,FALSE);

          if (fTranslated)
          {
            *piNumber = iNewNumber;

            EndDialog(hwnd,IDOK);
          }

          else
            PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,100)),1);

          }
          break;


        case IDCANCEL:
          EndDialog(hwnd,IDCANCEL);
          break;

      }

      return TRUE;

  }

  return FALSE;

}


//=============================================================================
//
//  ColumnWrapDlg()
//
BOOL ColumnWrapDlg(HWND hwnd,UINT uidDlg,int *iNumber)
{

  int iResult;

  iResult = DialogBoxParam(
              g_hInstance,
              MAKEINTRESOURCE(uidDlg),
              hwnd,
              ColumnWrapDlgProc,(LPARAM)iNumber);

  return (iResult == IDOK) ? TRUE : FALSE;

}


//=============================================================================
//
//  WordWrapSettingsDlgProc()
//
//  Controls: 100 Edit
//            101 Combo
//
extern BOOL bShowWordWrapSymbols;
extern int  iWordWrapSymbols;

BOOL CALLBACK WordWrapSettingsDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  static int *piNumber;

  switch(umsg)
  {

    case WM_INITDIALOG:
      {

        char tch[512];
        char *p1, *p2;

        piNumber = (int*)lParam;

        SetDlgItemInt(hwnd,100,*piNumber,FALSE);
        SendDlgItemMessage(hwnd,100,EM_LIMITTEXT,15,0);

        // Load options
        GetString(IDS_WORDWRAPVISUALS,tch,COUNTOF(tch));
        lstrcat(tch,"|");
        p1 = tch;
        while (p2 = strchr(p1,'|')) {
          *p2++ = '\0';
          if (*p1)
            SendDlgItemMessage(hwnd,101,CB_ADDSTRING,0,(LPARAM)p1);
          p1 = p2;
        }

        SendDlgItemMessage(hwnd,101,CB_SETCURSEL,(WPARAM)(bShowWordWrapSymbols) ? iWordWrapSymbols+1 : 0,0);
        SendDlgItemMessage(hwnd,101,CB_SETEXTENDEDUI,TRUE,0);

        CenterDlgInParent(hwnd);

      }
      return TRUE;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case IDOK: {

          BOOL fTranslated;
          int  iSel;

          int iNewNumber = GetDlgItemInt(hwnd,100,&fTranslated,FALSE);

          if (fTranslated)
          {
            *piNumber = iNewNumber;

            iSel = SendDlgItemMessage(hwnd,101,CB_GETCURSEL,0,0);
            if (iSel > 0) {
              bShowWordWrapSymbols = TRUE;
              iWordWrapSymbols = iSel-1;
            }
            else {
              bShowWordWrapSymbols = FALSE;
            }

            EndDialog(hwnd,IDOK);
          }

          else
            PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,100)),1);

          }
          break;


        case IDCANCEL:
          EndDialog(hwnd,IDCANCEL);
          break;

      }

      return TRUE;

  }

  return FALSE;

}


//=============================================================================
//
//  WordWrapSettingsDlg()
//
BOOL WordWrapSettingsDlg(HWND hwnd,UINT uidDlg,int *iNumber)
{

  int iResult;

  iResult = DialogBoxParam(
              g_hInstance,
              MAKEINTRESOURCE(uidDlg),
              hwnd,
              WordWrapSettingsDlgProc,(LPARAM)iNumber);

  return (iResult == IDOK) ? TRUE : FALSE;

}


//=============================================================================
//
//  LongLineSettingsDlgProc()
//
//  Controls: 100 Edit
//            101 Radio1
//            102 Radio2
//
extern int iLongLineMode;

BOOL CALLBACK LongLineSettingsDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  static int *piNumber;

  switch(umsg)
  {

    case WM_INITDIALOG:
      {

        piNumber = (int*)lParam;

        SetDlgItemInt(hwnd,100,*piNumber,FALSE);
        SendDlgItemMessage(hwnd,100,EM_LIMITTEXT,15,0);

        if (iLongLineMode == EDGE_LINE)
          CheckRadioButton(hwnd,101,102,101);
        else
          CheckRadioButton(hwnd,101,102,102);

        CenterDlgInParent(hwnd);

      }
      return TRUE;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case IDOK: {

          BOOL fTranslated;

          int iNewNumber = GetDlgItemInt(hwnd,100,&fTranslated,FALSE);

          if (fTranslated)
          {
            *piNumber = iNewNumber;

            iLongLineMode = (IsDlgButtonChecked(hwnd,101)) ? EDGE_LINE : EDGE_BACKGROUND;

            EndDialog(hwnd,IDOK);
          }

          else
            PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,100)),1);

          }
          break;


        case IDCANCEL:
          EndDialog(hwnd,IDCANCEL);
          break;

      }

      return TRUE;

  }

  return FALSE;

}


//=============================================================================
//
//  LongLineSettingsDlg()
//
BOOL LongLineSettingsDlg(HWND hwnd,UINT uidDlg,int *iNumber)
{

  int iResult;

  iResult = DialogBoxParam(
              g_hInstance,
              MAKEINTRESOURCE(uidDlg),
              hwnd,
              LongLineSettingsDlgProc,(LPARAM)iNumber);

  return (iResult == IDOK) ? TRUE : FALSE;

}


//=============================================================================
//
//  TabSettingsDlgProc()
//
//  Controls: 100 Edit
//            101 Edit
//            102 Check
//
extern int iTabWidth;
extern int iIndentWidth;
extern BOOL bTabsAsSpaces;

BOOL CALLBACK TabSettingsDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  switch(umsg)
  {

    case WM_INITDIALOG:
      {

        SetDlgItemInt(hwnd,100,iTabWidth,FALSE);
        SendDlgItemMessage(hwnd,100,EM_LIMITTEXT,15,0);

        SetDlgItemInt(hwnd,101,iIndentWidth,FALSE);
        SendDlgItemMessage(hwnd,101,EM_LIMITTEXT,15,0);

        if (bTabsAsSpaces)
          CheckDlgButton(hwnd,102,BST_CHECKED);

        CenterDlgInParent(hwnd);

      }
      return TRUE;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case IDOK: {

          BOOL fTranslated1,fTranslated2;

          int iNewTabWidth = GetDlgItemInt(hwnd,100,&fTranslated1,FALSE);
          int iNewIndentWidth = GetDlgItemInt(hwnd,101,&fTranslated2,FALSE);

          if (fTranslated1 && fTranslated2)
          {
            iTabWidth = iNewTabWidth;
            iIndentWidth = iNewIndentWidth;

            bTabsAsSpaces = (IsDlgButtonChecked(hwnd,102)) ? TRUE : FALSE;

            EndDialog(hwnd,IDOK);
          }

          else
            PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,(fTranslated1) ? 101 : 100)),1);

          }
          break;


        case IDCANCEL:
          EndDialog(hwnd,IDCANCEL);
          break;

      }

      return TRUE;

  }

  return FALSE;

}


//=============================================================================
//
//  TabSettingsDlg()
//
BOOL TabSettingsDlg(HWND hwnd,UINT uidDlg,int *iNumber)
{

  int iResult;

  iResult = DialogBoxParam(
              g_hInstance,
              MAKEINTRESOURCE(uidDlg),
              hwnd,
              TabSettingsDlgProc,(LPARAM)iNumber);

  return (iResult == IDOK) ? TRUE : FALSE;

}


//=============================================================================
//
//  SelectEncodingDlgProc()
//
//  Controls: 100 Combo
//            IDC_CPINFO
//
BOOL CALLBACK SelectEncodingDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{
  static int *piOption;

  switch(umsg)
  {
    case WM_INITDIALOG:
      {
        char tch[512], tchFmt[64];
        char *p1, *p2;
        CPINFOEX cpi;

        piOption = (int*)lParam;

        GetCPInfoEx(GetACP(),0,&cpi);
        GetDlgItemText(hwnd,101,tchFmt,COUNTOF(tchFmt));
        wsprintf(tch,tchFmt,cpi.CodePageName);
        lstrcat(tch,"|");
        p1 = tch;
        while (p2 = strchr(p1,'|')) {
          *p2++ = '\0';
          if (*p1)
            SendDlgItemMessage(hwnd,100,CB_ADDSTRING,0,(LPARAM)p1);
          p1 = p2;
        }

        SendDlgItemMessage(hwnd,100,CB_SETCURSEL,(WPARAM)*piOption,0);
        SendDlgItemMessage(hwnd,100,CB_SETEXTENDEDUI,TRUE,0);

        if (bSkipUnicodeDetection)
          CheckDlgButton(hwnd,IDC_NOUNICODEDETECTION,BST_CHECKED);

        CenterDlgInParent(hwnd);
      }
      return TRUE;


    case WM_COMMAND:
      switch(LOWORD(wParam))
      {
        case IDOK: {
            *piOption = SendDlgItemMessage(hwnd,100,CB_GETCURSEL,0,0);
            bSkipUnicodeDetection = (IsDlgButtonChecked(hwnd,IDC_NOUNICODEDETECTION) == BST_CHECKED) ? 1 : 0;
            EndDialog(hwnd,IDOK);
          }
          break;

        case IDCANCEL:
          EndDialog(hwnd,IDCANCEL);
          break;
      }
      return TRUE;
  }
  return FALSE;
}


//=============================================================================
//
//  SelectEncodingDlg()
//
BOOL SelectEncodingDlg(HWND hwnd,int *iOption)
{

  int iResult;

  iResult = DialogBoxParam(
              g_hInstance,
              MAKEINTRESOURCE(IDD_ENCODING),
              hwnd,
              SelectEncodingDlgProc,
              (LPARAM)iOption);

  return (iResult == IDOK) ? TRUE : FALSE;

}

//=============================================================================
//
//  SelectLineEndingDlgProc()
//
//  Controls: 100 Combo
//            IDC_CONSISTENTEOLS
//
BOOL CALLBACK SelectLineEndingDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{
  static int *piOption;

  switch(umsg)
  {
    case WM_INITDIALOG:
      {
        char tch[512];
        char *p1, *p2;

        piOption = (int*)lParam;

        // Load options
        GetDlgItemText(hwnd,101,tch,COUNTOF(tch));
        lstrcat(tch,"|");
        p1 = tch;
        while (p2 = strchr(p1,'|')) {
          *p2++ = '\0';
          if (*p1)
            SendDlgItemMessage(hwnd,100,CB_ADDSTRING,0,(LPARAM)p1);
          p1 = p2;
        }

        SendDlgItemMessage(hwnd,100,CB_SETCURSEL,(WPARAM)*piOption,0);
        SendDlgItemMessage(hwnd,100,CB_SETEXTENDEDUI,TRUE,0);

        if (bFixLineEndings)
          CheckDlgButton(hwnd,IDC_CONSISTENTEOLS,BST_CHECKED);

        CenterDlgInParent(hwnd);
      }
      return TRUE;


    case WM_COMMAND:
      switch(LOWORD(wParam))
      {
        case IDOK: {
            *piOption = SendDlgItemMessage(hwnd,100,CB_GETCURSEL,0,0);
            bFixLineEndings = (IsDlgButtonChecked(hwnd,IDC_CONSISTENTEOLS) == BST_CHECKED) ? 1 : 0;
            EndDialog(hwnd,IDOK);
          }
          break;

        case IDCANCEL:
          EndDialog(hwnd,IDCANCEL);
          break;
      }
      return TRUE;
  }
  return FALSE;
}


//=============================================================================
//
//  SelectLineEndingDlg()
//
BOOL SelectLineEndingDlg(HWND hwnd,int *iOption)
{

  int iResult;

  iResult = DialogBoxParam(
              g_hInstance,
              MAKEINTRESOURCE(IDD_EOLMODE),
              hwnd,
              SelectLineEndingDlgProc,
              (LPARAM)iOption);

  return (iResult == IDOK) ? TRUE : FALSE;

}


//=============================================================================
//
//  InfoBoxDlgProc()
//
//
typedef struct _infobox {
  int    uidMessage;
  LPCSTR lpstrSetting;
} INFOBOX, *LPINFOBOX;

BOOL CALLBACK InfoBoxDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{
  char tch[512];
  static LPINFOBOX lpib;

  switch(umsg)
  {
    case WM_INITDIALOG:
      lpib = (LPINFOBOX)lParam;
      SendDlgItemMessage(hwnd,IDC_INFOBOXICON,STM_SETICON,
        (WPARAM)LoadIcon(NULL,IDI_EXCLAMATION),0);
      GetString(lpib->uidMessage,tch,COUNTOF(tch));
      SetDlgItemText(hwnd,IDC_INFOBOXTEXT,tch);
      CenterDlgInParent(hwnd);
      return TRUE;

    case WM_COMMAND:
      switch(LOWORD(wParam))
      {
        case IDOK:
        case IDCANCEL:
        case IDYES:
        case IDNO:
          if (IsDlgButtonChecked(hwnd,IDC_INFOBOXCHECK))
            IniSetInt("Suppressed Messages",lpib->lpstrSetting,1);
          EndDialog(hwnd,LOWORD(wParam));
          break;
      }
      return TRUE;
  }
  return FALSE;
}


//=============================================================================
//
//  InfoBox()
//
//
int InfoBox(int iType,int uidMessage,LPCSTR lpstrSetting)
{

  HWND hwnd;
  int idDlg;
  INFOBOX ib;

  if (IniGetInt("Suppressed Messages",lpstrSetting,0))
    return (iType == MBYESNO) ? IDYES : IDOK;

  idDlg = (iType == MBYESNO) ? IDD_INFOBOX2 : IDD_INFOBOX;

  ib.uidMessage = uidMessage;
  ib.lpstrSetting = lpstrSetting;

  if (!(hwnd = GetFocus()))
    hwnd = hwndMain;

  MessageBeep(MB_ICONEXCLAMATION);

  return DialogBoxParam(
           g_hInstance,
           MAKEINTRESOURCE(idDlg),
           hwnd,
           InfoBoxDlgProc,
           (LPARAM)&ib);

}



//  End of Dialogs.c
