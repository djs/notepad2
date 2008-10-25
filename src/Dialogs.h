/******************************************************************************
*
*
* Notepad2
*
* Dialogs.h
*   Definitions for Notepad2 dialog boxes
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


#define MBINFO         0
#define MBWARN         1
#define MBYESNO        2
#define MBYESNOWARN    3
#define MBYESNOCANCEL  4
#define MBOKCANCEL     8

int MsgBox(int,UINT,...);

void DisplayCmdLineHelp();

BOOL GetDirectory(HWND,int,LPSTR,LPCSTR,BOOL);

BOOL CALLBACK AboutDlgProc(HWND,UINT,WPARAM,LPARAM);

void RunDlg(HWND,LPCSTR);
BOOL OpenWithDlg(HWND,LPCSTR);
BOOL FavoritesDlg(HWND,LPSTR);
BOOL AddToFavDlg(HWND,LPCSTR,LPCSTR);
BOOL FileMRUDlg(HWND,LPSTR);
BOOL ColumnWrapDlg(HWND,UINT,int *);
BOOL WordWrapSettingsDlg(HWND,UINT,int *);
BOOL LongLineSettingsDlg(HWND,UINT,int *);
BOOL TabSettingsDlg(HWND,UINT,int *);
BOOL SelectEncodingDlg(HWND,int *);
BOOL SelectLineEndingDlg(HWND,int *);
int  InfoBox(int,int,LPCSTR);


// End of Dialogs.h
