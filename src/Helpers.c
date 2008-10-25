/******************************************************************************
*
*
* Notepad2
*
* Helpers.c
*   General helper functions
*   Parts taken from SciTE, (c) Neil Hodgson, http://www.scintilla.org
*   MinimizeToTray (c) 2000 Matthew Ellis
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
#include <shlobj.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include "helpers.h"
#include "resource.h"



//=============================================================================
//
//  Manipulation of cached ini file sections
//
int IniSectionGetString(
      LPCSTR lpCachedIniSection,
      LPCSTR lpName,
      LPCSTR lpDefault,
      LPSTR lpReturnedString,
      int cchReturnedString)
{
  char *p = (char *)lpCachedIniSection;
  char tch[256];
  int  ich;

  if (p) {
    lstrcpy(tch,lpName);
    lstrcat(tch,"=");
    ich = lstrlen(tch);

    while (*p) {
      if (StrCmpNI(p,tch,ich) == 0) {
        lstrcpyn(lpReturnedString,p + ich,cchReturnedString);
        return(lstrlen(lpReturnedString));
      }
      else
        p = StrEnd(p) + 1;
    }
  }
  lstrcpyn(lpReturnedString,lpDefault,cchReturnedString);
  return(lstrlen(lpReturnedString));
}


int IniSectionGetInt(
      LPCSTR lpCachedIniSection,
      LPCSTR lpName,
      int iDefault)
{
  char *p = (char *)lpCachedIniSection;
  char tch[256];
  int  ich;
  int  i;

  if (p) {
    lstrcpy(tch,lpName);
    lstrcat(tch,"=");
    ich = lstrlen(tch);

    while (*p) {
      if (StrCmpNI(p,tch,ich) == 0) {
        if (sscanf(p + ich,"%i",&i) == 1)
          return(i);
        else
          return(iDefault);
      }
      else
        p = StrEnd(p) + 1;
    }
  }
  return(iDefault);
}


BOOL IniSectionSetString(LPSTR lpCachedIniSection,LPCSTR lpName,LPCSTR lpString)
{
  char tch[256];
  char *p = lpCachedIniSection;

  if (p) {
    while (*p) {
      p = StrEnd(p) + 1;
    }
    wsprintf(tch,"%s=%s",lpName,lpString);
    lstrcpy(p,tch);
    p = StrEnd(p) + 1;
    *p = 0;
    return(TRUE);
  }
  return(FALSE);
}


//=============================================================================
//
//  BeginWaitCursor()
//
void BeginWaitCursor()
{

  DestroyCursor(
    SetCursor(
    LoadCursor(NULL,IDC_WAIT)));

}


//=============================================================================
//
//  EndWaitCursor()
//
void EndWaitCursor()
{

  DestroyCursor(
    SetCursor(
    LoadCursor(NULL,IDC_ARROW)));

}


//=============================================================================
//
//  KeepWindowsAlive()
//
/*void KeepWindowsAlive()
{
  MSG msg;
  if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}*/


//=============================================================================
//
//  IsWindows2korLater()
//
BOOL IsWindows2korLater()
{
  static BOOL bIsWindows2korLater;
  static BOOL bCachedResult;
  if (bCachedResult)
    return(bIsWindows2korLater);
  else {
    OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    bIsWindows2korLater =
       (osvi.dwMajorVersion >= 5);
    bCachedResult = TRUE;
    return(bIsWindows2korLater);
  }
}


//=============================================================================
//
//  IsWindowsXPorLater()
//
//BOOL IsWindowsXPorLater()
//{
//  static BOOL bIsWindowsXPorLater;
//  static BOOL bCachedResult;
//  if (bCachedResult)
//    return(bIsWindowsXPorLater);
//  else {
//    OSVERSIONINFO osvi;
//    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
//    GetVersionEx(&osvi);
//    bIsWindowsXPorLater =
//       ((osvi.dwMajorVersion > 5) ||
//       ((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 1)));
//    bCachedResult = TRUE;
//    return(bIsWindowsXPorLater);
//  }
//}


//=============================================================================
//
//  PrivateIsAppThemed()
//
extern HMODULE hModUxTheme;
BOOL PrivateIsAppThemed()
{
  static FARPROC pfnIsAppThemed;
  BOOL bIsAppThemed = FALSE;

  if (hModUxTheme) {
    if (!pfnIsAppThemed)
      pfnIsAppThemed = GetProcAddress(hModUxTheme,"IsAppThemed");

    if (pfnIsAppThemed)
      bIsAppThemed = pfnIsAppThemed();
  }
  return bIsAppThemed;
}


//=============================================================================
//
//  SetWindowTitle()
//
BOOL bFreezeAppTitle = FALSE;

BOOL SetWindowTitle(HWND hwnd,UINT uIDAppName,UINT uIDUntitled,
                    LPCSTR lpszFile,int iFormat,BOOL bModified,
                    UINT uIDReadOnly,BOOL bReadOnly)
{

  char szUntitled[128],szAppName[128],szReadOnly[32],szTitle[512];
  static const char *pszSep = " - ";
  static const char *pszMod = "* ";

  if (bFreezeAppTitle)
    return FALSE;

  if (!GetString(uIDAppName,szAppName,COUNTOF(szAppName)) ||
      !GetString(uIDUntitled,szUntitled,COUNTOF(szUntitled)))
    return FALSE;

  if (bModified)
    lstrcpy(szTitle,pszMod);
  else
    lstrcpy(szTitle,"");

  if (lstrlen(lpszFile))
  {
    if (iFormat < 2 && !PathIsRoot(lpszFile))
    {
      SHFILEINFO shfi;
      if (SHGetFileInfo2(lpszFile,0,&shfi,sizeof(SHFILEINFO),SHGFI_DISPLAYNAME))
        lstrcat(szTitle,shfi.szDisplayName);
      else
        lstrcat(szTitle,PathFindFileName(lpszFile));
      if (iFormat == 1) {
        char tchPath[MAX_PATH];
        StrCpyN(tchPath,lpszFile,COUNTOF(tchPath));
        PathRemoveFileSpec(tchPath);
        StrCat(szTitle," [");
        StrCat(szTitle,tchPath);
        StrCat(szTitle,"]");
      }
    }
    else
      lstrcat(szTitle,lpszFile);
  }

  else
    lstrcat(szTitle,szUntitled);

  if (bReadOnly && GetString(uIDReadOnly,szReadOnly,COUNTOF(szReadOnly)))
  {
    lstrcat(szTitle," ");
    lstrcat(szTitle,szReadOnly);
  }

  lstrcat(szTitle,pszSep);
  lstrcat(szTitle,szAppName);

  return SetWindowText(hwnd,szTitle);

}


