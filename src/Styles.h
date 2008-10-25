/******************************************************************************
*
*
* Notepad2
*
* Styles.h
*   Scintilla Style Management
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


typedef struct _editstyle
{
  int   iStyle;
  char* pszName;
  char* pszDefault;
  char  szValue[128];

} EDITSTYLE, *PEDITSTYLE;


typedef struct _keywordlist
{
  char *pszKeyWords[9];

} KEYWORDLIST, *PKEYWORDLIST;


typedef struct _editlexer
{
  int   iLexer;
  char* pszName;
  char* pszDefExt;
  char  szExtensions[128];
  PKEYWORDLIST pKeyWords;
  EDITSTYLE    Styles[];

} EDITLEXER, *PEDITLEXER;


// Number of Lexers in pLexArray
#define NUMLEXERS 21


void   Style_Load();
void   Style_Save();
BOOL   Style_Import(HWND);
BOOL   Style_Export(HWND);
void   Style_SetLexer(HWND,PEDITLEXER);
//void   Style_SetACPfromCharSet(HWND);
void   Style_SetLongLineColors(HWND);
void   Style_SetCurrentLineBackground(HWND);
void   Style_SetLexerFromFile(HWND,LPCSTR);
void   Style_SetDefaultLexer(HWND);
void   Style_SetHTMLLexer(HWND);
void   Style_SetXMLLexer(HWND);
void   Style_SetLexerFromID(HWND,int);
void   Style_SetDefaultFont(HWND);
void   Style_ToggleUse2ndDefault(HWND);
BOOL   Style_GetUse2ndDefault(HWND);
void   Style_SetIndentGuides(HWND,BOOL);
BOOL   Style_GetOpenDlgFilterStr(LPSTR,int);
BOOL   Style_StrGetFont(LPCSTR,LPSTR,int);
BOOL   Style_StrGetCharSet(LPCSTR,int*);
BOOL   Style_StrGetSize(LPCSTR,int*);
BOOL   Style_StrGetSizeStr(LPCSTR,LPSTR,int);
BOOL   Style_StrGetColor(BOOL,LPCSTR,int*);
BOOL   Style_StrGetCase(LPCSTR,int*);
BOOL   Style_StrGetAlpha(LPCSTR,int*);
//BOOL   Style_StrGetAttribute(LPCSTR,LPCSTR);
//BOOL   Style_StrSetAttribute(LPSTR,int,LPCSTR,BOOL);
BOOL   Style_SelectFont(HWND,LPSTR,int);
BOOL   Style_SelectColor(HWND,BOOL,LPSTR,int);
void   Style_SetStyles(HWND,int,LPCSTR);
LPCSTR Style_GetCurrentLexerName();
int    Style_GetLexerIconId(PEDITLEXER);
void   Style_AddLexerToTreeView(HWND,PEDITLEXER);
BOOL CALLBACK Styles_ConfigDlgProc(HWND,UINT,WPARAM,LPARAM);
void   Style_ConfigDlg(HWND);
BOOL CALLBACK Style_SelectLexerDlgProc(HWND,UINT,WPARAM,LPARAM);
void   Style_SelectLexerDlg(HWND);


// End of Style.h
