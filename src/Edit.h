/******************************************************************************
*
*
* Notepad2
*
* Edit.h
*   Text File Editing Helper Stuff
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



typedef struct _editfindreplace
{
  char szFind[256];
  char szReplace[256];
  char szFindUTF8[256];
  char szReplaceUTF8[256];
  UINT fuFlags;
  BOOL bTransformBS;
  BOOL bFindUp;
  BOOL bFindClose;
  BOOL bReplaceClose;
  BOOL bNoFindWrap;
  HWND hwnd;
  //HANDLE hMRUFind;
  //HANDLE hMRUReplace;

} EDITFINDREPLACE, *LPEDITFINDREPLACE, *LPCEDITFINDREPLACE;


HWND  EditCreate(HWND);
void  EditSetNewText(HWND,LPCSTR,DWORD);
BOOL  EditConvertText(HWND,UINT,UINT,BOOL);
LPSTR EditGetClipboardText(HWND);
int   EditDetectEOLMode(HWND,LPCSTR,DWORD);
BOOL  EditLoadFile(HWND,LPCSTR,BOOL,int*,int*,BOOL*,BOOL*);
BOOL  EditSaveFile(HWND,LPCSTR,int,BOOL);

void  EditMakeUppercase(HWND);
void  EditMakeLowercase(HWND);
void  EditInvertCase(HWND);
void  EditTitleCase(HWND);
void  EditSentenceCase(HWND);

void  EditTabsToSpaces(HWND,int);
void  EditSpacesToTabs(HWND,int);

void  EditModifyLines(HWND,LPCWSTR,LPCWSTR);
void  EditEncloseSelection(HWND,LPCWSTR,LPCWSTR);
void  EditToggleLineComments(HWND,LPCWSTR,BOOL);
void  EditStripFirstCharacter(HWND);
void  EditStripTrailingBlanks(HWND);
void  EditCompressSpaces(HWND);
void  EditRemoveBlankLines(HWND);
void  EditWrapToColumn(HWND,int);

void  EditJumpTo(HWND,int,int);
void  EditSelectEx(HWND,int,int);

HWND  EditFindReplaceDlg(HWND,LPCEDITFINDREPLACE,BOOL);
BOOL  EditFindNext(HWND,LPCEDITFINDREPLACE,BOOL);
BOOL  EditFindPrev(HWND,LPCEDITFINDREPLACE,BOOL);
BOOL  EditReplace(HWND,LPCEDITFINDREPLACE);
BOOL  EditReplaceAll(HWND,LPCEDITFINDREPLACE,BOOL);
BOOL  EditReplaceAllInSelection(HWND,LPCEDITFINDREPLACE,BOOL);
BOOL  EditLinenumDlg(HWND);
BOOL  EditModifyLinesDlg(HWND,LPWSTR,LPWSTR);
BOOL  EditEncloseSelectionDlg(HWND,LPWSTR,LPWSTR);
BOOL  EditInsertTagDlg(HWND,LPWSTR,LPWSTR);
BOOL  EditPrint(HWND,LPCSTR,LPCSTR);
void  EditPrintSetup(HWND);
void  EditPrintInit();

// New codepage detection functions from Textview 6.0
#define NCP_DEFAULT            0
#define NCP_UTF8               1
#define NCP_UTF8_SIGN          2
#define NCP_UNICODE            4
#define NCP_UNICODE_REVERSE    8
#define NCP_UNICODE_BOM       16

BOOL IsUnicode(const char*,int,LPBOOL,LPBOOL);
BOOL IsUTF8(const char*,int);
BOOL IsUTF7(const char*,int);


void SciInitThemes(HWND);
LRESULT CALLBACK SciThemedWndProc(HWND,UINT,WPARAM,LPARAM);



///   End of Edit.h   \\\