//=============================================================================
//
//  SetWindowTransparentMode()
//
void SetWindowTransparentMode(HWND hwnd,BOOL bTransparentMode)
{
  FARPROC fp;
  int  iAlphaPercent;
  BYTE bAlpha;

  if (bTransparentMode) {
    if (fp = GetProcAddress(GetModuleHandle("User32"),"SetLayeredWindowAttributes")) {
      SetWindowLong(hwnd,GWL_EXSTYLE,
        GetWindowLong(hwnd,GWL_EXSTYLE) | /*WS_EX_LAYERED*/0x00080000);

      // get opacity level from registry
      iAlphaPercent = IniGetInt("Settings2","OpacityLevel",75);
      if (iAlphaPercent < 0 || iAlphaPercent > 100)
        iAlphaPercent = 75;
      bAlpha = iAlphaPercent * 255 / 100;

      fp(hwnd,0,bAlpha,/*LWA_ALPHA*/0x00000002);
    }
  }

  else
    SetWindowLong(hwnd,GWL_EXSTYLE,
      GetWindowLong(hwnd,GWL_EXSTYLE) & ~/*WS_EX_LAYERED*/0x00080000);
}


//=============================================================================
//
//  CenterDlgInParent()
//
void CenterDlgInParent(HWND hDlg)
{

  RECT rcDlg;
  HWND hParent;
  RECT rcParent;
  MONITORINFO mi;
  HMONITOR hMonitor;

  int xMin, yMin, xMax, yMax, x, y;

  GetWindowRect(hDlg,&rcDlg);

  hParent = GetParent(hDlg);
  GetWindowRect(hParent,&rcParent);

  hMonitor = MonitorFromRect(&rcParent,MONITOR_DEFAULTTONEAREST);
  mi.cbSize = sizeof(mi);
  GetMonitorInfo(hMonitor,&mi);

  xMin = mi.rcWork.left;
  yMin = mi.rcWork.top;

  xMax = (mi.rcWork.right) - (rcDlg.right - rcDlg.left);
  yMax = (mi.rcWork.bottom) - (rcDlg.bottom - rcDlg.top);

  if ((rcParent.right - rcParent.left) - (rcDlg.right - rcDlg.left) > 20)
    x = rcParent.left + (((rcParent.right - rcParent.left) - (rcDlg.right - rcDlg.left)) / 2);
  else
    x = rcParent.left + 70;

  if ((rcParent.bottom - rcParent.top) - (rcDlg.bottom - rcDlg.top) > 20)
    y = rcParent.top  + (((rcParent.bottom - rcParent.top) - (rcDlg.bottom - rcDlg.top)) / 2);
  else
    y = rcParent.top + 60;

  SetWindowPos(hDlg,NULL,max(xMin,min(xMax,x)),max(yMin,min(yMax,y)),0,0,SWP_NOZORDER|SWP_NOSIZE);

}


