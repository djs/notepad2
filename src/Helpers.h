/******************************************************************************
*
*
* Notepad2
*
* Helpers.h
*   Definitions for general helper functions and macros
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



extern HINSTANCE g_hInstance;


#define COUNTOF(ar) (sizeof(ar)/sizeof(ar[0]))


#define IniGetInt(lpSection,lpName,nDefault) \
  GetPrivateProfileInt(lpSection,lpName,nDefault,szIniFile)
#define IniGetString(lpSection,lpName,lpDefault,lpReturnedStr,nSize)\
  GetPrivateProfileString(lpSection,lpName,lpDefault,lpReturnedStr,nSize,szIniFile)
#define IniSetString(lpSection,lpName,lpString) \
  WritePrivateProfileString(lpSection,lpName,lpString,szIniFile)
#define IniDeleteSection(lpSection) \
  WritePrivateProfileSection(lpSection,NULL,szIniFile)

extern char szIniFile[MAX_PATH];
__inline BOOL IniSetInt(LPCSTR lpSection,LPCSTR lpName,int i) {
  char tch[32]; wsprintf(tch,"%i",i); return IniSetString(lpSection,lpName,tch);
}

#define LoadIniSection(lpSection,lpBuf,cchBuf) \
  GetPrivateProfileSection(lpSection,lpBuf,cchBuf,szIniFile);
#define SaveIniSection(lpSection,lpBuf) \
  WritePrivateProfileSection(lpSection,lpBuf,szIniFile)

int IniSectionGetString(LPCSTR,LPCSTR,LPCSTR,LPSTR,int);
int IniSectionGetInt(LPCSTR,LPCSTR,int);

BOOL IniSectionSetString(LPSTR,LPCSTR,LPCSTR);
__inline BOOL IniSectionSetInt(LPSTR lpCachedIniSection,LPCSTR lpName,int i) {
  char tch[32]; wsprintf(tch,"%i",i); return IniSectionSetString(lpCachedIniSection,lpName,tch);
}



void BeginWaitCursor();
void EndWaitCursor();


//void KeepWindowsAlive();


BOOL IsWindows2korLater();
//BOOL IsWindowsXPorLater();
BOOL PrivateIsAppThemed();


BOOL SetWindowTitle(HWND,UINT,UINT,LPCSTR,int,BOOL,UINT,BOOL);
void SetWindowTransparentMode(HWND,BOOL);


void CenterDlgInParent(HWND);
void MakeBitmapButton(HWND,int,HINSTANCE,UINT);


#define StatusSetSimple(hwnd,b) SendMessage(hwnd,SB_SIMPLE,(WPARAM)b,0)
BOOL StatusSetText(HWND,UINT,LPCSTR);
BOOL StatusSetTextID(HWND,UINT,UINT);
int  StatusCalcPaneWidth(HWND,LPCSTR);

int Toolbar_GetButtons(HWND,int,LPSTR,int);
int Toolbar_SetButtons(HWND,int,LPCSTR,void*,int);

LRESULT SendWMSize(HWND);

#define EnableCmd(hmenu,id,b) EnableMenuItem(hmenu,id,(b)\
                               ?MF_BYCOMMAND|MF_ENABLED:MF_BYCOMMAND|MF_GRAYED)

#define CheckCmd(hmenu,id,b)  CheckMenuItem(hmenu,id,(b)\
                               ?MF_BYCOMMAND|MF_CHECKED:MF_BYCOMMAND|MF_UNCHECKED)

BOOL IsCmdEnabled(HWND, UINT);


#define GetString(id,pb,cb) LoadString(g_hInstance,id,pb,cb)

#define StrEnd(pStart) (pStart + lstrlen(pStart))

int FormatString(LPSTR,int,UINT,...);
void FormatBytes(LPSTR,int,DWORD);


BOOL PathIsLnkFile(LPCSTR);
BOOL PathGetLnkPath(LPCSTR,LPSTR,int);
BOOL PathIsLnkToDirectory(LPCSTR,LPSTR,int);
BOOL PathCreateDeskLnk(LPCSTR);
BOOL PathCreateFavLnk(LPCSTR,LPCSTR,LPCSTR);


BOOL TrimString(LPSTR);
BOOL ExtractFirstArgument(LPCSTR, LPSTR, LPSTR);

void PrepareFilterStr(LPSTR);

void StrTab2Space(LPSTR);


void  ExpandEnvironmentStringsEx(LPSTR,DWORD);
void  PathCanonicalizeEx(LPSTR);
DWORD GetLongPathNameEx(LPCSTR,LPSTR,DWORD);
DWORD_PTR SHGetFileInfo2(LPCTSTR,DWORD,SHFILEINFO*,UINT,UINT);


int  FormatNumberStr(LPSTR);
BOOL SetDlgItemIntEx(HWND,int,UINT);


#define MBCSToWChar(c,a,w,i) MultiByteToWideChar(c,0,a,-1,w,i)
#define WCharToMBCS(c,w,a,i) WideCharToMultiByte(c,0,w,-1,a,i,NULL,NULL)

UINT    GetDlgItemTextA2W(UINT,HWND,int,LPSTR,int);
UINT    SetDlgItemTextA2W(UINT,HWND,int,LPSTR);
LRESULT ComboBox_AddStringA2W(UINT,HWND,LPCSTR);


UINT CodePageFromCharSet(UINT);


//==== UnSlash Functions ======================================================
void TransformBackslashes(LPSTR,BOOL);


//==== MRU Functions ==========================================================
#define MRU_MAXITEMS 24
#define MRU_NOCASE    1
#define MRU_UTF8      2

typedef struct _mrulist {

  char  szRegKey[256];
  int   iFlags;
  int   iSize;
  LPSTR pszItems[MRU_MAXITEMS];

} MRULIST, *PMRULIST, *LPMRULIST;

LPMRULIST MRU_Create(LPCSTR,int,int);
BOOL      MRU_Destroy(LPMRULIST);
BOOL      MRU_Add(LPMRULIST,LPCSTR);
BOOL      MRU_Delete(LPMRULIST,int);
BOOL      MRU_Empty(LPMRULIST);
int       MRU_Enum(LPMRULIST,int,LPSTR,int);
BOOL      MRU_Load(LPMRULIST);
BOOL      MRU_Save(LPMRULIST);


//==== MinimizeToTray Functions - see comments in Helpers.c ===================
VOID MinimizeWndToTray(HWND hWnd);
VOID RestoreWndFromTray(HWND hWnd);



///   End of Helpers.h   \\\