//=============================================================================
//
//  MakeBitmapButton()
//
void MakeBitmapButton(HWND hwnd,int nCtlId,HINSTANCE hInstance,UINT uBmpId)
{
  HWND hwndCtl = GetDlgItem(hwnd,nCtlId);
  BITMAP bmp;
  struct { HIMAGELIST himl; RECT margin; UINT uAlign; } bi;
  HBITMAP hBmp = LoadImage(hInstance,MAKEINTRESOURCE(uBmpId),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
  GetObject(hBmp,sizeof(BITMAP),&bmp);
  bi.himl = ImageList_Create(bmp.bmWidth,bmp.bmHeight,ILC_COLOR32|ILC_MASK,1,0);
  ImageList_AddMasked(bi.himl,hBmp,CLR_DEFAULT);
  DeleteObject(hBmp);
  SetRect(&bi.margin,0,0,0,0);
  bi.uAlign = 4/*BUTTON_IMAGELIST_ALIGN_CENTER*/;
  SendMessage(hwndCtl,0x1600+0x0002/*BCM_FIRST+BCM_SETIMAGELIST*/,0,(LPARAM)&bi);
}


//=============================================================================
//
//  StatusSetText()
//
BOOL StatusSetText(HWND hwnd,UINT nPart,LPCSTR lpszText)
{

  UINT uFlags = (nPart == 255) ? nPart|SBT_NOBORDERS : nPart;
  return SendMessage(hwnd,SB_SETTEXT,uFlags,(LPARAM)lpszText);

}


//=============================================================================
//
//  SendWMSize()
//
LRESULT SendWMSize(HWND hwnd)
{
  RECT rc; GetClientRect(hwnd,&rc);
  return(SendMessage(hwnd,WM_SIZE,SIZE_RESTORED,
         MAKELPARAM(rc.right,rc.bottom)));
}


//=============================================================================
//
//  StatusSetTextID()
//
BOOL StatusSetTextID(HWND hwnd,UINT nPart,UINT uID)
{

  char szText[256];
  UINT uFlags = (nPart == 255) ? nPart|SBT_NOBORDERS : nPart;

  if (!uID)
  {
    SendMessage(hwnd,SB_SETTEXT,uFlags,0);
    return TRUE;
  }

  if (!GetString(uID,szText,256))
    return FALSE;

  return SendMessage(hwnd,SB_SETTEXT,uFlags,(LPARAM)szText);

}


//=============================================================================
//
//  StatusCalcPaneWidth()
//
int StatusCalcPaneWidth(HWND hwnd,LPCSTR lpsz)
{
  SIZE  size;
  HDC   hdc   = GetDC(hwnd);
  HFONT hfont = (HFONT)SendMessage(hwnd,WM_GETFONT,0,0);
  HFONT hfold = SelectObject(hdc,hfont);
  int   mmode = SetMapMode(hdc,MM_TEXT);

  GetTextExtentPoint32(hdc,lpsz,lstrlen(lpsz),&size);

  SetMapMode(hdc,mmode);
  SelectObject(hdc,hfold);
  ReleaseDC(hwnd,hdc);

  return(size.cx + 9);
}


//=============================================================================
//
//  Toolbar_Get/SetButtons()
//
int Toolbar_GetButtons(HWND hwnd,int cmdBase,LPSTR lpszButtons,int cchButtons)
{
  char tchButtons[512];
  char tchItem[32];
  int i,c;
  TBBUTTON tbb;

  lstrcpy(tchButtons,"");
  c = min(50,SendMessage(hwnd,TB_BUTTONCOUNT,0,0));

  for (i = 0; i < c; i++) {
    SendMessage(hwnd,TB_GETBUTTON,(WPARAM)i,(LPARAM)&tbb);
    wsprintf(tchItem,"%i ",
      (tbb.idCommand==0)?0:tbb.idCommand-cmdBase+1);
    lstrcat(tchButtons,tchItem);
  }
  TrimString(tchButtons);
  lstrcpyn(lpszButtons,tchButtons,cchButtons);
  return(c);
}

int Toolbar_SetButtons(HWND hwnd,int cmdBase,LPCSTR lpszButtons,LPCTBBUTTON ptbb,int ctbb)
{
  char tchButtons[512];
  char *p;
  int i,c;
  int iCmd;

  ZeroMemory(tchButtons,COUNTOF(tchButtons)/sizeof(tchButtons[1]));
  lstrcpyn(tchButtons,lpszButtons,COUNTOF(tchButtons)-2);

  c = SendMessage(hwnd,TB_BUTTONCOUNT,0,0);
  for (i = 0; i < c; i++)
    SendMessage(hwnd,TB_DELETEBUTTON,0,0);

  for (i = 0; i < COUNTOF(tchButtons); i++)
    if (tchButtons[i] == ' ') tchButtons[i] = 0;

  p = tchButtons;
  while (*p) {
    if (sscanf(p,"%i",&iCmd) == 1) {
      iCmd = (iCmd==0)?0:iCmd+cmdBase-1;
      for (i = 0; i < ctbb; i++) {
        if (ptbb[i].idCommand == iCmd) {
          SendMessage(hwnd,TB_ADDBUTTONS,(WPARAM)1,(LPARAM)&ptbb[i]);
          break;
        }
      }
    }
    p = StrEnd(p)+1;
  }
  return(SendMessage(hwnd,TB_BUTTONCOUNT,0,0));
}


//=============================================================================
//
//  IsCmdEnabled()
//
BOOL IsCmdEnabled(HWND hwnd,UINT uId)
{

  HMENU hmenu;
  UINT ustate;

  hmenu = GetMenu(hwnd);

  SendMessage(hwnd,WM_INITMENU,(WPARAM)hmenu,0);

  ustate = GetMenuState(hmenu,uId,MF_BYCOMMAND);

  if (ustate == 0xFFFFFFFF)
    return TRUE;

  else
    return (!(ustate & (MF_GRAYED|MF_DISABLED)));

}


//=============================================================================
//
//  FormatString()
//
int FormatString(LPSTR lpOutput,int nOutput,UINT uIdFormat,...)
{

  char *p = malloc(nOutput);

  if (GetString(uIdFormat,p,nOutput))
    wvsprintf(lpOutput,p,(LPVOID)(&uIdFormat+1));

  free(p);

  return lstrlen(lpOutput);

}


//=============================================================================
//
//  FormatBytes()
//
void FormatBytes(LPSTR lpOutput,int nOutput,DWORD dwBytes)
{

  char tch[256];
  int i;
  double dBytes = dwBytes;
  static const char *pBytes[] = { "Bytes","KB","MB","GB" };

  if (dwBytes > 1023)
  {
    for (i = 0; i < 4; i++)
    {

      if (dBytes >= 1024.0)
        dBytes /= 1024.0;

      else
        break;
    }
    sprintf(tch,"%.2f",dBytes);
    GetNumberFormat(LOCALE_USER_DEFAULT,0,tch,NULL,lpOutput,nOutput);
    lstrcat(lpOutput," ");
    lstrcat(lpOutput,pBytes[i]);
  }

  else
  {
    sprintf(lpOutput,"%i",dwBytes);
    FormatNumberStr(lpOutput);
    lstrcat(lpOutput," ");
    lstrcat(lpOutput,pBytes[0]);
  }

}


///////////////////////////////////////////////////////////////////////////////
//
//
//  Name: PathIsLnkFile()
//
//  Purpose: Determine wheter pszPath is a Windows Shell Link File by
//           comparing the filename extension with ".lnk"
//
//  Manipulates:
//
BOOL PathIsLnkFile(LPCSTR pszPath)
{

  //char *pszExt;

  char tchResPath[256];

  if (!pszPath || !*pszPath)
    return FALSE;

/*pszExt = strrchr(pszPath,'.');

  if (!pszExt)
    return FALSE;

  if (!lstrcmpi(pszExt,".lnk"))
    return TRUE;

  else
    return FALSE;*/

  //if (!lstrcmpi(PathFindExtension(pszPath),".lnk"))
  //  return TRUE;

  //else
  //  return FALSE;

  if (lstrcmpi(PathFindExtension(pszPath),".lnk"))
    return FALSE;

  else
    return PathGetLnkPath(pszPath,tchResPath,COUNTOF(tchResPath));

}


///////////////////////////////////////////////////////////////////////////////
//
//
//  Name: PathGetLnkPath()
//
//  Purpose: Try to get the path to which a lnk-file is linked
//
//
//  Manipulates: pszResPath
//
BOOL PathGetLnkPath(LPCSTR pszLnkFile,LPSTR pszResPath,int cchResPath)
{

  IShellLink       *psl;
  WIN32_FIND_DATA  fd;
  BOOL             bSucceeded = FALSE;

  if (SUCCEEDED(CoCreateInstance(&CLSID_ShellLink,NULL,
                                 CLSCTX_INPROC_SERVER,
                                 &IID_IShellLink,&psl)))
  {
    IPersistFile *ppf;

    if (SUCCEEDED(psl->lpVtbl->QueryInterface(psl,&IID_IPersistFile,&ppf)))
    {
      WORD wsz[MAX_PATH];

      MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,
                          pszLnkFile,-1,wsz,MAX_PATH);

      if (SUCCEEDED(ppf->lpVtbl->Load(ppf,wsz,STGM_READ)))
      {
        if (NOERROR == psl->lpVtbl->GetPath(psl,pszResPath,cchResPath,&fd,0))
          bSucceeded = TRUE;
      }
      ppf->lpVtbl->Release(ppf);
    }
    psl->lpVtbl->Release(psl);
  }

  // This additional check seems reasonable
  if (!lstrlen(pszResPath))
    bSucceeded = FALSE;

  if (bSucceeded) {
    ExpandEnvironmentStringsEx(pszResPath,cchResPath);
    PathCanonicalizeEx(pszResPath);
  }

  return(bSucceeded);

}


///////////////////////////////////////////////////////////////////////////////
//
//
//  Name: PathIsLnkToDirectory()
//
//  Purpose: Determine wheter pszPath is a Windows Shell Link File which
//           refers to a directory
//
//  Manipulates: pszResPath
//
PathIsLnkToDirectory(LPCSTR pszPath,LPSTR pszResPath,int cchResPath)
{

  char tchResPath[MAX_PATH];

  if (PathIsLnkFile(pszPath)) {

    if (PathGetLnkPath(pszPath,tchResPath,sizeof(char)*COUNTOF(tchResPath))) {

      if (PathIsDirectory(tchResPath)) {

        lstrcpyn(pszResPath,tchResPath,cchResPath);
        return (TRUE);
      }

      else
        return FALSE;
      }

    else
      return FALSE;
    }

  else
    return FALSE;

}


///////////////////////////////////////////////////////////////////////////////
//
//
//  Name: PathCreateDeskLnk()
//
//  Purpose: Modified to create a desktop link to Notepad2
//
//  Manipulates:
//
BOOL PathCreateDeskLnk(LPCSTR pszDocument)
{

  char tchExeFile[MAX_PATH];
  char tchDocTemp[MAX_PATH];
  char tchArguments[MAX_PATH+16];
  char tchLinkDir[MAX_PATH];
  char tchDescription[64];

  char tchLnkFileName[MAX_PATH];

  IShellLink *psl;
  BOOL bSucceeded = FALSE;
  BOOL fMustCopy;

  if (!pszDocument || lstrlen(pszDocument) == 0)
    return TRUE;

  // init strings
  GetModuleFileName(NULL,tchExeFile,COUNTOF(tchExeFile));

  lstrcpy(tchDocTemp,pszDocument);
  PathQuoteSpaces(tchDocTemp);

  lstrcpy(tchArguments,"-n ");
  lstrcat(tchArguments,tchDocTemp);

  SHGetSpecialFolderPath(NULL,tchLinkDir,CSIDL_DESKTOP,TRUE);

  GetString(IDS_LINKDESCRIPTION,tchDescription,COUNTOF(tchDescription));

  // Try to construct a valid filename...
  if (!SHGetNewLinkInfo(pszDocument,tchLinkDir,tchLnkFileName,&fMustCopy,SHGNLI_PREFIXNAME))
    return(FALSE);

  if (SUCCEEDED(CoCreateInstance(&CLSID_ShellLink,NULL,
                                 CLSCTX_INPROC_SERVER,
                                 &IID_IShellLink,&psl)))
  {
    IPersistFile *ppf;

    if (SUCCEEDED(psl->lpVtbl->QueryInterface(psl,&IID_IPersistFile,&ppf)))
    {
      WORD wsz[MAX_PATH];

      MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,
                          tchLnkFileName,-1,wsz,MAX_PATH);

      psl->lpVtbl->SetPath(psl,tchExeFile);
      psl->lpVtbl->SetArguments(psl,tchArguments);
      psl->lpVtbl->SetDescription(psl,tchDescription);

      if (SUCCEEDED(ppf->lpVtbl->Save(ppf,wsz,TRUE)))
        bSucceeded = TRUE;

      ppf->lpVtbl->Release(ppf);
    }
    psl->lpVtbl->Release(psl);
  }

  return(bSucceeded);

}


///////////////////////////////////////////////////////////////////////////////
//
//
//  Name: PathCreateFavLnk()
//
//  Purpose: Modified to create a Notepad2 favorites link
//
//  Manipulates:
//
BOOL PathCreateFavLnk(LPCSTR pszName,LPCSTR pszTarget,LPCSTR pszDir)
{

  char tchLnkFileName[MAX_PATH];

  IShellLink *psl;
  BOOL bSucceeded = FALSE;

  if (!pszName || lstrlen(pszName) == 0)
    return TRUE;

  lstrcpy(tchLnkFileName,pszDir);
  PathAppend(tchLnkFileName,pszName);
  lstrcat(tchLnkFileName,".lnk");

  if (PathFileExists(tchLnkFileName))
    return FALSE;

  if (SUCCEEDED(CoCreateInstance(&CLSID_ShellLink,NULL,
                                 CLSCTX_INPROC_SERVER,
                                 &IID_IShellLink,&psl)))
  {
    IPersistFile *ppf;

    if (SUCCEEDED(psl->lpVtbl->QueryInterface(psl,&IID_IPersistFile,&ppf)))
    {
      WORD wsz[MAX_PATH];

      MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,
                          tchLnkFileName,-1,wsz,MAX_PATH);

      psl->lpVtbl->SetPath(psl,pszTarget);

      if (SUCCEEDED(ppf->lpVtbl->Save(ppf,wsz,TRUE)))
        bSucceeded = TRUE;

      ppf->lpVtbl->Release(ppf);
    }
    psl->lpVtbl->Release(psl);
  }

  return(bSucceeded);

}


//=============================================================================
//
//  TrimString()
//
BOOL TrimString(LPSTR lpString)
{

  LPSTR psz;


  if (!lpString || !*lpString)
    return FALSE;

  // Trim left
  psz = lpString;

  while (*psz == ' ')
    psz = CharNext(psz);

  MoveMemory(lpString,psz,lstrlen(psz) + 1);

  // Trim right
  psz = StrEnd(lpString);

  while (*(psz = CharPrev(lpString,psz)) == ' ')
    *psz = '\0';

  return TRUE;

}


//=============================================================================
//
//  ExtractFirstArgument()
//
BOOL ExtractFirstArgument(LPCSTR lpArgs,LPSTR lpArg1,LPSTR lpArg2)
{

  LPSTR psz;
  BOOL bQuoted = FALSE;

  lstrcpy(lpArg1,lpArgs);
  if (lpArg2)
    *lpArg2 = '\0';

  if (!TrimString(lpArg1))
    return FALSE;

  if (*lpArg1 == '\"')
  {
    *lpArg1 = ' ';
    TrimString(lpArg1);
    bQuoted = TRUE;
  }

  if (bQuoted)
    psz = strchr(lpArg1,'\"');
  else
    psz = strchr(lpArg1,' ');;

  if (psz)
  {
    *psz = '\0';
    if (lpArg2)
      lstrcpy(lpArg2,psz + 1);
  }

  TrimString(lpArg1);

  if (lpArg2)
    TrimString(lpArg2);

  return TRUE;

}


//=============================================================================
//
//  PrepareFilterStr()
//
void PrepareFilterStr(LPSTR lpFilter)
{
  LPSTR psz = StrEnd(lpFilter);
  while (psz != lpFilter)
  {
    if (*(psz = CharPrev(lpFilter,psz)) == '|')
      *psz = '\0';
  }
}


//=============================================================================
//
//  StrTab2Space() - in place conversion
//
void StrTab2Space(LPSTR lpsz)
{
  char *c;
  while (c = strchr(lpsz,'\t'))
    *c = ' ';
}


//=============================================================================
//
//  ExpandEnvironmentStringsEx()
//
//  Adjusted for Windows 95
//
void ExpandEnvironmentStringsEx(LPSTR lpSrc,DWORD dwSrc)
{
  char szBuf[312];

  if (ExpandEnvironmentStrings(lpSrc,szBuf,COUNTOF(szBuf)))
    lstrcpyn(lpSrc,szBuf,dwSrc);
}


//=============================================================================
//
//  PathCanonicalizeEx()
//
//
void PathCanonicalizeEx(LPSTR lpSrc)
{
  char szDst[MAX_PATH];

  if (PathCanonicalize(szDst,lpSrc))
    lstrcpy(lpSrc,szDst);
}


//=============================================================================
//
//  GetLongPathNameEx()
//
//  Works fine with Windows 95!
//
extern LPMALLOC g_lpMalloc;

DWORD GetLongPathNameEx(LPCSTR lpszShortPath,LPSTR lpszLongPath,DWORD cchBuffer)
{
  WCHAR wszShortPath[MAX_PATH];
  LPSHELLFOLDER lpsfDesktop;
  ULONG chParsed = 0;
  ULONG dwAttributes = 0;
  LPITEMIDLIST  pidl = NULL;
  BOOL fSucceeded = FALSE;

  //MessageBox(GetFocus(),lpszShortPath,"GetLongPathNameEx(): lpszShortPath",0);

  // Convert lpszShortPath to a UNICODE string
  MultiByteToWideChar(
    CP_ACP,MB_PRECOMPOSED,lpszShortPath,-1,wszShortPath,MAX_PATH);

  // Get Desktop Folder
  if (NOERROR == SHGetDesktopFolder(&lpsfDesktop))
  {
    // Convert wszShortPath to a pidl
    if (NOERROR == lpsfDesktop->lpVtbl->ParseDisplayName(
                     lpsfDesktop,NULL,NULL,wszShortPath,&chParsed,&pidl,&dwAttributes))
    {
      if (SHGetPathFromIDList(pidl,lpszLongPath))
        fSucceeded = FALSE;
      g_lpMalloc->lpVtbl->Free(g_lpMalloc,pidl);
    }
  }

  //MessageBox(GetFocus(),lpszLongPath,"GetLongPathNameEx(): lpszLongPath",0);

  if (fSucceeded)
    return(lstrlen(lpszLongPath));
  else {
    if (lpszShortPath != lpszLongPath)
      lstrcpy(lpszLongPath,lpszShortPath);
    return(0);
  }

  cchBuffer;
}


//=============================================================================
//
//  SHGetFileInfo2() - return a default name when the file has been removed
//
DWORD_PTR SHGetFileInfo2(LPCTSTR pszPath,DWORD dwFileAttributes,
                         SHFILEINFO *psfi,UINT cbFileInfo,UINT uFlags)
{

  if (PathFileExists(pszPath))
    return SHGetFileInfo(pszPath,dwFileAttributes,psfi,cbFileInfo,uFlags);

  else
    return SHGetFileInfo(pszPath,FILE_ATTRIBUTE_NORMAL,
                         psfi,cbFileInfo,uFlags|SHGFI_USEFILEATTRIBUTES);

}


//=============================================================================
//
//  FormatNumberStr()
//
int FormatNumberStr(LPSTR lpNumberStr)
{
  char *c;
  char szSep[8];
  int  i = 0;

  if (!lstrlen(lpNumberStr))
    return(0);

  if (!GetLocaleInfo(LOCALE_USER_DEFAULT,
                     LOCALE_STHOUSAND,
                     szSep,
                     COUNTOF(szSep)))
    szSep[0] = '\'';

  c = StrEnd(lpNumberStr);

  while ((c = CharPrev(lpNumberStr,c)) != lpNumberStr)
  {
    if (++i == 3)
    {
      i = 0;
      MoveMemory(c+1,c,lstrlen(c)+1);
      *c = szSep[0];
    }
  }

  return(lstrlen(lpNumberStr));
}


//=============================================================================
//
//  SetDlgItemIntEx()
//
BOOL SetDlgItemIntEx(HWND hwnd,int nIdItem,UINT uValue)
{
  char szBuf[64];

  wsprintf(szBuf,"%u",uValue);
  FormatNumberStr(szBuf);

  return(SetDlgItemText(hwnd,nIdItem,szBuf));
}


//=============================================================================
//
//  A2W: Convert Dialog Item Text form Unicode to UTF-8 and vice versa
//
UINT GetDlgItemTextA2W(UINT uCP,HWND hDlg,int nIDDlgItem,LPSTR lpString,int nMaxCount)
{
  WCHAR wsz[256];
  UINT uRet = GetDlgItemTextW(hDlg,nIDDlgItem,wsz,256);
  WCharToMBCS(uCP,wsz,lpString,nMaxCount);
  return uRet;
}

UINT SetDlgItemTextA2W(UINT uCP,HWND hDlg,int nIDDlgItem,LPSTR lpString)
{
  WCHAR wsz[256];
  MBCSToWChar(uCP,lpString,wsz,256);
  return SetDlgItemTextW(hDlg,nIDDlgItem,wsz);
}

LRESULT ComboBox_AddStringA2W(UINT uCP,HWND hwnd,LPCSTR lpString)
{
  WCHAR wsz[256];
  MBCSToWChar(uCP,lpString,wsz,256);
  return SendMessageW(hwnd,CB_ADDSTRING,0,(LPARAM)wsz);
}


//=============================================================================
//
//  CodePageFromCharSet()
//
UINT CodePageFromCharSet(UINT uCharSet)
{
  CHARSETINFO ci;
  if (TranslateCharsetInfo((DWORD*)uCharSet,&ci,TCI_SRCCHARSET))
    return(ci.ciACP);
  else
    return(GetACP());
}


//=============================================================================
//
//  MRU functions
//
LPMRULIST MRU_Create(LPCSTR pszRegKey,int iFlags,int iSize) {

  LPMRULIST pmru = LocalAlloc(LPTR,sizeof(MRULIST));
  ZeroMemory(pmru,sizeof(MRULIST));
  lstrcpyn(pmru->szRegKey,pszRegKey,COUNTOF(pmru->szRegKey));
  pmru->iFlags = iFlags;
  pmru->iSize = min(iSize,MRU_MAXITEMS);
  return(pmru);
}

BOOL MRU_Destroy(LPMRULIST pmru) {

  int i;
  for (i = 0; i < pmru->iSize; i++) {
    if (pmru->pszItems[i])
      LocalFree(pmru->pszItems[i]);
    }
  ZeroMemory(pmru,sizeof(MRULIST));
  LocalFree(pmru);
  return(1);
}

int MRU_Compare(LPMRULIST pmru,LPCSTR psz1,LPCSTR psz2) {

  if (pmru->iFlags & MRU_NOCASE)
    return(lstrcmpi(psz1,psz2));
  else
    return(lstrcmp(psz1,psz2));
}

BOOL MRU_Add(LPMRULIST pmru,LPCSTR pszNew) {

  int i;
  for (i = 0; i < pmru->iSize; i++) {
    if (MRU_Compare(pmru,pmru->pszItems[i],pszNew) == 0) {
      LocalFree(pmru->pszItems[i]);
      break;
    }
  }
  i = min(i,pmru->iSize-1);
  for (; i > 0; i--)
    pmru->pszItems[i] = pmru->pszItems[i-1];
  pmru->pszItems[0] = StrDup(pszNew);
  return(1);
}

BOOL MRU_Delete(LPMRULIST pmru,int iIndex) {

  int i;
  if (iIndex < 0 || iIndex > pmru->iSize-1)
    return(0);
  if (pmru->pszItems[iIndex])
    LocalFree(pmru->pszItems[iIndex]);
  for (i = iIndex; i < pmru->iSize-1; i++) {
    pmru->pszItems[i] = pmru->pszItems[i+1];
    pmru->pszItems[i+1] = NULL;
  }
  return(1);
}

BOOL MRU_Empty(LPMRULIST pmru) {

  int i;
  for (i = 0; i < pmru->iSize; i++) {
    if (pmru->pszItems[i]) {
      LocalFree(pmru->pszItems[i]);
      pmru->pszItems[i] = NULL;
    }
  }
  return(1);
}

int MRU_Enum(LPMRULIST pmru,int iIndex,LPSTR pszItem,int cchItem) {

  if (pszItem == NULL || cchItem == 0) {
    int i = 0;
    while (pmru->pszItems[i] && i < pmru->iSize)
      i++;
    return(i);
  }
  else {
    if (iIndex < 0 || iIndex > pmru->iSize-1)
      return(-1);
    else {
      lstrcpyn(pszItem,pmru->pszItems[iIndex],cchItem);
      return(lstrlen(pszItem));
    }
  }
}

BOOL MRU_Load(LPMRULIST pmru) {

  int i,n = 0;
  char tchName[32];
  char tchItem[1024];
  char *pIniSection = LocalAlloc(LPTR,32*1024);

  MRU_Empty(pmru);
  LoadIniSection(pmru->szRegKey,pIniSection,LocalSize(pIniSection));

  for (i = 0; i < pmru->iSize; i++) {
    wsprintf(tchName,"%.2i",i+1);
    if (IniSectionGetString(pIniSection,tchName,"",tchItem,COUNTOF(tchItem))) {
      if (pmru->iFlags & MRU_UTF8) {
        WCHAR wchItem[1024];
        int cbw = MultiByteToWideChar(CP_UTF7,0,tchItem,-1,wchItem,COUNTOF(wchItem));
        WideCharToMultiByte(CP_UTF8,0,wchItem,cbw,tchItem,COUNTOF(tchItem),NULL,NULL);
        pmru->pszItems[n++] = StrDup(tchItem);
      }
      else
        pmru->pszItems[n++] = StrDup(tchItem);
    }
  }
  LocalFree(pIniSection);
  return(1);
}

BOOL MRU_Save(LPMRULIST pmru) {

  int i;
  char tchName[32];
  char *pIniSection = LocalAlloc(LPTR,32*1024);

  //IniDeleteSection(pmru->szRegKey);

  for (i = 0; i < pmru->iSize; i++) {
    if (pmru->pszItems[i]) {
      wsprintf(tchName,"%.2i",i+1);
      if (pmru->iFlags & MRU_UTF8) {
        char  tchItem[1024];
        WCHAR wchItem[1024];
        int cbw = MultiByteToWideChar(CP_UTF8,0,pmru->pszItems[i],-1,wchItem,COUNTOF(wchItem));
        WideCharToMultiByte(CP_UTF7,0,wchItem,cbw,tchItem,COUNTOF(tchItem),NULL,NULL);
        IniSectionSetString(pIniSection,tchName,tchItem);
      }
      else
        IniSectionSetString(pIniSection,tchName,pmru->pszItems[i]);
    }
  }
  SaveIniSection(pmru->szRegKey,pIniSection);
  LocalFree(pIniSection);
  return(1);
}


/******************************************************************************
*
*  UnSlash functions
*  Mostly taken from SciTE, (c) Neil Hodgson, http://www.scintilla.org
*
/

/**
 * Is the character an octal digit?
 */
static BOOL IsOctalDigit(char ch) {
  return ch >= '0' && ch <= '7';
}

/**
 * If the character is an hexa digit, get its value.
 */
static int GetHexaDigit(char ch) {
  if (ch >= '0' && ch <= '9') {
    return ch - '0';
  }
  if (ch >= 'A' && ch <= 'F') {
    return ch - 'A' + 10;
  }
  if (ch >= 'a' && ch <= 'f') {
    return ch - 'a' + 10;
  }
  return -1;
}

/**
 * Convert C style \a, \b, \f, \n, \r, \t, \v, \ooo and \xhh into their indicated characters.
 */
unsigned int UnSlash(char *s) {
  char *sStart = s;
  char *o = s;

  while (*s) {
    if (*s == '\\') {
      s++;
      if (*s == 'a') {
        *o = '\a';
      } else if (*s == 'b') {
        *o = '\b';
      } else if (*s == 'f') {
        *o = '\f';
      } else if (*s == 'n') {
        *o = '\n';
      } else if (*s == 'r') {
        *o = '\r';
      } else if (*s == 't') {
        *o = '\t';
      } else if (*s == 'v') {
        *o = '\v';
      } else if (IsOctalDigit(*s)) {
        int val = *s - '0';
        if (IsOctalDigit(*(s + 1))) {
          s++;
          val *= 8;
          val += *s - '0';
          if (IsOctalDigit(*(s + 1))) {
            s++;
            val *= 8;
            val += *s - '0';
          }
        }
        *o = (char)(val);
      } else if (*s == 'x') {
        int val, ghd;
        s++;
        val = 0;
        ghd = GetHexaDigit(*s);
        if (ghd >= 0) {
          s++;
          val = ghd;
          ghd = GetHexaDigit(*s);
          if (ghd >= 0) {
            s++;
            val *= 16;
            val += ghd;
          }
        }
        *o = (char)(val);
      } else {
        *o = *s;
      }
    } else {
      *o = *s;
    }
    o++;
    if (*s) {
      s++;
    }
  }
  *o = '\0';
  return o - sStart;
}

/**
 * Convert C style \0oo into their indicated characters.
 * This is used to get control characters into the regular expresion engine.
 */
unsigned int UnSlashLowOctal(char *s) {
  char *sStart = s;
  char *o = s;
  while (*s) {
    if ((s[0] == '\\') && (s[1] == '0') && IsOctalDigit(s[2]) && IsOctalDigit(s[3])) {
      *o = (char)(8 * (s[2] - '0') + (s[3] - '0'));
      s += 3;
    } else {
      *o = *s;
    }
    o++;
    if (*s)
      s++;
  }
  *o = '\0';
  return o - sStart;
}

void TransformBackslashes(LPSTR pszInput,BOOL bRegEx)
{
  if (bRegEx)
    UnSlashLowOctal(pszInput);
  else
    UnSlash(pszInput);
}


/*

  MinimizeToTray - Copyright 2000 Matthew Ellis <m.t.ellis@bigfoot.com>

  Changes made by flo:
   - Commented out: #include "stdafx.h"
   - Moved variable declaration: APPBARDATA appBarData;

*/

// MinimizeToTray
//
// A couple of routines to show how to make it produce a custom caption
// animation to make it look like we are minimizing to and maximizing
// from the system tray
//
// These routines are public domain, but it would be nice if you dropped
// me a line if you use them!
//
// 1.0 29.06.2000 Initial version
// 1.1 01.07.2000 The window retains it's place in the Z-order of windows
//     when minimized/hidden. This means that when restored/shown, it doen't
//     always appear as the foreground window unless we call SetForegroundWindow
//
// Copyright 2000 Matthew Ellis <m.t.ellis@bigfoot.com>
/*#include "stdafx.h"*/

// Odd. VC++6 winuser.h has IDANI_CAPTION defined (as well as IDANI_OPEN and
// IDANI_CLOSE), but the Platform SDK only has IDANI_OPEN...

// I don't know what IDANI_OPEN or IDANI_CLOSE do. Trying them in this code
// produces nothing. Perhaps they were intended for window opening and closing
// like the MAC provides...
#ifndef IDANI_OPEN
#define IDANI_OPEN 1
#endif
#ifndef IDANI_CLOSE
#define IDANI_CLOSE 2
#endif
#ifndef IDANI_CAPTION
#define IDANI_CAPTION 3
#endif

#define DEFAULT_RECT_WIDTH 150
#define DEFAULT_RECT_HEIGHT 30

// Returns the rect of where we think the system tray is. This will work for
// all current versions of the shell. If explorer isn't running, we try our
// best to work with a 3rd party shell. If we still can't find anything, we
// return a rect in the lower right hand corner of the screen
static VOID GetTrayWndRect(LPRECT lpTrayRect)
{
  APPBARDATA appBarData;
  // First, we'll use a quick hack method. We know that the taskbar is a window
  // of class Shell_TrayWnd, and the status tray is a child of this of class
  // TrayNotifyWnd. This provides us a window rect to minimize to. Note, however,
  // that this is not guaranteed to work on future versions of the shell. If we
  // use this method, make sure we have a backup!
  HWND hShellTrayWnd=FindWindowEx(NULL,NULL,TEXT("Shell_TrayWnd"),NULL);
  if(hShellTrayWnd)
  {
    HWND hTrayNotifyWnd=FindWindowEx(hShellTrayWnd,NULL,TEXT("TrayNotifyWnd"),NULL);
    if(hTrayNotifyWnd)
    {
      GetWindowRect(hTrayNotifyWnd,lpTrayRect);
      return;
    }
  }

  // OK, we failed to get the rect from the quick hack. Either explorer isn't
  // running or it's a new version of the shell with the window class names
  // changed (how dare Microsoft change these undocumented class names!) So, we
  // try to find out what side of the screen the taskbar is connected to. We
  // know that the system tray is either on the right or the bottom of the
  // taskbar, so we can make a good guess at where to minimize to
  /*APPBARDATA appBarData;*/
  appBarData.cbSize=sizeof(appBarData);
  if(SHAppBarMessage(ABM_GETTASKBARPOS,&appBarData))
  {
    // We know the edge the taskbar is connected to, so guess the rect of the
    // system tray. Use various fudge factor to make it look good
    switch(appBarData.uEdge)
    {
      case ABE_LEFT:
      case ABE_RIGHT:
  // We want to minimize to the bottom of the taskbar
  lpTrayRect->top=appBarData.rc.bottom-100;
  lpTrayRect->bottom=appBarData.rc.bottom-16;
  lpTrayRect->left=appBarData.rc.left;
  lpTrayRect->right=appBarData.rc.right;
  break;

      case ABE_TOP:
      case ABE_BOTTOM:
  // We want to minimize to the right of the taskbar
  lpTrayRect->top=appBarData.rc.top;
  lpTrayRect->bottom=appBarData.rc.bottom;
  lpTrayRect->left=appBarData.rc.right-100;
  lpTrayRect->right=appBarData.rc.right-16;
  break;
    }

    return;
  }

  // Blimey, we really aren't in luck. It's possible that a third party shell
  // is running instead of explorer. This shell might provide support for the
  // system tray, by providing a Shell_TrayWnd window (which receives the
  // messages for the icons) So, look for a Shell_TrayWnd window and work out
  // the rect from that. Remember that explorer's taskbar is the Shell_TrayWnd,
  // and stretches either the width or the height of the screen. We can't rely
  // on the 3rd party shell's Shell_TrayWnd doing the same, in fact, we can't
  // rely on it being any size. The best we can do is just blindly use the
  // window rect, perhaps limiting the width and height to, say 150 square.
  // Note that if the 3rd party shell supports the same configuraion as
  // explorer (the icons hosted in NotifyTrayWnd, which is a child window of
  // Shell_TrayWnd), we would already have caught it above
  hShellTrayWnd=FindWindowEx(NULL,NULL,TEXT("Shell_TrayWnd"),NULL);
  if(hShellTrayWnd)
  {
    GetWindowRect(hShellTrayWnd,lpTrayRect);
    if(lpTrayRect->right-lpTrayRect->left>DEFAULT_RECT_WIDTH)
      lpTrayRect->left=lpTrayRect->right-DEFAULT_RECT_WIDTH;
    if(lpTrayRect->bottom-lpTrayRect->top>DEFAULT_RECT_HEIGHT)
      lpTrayRect->top=lpTrayRect->bottom-DEFAULT_RECT_HEIGHT;

    return;
  }

  // OK. Haven't found a thing. Provide a default rect based on the current work
  // area
  SystemParametersInfo(SPI_GETWORKAREA,0,lpTrayRect,0);
  lpTrayRect->left=lpTrayRect->right-DEFAULT_RECT_WIDTH;
  lpTrayRect->top=lpTrayRect->bottom-DEFAULT_RECT_HEIGHT;
}

// Check to see if the animation has been disabled
static BOOL GetDoAnimateMinimize(VOID)
{
  ANIMATIONINFO ai;

  ai.cbSize=sizeof(ai);
  SystemParametersInfo(SPI_GETANIMATION,sizeof(ai),&ai,0);

  return ai.iMinAnimate?TRUE:FALSE;
}

VOID MinimizeWndToTray(HWND hWnd)
{
  if(GetDoAnimateMinimize())
  {
    RECT rcFrom,rcTo;

    // Get the rect of the window. It is safe to use the rect of the whole
    // window - DrawAnimatedRects will only draw the caption
    GetWindowRect(hWnd,&rcFrom);
    GetTrayWndRect(&rcTo);

    // Get the system to draw our animation for us
    DrawAnimatedRects(hWnd,IDANI_CAPTION,&rcFrom,&rcTo);
  }

  // Add the tray icon. If we add it before the call to DrawAnimatedRects,
  // the taskbar gets erased, but doesn't get redrawn until DAR finishes.
  // This looks untidy, so call the functions in this order

  // Hide the window
  ShowWindow(hWnd,SW_HIDE);
}

VOID RestoreWndFromTray(HWND hWnd)
{
  if(GetDoAnimateMinimize())
  {
    // Get the rect of the tray and the window. Note that the window rect
    // is still valid even though the window is hidden
    RECT rcFrom,rcTo;
    GetTrayWndRect(&rcFrom);
    GetWindowRect(hWnd,&rcTo);

    // Get the system to draw our animation for us
    DrawAnimatedRects(hWnd,IDANI_CAPTION,&rcFrom,&rcTo);
  }

  // Show the window, and make sure we're the foreground window
  ShowWindow(hWnd,SW_SHOW);
  SetActiveWindow(hWnd);
  SetForegroundWindow(hWnd);

  // Remove the tray icon. As described above, remove the icon after the
  // call to DrawAnimatedRects, or the taskbar will not refresh itself
  // properly until DAR finished
}



///   End of Helpers.c   \\\
