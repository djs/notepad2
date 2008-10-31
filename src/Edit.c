/******************************************************************************
*
*
* Notepad2
*
* Edit.c
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
#define _WIN32_WINNT 0x501
#include <windows.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <string.h>
#include "notepad2.h"
#include "helpers.h"
#include "dialogs.h"
#include "scintilla.h"
#include "scilexer.h"
#include "styles.h"
#include "edit.h"
#include "resource.h"


extern HWND  hwndMain;
extern HWND  hwndEdit;
extern HINSTANCE g_hInstance;
extern LPMALLOC  g_lpMalloc;
extern HWND hDlgFindReplace;
extern UINT cpLastFind;

static EDITFINDREPLACE efrSave;
static BOOL bSwitchedFindReplace = FALSE;
static int xFindReplaceDlgSave;
static int yFindReplaceDlgSave;
extern int xFindReplaceDlg;
extern int yFindReplaceDlg;

extern int iDefaultEncoding;
extern int iEncodings[5];
extern int iDefaultEOLMode;
extern int iLineEndings[3];
extern int bFixLineEndings;


// Default Codepage and Character Set
extern int iDefaultCodePage;
extern int iDefaultCharSet;
extern BOOL bSkipUnicodeDetection;


extern LPMRULIST mruFind;
extern LPMRULIST mruReplace;


//=============================================================================
//
//  EditCreate()
//
HWND EditCreate(HWND hwndParent)
{

  HWND hwnd;

  hwnd = CreateWindowEx(
           WS_EX_CLIENTEDGE,
           L"Scintilla",
           NULL,
           WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
           0,0,0,0,
           hwndParent,
           (HMENU)IDC_EDIT,
           g_hInstance,
           NULL);

  SendMessage(hwnd,SCI_SETCODEPAGE,iDefaultCodePage,0);
  SendMessage(hwnd,SCI_SETEOLMODE,SC_EOL_CRLF,0);
  SendMessage(hwnd,SCI_SETPASTECONVERTENDINGS,1,0);
  SendMessage(hwnd,SCI_SETMODEVENTMASK,/*SC_MODEVENTMASKALL*/SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT,0);
  SendMessage(hwnd,SCI_USEPOPUP,FALSE,0);
  SendMessage(hwnd,SCI_SETCARETPERIOD,GetCaretBlinkTime(),0);
  SendMessage(hwnd,SCI_SETSCROLLWIDTH,2048,0);
  SendMessage(hwnd,SCI_SETSCROLLWIDTHTRACKING,TRUE,0);
  SendMessage(hwnd,SCI_SETENDATLASTLINE,TRUE,0);
  SendMessage(hwnd,SCI_SETCARETSTICKY,FALSE,0);
  SendMessage(hwnd,SCI_SETXCARETPOLICY,CARET_SLOP|CARET_EVEN,50);
  SendMessage(hwnd,SCI_SETYCARETPOLICY,CARET_EVEN,0);
  SendMessage(hwnd,SCI_SETTABINDENTS,TRUE,0);
  SendMessage(hwnd,SCI_SETBACKSPACEUNINDENTS,FALSE,0);

  SendMessage(hwnd,SCI_ASSIGNCMDKEY,(SCK_NEXT + (SCMOD_CTRL << 16)),SCI_PARADOWN);
  SendMessage(hwnd,SCI_ASSIGNCMDKEY,(SCK_PRIOR + (SCMOD_CTRL << 16)),SCI_PARAUP);
  SendMessage(hwnd,SCI_ASSIGNCMDKEY,(SCK_NEXT + ((SCMOD_CTRL | SCMOD_SHIFT) << 16)),SCI_PARADOWNEXTEND);
  SendMessage(hwnd,SCI_ASSIGNCMDKEY,(SCK_PRIOR + ((SCMOD_CTRL | SCMOD_SHIFT) << 16)),SCI_PARAUPEXTEND);
  SendMessage(hwnd,SCI_ASSIGNCMDKEY,(SCK_HOME + (0 << 16)),SCI_VCHOMEWRAP);
  SendMessage(hwnd,SCI_ASSIGNCMDKEY,(SCK_END + (0 << 16)),SCI_LINEENDWRAP);
  SendMessage(hwnd,SCI_ASSIGNCMDKEY,(SCK_HOME + (SCMOD_SHIFT << 16)),SCI_VCHOMEWRAPEXTEND);
  SendMessage(hwnd,SCI_ASSIGNCMDKEY,(SCK_END + (SCMOD_SHIFT << 16)),SCI_LINEENDWRAPEXTEND);

  // Init default values for printing
  EditPrintInit();

  //SciInitThemes(hwnd);

  return(hwnd);

}


//=============================================================================
//
//  EditSetNewText()
//
extern BOOL bFreezeAppTitle;
extern FILEVARS fvCurFile;

void EditSetNewText(HWND hwnd,char* lpstrText,DWORD cbText)
{
  bFreezeAppTitle = TRUE;

  if (SendMessage(hwnd,SCI_GETREADONLY,0,0))
    SendMessage(hwnd,SCI_SETREADONLY,FALSE,0);

  SendMessage(hwnd,SCI_CANCEL,0,0);
  SendMessage(hwnd,SCI_SETUNDOCOLLECTION,0,0);
  SendMessage(hwnd,SCI_EMPTYUNDOBUFFER,0,0);
  SendMessage(hwnd,SCI_CLEARALL,0,0);
  SendMessage(hwnd,SCI_MARKERDELETEALL,(WPARAM)-1,0);
  SendMessage(hwnd,SCI_SETSCROLLWIDTH,2048,0);

  FileVars_Apply(hwnd,&fvCurFile);

  if (cbText > 0)
    SendMessage(hwnd,SCI_ADDTEXT,cbText,(LPARAM)lpstrText);

  SendMessage(hwnd,SCI_SETUNDOCOLLECTION,1,0);
  SendMessage(hwnd,EM_EMPTYUNDOBUFFER,0,0);
  SendMessage(hwnd,SCI_SETSAVEPOINT,0,0);
  SendMessage(hwnd,SCI_GOTOPOS,0,0);
  SendMessage(hwnd,SCI_CHOOSECARETX,0,0);

  bFreezeAppTitle = FALSE;
}


//=============================================================================
//
//  EditConvertText()
//
BOOL EditConvertText(HWND hwnd,UINT cpSource,UINT cpDest,BOOL bSetSavePoint)
{
  struct TextRange tr = { { 0, -1 }, NULL };
  int length, cbText, cbwText;
  char *pchText;
  WCHAR *pwchText;

  if (cpSource == cpDest)
    return(TRUE);

  length  = SendMessage(hwnd,SCI_GETLENGTH,0,0);

  if (length == 0) {
    SendMessage(hwnd,SCI_CANCEL,0,0);
    SendMessage(hwnd,SCI_SETUNDOCOLLECTION,0,0);
    SendMessage(hwnd,SCI_EMPTYUNDOBUFFER,0,0);
    SendMessage(hwnd,SCI_CLEARALL,0,0);
    SendMessage(hwnd,SCI_MARKERDELETEALL,(WPARAM)-1,0);
    SendMessage(hwnd,SCI_SETCODEPAGE,cpDest,0);
    SendMessage(hwnd,SCI_SETUNDOCOLLECTION,1,0);
    SendMessage(hwnd,EM_EMPTYUNDOBUFFER,0,0);
    SendMessage(hwnd,SCI_GOTOPOS,0,0);
    SendMessage(hwnd,SCI_CHOOSECARETX,0,0);

    if (bSetSavePoint)
      SendMessage(hwnd,SCI_SETSAVEPOINT,0,0);
  }

  else {
    pchText = GlobalAlloc(GPTR,length*5+2);

    tr.lpstrText = pchText;
    SendMessage(hwnd,SCI_GETTEXTRANGE,0,(LPARAM)&tr);

    pwchText = GlobalAlloc(GPTR,length*3+2);
    cbwText  = MultiByteToWideChar(cpSource,0,pchText,length,pwchText,length*3+2);
    cbText   = WideCharToMultiByte(cpDest,0,pwchText,cbwText,pchText,length*5+2,NULL,NULL);

    SendMessage(hwnd,SCI_CANCEL,0,0);
    SendMessage(hwnd,SCI_SETUNDOCOLLECTION,0,0);
    SendMessage(hwnd,SCI_EMPTYUNDOBUFFER,0,0);
    SendMessage(hwnd,SCI_CLEARALL,0,0);
    SendMessage(hwnd,SCI_MARKERDELETEALL,(WPARAM)-1,0);
    SendMessage(hwnd,SCI_SETCODEPAGE,cpDest,0);
    SendMessage(hwnd,SCI_ADDTEXT,cbText,(LPARAM)pchText);
    SendMessage(hwnd,SCI_EMPTYUNDOBUFFER,0,0);
    SendMessage(hwnd,SCI_SETUNDOCOLLECTION,1,0);
    SendMessage(hwnd,SCI_GOTOPOS,0,0);
    SendMessage(hwnd,SCI_CHOOSECARETX,0,0);

    GlobalFree(pchText);
    GlobalFree(pwchText);
  }
  return(TRUE);
}


//=============================================================================
//
//  EditGetClipboardText()
//
char* EditGetClipboardText(HWND hwnd)
{
  HANDLE hmem;
  WCHAR *pwch;
  char  *pmch;
  char  *ptmp;
  int   wlen, mlen, mlen2;
  int   codepage;
  int   eolmode;

  if (!IsClipboardFormatAvailable(CF_UNICODETEXT) || !OpenClipboard(GetParent(hwnd)))
    return(NULL);

  hmem = GetClipboardData(CF_UNICODETEXT);
  pwch = GlobalLock(hmem);

  wlen = lstrlenW(pwch);

  codepage = SendMessage(hwnd,SCI_GETCODEPAGE,0,0);
  eolmode  = SendMessage(hwnd,SCI_GETEOLMODE,0,0);

  mlen = WideCharToMultiByte(codepage,0,pwch,wlen + 1,NULL,0,0,0) - 1;
  pmch = LocalAlloc(LPTR,mlen + 1);
  if (pmch)
    WideCharToMultiByte(codepage,0,pwch,wlen + 1,pmch,mlen + 1,NULL,NULL);

  ptmp = LocalAlloc(LPTR,mlen * 2 + 1);
  if (ptmp) {
    char *s = pmch;
    char *d = ptmp;
    int i;

    for (i = 0; (i < mlen) && (*s != 0); i++) {
      if (*s == '\n' || *s == '\r') {
        if (eolmode == SC_EOL_CR) {
          *d++ = '\r';
        } else if (eolmode == SC_EOL_LF) {
          *d++ = '\n';
        } else { // eolmode == SC_EOL_CRLF
          *d++ = '\r';
          *d++ = '\n';
        }
        if ((*s == '\r') && (i+1 < mlen) && (*(s+1) == '\n')) {
          i++;
          s++;
        }
        s++;
      } else {
        *d++ = *s++;
      }
    }
    *d++ = 0;
    mlen2 = (d - ptmp) - 1;

    LocalFree(pmch);
    pmch = LocalAlloc(LPTR,mlen2 + 1);
    lstrcpyA(pmch,ptmp);
    LocalFree(ptmp);
  }

  GlobalUnlock(hmem);
  CloseClipboard();

  return(pmch);
}


//=============================================================================
//
//  EditCopyAppend()
//
BOOL EditCopyAppend(HWND hwnd)
{
  HANDLE hOld;
  WCHAR  *pszOld;

  HANDLE hNew;
  WCHAR  *pszNew;

  char  *pszText;
  int   cchTextW;
  WCHAR *pszTextW;

  WCHAR *pszSep = L"\r\n\r\n";

  UINT  uCodePage;

  int iCurPos;
  int iAnchorPos;

  if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) {
    SendMessage(hwnd,SCI_COPY,0,0);
    return(TRUE);
  }

  iCurPos    = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
  iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);

  if (iCurPos != iAnchorPos) {

    if (SC_SEL_RECTANGLE == SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0)) {
      MsgBox(MBINFO,IDS_SELRECT);
      return(FALSE);
    }

    else {
      int iSelCount =
        SendMessage(hwnd,SCI_GETSELECTIONEND,0,0) -
        SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);

      pszText = LocalAlloc(LPTR,iSelCount + 1);
      SendMessage(hwnd,SCI_GETSELTEXT,0,(LPARAM)pszText);
    }
  }

  else {
    int cchText = SendMessage(hwnd,SCI_GETLENGTH,0,0);
    pszText = LocalAlloc(LPTR,cchText + 1);
    SendMessage(hwnd,SCI_GETTEXT,LocalSize(pszText),(LPARAM)pszText);
  }

  uCodePage = (SendMessage(hwnd,SCI_GETCODEPAGE,0,0) == SC_CP_UTF8) ? CP_UTF8 : CP_ACP;

  cchTextW = MultiByteToWideChar(uCodePage,0,pszText,-1,NULL,0);
  if (cchTextW > 0) {
    pszTextW = LocalAlloc(LPTR,sizeof(WCHAR)*(lstrlen(pszSep) + cchTextW + 1));
    lstrcpy(pszTextW,pszSep);
    MultiByteToWideChar(uCodePage,0,pszText,-1,StrEnd(pszTextW),LocalSize(pszTextW));
  }
  else {
    pszTextW = L"";
  }

  LocalFree(pszText);

  if (!OpenClipboard(GetParent(hwnd))) {
    LocalFree(pszTextW);
    return(FALSE);
  }

  hOld   = GetClipboardData(CF_UNICODETEXT);
  pszOld = GlobalLock(hOld);

  hNew = GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,
           sizeof(WCHAR) * (lstrlen(pszOld) + lstrlen(pszTextW) + 1));
  pszNew = GlobalLock(hNew);

  lstrcpy(pszNew,pszOld);
  lstrcat(pszNew,pszTextW);

  GlobalUnlock(hNew);
  GlobalUnlock(hOld);

  EmptyClipboard();
  SetClipboardData(CF_UNICODETEXT,hNew);
  CloseClipboard();

  return(TRUE);
}


//=============================================================================
//
//  EditDetectEOLMode() - moved here to handle Unicode files correctly
//
int EditDetectEOLMode(HWND hwnd,char* lpData,DWORD cbData)
{
  int iEOLMode = iLineEndings[iDefaultEOLMode];

  char *cp = (char*)lpData;
  while (*cp && (*cp != '\x0D' && *cp != '\x0A')) cp++;

  if (*cp == '\x0D' && *(cp+1) == '\x0A')
    iEOLMode = SC_EOL_CRLF;
  else if (*cp == '\x0D' && *(cp+1) != '\x0A')
    iEOLMode = SC_EOL_CR;
  else if (*cp == '\x0A')
    iEOLMode = SC_EOL_LF;

  return (iEOLMode);
}



//=============================================================================
//
//  IsUnicode(), IsUTF8(), IsUTF7()
//
BOOL IsUnicode(const char* pBuffer,int cb,LPBOOL lpbBOM,LPBOOL lpbReverse)
{

  int i = 0xFFFF;

  BOOL bIsTextUnicode;

  BOOL bHasBOM;
  BOOL bHasRBOM;

  if (!pBuffer || cb < 2)
    return FALSE;

  if (!bSkipUnicodeDetection)
    bIsTextUnicode = IsTextUnicode(pBuffer,cb,&i);
  else
    bIsTextUnicode = FALSE;

  bHasBOM  = (*pBuffer == '\xFF' && *(pBuffer+1) == '\xFE');
  bHasRBOM = (*pBuffer == '\xFE' && *(pBuffer+1) == '\xFF');

  /*
  #define IS_TEXT_UNICODE_ASCII16               0x0001
  #define IS_TEXT_UNICODE_REVERSE_ASCII16       0x0010
  #define IS_TEXT_UNICODE_STATISTICS            0x0002
  #define IS_TEXT_UNICODE_REVERSE_STATISTICS    0x0020
  #define IS_TEXT_UNICODE_CONTROLS              0x0004
  #define IS_TEXT_UNICODE_REVERSE_CONTROLS      0x0040
  #define IS_TEXT_UNICODE_SIGNATURE             0x0008
  #define IS_TEXT_UNICODE_REVERSE_SIGNATURE     0x0080
  #define IS_TEXT_UNICODE_ILLEGAL_CHARS         0x0100
  #define IS_TEXT_UNICODE_ODD_LENGTH            0x0200
  #define IS_TEXT_UNICODE_DBCS_LEADBYTE         0x0400
  #define IS_TEXT_UNICODE_NULL_BYTES            0x1000
  *//*
  {
    char szBuf[512];

    wsprintf(szBuf,
      "IS_TEXT_UNICODE_ASCII16\t\t\t%s\n"\
      "IS_TEXT_UNICODE_REVERSE_ASCII16\t\t%s\n"\
      "IS_TEXT_UNICODE_STATISTICS\t\t%s\n"\
      "IS_TEXT_UNICODE_REVERSE_STATISTICS\t%s\n"\
      "IS_TEXT_UNICODE_CONTROLS\t\t%s\n"\
      "IS_TEXT_UNICODE_REVERSE_CONTROLS\t%s\n"\
      "IS_TEXT_UNICODE_SIGNATURE\t\t%s\n"\
      "IS_TEXT_UNICODE_REVERSE_SIGNATURE\t%s\n"\
      "IS_TEXT_UNICODE_ILLEGAL_CHARS\t\t%s\n"\
      "IS_TEXT_UNICODE_ODD_LENGTH\t\t%s\n"\
      "IS_TEXT_UNICODE_DBCS_LEADBYTE\t\t%s\n"\
      "IS_TEXT_UNICODE_NULL_BYTES\t\t%s\n\n"\
      "bIsTextUnicode\t\t\t\t%i\n\n"\
      "bHasBOM\t\t\t\t\t%i\n"\
      "bHasRBOM\t\t\t\t%i",
      i & IS_TEXT_UNICODE_ASCII16 ? "1" :"-",
      i & IS_TEXT_UNICODE_REVERSE_ASCII16 ? "1" :"-",
      i & IS_TEXT_UNICODE_STATISTICS ? "1" :"-",
      i & IS_TEXT_UNICODE_REVERSE_STATISTICS ? "1" :"-",
      i & IS_TEXT_UNICODE_CONTROLS ? "1" :"-",
      i & IS_TEXT_UNICODE_REVERSE_CONTROLS ? "1" :"-",
      i & IS_TEXT_UNICODE_SIGNATURE ? "1" :"-",
      i & IS_TEXT_UNICODE_REVERSE_SIGNATURE ? "1" :"-",
      i & IS_TEXT_UNICODE_ILLEGAL_CHARS ? "1" :"-",
      i & IS_TEXT_UNICODE_ODD_LENGTH ? "1" :"-",
      i & IS_TEXT_UNICODE_DBCS_LEADBYTE ? "1" :"-",
      i & IS_TEXT_UNICODE_NULL_BYTES ? "1" :"-",
      bIsTextUnicode,bHasBOM,bHasRBOM);

    MessageBox(hwnd,szBuf,"Unicode file guessing",0);
  }*/
  /*
  Andere Variante: IsTextUnicode() einmal auf normal, einmal auf _swab(),
  wenn vorhanden BOM und RBOM selber entfernen

  Auch noch zu Probieren: Einfach konvertieren, und mit lpUsedDefaultChar
  bei WideCharToMultiByte() checken, ob Zeichen nicht interpretiert werden
  konnten -> Hinweis auf non-Unicode-File.
  */

  if (i == 0xFFFF) // i doesn't seem to have been modified ...
    i = 0;

  if (bIsTextUnicode || bHasBOM || bHasRBOM ||
        ((i & (IS_TEXT_UNICODE_UNICODE_MASK | IS_TEXT_UNICODE_REVERSE_MASK)) &&
       !((i & IS_TEXT_UNICODE_UNICODE_MASK) && (i & IS_TEXT_UNICODE_REVERSE_MASK)) &&
        !(i & IS_TEXT_UNICODE_ODD_LENGTH) &&
        !(i & IS_TEXT_UNICODE_ILLEGAL_CHARS && !(i & IS_TEXT_UNICODE_REVERSE_SIGNATURE)))) {

    if (lpbBOM)
      *lpbBOM = (bHasBOM || bHasRBOM ||
        (i & (IS_TEXT_UNICODE_SIGNATURE | IS_TEXT_UNICODE_REVERSE_SIGNATURE)))
          ? TRUE : FALSE;

    if (lpbReverse)
      *lpbReverse = (bHasRBOM || (i & IS_TEXT_UNICODE_REVERSE_MASK)) ? TRUE : FALSE;

    return TRUE;
  }

  else

    return FALSE;

}


BOOL IsUTF8(const char* pTest,int nLength)
{
  static int byte_class_table[256] = {
  /*       00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  */
  /* 00 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 10 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 20 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 30 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 40 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 50 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 60 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 70 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 80 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  /* 90 */ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  /* A0 */ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  /* B0 */ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  /* C0 */ 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
  /* D0 */ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
  /* E0 */ 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 7, 7,
  /* F0 */ 9,10,10,10,11, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
  /*       00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  */ };

  /* state table */
  typedef enum {
    kSTART = 0,kA,kB,kC,kD,kE,kF,kG,kERROR,kNumOfStates } utf8_state;

  static utf8_state state_table[] = {
  /*                            kSTART, kA,     kB,     kC,     kD,     kE,     kF,     kG,     kERROR */
  /* 0x00-0x7F: 0            */ kSTART, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
  /* 0x80-0x8F: 1            */ kERROR, kSTART, kA,     kERROR, kA,     kB,     kERROR, kB,     kERROR,
  /* 0x90-0x9f: 2            */ kERROR, kSTART, kA,     kERROR, kA,     kB,     kB,     kERROR, kERROR,
  /* 0xa0-0xbf: 3            */ kERROR, kSTART, kA,     kA,     kERROR, kB,     kB,     kERROR, kERROR,
  /* 0xc0-0xc1, 0xf5-0xff: 4 */ kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
  /* 0xc2-0xdf: 5            */ kA,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
  /* 0xe0: 6                 */ kC,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
  /* 0xe1-0xec, 0xee-0xef: 7 */ kB,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
  /* 0xed: 8                 */ kD,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
  /* 0xf0: 9                 */ kF,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
  /* 0xf1-0xf3: 10           */ kE,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
  /* 0xf4: 11                */ kG,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR };

  #define BYTE_CLASS(b) (byte_class_table[(unsigned char)b])
  #define NEXT_STATE(b,cur) (state_table[(BYTE_CLASS(b) * kNumOfStates) + (cur)])

    utf8_state current = kSTART;
    int i;

    const char* pt = pTest;
    int len = nLength;

    for(i = 0; i < len ; i++, pt++) {

      current = NEXT_STATE(*pt,current);
      if (kERROR == current)
        break;
      }

    return (current == kSTART) ? TRUE : FALSE;
  }


BOOL IsUTF7(const char* pTest,int nLength)
{
  int i;
  const char *pt = pTest;

  for (i = 0; i < nLength; i++) {
    if (*pt & 0x80 || !*pt)
      return FALSE;
    pt++;
  }

  return TRUE;
}


#define IsUTF8Signature(p) \
          ((*(p+0) == '\xEF' && *(p+1) == '\xBB' && *(p+2) == '\xBF'))


#define UTF8StringStart(p) \
          (IsUTF8Signature(p)) ? (p+3) : (p)


/* byte length of UTF-8 sequence based on value of first byte.
   for UTF-16 (21-bit space), max. code length is 4, so we only need to look
   at 4 upper bits.
 */
static const INT utf8_lengths[16]=
{
    1,1,1,1,1,1,1,1,        /* 0000 to 0111 : 1 byte (plain ASCII) */
    0,0,0,0,                /* 1000 to 1011 : not valid */
    2,2,                    /* 1100, 1101 : 2 bytes */
    3,                      /* 1110 : 3 bytes */
    4                       /* 1111 :4 bytes */
};

/*++
Function :
    UTF8_mbslen_bytes [INTERNAL]

    Calculates the byte size of a NULL-terminated UTF-8 string.

Parameters :
    char *utf8_string : string to examine

Return value :
    size (in bytes) of a NULL-terminated UTF-8 string.
    -1 if invalid NULL-terminated UTF-8 string
--*/
static INT UTF8_mbslen_bytes(LPCSTR utf8_string)
{
    INT length=0;
    INT code_size;
    BYTE byte;

    while(*utf8_string)
    {
        byte=(BYTE)*utf8_string;

        if( (byte <= 0xF7) && (0 != (code_size = utf8_lengths[ byte >> 4 ])))
        {
            length+=code_size;
            utf8_string+=code_size;
        }
        else
        {
            /* we got an invalid byte value but need to count it,
               it will be later ignored during the string conversion */
            //WARN("invalid first byte value 0x%02X in UTF-8 sequence!\n",byte);
            length++;
            utf8_string++;
        }
    }
    length++; /* include NULL terminator */
    return length;
}

/*++
Function :
    UTF8_mbslen [INTERNAL]

    Calculates the character size of a NULL-terminated UTF-8 string.

Parameters :
    char *utf8_string : string to examine
    int byte_length : byte size of string

Return value :
    size (in characters) of a UTF-8 string.
    -1 if invalid UTF-8 string
--*/
static INT UTF8_mbslen(LPCSTR source, INT byte_length)
{
    INT wchar_length=0;
    INT code_size;
    BYTE byte;

    while(byte_length > 0)
    {
        byte=(BYTE)*source;

        /* UTF-16 can't encode 5-byte and 6-byte sequences, so maximum value
           for first byte is 11110111. Use lookup table to determine sequence
           length based on upper 4 bits of first byte */
        if ((byte <= 0xF7) && (0 != (code_size=utf8_lengths[ byte >> 4])))
        {
            /* 1 sequence == 1 character */
            wchar_length++;

            if(code_size==4)
                wchar_length++;

            source+=code_size;        /* increment pointer */
            byte_length-=code_size;   /* decrement counter*/
        }
        else
        {
            /*
               unlike UTF8_mbslen_bytes, we ignore the invalid characters.
               we only report the number of valid characters we have encountered
               to match the Windows behavior.
            */
            //WARN("invalid byte 0x%02X in UTF-8 sequence, skipping it!\n",
            //     byte);
            source++;
            byte_length--;
        }
    }
    return wchar_length;
}


//=============================================================================
//
//  EditLoadFile()
//
BOOL EditLoadFile(HWND hwnd,LPCWSTR pszFile,BOOL bSkipEncodingDetection,
                  int* iCodePage,int* iEOLMode,BOOL *pbUnicodeErr,BOOL *pbFileTooBig)
{

  HANDLE hFile;

  DWORD  dwFileSize;
  DWORD  dwFileSizeLimit;
  DWORD  dwBufSize;
  BOOL   bReadSuccess;

  char* lpData;
  DWORD cbData;
  //char  *cp;

  BOOL bBOM;
  BOOL bReverse;

  *pbUnicodeErr = FALSE;
  *pbFileTooBig = FALSE;

  hFile = CreateFile(pszFile,
                     GENERIC_READ,
                     FILE_SHARE_READ|FILE_SHARE_WRITE,
                     NULL,
                     OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL,
                     NULL);

  if (hFile == INVALID_HANDLE_VALUE)
    return FALSE;

  // calculate buffer limit
  dwFileSize = GetFileSize(hFile,NULL);
  dwBufSize  = dwFileSize + 10;

  // Check if a warning message should be displayed for large files
  dwFileSizeLimit = IniGetInt(L"Settings2",L"FileLoadWarningMB",1);
  if (dwFileSizeLimit != 0 && dwFileSizeLimit * 1024 * 1024 < dwFileSize) {
    if (InfoBox(MBYESNO,IDS_WARNLOADBIGFILE,L"MsgFileSizeWarning") != IDYES) {
      CloseHandle(hFile);
      *pbFileTooBig = TRUE;
      return FALSE;
    }
  }

  lpData = GlobalAlloc(GPTR,dwBufSize);
  bReadSuccess = ReadFile(hFile,lpData,GlobalSize(lpData)-2,&cbData,NULL);
  CloseHandle(hFile);

  if (!bReadSuccess)
  {
    GlobalFree(lpData);
    return FALSE;
  }

  // default codepage
  *iCodePage = NCP_DEFAULT;

  if (cbData == 0) {
    SendMessage(hwnd,SCI_SETCODEPAGE,(iDefaultEncoding == 0) ? iDefaultCodePage : SC_CP_UTF8,0);
    FileVars_Init(NULL,0,&fvCurFile);
    EditSetNewText(hwnd,"",0);
    SendMessage(hwnd,SCI_SETEOLMODE,iLineEndings[iDefaultEOLMode],0);
    *iEOLMode = iLineEndings[iDefaultEOLMode];
    *iCodePage = iEncodings[iDefaultEncoding];
    GlobalFree(lpData);
  }

  else if (!bSkipEncodingDetection &&
      IsUnicode(lpData,cbData,&bBOM,&bReverse) && !IsUTF8Signature(lpData)) // check for UTF-8 signature
  {
    char*  lpDataUTF8;
    CPINFO cpi;
    UINT   uCP_UTF8;

    *iCodePage = NCP_UNICODE;
    if (bBOM)
      *iCodePage |= NCP_UNICODE_BOM;

    if (bReverse)
    {
      _swab(lpData,lpData,cbData);
      *iCodePage |= NCP_UNICODE_REVERSE;
    }

    // Unicode text is converted to ANSI and not to UTF-8 on Windows 95
    uCP_UTF8 = (GetCPInfo(CP_UTF8, &cpi) || IsValidCodePage(CP_UTF8)) ? CP_UTF8 : CP_ACP;

    lpDataUTF8 = GlobalAlloc(GPTR,(cbData * 3) + 2);
    cbData = WideCharToMultiByte(uCP_UTF8,0,(bBOM) ? (LPWSTR)lpData + 1 : (LPWSTR)lpData,
              (-1),lpDataUTF8,GlobalSize(lpDataUTF8),NULL,NULL);

    if (cbData == 0 && uCP_UTF8 == CP_UTF8)
    {
      cbData = WideCharToMultiByte(CP_ACP,0,(bBOM) ? (LPWSTR)lpData + 1 : (LPWSTR)lpData,
                (-1),lpDataUTF8,GlobalSize(lpDataUTF8),NULL,NULL);
      *pbUnicodeErr = TRUE;
    }

    GlobalFree(lpData);
    SendMessage(hwnd,SCI_SETCODEPAGE,SC_CP_UTF8,0);
    EditSetNewText(hwnd,"",0);
    FileVars_Init(lpDataUTF8,cbData-1,&fvCurFile);
    EditSetNewText(hwnd,lpDataUTF8,cbData-1);
    *iEOLMode = EditDetectEOLMode(hwnd,lpDataUTF8,cbData-1);
    GlobalFree(lpDataUTF8);
  }

  else {
    FileVars_Init(lpData,cbData,&fvCurFile);
    if (!bSkipEncodingDetection &&
            ((IsUTF8Signature(lpData) ||
              FileVars_IsUTF8(&fvCurFile) ||
              (IsUTF8(lpData,cbData) &&
              (((UTF8_mbslen_bytes(UTF8StringStart(lpData)) - 1 !=
                UTF8_mbslen(UTF8StringStart(lpData),IsUTF8Signature(lpData) ? cbData-3 : cbData)) ||
                iEncodings[iDefaultEncoding] == NCP_UTF8))))) && !FileVars_IsANSI(&fvCurFile))
    {
      SendMessage(hwnd,SCI_SETCODEPAGE,SC_CP_UTF8,0);
      EditSetNewText(hwnd,"",0);
      if (IsUTF8Signature(lpData)) {
        EditSetNewText(hwnd,UTF8StringStart(lpData),cbData-3);
        *iEOLMode = EditDetectEOLMode(hwnd,UTF8StringStart(lpData),cbData-3);
        *iCodePage = NCP_UTF8 | NCP_UTF8_SIGN;
      }
      else {
        EditSetNewText(hwnd,lpData,cbData);
        *iEOLMode = EditDetectEOLMode(hwnd,lpData,cbData);
        *iCodePage = NCP_UTF8;
      }
      GlobalFree(lpData);
    }

    else
    {
      SendMessage(hwnd,SCI_SETCODEPAGE,iDefaultCodePage,0);
      EditSetNewText(hwnd,"",0);
      EditSetNewText(hwnd,lpData,cbData);
      *iEOLMode = EditDetectEOLMode(hwnd,lpData,cbData);
      *iCodePage = NCP_DEFAULT;
      GlobalFree(lpData);
    }
  }

  return TRUE;

}


//=============================================================================
//
//  EditSaveFile()
//
BOOL EditSaveFile(HWND hwnd,LPCWSTR pszFile,int iCodePage,BOOL bSaveCopy)
{

  HANDLE hFile;
  BOOL   bWriteSuccess;

  char* lpData;
  DWORD cbData;
  DWORD dwBytesWritten;

  hFile = CreateFile(pszFile,
                     GENERIC_WRITE,
                     FILE_SHARE_READ|FILE_SHARE_WRITE,
                     NULL,
                     OPEN_ALWAYS,
                     FILE_ATTRIBUTE_NORMAL,
                     NULL);

  // failure could be due to missing attributes (2k/XP)
  if (hFile == INVALID_HANDLE_VALUE)
  {
    DWORD dwAttributes = GetFileAttributes(pszFile);
    if (dwAttributes != INVALID_FILE_ATTRIBUTES)
    {
      dwAttributes = dwAttributes & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
      hFile = CreateFile(pszFile,
                        GENERIC_WRITE,
                        FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL,
                        OPEN_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL | dwAttributes,
                        NULL);
    }
  }

  if (hFile == INVALID_HANDLE_VALUE)
    return FALSE;

  // ensure consistent line endings
  if (bFixLineEndings)
    SendMessage(hwnd,SCI_CONVERTEOLS,SendMessage(hwnd,SCI_GETEOLMODE,0,0),0);

  // get text
  cbData = SendMessage(hwnd,SCI_GETLENGTH,0,0);
  lpData = GlobalAlloc(GPTR,cbData + 1);
  SendMessage(hwnd,SCI_GETTEXT,GlobalSize(lpData),(LPARAM)lpData);

  if (cbData == 0)
    bWriteSuccess = SetEndOfFile(hFile);

  else
  {
    SetEndOfFile(hFile);

    if (iCodePage & NCP_UNICODE)
    {
      LPWSTR lpDataWide;
      int    cbDataWide;
      CPINFO cpi;
      UINT   uCP_UTF8;

      // UTF-8 text is interpreted as ANSI when saving as Unicode on Windows 95
      uCP_UTF8 = (GetCPInfo(CP_UTF8, &cpi) || IsValidCodePage(CP_UTF8)) ? CP_UTF8 : CP_ACP;

      lpDataWide = GlobalAlloc(GPTR,cbData * 2 + 16);
      cbDataWide = MultiByteToWideChar(uCP_UTF8,0,lpData,cbData,lpDataWide,GlobalSize(lpDataWide)/sizeof(WCHAR));

      if (iCodePage & NCP_UNICODE_BOM) {
        if (iCodePage & NCP_UNICODE_REVERSE)
          WriteFile(hFile,(LPCVOID)"\xFE\xFF",2,&dwBytesWritten,NULL);
        else
          WriteFile(hFile,(LPCVOID)"\xFF\xFE",2,&dwBytesWritten,NULL);
        }

      if (iCodePage & NCP_UNICODE_REVERSE)
        _swab((char*)lpDataWide,(char*)lpDataWide,cbDataWide * sizeof(WCHAR));

      bWriteSuccess = WriteFile(hFile,lpDataWide,cbDataWide * sizeof(WCHAR),&dwBytesWritten,NULL);

      GlobalFree(lpData);
    }

    else if (iCodePage & NCP_UTF8)
    {
      if (iCodePage & NCP_UTF8_SIGN)
        WriteFile(hFile,(LPCVOID)"\xEF\xBB\xBF",3,&dwBytesWritten,NULL);

      bWriteSuccess = WriteFile(hFile,lpData,cbData,&dwBytesWritten,NULL);
    }

    else // convert text to 8bit
    {
      //LPWSTR lpDataWide = GlobalAlloc(GPTR,cbData * 2 + 16);
      //int    cbDataWide = MultiByteToWideChar(CP_UTF8,0,lpData,cbData,lpDataWide,GlobalSize(lpDataWide)/sizeof(WCHAR));

      //ZeroMemory(lpData,GlobalSize(lpData));
      //cbData = WideCharToMultiByte(CP_ACP,0,lpDataWide,cbDataWide,lpData,GlobalSize(lpData)/sizeof(WCHAR),NULL,NULL);
      //GlobalFree(lpDataWide);

      bWriteSuccess = WriteFile(hFile,lpData,cbData,&dwBytesWritten,NULL);

      GlobalFree(lpData);
    }
  }

  CloseHandle(hFile);

  if (bWriteSuccess)
  {
    if (!bSaveCopy)
      SendMessage(hwnd,SCI_SETSAVEPOINT,0,0);

    return TRUE;
  }

  else
    return FALSE;

}


//=============================================================================
//
//  EditMakeUppercase()
//
void EditMakeUppercase(HWND hwnd)
{
  int cchTextW;
  int iCurPos;
  int iAnchorPos;
  UINT cpEdit;
  int i;
  BOOL bChanged = FALSE;

  iCurPos    = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
  iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);

  if (iCurPos != iAnchorPos)
  {
    if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0))
    {
      int iSelCount = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0) -
                        SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);

      char*  pszText  = GlobalAlloc(GPTR,(iSelCount)+2);
      LPWSTR pszTextW = GlobalAlloc(GPTR,(iSelCount*2)+2);

      if (pszText == NULL || pszTextW == NULL) {
        GlobalFree(pszText);
        GlobalFree(pszTextW);
        return;
      }

      SendMessage(hwnd,SCI_GETSELTEXT,0,(LPARAM)pszText);

      cpEdit = SendMessage(hwnd,SCI_GETCODEPAGE,0,0);

      cchTextW = MultiByteToWideChar(cpEdit,0,pszText,iSelCount,pszTextW,GlobalSize(pszTextW)/sizeof(WCHAR));

      for (i = 0; i < cchTextW; i++) {
        if (IsCharLowerW(pszTextW[i])) {
          pszTextW[i] = LOWORD(CharUpperW((LPWSTR)MAKELONG(pszTextW[i],0)));
          bChanged = TRUE;
        }
      }

      if (bChanged) {

        WideCharToMultiByte(cpEdit,0,pszTextW,cchTextW,pszText,GlobalSize(pszText),NULL,NULL);
        GlobalFree(pszTextW);

        SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);
        SendMessage(hwnd,SCI_CLEAR,0,0);
        SendMessage(hwnd,SCI_ADDTEXT,(WPARAM)iSelCount,(LPARAM)pszText);
        SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
        SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);

        GlobalFree(pszText);
      }

      else
        GlobalFree(pszTextW);

    }
    else
      MsgBox(MBINFO,IDS_SELRECT);
  }
}


//=============================================================================
//
//  EditMakeLowercase()
//
void EditMakeLowercase(HWND hwnd)
{
  int cchTextW;
  int iCurPos;
  int iAnchorPos;
  UINT cpEdit;
  int i;
  BOOL bChanged = FALSE;

  iCurPos    = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
  iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);

  if (iCurPos != iAnchorPos)
  {
    if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0))
    {
      int iSelCount = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0) -
                        SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);

      char*  pszText  = GlobalAlloc(GPTR,(iSelCount)+2);
      LPWSTR pszTextW = GlobalAlloc(GPTR,(iSelCount*2)+2);

      if (pszText == NULL || pszTextW == NULL) {
        GlobalFree(pszText);
        GlobalFree(pszTextW);
        return;
      }

      SendMessage(hwnd,SCI_GETSELTEXT,0,(LPARAM)pszText);

      cpEdit = SendMessage(hwnd,SCI_GETCODEPAGE,0,0);

      cchTextW = MultiByteToWideChar(cpEdit,0,pszText,iSelCount,pszTextW,GlobalSize(pszTextW)/sizeof(WCHAR));

      for (i = 0; i < cchTextW; i++) {
        if (IsCharUpperW(pszTextW[i])) {
          pszTextW[i] = LOWORD(CharLowerW((LPWSTR)MAKELONG(pszTextW[i],0)));
          bChanged = TRUE;
        }
      }

      if (bChanged) {

        WideCharToMultiByte(cpEdit,0,pszTextW,cchTextW,pszText,GlobalSize(pszText),NULL,NULL);
        GlobalFree(pszTextW);

        SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);
        SendMessage(hwnd,SCI_CLEAR,0,0);
        SendMessage(hwnd,SCI_ADDTEXT,(WPARAM)iSelCount,(LPARAM)pszText);
        SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
        SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);

        GlobalFree(pszText);
      }

      else
        GlobalFree(pszTextW);

    }
    else
      MsgBox(MBINFO,IDS_SELRECT);
  }
}


//=============================================================================
//
//  EditInvertCase()
//
void EditInvertCase(HWND hwnd)
{
  int cchTextW;
  int iCurPos;
  int iAnchorPos;
  UINT cpEdit;
  int i;
  BOOL bChanged = FALSE;

  iCurPos    = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
  iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);

  if (iCurPos != iAnchorPos)
  {
    if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0))
    {
      int iSelCount = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0) -
                        SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);

      char*  pszText  = GlobalAlloc(GPTR,(iSelCount)+2);
      LPWSTR pszTextW = GlobalAlloc(GPTR,(iSelCount*2)+2);

      if (pszText == NULL || pszTextW == NULL) {
        GlobalFree(pszText);
        GlobalFree(pszTextW);
        return;
      }

      SendMessage(hwnd,SCI_GETSELTEXT,0,(LPARAM)pszText);

      cpEdit = SendMessage(hwnd,SCI_GETCODEPAGE,0,0);

      cchTextW = MultiByteToWideChar(cpEdit,0,pszText,iSelCount,pszTextW,GlobalSize(pszTextW)/sizeof(WCHAR));

      for (i = 0; i < cchTextW; i++) {
        if (IsCharUpperW(pszTextW[i])) {
          pszTextW[i] = LOWORD(CharLowerW((LPWSTR)MAKELONG(pszTextW[i],0)));
          bChanged = TRUE;
        }
        else if (IsCharLowerW(pszTextW[i])) {
          pszTextW[i] = LOWORD(CharUpperW((LPWSTR)MAKELONG(pszTextW[i],0)));
          bChanged = TRUE;
        }
      }

      if (bChanged) {

        WideCharToMultiByte(cpEdit,0,pszTextW,cchTextW,pszText,GlobalSize(pszText),NULL,NULL);
        GlobalFree(pszTextW);

        SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);
        SendMessage(hwnd,SCI_CLEAR,0,0);
        SendMessage(hwnd,SCI_ADDTEXT,(WPARAM)iSelCount,(LPARAM)pszText);
        SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
        SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);

        GlobalFree(pszText);
      }

      else
        GlobalFree(pszTextW);

    }
    else
      MsgBox(MBINFO,IDS_SELRECT);
  }
}


//=============================================================================
//
//  EditTitleCase()
//
void EditTitleCase(HWND hwnd)
{
  int cchTextW;
  int iCurPos;
  int iAnchorPos;
  UINT cpEdit;
  int i;
  BOOL bNewWord = TRUE;
  BOOL bChanged = FALSE;

  iCurPos    = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
  iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);

  if (iCurPos != iAnchorPos)
  {
    if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0))
    {
      int iSelCount = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0) -
                        SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);

      char*  pszText  = GlobalAlloc(GPTR,(iSelCount)+2);
      LPWSTR pszTextW = GlobalAlloc(GPTR,(iSelCount*2)+2);

      if (pszText == NULL || pszTextW == NULL) {
        GlobalFree(pszText);
        GlobalFree(pszTextW);
        return;
      }

      SendMessage(hwnd,SCI_GETSELTEXT,0,(LPARAM)pszText);

      cpEdit = SendMessage(hwnd,SCI_GETCODEPAGE,0,0);

      cchTextW = MultiByteToWideChar(cpEdit,0,pszText,iSelCount,pszTextW,GlobalSize(pszTextW)/sizeof(WCHAR));

      for (i = 0; i < cchTextW; i++) {
        if (!IsCharAlphaNumericW(pszTextW[i]) && !StrChr(L"'’",pszTextW[i])) {
          bNewWord = TRUE;
        }
        else {
          if (bNewWord) {
            if (IsCharLowerW(pszTextW[i])) {
              pszTextW[i] = LOWORD(CharUpperW((LPWSTR)MAKELONG(pszTextW[i],0)));
              bChanged = TRUE;
            }
          }
          else {
            if (IsCharUpperW(pszTextW[i])) {
              pszTextW[i] = LOWORD(CharLowerW((LPWSTR)MAKELONG(pszTextW[i],0)));
              bChanged = TRUE;
            }
          }
          bNewWord = FALSE;
        }
      }

      if (bChanged) {

        WideCharToMultiByte(cpEdit,0,pszTextW,cchTextW,pszText,GlobalSize(pszText),NULL,NULL);
        GlobalFree(pszTextW);

        SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);
        SendMessage(hwnd,SCI_CLEAR,0,0);
        SendMessage(hwnd,SCI_ADDTEXT,(WPARAM)iSelCount,(LPARAM)pszText);
        SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
        SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);

        GlobalFree(pszText);
      }

      else
        GlobalFree(pszTextW);

    }
    else
      MsgBox(MBINFO,IDS_SELRECT);
  }
}


//=============================================================================
//
//  EditSentenceCase()
//
void EditSentenceCase(HWND hwnd)
{
  int cchTextW;
  int iCurPos;
  int iAnchorPos;
  UINT cpEdit;
  int i;
  BOOL bNewSentence = TRUE;
  BOOL bChanged = FALSE;

  iCurPos    = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
  iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);

  if (iCurPos != iAnchorPos)
  {
    if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0))
    {
      int iSelCount = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0) -
                        SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);

      char*  pszText  = GlobalAlloc(GPTR,(iSelCount)+2);
      LPWSTR pszTextW = GlobalAlloc(GPTR,(iSelCount*2)+2);

      if (pszText == NULL || pszTextW == NULL) {
        GlobalFree(pszText);
        GlobalFree(pszTextW);
        return;
      }

      SendMessage(hwnd,SCI_GETSELTEXT,0,(LPARAM)pszText);

      cpEdit = SendMessage(hwnd,SCI_GETCODEPAGE,0,0);

      cchTextW = MultiByteToWideChar(cpEdit,0,pszText,iSelCount,pszTextW,GlobalSize(pszTextW)/sizeof(WCHAR));

      for (i = 0; i < cchTextW; i++) {
        if (StrChr(L".;!?\r\n",pszTextW[i])) {
          bNewSentence = TRUE;
        }
        else {
          if (IsCharAlphaNumericW(pszTextW[i])) {
            if (bNewSentence) {
              if (IsCharLowerW(pszTextW[i])) {
                pszTextW[i] = LOWORD(CharUpperW((LPWSTR)MAKELONG(pszTextW[i],0)));
                bChanged = TRUE;
              }
              bNewSentence = FALSE;
            }
            else {
              if (IsCharUpperW(pszTextW[i])) {
                pszTextW[i] = LOWORD(CharLowerW((LPWSTR)MAKELONG(pszTextW[i],0)));
                bChanged = TRUE;
              }
            }
          }
        }
      }

      if (bChanged) {

        WideCharToMultiByte(cpEdit,0,pszTextW,cchTextW,pszText,GlobalSize(pszText),NULL,NULL);
        GlobalFree(pszTextW);

        SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);
        SendMessage(hwnd,SCI_CLEAR,0,0);
        SendMessage(hwnd,SCI_ADDTEXT,(WPARAM)iSelCount,(LPARAM)pszText);
        SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
        SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);

        GlobalFree(pszText);
      }

      else
        GlobalFree(pszTextW);

    }
    else
      MsgBox(MBINFO,IDS_SELRECT);
  }
}


//=============================================================================
//
//  EditTabsToSpaces()
//
void EditTabsToSpaces(HWND hwnd,int nTabWidth)
{
  char* pszText;
  LPWSTR pszTextW;
  int cchTextW;
  int iTextW;
  LPWSTR pszConvW;
  int cchConvW;
  int cchConvM;
  int i,j;
  int iLine;
  int iCurPos;
  int iAnchorPos;
  int iSelStart;
  int iSelEnd;
  int iSelCount;
  UINT cpEdit;
  struct TextRange tr;

  if (SC_SEL_RECTANGLE == SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0)) {
    MsgBox(MBINFO,IDS_SELRECT);
    return;
  }

  iCurPos    = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
  iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);

  if (iCurPos == iAnchorPos) /*{
    iSelStart = 0;
    iSelEnd   = SendMessage(hwnd,SCI_GETLENGTH,0,0);
  }*/
    return;

  else {
    iSelStart = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);
    iSelEnd   = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0);
  }

  iLine = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelStart,0);
  iSelStart = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLine,0);

  iSelCount = iSelEnd - iSelStart;

  pszText = GlobalAlloc(GPTR,(iSelCount)+2);
  if (pszText == NULL)
    return;

  pszTextW = GlobalAlloc(GPTR,(iSelCount*2)+2);
  if (pszTextW == NULL) {
    GlobalFree(pszText);
    return;
  }

  tr.chrg.cpMin = iSelStart;
  tr.chrg.cpMax = iSelEnd;
  tr.lpstrText = pszText;
  SendMessage(hwnd,SCI_GETTEXTRANGE,0,(LPARAM)&tr);

  cpEdit = SendMessage(hwnd,SCI_GETCODEPAGE,0,0);

  cchTextW = MultiByteToWideChar(cpEdit,0,pszText,iSelCount,pszTextW,GlobalSize(pszTextW)/sizeof(WCHAR));
  GlobalFree(pszText);

  pszConvW = GlobalAlloc(GPTR,cchTextW*sizeof(WCHAR)*nTabWidth+2);
  if (pszConvW == NULL) {
    GlobalFree(pszTextW);
    return;
  }

  cchConvW = 0;

  // Contributed by Homam
  // Thank you very much!
  i=0;
  for (iTextW = 0; iTextW < cchTextW; iTextW++)
  {
    WCHAR w = pszTextW[iTextW];
    if (w == L'\t') {
      for (j = 0; j < nTabWidth - i % nTabWidth; j++)
        pszConvW[cchConvW++] = L' ';
      i = 0;
    }
    else {
      i++;
      if (w == L'\n' || w == L'\r')
        i = 0;
      pszConvW[cchConvW++] = w;
    }
  }

  GlobalFree(pszTextW);

  if (cchConvW != cchTextW) {
    pszText = GlobalAlloc(GPTR,cchConvW * 3);

    cchConvM = WideCharToMultiByte(cpEdit,0,pszConvW,cchConvW,pszText,GlobalSize(pszText),NULL,NULL);
    GlobalFree(pszConvW);

    if (iAnchorPos > iCurPos) {
      iCurPos = iSelStart;
      iAnchorPos = iSelStart + cchConvM;
    }
    else {
      iAnchorPos = iSelStart;
      iCurPos = iSelStart + cchConvM;
    }

    SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);
    SendMessage(hwnd,SCI_SETTARGETSTART,(WPARAM)iSelStart,0);
    SendMessage(hwnd,SCI_SETTARGETEND,(WPARAM)iSelEnd,0);
    SendMessage(hwnd,SCI_REPLACETARGET,(WPARAM)cchConvM,(LPARAM)pszText);
    //SendMessage(hwnd,SCI_CLEAR,0,0);
    //SendMessage(hwnd,SCI_ADDTEXT,(WPARAM)cchConvW,(LPARAM)pszText);
    SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
    SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);

    GlobalFree(pszText);
  }

  else
    GlobalFree(pszConvW);
}


//=============================================================================
//
//  EditSpacesToTabs()
//
void EditSpacesToTabs(HWND hwnd,int nTabWidth)
{
  char* pszText;
  LPWSTR pszTextW;
  int cchTextW;
  int iTextW;
  LPWSTR pszConvW;
  int cchConvW;
  int cchConvM;
  int i,j,t;
  int iLine;
  int iCurPos;
  int iAnchorPos;
  int iSelStart;
  int iSelEnd;
  int iSelCount;
  UINT cpEdit;
  struct TextRange tr;
  WCHAR space[64];

  if (SC_SEL_RECTANGLE == SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0)) {
    MsgBox(MBINFO,IDS_SELRECT);
    return;
  }

  iCurPos    = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
  iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);

  if (iCurPos == iAnchorPos) /*{
    iSelStart = 0;
    iSelEnd   = SendMessage(hwnd,SCI_GETLENGTH,0,0);
  }*/
    return;

  else {
    iSelStart = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);
    iSelEnd   = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0);
  }

  iLine = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelStart,0);
  iSelStart = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLine,0);

  iSelCount = iSelEnd - iSelStart;

  pszText = GlobalAlloc(GPTR,(iSelCount)+2);
  if (pszText == NULL)
    return;

  pszTextW = GlobalAlloc(GPTR,(iSelCount*2)+2);
  if (pszTextW == NULL) {
    GlobalFree(pszText);
    return;
  }

  tr.chrg.cpMin = iSelStart;
  tr.chrg.cpMax = iSelEnd;
  tr.lpstrText = pszText;
  SendMessage(hwnd,SCI_GETTEXTRANGE,0,(LPARAM)&tr);

  cpEdit = SendMessage(hwnd,SCI_GETCODEPAGE,0,0);

  cchTextW = MultiByteToWideChar(cpEdit,0,pszText,iSelCount,pszTextW,GlobalSize(pszTextW)/sizeof(WCHAR));
  GlobalFree(pszText);

  pszConvW = GlobalAlloc(GPTR,cchTextW*sizeof(WCHAR)+2);
  if (pszConvW == NULL) {
    GlobalFree(pszTextW);
    return;
  }

  cchConvW = 0;

  // Contributed by Homam
  // Thank you very much!
  i = j = 0;
  for (iTextW = 0; iTextW < cchTextW; iTextW++)
  {
    WCHAR w = pszTextW[iTextW];
    if ((w == L' ' || w == L'\t')) {
      space[j++] = w;
      if (j == nTabWidth - i % nTabWidth || w == L'\t') {
        if (j > 1 || pszTextW[iTextW+1] == L' ' || pszTextW[iTextW+1] == L'\t')
          pszConvW[cchConvW++] = L'\t';
        else
          pszConvW[cchConvW++] = w;
        i = j = 0;
      }
    }
    else {
      i += j + 1;
      if (j > 0) {
        //space[j] = '\0';
        for (t = 0; t < j; t++)
          pszConvW[cchConvW++] = space[t];
        j = 0;
      }
      if (w == L'\n' || w == L'\r')
        i = 0;
      pszConvW[cchConvW++] = w;
    }
  }
  if (j > 0) {
    for (t = 0; t < j; t++)
      pszConvW[cchConvW++] = space[t];
    }

  GlobalFree(pszTextW);

  if (cchConvW != cchTextW) {
    pszText = GlobalAlloc(GPTR,cchConvW * 3);

    cchConvM = WideCharToMultiByte(cpEdit,0,pszConvW,cchConvW,pszText,GlobalSize(pszText),NULL,NULL);
    GlobalFree(pszConvW);

    if (iAnchorPos > iCurPos) {
      iCurPos = iSelStart;
      iAnchorPos = iSelStart + cchConvM;
    }
    else {
      iAnchorPos = iSelStart;
      iCurPos = iSelStart + cchConvM;
    }

    SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);
    SendMessage(hwnd,SCI_SETTARGETSTART,(WPARAM)iSelStart,0);
    SendMessage(hwnd,SCI_SETTARGETEND,(WPARAM)iSelEnd,0);
    SendMessage(hwnd,SCI_REPLACETARGET,(WPARAM)cchConvM,(LPARAM)pszText);
    //SendMessage(hwnd,SCI_CLEAR,0,0);
    //SendMessage(hwnd,SCI_ADDTEXT,(WPARAM)cchConvW,(LPARAM)pszText);
    SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
    SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);

    GlobalFree(pszText);
  }

  else
    GlobalFree(pszConvW);
}


//=============================================================================
//
//  EditModifyLines()
//
void EditModifyLines(HWND hwnd,LPCWSTR pwszPrefix,LPCWSTR pwszAppend)
{
  char  mszPrefix[256*3] = "";
  char  mszAppend[256*3] = "";
  int   mbcp;

  int iSelStart = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);
  int iSelEnd   = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0);

  //if (iSelStart == iSelEnd) {
  //  iSelStart = 0;
  //  iSelEnd   = SendMessage(hwnd,SCI_GETLENGTH,0,0);
  //}

  if (SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0) == SC_CP_UTF8)
    mbcp = CP_UTF8;
  else
    mbcp = CP_ACP;

  if (lstrlen(pwszPrefix))
    WideCharToMultiByte(mbcp,0,pwszPrefix,-1,mszPrefix,COUNTOF(mszPrefix),NULL,NULL);
  if (lstrlen(pwszAppend))
    WideCharToMultiByte(mbcp,0,pwszAppend,-1,mszAppend,COUNTOF(mszAppend),NULL,NULL);

  if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0))
  {
    int iLine;

    int iLineStart = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelStart,0);
    int iLineEnd   = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelEnd,0);

    //if (iSelStart > SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineStart,0))
    //  iLineStart++;

    if (iSelEnd <= SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineEnd,0))
    {
      if (iLineEnd - iLineStart >= 1)
        iLineEnd--;
    }

    SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);

    for (iLine = iLineStart; iLine <= iLineEnd; iLine++)
    {
      int iPos;

      if (lstrlen(pwszPrefix)) {
        iPos = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLine,0);
        SendMessage(hwnd,SCI_SETTARGETSTART,(WPARAM)iPos,0);
        SendMessage(hwnd,SCI_SETTARGETEND,(WPARAM)iPos,0);
        SendMessage(hwnd,SCI_REPLACETARGET,(WPARAM)lstrlenA(mszPrefix),(LPARAM)mszPrefix);
      }

      if (lstrlen(pwszAppend)) {
        iPos = SendMessage(hwnd,SCI_GETLINEENDPOSITION,(WPARAM)iLine,0);
        SendMessage(hwnd,SCI_SETTARGETSTART,(WPARAM)iPos,0);
        SendMessage(hwnd,SCI_SETTARGETEND,(WPARAM)iPos,0);
        SendMessage(hwnd,SCI_REPLACETARGET,(WPARAM)lstrlenA(mszAppend),(LPARAM)mszAppend);
      }
    }
    SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);

    //// Fix selection
    //if (iSelStart != iSelEnd && SendMessage(hwnd,SCI_GETTARGETEND,0,0) > SendMessage(hwnd,SCI_GETSELECTIONEND,0,0))
    //{
    //  int iCurPos = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
    //  int iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);
    //  if (iCurPos > iAnchorPos)
    //    iCurPos = SendMessage(hwnd,SCI_GETTARGETEND,0,0);
    //  else
    //    iAnchorPos = SendMessage(hwnd,SCI_GETTARGETEND,0,0);
    //  SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
    //}

    // extend selection to start of first line
    // the above code is not required when last line has been excluded
    if (iSelStart != iSelEnd)
    {
      int iCurPos = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
      int iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);
      if (iCurPos < iAnchorPos) {
        iCurPos = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineStart,0);
        iAnchorPos = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineEnd+1,0);
      }
      else {
        iAnchorPos = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineStart,0);
        iCurPos = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineEnd+1,0);
      }
      SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
    }

  }
  else
    MsgBox(MBINFO,IDS_SELRECT);
}


//=============================================================================
//
//  EditEncloseSelection()
//
void EditEncloseSelection(HWND hwnd,LPCWSTR pwszOpen,LPCWSTR pwszClose)
{
  char  mszOpen[256*3] = "";
  char  mszClose[256*3] = "";
  int   mbcp;

  int iSelStart = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);
  int iSelEnd   = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0);

  if (SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0) == SC_CP_UTF8)
    mbcp = CP_UTF8;
  else
    mbcp = CP_ACP;

  if (lstrlen(pwszOpen))
    WideCharToMultiByte(mbcp,0,pwszOpen,-1,mszOpen,COUNTOF(mszOpen),NULL,NULL);
  if (lstrlen(pwszClose))
    WideCharToMultiByte(mbcp,0,pwszClose,-1,mszClose,COUNTOF(mszClose),NULL,NULL);

  if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0))
  {

    SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);

    if (lstrlenA(mszOpen)) {
      SendMessage(hwnd,SCI_SETTARGETSTART,(WPARAM)iSelStart,0);
      SendMessage(hwnd,SCI_SETTARGETEND,(WPARAM)iSelStart,0);
      SendMessage(hwnd,SCI_REPLACETARGET,(WPARAM)lstrlenA(mszOpen),(LPARAM)mszOpen);
    }

    if (lstrlenA(mszClose)) {
      SendMessage(hwnd,SCI_SETTARGETSTART,(WPARAM)iSelEnd + lstrlenA(mszOpen),0);
      SendMessage(hwnd,SCI_SETTARGETEND,(WPARAM)iSelEnd + lstrlenA(mszOpen),0);
      SendMessage(hwnd,SCI_REPLACETARGET,(WPARAM)lstrlenA(mszClose),(LPARAM)mszClose);
    }

    SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);

    // Fix selection
    if (iSelStart == iSelEnd)
      SendMessage(hwnd,SCI_SETSEL,(WPARAM)iSelStart + lstrlenA(mszOpen),(WPARAM)iSelStart + lstrlenA(mszOpen));

    else {
      int iCurPos = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
      int iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);
      if (iCurPos < iAnchorPos) {
        iCurPos = iSelStart + lstrlenA(mszOpen);
        iAnchorPos = iSelEnd + lstrlenA(mszOpen);
      }
      else {
        iAnchorPos = iSelStart + lstrlenA(mszOpen);
        iCurPos = iSelEnd + lstrlenA(mszOpen);
      }
      SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
    }

  }
  else
    MsgBox(MBINFO,IDS_SELRECT);
}


//=============================================================================
//
//  EditToggleLineComments()
//
void EditToggleLineComments(HWND hwnd,LPCWSTR pwszComment,BOOL bInsertAtStart)
{
  char  mszComment[256*3] = "";
  int   cchComment;
  int   mbcp;
  int   iAction = 0;

  int iSelStart = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);
  int iSelEnd   = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0);
  int iCurPos   = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);

  if (SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0) == SC_CP_UTF8)
    mbcp = CP_UTF8;
  else
    mbcp = CP_ACP;

  if (lstrlen(pwszComment))
    WideCharToMultiByte(mbcp,0,pwszComment,-1,mszComment,COUNTOF(mszComment),NULL,NULL);
  cchComment = lstrlenA(mszComment);

  if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0) && cchComment)
  {
    int iLine;
    int iCommentCol = 0;

    int iLineStart = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelStart,0);
    int iLineEnd   = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelEnd,0);

    if (iSelEnd <= SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineEnd,0))
    {
      if (iLineEnd - iLineStart >= 1)
        iLineEnd--;
    }

    if (!bInsertAtStart) {
      iCommentCol = 1024;
      for (iLine = iLineStart; iLine <= iLineEnd; iLine++) {
        int iLineEndPos = SendMessage(hwnd,SCI_GETLINEENDPOSITION,(WPARAM)iLine,0);
        int iLineIndentPos = SendMessage(hwnd,SCI_GETLINEINDENTPOSITION,(WPARAM)iLine,0);
        if (iLineIndentPos != iLineEndPos) {
          int iIndentColumn = SendMessage(hwnd,SCI_GETCOLUMN,(WPARAM)iLineIndentPos,0);
          iCommentCol = min(iCommentCol,iIndentColumn);
        }
      }
    }

    SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);

    for (iLine = iLineStart; iLine <= iLineEnd; iLine++)
    {
      int iCommentPos;
      int iIndentPos = SendMessage(hwnd,SCI_GETLINEINDENTPOSITION,(WPARAM)iLine,0);
      char tchBuf[32];
      struct TextRange tr;

      if (iIndentPos == SendMessage(hwnd,SCI_GETLINEENDPOSITION,(WPARAM)iLine,0))
        continue;

      tr.chrg.cpMin = iIndentPos;
      tr.chrg.cpMax = tr.chrg.cpMin + min(31,cchComment);
      tr.lpstrText = tchBuf;
      SendMessage(hwnd,SCI_GETTEXTRANGE,0,(LPARAM)&tr);

      if (StrCmpNIA(tchBuf,mszComment,cchComment) == 0) {
        switch (iAction) {
          case 0:
            iAction = 2;
          case 2:
            SendMessage(hwnd,SCI_SETTARGETSTART,(WPARAM)iIndentPos,0);
            SendMessage(hwnd,SCI_SETTARGETEND,(WPARAM)iIndentPos+cchComment,0);
            SendMessage(hwnd,SCI_REPLACETARGET,0,(LPARAM)"");
            break;
          case 1:
            break;
        }
      }
      else {
        switch (iAction) {
          case 0:
            iAction = 1;
          case 1:
            iCommentPos = SendMessage(hwnd,SCI_FINDCOLUMN,(WPARAM)iLine,(LPARAM)iCommentCol);
            SendMessage(hwnd,SCI_INSERTTEXT,(WPARAM)iCommentPos,(LPARAM)mszComment);
            break;
          case 2:
            break;
        }
      }
    }
    SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);

    // extend selection to start of first line
    // the above code is not required when last line has been excluded
    if (iSelStart != iSelEnd)
    {
      int iAnchorPos;
      if (iCurPos == iSelStart) {
        iCurPos = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineStart,0);
        iAnchorPos = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineEnd+1,0);
      }
      else {
        iAnchorPos = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineStart,0);
        iCurPos = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineEnd+1,0);
      }
      SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
    }

  }
  else
    MsgBox(MBINFO,IDS_SELRECT);
}


//=============================================================================
//
//  EditStripFirstCharacter()
//
void EditStripFirstCharacter(HWND hwnd)
{
  int iSelStart = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);
  int iSelEnd   = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0);

  if (iSelStart == iSelEnd) {
    iSelStart = 0;
    iSelEnd   = SendMessage(hwnd,SCI_GETLENGTH,0,0);
  }

  if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0))
  {
    int iLine;

    int iLineStart = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelStart,0);
    int iLineEnd   = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelEnd,0);

    if (iSelStart > SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineStart,0))
      iLineStart++;

    if (iSelEnd <= SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineEnd,0))
      iLineEnd--;

    SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);

    for (iLine = iLineStart; iLine <= iLineEnd; iLine++)
    {
      int iPos = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLine,0);
      if (SendMessage(hwnd,SCI_GETLINEENDPOSITION,(WPARAM)iLine,0)- iPos > 0)
      {
        SendMessage(hwnd,SCI_SETTARGETSTART,(WPARAM)iPos,0);
        SendMessage(hwnd,SCI_SETTARGETEND,
          (WPARAM)SendMessage(hwnd,SCI_POSITIONAFTER,(WPARAM)iPos,0),0);
        SendMessage(hwnd,SCI_REPLACETARGET,0,(LPARAM)"");
      }
    }
    SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);
  }
  else
    MsgBox(MBINFO,IDS_SELRECT);
}


//=============================================================================
//
//  EditStripLastCharacter()
//
void EditStripLastCharacter(HWND hwnd)
{
  int iSelStart = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);
  int iSelEnd   = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0);

  if (iSelStart == iSelEnd) {
    iSelStart = 0;
    iSelEnd   = SendMessage(hwnd,SCI_GETLENGTH,0,0);
  }

  if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0))
  {
    int iLine;

    int iLineStart = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelStart,0);
    int iLineEnd   = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelEnd,0);

    if (iSelStart >= SendMessage(hwnd,SCI_GETLINEENDPOSITION,(WPARAM)iLineStart,0))
      iLineStart++;

    if (iSelEnd < SendMessage(hwnd,SCI_GETLINEENDPOSITION,(WPARAM)iLineEnd,0))
      iLineEnd--;

    SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);

    for (iLine = iLineStart; iLine <= iLineEnd; iLine++)
    {
      int iStartPos = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLine,0);
      int iEndPos   = SendMessage(hwnd,SCI_GETLINEENDPOSITION,(WPARAM)iLine,0);
      if (iEndPos - iStartPos > 0)
      {
        SendMessage(hwnd,SCI_SETTARGETSTART,
          (WPARAM)SendMessage(hwnd,SCI_POSITIONBEFORE,(WPARAM)iEndPos,0),0);
        SendMessage(hwnd,SCI_SETTARGETEND,(WPARAM)iEndPos,0);
        SendMessage(hwnd,SCI_REPLACETARGET,0,(LPARAM)"");
      }
    }
    SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);
  }
  else
    MsgBox(MBINFO,IDS_SELRECT);
}


//=============================================================================
//
//  EditStripTrailingBlanks()
//
void EditStripTrailingBlanks(HWND hwnd)
{
  // Check if there is any selection... simply use a regular expression replace!
  if (SendMessage(hwnd,SCI_GETSELECTIONEND,0,0) - SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0))
  {
    if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0))
    {
      EDITFINDREPLACE efrTrim = { "[ \t]+$", "", "", "", SCFIND_REGEXP, 0, 0, 0, 0, 0, hwnd };
      EditReplaceAllInSelection(hwnd,&efrTrim,FALSE);
    }
    else
      MsgBox(MBINFO,IDS_SELRECT);
  }
  // Code from SciTE...
  else
  {
    int line;
    int maxLines;
    int lineStart;
    int lineEnd;
    int i;
    char ch;

    SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);
    maxLines = SendMessage(hwnd,SCI_GETLINECOUNT,0,0);
    for (line = 0; line < maxLines; line++)
    {
      lineStart = SendMessage(hwnd,SCI_POSITIONFROMLINE,line,0);
      lineEnd = SendMessage(hwnd,SCI_GETLINEENDPOSITION,line,0);
      i = lineEnd-1;
      ch = (char)SendMessage(hwnd,SCI_GETCHARAT,i,0);
      while ((i >= lineStart) && ((ch == ' ') || (ch == '\t')))
      {
        i--;
        ch = (char)SendMessage(hwnd,SCI_GETCHARAT,i,0);
      }
      if (i < (lineEnd-1))
      {
        SendMessage(hwnd,SCI_SETTARGETSTART,i + 1,0);
        SendMessage(hwnd,SCI_SETTARGETEND,lineEnd,0);
        SendMessage(hwnd,SCI_REPLACETARGET,0,(LPARAM)"");
      }
    }
    SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);
  }
}


//=============================================================================
//
//  EditCompressSpaces()
//
void EditCompressSpaces(HWND hwnd)
{
  if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0))
  {
    int iSelStart  = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);
    int iSelEnd    = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0);
    int iCurPos    = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
    int iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);
    int iLineStart = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelStart,0);
    int iLineEnd   = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelEnd,0);
    int iLength    = SendMessage(hwnd,SCI_GETLENGTH,0,0);

    char* pszIn;
    char* pszOut;
    BOOL bIsLineStart, bIsLineEnd;

    if (iSelStart != iSelEnd) {
      int cch = SendMessage(hwnd,SCI_GETSELTEXT,0,0);
      pszIn = LocalAlloc(LPTR,cch);
      pszOut = LocalAlloc(LPTR,cch);
      SendMessage(hwnd,SCI_GETSELTEXT,0,(LPARAM)pszIn);
      bIsLineStart =
        (iSelStart == SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineStart,0));
      bIsLineEnd =
        (iSelEnd == SendMessage(hwnd,SCI_GETLINEENDPOSITION,(WPARAM)iLineEnd,0));
    }
    else {
      int cch = iLength + 1;
      pszIn = LocalAlloc(GPTR,cch);
      pszOut = LocalAlloc(GPTR,cch);
      SendMessage(hwnd,SCI_GETTEXT,(WPARAM)cch,(LPARAM)pszIn);
      bIsLineStart = TRUE;
      bIsLineEnd   = TRUE;
    }

    if (pszIn && pszOut) {
      char *ci, *co = pszOut;
      for (ci = pszIn; *ci; ci++) {
        if (*ci == ' ' || *ci == '\t') {
          while (*(ci+1) == ' ' || *(ci+1) == '\t')
            ci++;
          if (!bIsLineStart && (*(ci+1) != '\n' && *(ci+1) != '\r'))
            *co++ = ' ';
        }
        else {
          if (*ci == '\n' || *ci == '\r')
            bIsLineStart = TRUE;
          else
            bIsLineStart = FALSE;
          *co++ = *ci;
        }
      }
      if (bIsLineEnd && co > pszOut && *(co-1) == ' ')
        *--co = 0;

      if (iSelStart != iSelEnd)
        SendMessage(hwnd,SCI_TARGETFROMSELECTION,0,0);
      else {
        SendMessage(hwnd,SCI_SETTARGETSTART,0,0);
        SendMessage(hwnd,SCI_SETTARGETEND,(WPARAM)iLength,0);
      }
      SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);
      SendMessage(hwnd,SCI_REPLACETARGET,(WPARAM)-1,(LPARAM)pszOut);
      if (iCurPos > iAnchorPos) {
        iCurPos    = SendMessage(hwnd,SCI_GETTARGETEND,0,0);
        iAnchorPos = SendMessage(hwnd,SCI_GETTARGETSTART,0,0);
        SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
      }
      else if (iCurPos < iAnchorPos) {
        iCurPos    = SendMessage(hwnd,SCI_GETTARGETSTART,0,0);
        iAnchorPos = SendMessage(hwnd,SCI_GETTARGETEND,0,0);
        SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
      }
      SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);
    }
    if (pszIn)
      LocalFree(pszIn);
    if (pszOut)
      LocalFree(pszOut);
  }
  else
    MsgBox(MBINFO,IDS_SELRECT);
}


//=============================================================================
//
//  EditRemoveBlankLines()
//
void EditRemoveBlankLines(HWND hwnd)
{
  int iSelStart = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);
  int iSelEnd   = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0);

  if (iSelStart == iSelEnd) {
    iSelStart = 0;
    iSelEnd   = SendMessage(hwnd,SCI_GETLENGTH,0,0);
  }

  if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0))
  {
    int iLine;

    int iLineStart = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelStart,0);
    int iLineEnd   = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelEnd,0);

    if (iSelStart > SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineStart,0))
      iLineStart++;

    if (iSelEnd <= SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLineEnd,0))
      iLineEnd--;

    SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);

    for (iLine = iLineStart; iLine <= iLineEnd; )
    {
      int iPos = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLine,0);
      if (SendMessage(hwnd,SCI_GETLINEENDPOSITION,(WPARAM)iLine,0) == iPos)
      {
        int iPos2 = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLine+1,0);
        SendMessage(hwnd,SCI_SETTARGETSTART,(WPARAM)iPos,0);
        SendMessage(hwnd,SCI_SETTARGETEND,(WPARAM)iPos2,0);
        SendMessage(hwnd,SCI_REPLACETARGET,0,(LPARAM)"");
        iLineEnd--;
      }
      else
        iLine++;
    }
    SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);
  }
  else
    MsgBox(MBINFO,IDS_SELRECT);
}


//=============================================================================
//
//  EditWrapToColumn()
//
void EditWrapToColumn(HWND hwnd,int nColumn/*,int nTabWidth*/)
{
  char* pszText;
  LPWSTR pszTextW;
  int cchTextW;
  int iTextW;
  LPWSTR pszConvW;
  int cchConvW;
  int cchConvM;
  int iLineLength;
  int iLine;
  int iCurPos;
  int iAnchorPos;
  int iSelStart;
  int iSelEnd;
  int iSelCount;
  UINT cpEdit;
  struct TextRange tr;
  int   cEOLMode;
  WCHAR wszEOL[] = L"\r\n";
  int   cchEOL = 2;
  BOOL bModified = FALSE;

  if (SC_SEL_RECTANGLE == SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0)) {
    MsgBox(MBINFO,IDS_SELRECT);
    return;
  }

  iCurPos    = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
  iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);

  if (iCurPos == iAnchorPos) /*{
    iSelStart = 0;
    iSelEnd   = SendMessage(hwnd,SCI_GETLENGTH,0,0);
  }*/
    return;

  else {
    iSelStart = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);
    iSelEnd   = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0);
  }

  iLine = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelStart,0);
  iSelStart = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLine,0);

  iSelCount = iSelEnd - iSelStart;

  pszText = GlobalAlloc(GPTR,(iSelCount)+2);
  if (pszText == NULL)
    return;

  pszTextW = GlobalAlloc(GPTR,(iSelCount*2)+2);
  if (pszTextW == NULL) {
    GlobalFree(pszText);
    return;
  }

  tr.chrg.cpMin = iSelStart;
  tr.chrg.cpMax = iSelEnd;
  tr.lpstrText = pszText;
  SendMessage(hwnd,SCI_GETTEXTRANGE,0,(LPARAM)&tr);

  cpEdit = SendMessage(hwnd,SCI_GETCODEPAGE,0,0);

  cchTextW = MultiByteToWideChar(cpEdit,0,pszText,iSelCount,pszTextW,GlobalSize(pszTextW)/sizeof(WCHAR));
  GlobalFree(pszText);

  pszConvW = GlobalAlloc(GPTR,cchTextW*sizeof(WCHAR)*3+2);
  if (pszConvW == NULL) {
    GlobalFree(pszTextW);
    return;
  }

  cEOLMode = SendMessage(hwnd,SCI_GETEOLMODE,0,0);
  if (cEOLMode == SC_EOL_CR)
    cchEOL = 1;
  else if (cEOLMode == SC_EOL_LF) {
    cchEOL = 1; wszEOL[0] = L'\n';
  }

  cchConvW = 0;
  iLineLength = 0;

#define ISDELIMITER(wc) StrChr(L",;.:-+%&¦|/*?!\"\'~´#=",wc)
#define ISWHITE(wc) StrChr(L" \t",wc)
#define ISWORDEND(wc) (/*ISDELIMITER(wc) ||*/ StrChr(L" \t\r\n",wc))

  for (iTextW = 0; iTextW < cchTextW; iTextW++)
  {
    WCHAR w;
    w = pszTextW[iTextW];

    //if (ISDELIMITER(w))
    //{
    //  int iNextWordLen = 0;
    //  WCHAR w2 = pszTextW[iTextW + 1];

    //  if (iLineLength + iNextWordLen + 1 > nColumn) {
    //    pszConvW[cchConvW++] = wszEOL[0];
    //    if (cchEOL > 1)
    //      pszConvW[cchConvW++] = wszEOL[1];
    //    iLineLength = 0;
    //    bModified = TRUE;
    //  }

    //  while (w2 != L'\0' && !ISWORDEND(w2)) {
    //    iNextWordLen++;
    //    w2 = pszTextW[iTextW + iNextWordLen + 1];
    //  }

    //  if (ISDELIMITER(w2) && iNextWordLen > 0) // delimiters go with the word
    //    iNextWordLen++;

    //  pszConvW[cchConvW++] = w;
    //  iLineLength++;

    //  if (iNextWordLen > 0)
    //  {
    //    if (iLineLength + iNextWordLen + 1 > nColumn) {
    //      pszConvW[cchConvW++] = wszEOL[0];
    //      if (cchEOL > 1)
    //        pszConvW[cchConvW++] = wszEOL[1];
    //      iLineLength = 0;
    //      bModified = TRUE;
    //    }
    //  }
    //}

    if (ISWHITE(w))
    {
      int iNextWordLen = 0;
      WCHAR w2;

      while (pszTextW[iTextW+1] == L' ' || pszTextW[iTextW+1] == L'\t') {
        iTextW++;
        bModified = TRUE;
      } // Modified: left out some whitespaces

      w2 = pszTextW[iTextW + 1];

      while (w2 != L'\0' && !ISWORDEND(w2)) {
        iNextWordLen++;
        w2 = pszTextW[iTextW + iNextWordLen + 1];
      }

      //if (ISDELIMITER(w2) /*&& iNextWordLen > 0*/) // delimiters go with the word
      //  iNextWordLen++;

      if (iNextWordLen > 0)
      {
        if (iLineLength + iNextWordLen + 1 > nColumn) {
          pszConvW[cchConvW++] = wszEOL[0];
          if (cchEOL > 1)
            pszConvW[cchConvW++] = wszEOL[1];
          iLineLength = 0;
          bModified = TRUE;
        }
        else {
          if (iLineLength > 0) {
            pszConvW[cchConvW++] = L' ';
            iLineLength++;
          }
        }
      }
    }
    else
    {
      pszConvW[cchConvW++] = w;
      if (w == L'\r' || w == L'\n') {
        iLineLength = 0;
      }
      else {
        iLineLength++;
      }
    }
  }

  GlobalFree(pszTextW);

  if (bModified) {
    pszText = GlobalAlloc(GPTR,cchConvW * 3);

    cchConvM = WideCharToMultiByte(cpEdit,0,pszConvW,cchConvW,pszText,GlobalSize(pszText),NULL,NULL);
    GlobalFree(pszConvW);

    if (iAnchorPos > iCurPos) {
      iCurPos = iSelStart;
      iAnchorPos = iSelStart + cchConvM;
    }
    else {
      iAnchorPos = iSelStart;
      iCurPos = iSelStart + cchConvM;
    }

    SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);
    SendMessage(hwnd,SCI_SETTARGETSTART,(WPARAM)iSelStart,0);
    SendMessage(hwnd,SCI_SETTARGETEND,(WPARAM)iSelEnd,0);
    SendMessage(hwnd,SCI_REPLACETARGET,(WPARAM)cchConvM,(LPARAM)pszText);
    //SendMessage(hwnd,SCI_CLEAR,0,0);
    //SendMessage(hwnd,SCI_ADDTEXT,(WPARAM)cchConvW,(LPARAM)pszText);
    SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
    SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);

    GlobalFree(pszText);
  }

  else
    GlobalFree(pszConvW);
}


//=============================================================================
//
//  EditJoinLinesEx()
//
void EditJoinLinesEx(HWND hwnd)
{
  char* pszText;
  char* pszJoin;
  int   cchJoin = 0;
  int i;
  int iLine;
  int iCurPos;
  int iAnchorPos;
  int iSelStart;
  int iSelEnd;
  int iSelCount;
  struct TextRange tr;
  int  cEOLMode;
  char szEOL[] = "\r\n";
  int  cchEOL = 2;
  BOOL bModified = FALSE;

  if (SC_SEL_RECTANGLE == SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0)) {
    MsgBox(MBINFO,IDS_SELRECT);
    return;
  }

  iCurPos    = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
  iAnchorPos = SendMessage(hwnd,SCI_GETANCHOR,0,0);

  if (iCurPos == iAnchorPos)
    return;

  else {
    iSelStart = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);
    iSelEnd   = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0);
  }

  iLine = SendMessage(hwnd,SCI_LINEFROMPOSITION,(WPARAM)iSelStart,0);
  iSelStart = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iLine,0);

  iSelCount = iSelEnd - iSelStart;

  pszText = LocalAlloc(LPTR,(iSelCount)+2);
  if (pszText == NULL)
    return;

  pszJoin = LocalAlloc(LPTR,LocalSize(pszText));
  if (pszJoin == NULL) {
    LocalFree(pszText);
    return;
  }

  tr.chrg.cpMin = iSelStart;
  tr.chrg.cpMax = iSelEnd;
  tr.lpstrText = pszText;
  SendMessage(hwnd,SCI_GETTEXTRANGE,0,(LPARAM)&tr);

  cEOLMode = SendMessage(hwnd,SCI_GETEOLMODE,0,0);
  if (cEOLMode == SC_EOL_CR)
    cchEOL = 1;
  else if (cEOLMode == SC_EOL_LF) {
    cchEOL = 1;
    szEOL[0] = '\n';
  }

  cchJoin = 0;
  for (i = 0; i < iSelCount; i++)
  {
    if (pszText[i] == '\r' || pszText[i] == '\n') {
      if (pszText[i] == '\r' && pszText[i+1] == '\n')
        i++;
      if (!StrChrA("\r\n",pszText[i+1]) && pszText[i+1] != 0) {
        pszJoin[cchJoin++] = ' ';
        bModified = TRUE;
      }
      else {
        while (StrChrA("\r\n",pszText[i+1])) {
          i++;
          bModified = TRUE;
        }
        if (pszText[i+1] != 0) {
          pszJoin[cchJoin++] = szEOL[0];
          if (cchEOL > 1)
            pszJoin[cchJoin++] = szEOL[1];
          if (cchJoin > cchEOL) {
            pszJoin[cchJoin++] = szEOL[0];
            if (cchEOL > 1)
              pszJoin[cchJoin++] = szEOL[1];
          }
        }
      }
    }
    else {
      pszJoin[cchJoin++] = pszText[i];
    }
  }

  LocalFree(pszText);

  if (bModified) {
    if (iAnchorPos > iCurPos) {
      iCurPos = iSelStart;
      iAnchorPos = iSelStart + cchJoin;
    }
    else {
      iAnchorPos = iSelStart;
      iCurPos = iSelStart + cchJoin;
    }

    SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);
    SendMessage(hwnd,SCI_SETTARGETSTART,(WPARAM)iSelStart,0);
    SendMessage(hwnd,SCI_SETTARGETEND,(WPARAM)iSelEnd,0);
    SendMessage(hwnd,SCI_REPLACETARGET,(WPARAM)cchJoin,(LPARAM)pszJoin);
    SendMessage(hwnd,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
    SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);
  }

  LocalFree(pszJoin);
}


//=============================================================================
//
//  EditJumpTo()
//
void EditJumpTo(HWND hwnd,int iNewLine,int iNewCol)
{
  int iMaxLine = SendMessage(hwnd,SCI_GETLINECOUNT,0,0);

  // Jumpt to end with line set to -1
  if (iNewLine == -1) {
    SendMessage(hwnd,SCI_DOCUMENTEND,0,0);
    return;
  }

  // Line maximum is iMaxLine
  iNewLine = min(iNewLine,iMaxLine);

  // Column minimum is 1
  iNewCol = max(iNewCol,1);

  if (iNewLine > 0 && iNewLine <= iMaxLine && iNewCol > 0)
  {
    int iNewPos  = SendMessage(hwnd,SCI_POSITIONFROMLINE,(WPARAM)iNewLine-1,0);
    int iLineEndPos = SendMessage(hwnd,SCI_GETLINEENDPOSITION,(WPARAM)iNewLine-1,0);

    while (iNewCol-1 > SendMessage(hwnd,SCI_GETCOLUMN,(WPARAM)iNewPos,0))
    {
      if (iNewPos >= iLineEndPos)
        break;

      iNewPos = SendMessage(hwnd,SCI_POSITIONAFTER,(WPARAM)iNewPos,0);
    }

    iNewPos = min(iNewPos,iLineEndPos);
    SendMessage(hwnd,SCI_GOTOPOS,(WPARAM)iNewPos,0);
    SendMessage(hwnd,SCI_CHOOSECARETX,0,0);
  }
}


//=============================================================================
//
//  EditSelectEx()
//
void EditSelectEx(HWND hwnd,int iAnchorPos,int iCurrentPos)
{
  SendMessage(hwnd,SCI_SETXCARETPOLICY,CARET_SLOP|CARET_STRICT|CARET_EVEN,50);
  SendMessage(hwnd,SCI_SETSEL,iAnchorPos,iCurrentPos);
  SendMessage(hwnd,SCI_SETXCARETPOLICY,CARET_SLOP|CARET_EVEN,50);
}


//=============================================================================
//
//  EditFindReplaceDlgProcW()
//
BOOL CALLBACK EditFindReplaceDlgProcW(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  LPEDITFINDREPLACE lpefr;
  int i;
  WCHAR tch[512+32];
  BOOL bCloseDlg;
  BOOL bIsFindDlg;

  static UINT uCPEdit;

  switch(umsg)
  {

    case WM_INITDIALOG:
      {
        int cchSelection;
        char *lpszSelection;
        char *lpsz;

        SetWindowLongPtr(hwnd,DWLP_USER,lParam);
        lpefr = (LPEDITFINDREPLACE)lParam;

        // Get the current code page for Unicode conversion
        uCPEdit = SendMessage(lpefr->hwnd,SCI_GETCODEPAGE,0,0);

        // Load MRUs
        for (i = 0; i < MRU_Enum(mruFind,0,NULL,0); i++) {
          MRU_Enum(mruFind,i,tch,COUNTOF(tch));
          SendDlgItemMessage(hwnd,IDC_FINDTEXT,CB_ADDSTRING,0,(LPARAM)tch);
        }
        for (i = 0; i < MRU_Enum(mruReplace,0,NULL,0); i++) {
          MRU_Enum(mruReplace,i,tch,COUNTOF(tch));
          SendDlgItemMessage(hwnd,IDC_REPLACETEXT,CB_ADDSTRING,0,(LPARAM)tch);
        }

        if (!bSwitchedFindReplace) {
          cchSelection = SendMessage(lpefr->hwnd,SCI_GETSELECTIONEND,0,0) -
                         SendMessage(lpefr->hwnd,SCI_GETSELECTIONSTART,0,0);

          if (cchSelection <= 500)
          {
            lpszSelection = GlobalAlloc(GPTR,cchSelection+2);
            SendMessage(lpefr->hwnd,SCI_GETSELTEXT,0,(LPARAM)lpszSelection);

            // Check lpszSelection and truncate bad chars
            lpsz = StrChrA(lpszSelection,13);
            if (lpsz) *lpsz = '\0';

            lpsz = StrChrA(lpszSelection,10);
            if (lpsz) *lpsz = '\0';

            lpsz = StrChrA(lpszSelection,9);
            if (lpsz) *lpsz = '\0';

            SetDlgItemTextA2W(uCPEdit,hwnd,IDC_FINDTEXT,lpszSelection);
            GlobalFree(lpszSelection);
          }
        }

        SendDlgItemMessage(hwnd,IDC_FINDTEXT,CB_LIMITTEXT,500,0);
        SendDlgItemMessage(hwnd,IDC_FINDTEXT,CB_SETEXTENDEDUI,TRUE,0);

        if (!GetWindowTextLengthW(GetDlgItem(hwnd,IDC_FINDTEXT)))
          SetDlgItemTextA2W(CP_UTF8,hwnd,IDC_FINDTEXT,lpefr->szFindUTF8);

        if (GetDlgItem(hwnd,IDC_REPLACETEXT))
        {
          SendDlgItemMessage(hwnd,IDC_REPLACETEXT,CB_LIMITTEXT,500,0);
          SendDlgItemMessage(hwnd,IDC_REPLACETEXT,CB_SETEXTENDEDUI,TRUE,0);
          SetDlgItemTextA2W(CP_UTF8,hwnd,IDC_REPLACETEXT,lpefr->szReplaceUTF8);
        }

        if (lpefr->fuFlags & SCFIND_MATCHCASE)
          CheckDlgButton(hwnd,IDC_FINDCASE,BST_CHECKED);

        if (lpefr->fuFlags & SCFIND_WHOLEWORD)
          CheckDlgButton(hwnd,IDC_FINDWORD,BST_CHECKED);

        if (lpefr->fuFlags & SCFIND_WORDSTART)
          CheckDlgButton(hwnd,IDC_FINDSTART,BST_CHECKED);

        if (lpefr->fuFlags & SCFIND_REGEXP)
          CheckDlgButton(hwnd,IDC_FINDREGEXP,BST_CHECKED);

        if (lpefr->bTransformBS)
          CheckDlgButton(hwnd,IDC_FINDTRANSFORMBS,BST_CHECKED);

        if (lpefr->bNoFindWrap)
          CheckDlgButton(hwnd,IDC_NOWRAP,BST_CHECKED);

        if (lpefr->bFindUp)
          CheckDlgButton(hwnd,IDC_FINDUP,BST_CHECKED);

        if (GetDlgItem(hwnd,IDC_REPLACE)) {
          if (bSwitchedFindReplace) {
            if (lpefr->bFindClose)
              CheckDlgButton(hwnd,IDC_FINDCLOSE,BST_CHECKED);
          }
          else {
            if (lpefr->bReplaceClose)
              CheckDlgButton(hwnd,IDC_FINDCLOSE,BST_CHECKED);
          }
        }
        else {
          if (bSwitchedFindReplace) {
            if (lpefr->bReplaceClose)
              CheckDlgButton(hwnd,IDC_FINDCLOSE,BST_CHECKED);
          }
          else {
            if (lpefr->bFindClose)
              CheckDlgButton(hwnd,IDC_FINDCLOSE,BST_CHECKED);
          }
        }

        if (!bSwitchedFindReplace) {
          if (xFindReplaceDlg == 0 || yFindReplaceDlg == 0)
            CenterDlgInParent(hwnd);
          else
            SetDlgPos(hwnd,xFindReplaceDlg,yFindReplaceDlg);
        }

        else {
          SetDlgPos(hwnd,xFindReplaceDlgSave,yFindReplaceDlgSave);
          bSwitchedFindReplace = FALSE;
          CopyMemory(lpefr,&efrSave,sizeof(EDITFINDREPLACE));
        }
      }
      return TRUE;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case IDC_FINDTEXT:
        case IDC_REPLACETEXT:
          {
            BOOL bEnable = (GetWindowTextLengthW(GetDlgItem(hwnd,IDC_FINDTEXT)) ||
                            CB_ERR != SendDlgItemMessage(hwnd,IDC_FINDTEXT,CB_GETCURSEL,0,0));

            EnableWindow(GetDlgItem(hwnd,IDOK),bEnable);
            EnableWindow(GetDlgItem(hwnd,IDC_SELECTTO),bEnable);
            EnableWindow(GetDlgItem(hwnd,IDC_REPLACE),bEnable);
            EnableWindow(GetDlgItem(hwnd,IDC_REPLACEALL),bEnable);
            EnableWindow(GetDlgItem(hwnd,IDC_REPLACEINSEL),bEnable);

            if (HIWORD(wParam) == CBN_CLOSEUP) {
              LONG lSelEnd;
              SendDlgItemMessage(hwnd,LOWORD(wParam),CB_GETEDITSEL,0,(LPARAM)&lSelEnd);
              SendDlgItemMessage(hwnd,LOWORD(wParam),CB_SETEDITSEL,0,MAKELPARAM(lSelEnd,lSelEnd));
            }
          }
          break;

        case IDC_FINDREGEXP:
          if (IsDlgButtonChecked(hwnd,IDC_FINDREGEXP) == BST_CHECKED)
            CheckDlgButton(hwnd,IDC_FINDTRANSFORMBS,BST_UNCHECKED);
          break;

        case IDC_FINDTRANSFORMBS:
          if (IsDlgButtonChecked(hwnd,IDC_FINDTRANSFORMBS) == BST_CHECKED)
            CheckDlgButton(hwnd,IDC_FINDREGEXP,BST_UNCHECKED);
          break;

        case IDOK:
        case IDC_SELECTTO:
        case IDC_REPLACE:
        case IDC_REPLACEALL:
        case IDC_REPLACEINSEL:
        case IDMSG_SWITCHTOFIND:
        case IDMSG_SWITCHTOREPLACE:

          lpefr = (LPEDITFINDREPLACE) GetWindowLongPtr(hwnd,DWLP_USER);

          bIsFindDlg = (GetDlgItem(hwnd,IDC_REPLACE) == NULL);

          if ((bIsFindDlg && LOWORD(wParam) == IDMSG_SWITCHTOREPLACE ||
              !bIsFindDlg && LOWORD(wParam) == IDMSG_SWITCHTOFIND)) {
            GetDlgPos(hwnd,&xFindReplaceDlgSave,&yFindReplaceDlgSave);
            bSwitchedFindReplace = TRUE;
            CopyMemory(&efrSave,lpefr,sizeof(EDITFINDREPLACE));
          }

          // Get current code page for Unicode conversion
          uCPEdit = SendMessage(lpefr->hwnd,SCI_GETCODEPAGE,0,0);
          cpLastFind = uCPEdit;

          if (!bSwitchedFindReplace &&
              !GetDlgItemTextA2W(uCPEdit,hwnd,IDC_FINDTEXT,lpefr->szFind,COUNTOF(lpefr->szFind))) {

            EnableWindow(GetDlgItem(hwnd,IDOK),FALSE);
            EnableWindow(GetDlgItem(hwnd,IDC_SELECTTO),FALSE);
            EnableWindow(GetDlgItem(hwnd,IDC_REPLACE),FALSE);
            EnableWindow(GetDlgItem(hwnd,IDC_REPLACEALL),FALSE);
            EnableWindow(GetDlgItem(hwnd,IDC_REPLACEINSEL),FALSE);
            return TRUE;
          }

          if (GetDlgItem(hwnd,IDC_REPLACETEXT))
            GetDlgItemTextA2W(uCPEdit,hwnd,IDC_REPLACETEXT,lpefr->szReplace,COUNTOF(lpefr->szReplace));

          lpefr->fuFlags = 0;

          if (IsDlgButtonChecked(hwnd,IDC_FINDCASE) == BST_CHECKED)
            lpefr->fuFlags |= SCFIND_MATCHCASE;

          if (IsDlgButtonChecked(hwnd,IDC_FINDWORD) == BST_CHECKED)
            lpefr->fuFlags |= SCFIND_WHOLEWORD;

          if (IsDlgButtonChecked(hwnd,IDC_FINDSTART) == BST_CHECKED)
            lpefr->fuFlags |= SCFIND_WORDSTART;

          if (IsDlgButtonChecked(hwnd,IDC_FINDREGEXP) == BST_CHECKED)
            lpefr->fuFlags |= SCFIND_REGEXP;

          lpefr->bTransformBS =
            (IsDlgButtonChecked(hwnd,IDC_FINDTRANSFORMBS) == BST_CHECKED) ? TRUE : FALSE;

          lpefr->bNoFindWrap = (IsDlgButtonChecked(hwnd,IDC_NOWRAP) == BST_CHECKED) ? TRUE : FALSE;

          lpefr->bFindUp = (IsDlgButtonChecked(hwnd,IDC_FINDUP) == BST_CHECKED) ? TRUE : FALSE;

          if (bIsFindDlg) {
            lpefr->bFindClose = (IsDlgButtonChecked(hwnd,IDC_FINDCLOSE) == BST_CHECKED) ? TRUE : FALSE;
          }
          else {
            lpefr->bReplaceClose = (IsDlgButtonChecked(hwnd,IDC_FINDCLOSE) == BST_CHECKED) ? TRUE : FALSE;
          }

          if (!bSwitchedFindReplace) {
            // Save MRUs
            if (lstrlenA(lpefr->szFind)) {
              if (GetDlgItemTextA2W(CP_UTF8,hwnd,IDC_FINDTEXT,lpefr->szFindUTF8,COUNTOF(lpefr->szFindUTF8))) {
                GetDlgItemText(hwnd,IDC_FINDTEXT,tch,COUNTOF(tch));
                MRU_Add(mruFind,tch);
              }
            }
            if (lstrlenA(lpefr->szReplace)) {
              if (GetDlgItemTextA2W(CP_UTF8,hwnd,IDC_REPLACETEXT,lpefr->szReplaceUTF8,COUNTOF(lpefr->szReplaceUTF8))) {
                GetDlgItemText(hwnd,IDC_REPLACETEXT,tch,COUNTOF(tch));
                MRU_Add(mruReplace,tch);
              }
            }
            else
              lstrcpyA(lpefr->szReplaceUTF8,"");
          }
          else {
            GetDlgItemTextA2W(CP_UTF8,hwnd,IDC_FINDTEXT,lpefr->szFindUTF8,COUNTOF(lpefr->szFindUTF8));
            if (!GetDlgItemTextA2W(CP_UTF8,hwnd,IDC_REPLACETEXT,lpefr->szReplaceUTF8,COUNTOF(lpefr->szReplaceUTF8)))
              lstrcpyA(lpefr->szReplaceUTF8,"");
          }

          if (bIsFindDlg) {
            bCloseDlg = lpefr->bFindClose;
          }
          else {
            if (LOWORD(wParam) == IDOK)
              bCloseDlg = FALSE;
            else
              bCloseDlg = lpefr->bReplaceClose;
          }

          // Reload MRUs
          SendDlgItemMessage(hwnd,IDC_FINDTEXT,CB_RESETCONTENT,0,0);
          SendDlgItemMessage(hwnd,IDC_REPLACETEXT,CB_RESETCONTENT,0,0);

          for (i = 0; i < MRU_Enum(mruFind,0,NULL,0); i++) {
            MRU_Enum(mruFind,i,tch,COUNTOF(tch));
            SendDlgItemMessage(hwnd,IDC_FINDTEXT,CB_ADDSTRING,0,(LPARAM)tch);
          }
          for (i = 0; i < MRU_Enum(mruReplace,0,NULL,0); i++) {
            MRU_Enum(mruReplace,i,tch,COUNTOF(tch));
            SendDlgItemMessage(hwnd,IDC_REPLACETEXT,CB_ADDSTRING,0,(LPARAM)tch);
          }

          SetDlgItemTextA2W(CP_UTF8,hwnd,IDC_FINDTEXT,lpefr->szFindUTF8);
          SetDlgItemTextA2W(CP_UTF8,hwnd,IDC_REPLACETEXT,lpefr->szReplaceUTF8);

          if (!bSwitchedFindReplace)
            SendMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetFocus()),1);

          if (bCloseDlg) {
            //EndDialog(hwnd,LOWORD(wParam));
            DestroyWindow(hwnd);
            hDlgFindReplace = NULL;
          }

          switch (LOWORD(wParam))
          {
            case IDOK: // find
              if (!lpefr->bFindUp)
                EditFindNext(lpefr->hwnd,lpefr,FALSE);
              else
                EditFindPrev(lpefr->hwnd,lpefr,FALSE);
              break;

            case IDC_SELECTTO: // select to
              if (!lpefr->bFindUp)
                EditFindNext(lpefr->hwnd,lpefr,TRUE);
              else
                EditFindPrev(lpefr->hwnd,lpefr,TRUE);
              break;

            case IDC_REPLACE:
              EditReplace(lpefr->hwnd,lpefr);
              break;

            case IDC_REPLACEALL:
              EditReplaceAll(lpefr->hwnd,lpefr,TRUE);
              break;

            case IDC_REPLACEINSEL:
              EditReplaceAllInSelection(lpefr->hwnd,lpefr,TRUE);
              break;
          }
          break;


        case IDCANCEL:

          //EndDialog(hwnd,IDCANCEL);
          DestroyWindow(hwnd);
          break;

        case IDACC_FIND:
          PostMessage(GetParent(hwnd),WM_COMMAND,MAKELONG(IDM_EDIT_FIND,1),0);
          break;

        case IDACC_REPLACE:
          PostMessage(GetParent(hwnd),WM_COMMAND,MAKELONG(IDM_EDIT_REPLACE,1),0);
          break;

        case IDACC_SAVEPOS:
          GetDlgPos(hwnd,&xFindReplaceDlg,&yFindReplaceDlg);
          break;

        case IDACC_RESETPOS:
          CenterDlgInParent(hwnd);
          xFindReplaceDlg = yFindReplaceDlg = 0;
          break;

      }

      return TRUE;

  }

  return FALSE;

}


//=============================================================================
//
//  EditFindReplaceDlg()
//
HWND EditFindReplaceDlg(HWND hwnd,LPCEDITFINDREPLACE lpefr,BOOL bReplace)
{

  HWND hDlg;

  lpefr->hwnd = hwnd;

  hDlg = CreateDialogParamW(g_hInstance,
            (bReplace) ? MAKEINTRESOURCEW(IDD_REPLACE) : MAKEINTRESOURCEW(IDD_FIND),
            GetParent(hwnd),
            EditFindReplaceDlgProcW,
            (LPARAM) lpefr);

  ShowWindow(hDlg,SW_SHOW);

  return hDlg;

}


//=============================================================================
//
//  EditFindNext()
//
BOOL EditFindNext(HWND hwnd,LPCEDITFINDREPLACE lpefr,BOOL fExtendSelection)
{

  struct TextToFind ttf;
  int iPos;
  int iSelPos, iSelAnchor;
  char szFind2[512];

  if (!lstrlenA(lpefr->szFind))
    return /*EditFindReplaceDlg(hwnd,lpefr,FALSE)*/FALSE;

  lstrcpynA(szFind2,lpefr->szFind,COUNTOF(szFind2));
  if (lpefr->bTransformBS)
    TransformBackslashes(szFind2,(lpefr->fuFlags & SCFIND_REGEXP));

  if (lstrlenA(szFind2) == 0)
  {
    InfoBox(0,IDS_NOTFOUND,L"MsgNotFound");
    return FALSE;
  }

  iSelPos = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
  iSelAnchor = SendMessage(hwnd,SCI_GETANCHOR,0,0);

  ZeroMemory(&ttf,sizeof(ttf));

  ttf.chrg.cpMin = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0);
  ttf.chrg.cpMax = SendMessage(hwnd,SCI_GETLENGTH,0,0);
  ttf.lpstrText = szFind2;

  iPos = SendMessage(hwnd,SCI_FINDTEXT,lpefr->fuFlags,(LPARAM)&ttf);

  if (iPos == -1 && ttf.chrg.cpMin > 0 && !lpefr->bNoFindWrap && !fExtendSelection)
  {
    // wrapped
    InfoBox(0,IDS_FIND_WRAPFW,L"MsgWrap1");

    ttf.chrg.cpMin = 0;
    iPos = SendMessage(hwnd,SCI_FINDTEXT,lpefr->fuFlags,(LPARAM)&ttf);
  }

  if (iPos == -1)
  {
    // notfound
    InfoBox(0,IDS_NOTFOUND,L"MsgNotFound");
    return FALSE;
  }

  if (!fExtendSelection)
    EditSelectEx(hwnd,ttf.chrgText.cpMin,ttf.chrgText.cpMax);
  else
    EditSelectEx(hwnd,min(iSelAnchor,iSelPos),ttf.chrgText.cpMax);

  return TRUE;

}


//=============================================================================
//
//  EditFindPrev()
//
BOOL EditFindPrev(HWND hwnd,LPCEDITFINDREPLACE lpefr,BOOL fExtendSelection)
{

  struct TextToFind ttf;
  int iPos;
  int iSelPos, iSelAnchor;
  int iLength;
  char szFind2[512];

  if (!lstrlenA(lpefr->szFind))
    return /*EditFindReplaceDlg(hwnd,lpefr,FALSE)*/FALSE;

  lstrcpynA(szFind2,lpefr->szFind,COUNTOF(szFind2));
  if (lpefr->bTransformBS)
    TransformBackslashes(szFind2,(lpefr->fuFlags & SCFIND_REGEXP));

  if (lstrlenA(szFind2) == 0)
  {
    InfoBox(0,IDS_NOTFOUND,L"MsgNotFound");
    return FALSE;
  }

  iSelPos = SendMessage(hwnd,SCI_GETCURRENTPOS,0,0);
  iSelAnchor = SendMessage(hwnd,SCI_GETANCHOR,0,0);

  ZeroMemory(&ttf,sizeof(ttf));

  ttf.chrg.cpMin = max(0,SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0));
  ttf.chrg.cpMax = 0;
  ttf.lpstrText = szFind2;

  iPos = SendMessage(hwnd,SCI_FINDTEXT,lpefr->fuFlags,(LPARAM)&ttf);

  iLength = SendMessage(hwnd,SCI_GETLENGTH,0,0);
  if (iPos == -1 && ttf.chrg.cpMin < iLength && !lpefr->bNoFindWrap && !fExtendSelection)
  {
    // wrapped
    InfoBox(0,IDS_FIND_WRAPRE,L"MsgWrap2");

    ttf.chrg.cpMin = iLength;
    iPos = SendMessage(hwnd,SCI_FINDTEXT,lpefr->fuFlags,(LPARAM)&ttf);
  }

  if (iPos == -1)
  {
    // notfound
    InfoBox(0,IDS_NOTFOUND,L"MsgNotFound");
    return FALSE;
  }

  if (!fExtendSelection)
    EditSelectEx(hwnd,ttf.chrgText.cpMin,ttf.chrgText.cpMax);
  else
    EditSelectEx(hwnd,max(iSelPos,iSelAnchor),ttf.chrgText.cpMin);

  return TRUE;

}


//=============================================================================
//
//  EditReplace()
//
BOOL EditReplace(HWND hwnd,LPCEDITFINDREPLACE lpefr)
{

  struct TextToFind ttf;
  int iPos;
  int iSelStart;
  int iSelEnd;
  int iReplaceMsg = (lpefr->fuFlags & SCFIND_REGEXP) ? SCI_REPLACETARGETRE : SCI_REPLACETARGET;
  char szFind2[512];
  char *pszReplace2;

  if (!lstrlenA(lpefr->szFind))
    return /*EditFindReplaceDlg(hwnd,lpefr,TRUE)*/FALSE;

  lstrcpynA(szFind2,lpefr->szFind,COUNTOF(szFind2));
  if (lpefr->bTransformBS)
    TransformBackslashes(szFind2,(lpefr->fuFlags & SCFIND_REGEXP));

  if (lstrlenA(szFind2) == 0)
  {
    InfoBox(0,IDS_NOTFOUND,L"MsgNotFound");
    return FALSE;
  }

  if (lstrcmpA(lpefr->szReplace,"^c") == 0) {
    iReplaceMsg = SCI_REPLACETARGET;
    pszReplace2 = EditGetClipboardText(hwnd);
  }
  else {
    //lstrcpyA(szReplace2,lpefr->szReplace);
    pszReplace2 = StrDupA(lpefr->szReplace);
    if (lpefr->bTransformBS)
      TransformBackslashes(pszReplace2,(lpefr->fuFlags & SCFIND_REGEXP));
  }

  if (!pszReplace2)
    pszReplace2 = StrDupA("");

  iSelStart = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);
  iSelEnd   = SendMessage(hwnd,SCI_GETSELECTIONEND,0,0);

  ZeroMemory(&ttf,sizeof(ttf));

  ttf.chrg.cpMin = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0); // Start!
  ttf.chrg.cpMax = SendMessage(hwnd,SCI_GETLENGTH,0,0);
  ttf.lpstrText = szFind2;

  iPos = SendMessage(hwnd,SCI_FINDTEXT,lpefr->fuFlags,(LPARAM)&ttf);

  if (iPos == -1 && ttf.chrg.cpMin > 0 && !lpefr->bNoFindWrap)
  {
    // wrapped
    InfoBox(0,IDS_FIND_WRAPFW,L"MsgWrap1");

    ttf.chrg.cpMin = 0;
    iPos = SendMessage(hwnd,SCI_FINDTEXT,lpefr->fuFlags,(LPARAM)&ttf);
  }

  if (iPos == -1)
  {
    // notfound
    LocalFree(pszReplace2);
    InfoBox(0,IDS_NOTFOUND,L"MsgNotFound");
    return FALSE;
  }

  if (iSelStart != ttf.chrgText.cpMin || iSelEnd != ttf.chrgText.cpMax) {
    LocalFree(pszReplace2);
    EditSelectEx(hwnd,ttf.chrgText.cpMin,ttf.chrgText.cpMax);
    return FALSE;
  }

  SendMessage(hwnd,SCI_SETTARGETSTART,ttf.chrgText.cpMin,0);
  SendMessage(hwnd,SCI_SETTARGETEND,ttf.chrgText.cpMax,0);
  SendMessage(hwnd,iReplaceMsg,(WPARAM)-1,(LPARAM)pszReplace2);

  ttf.chrg.cpMin = SendMessage(hwnd,SCI_GETTARGETEND,0,0);
  ttf.chrg.cpMax = SendMessage(hwnd,SCI_GETLENGTH,0,0);

  iPos = SendMessage(hwnd,SCI_FINDTEXT,lpefr->fuFlags,(LPARAM)&ttf);

  if (iPos == -1 && ttf.chrg.cpMin > 0 && !lpefr->bNoFindWrap)
  {
    ttf.chrg.cpMin = 0;
    iPos = SendMessage(hwnd,SCI_FINDTEXT,lpefr->fuFlags,(LPARAM)&ttf);
  }

  if (iPos != -1)
    EditSelectEx(hwnd,ttf.chrgText.cpMin,ttf.chrgText.cpMax);

  else
    EditSelectEx(hwnd,
      SendMessage(hwnd,SCI_GETTARGETSTART,0,0),
      SendMessage(hwnd,SCI_GETTARGETEND,0,0));

  LocalFree(pszReplace2);
  return TRUE;

}


//=============================================================================
//
//  EditReplaceAll()
//
BOOL EditReplaceAll(HWND hwnd,LPCEDITFINDREPLACE lpefr,BOOL bShowInfo)
{

  struct TextToFind ttf;
  int iPos;
  int iCount = 0;
  int iReplaceMsg = (lpefr->fuFlags & SCFIND_REGEXP) ? SCI_REPLACETARGETRE : SCI_REPLACETARGET;
  char szFind2[512];
  char *pszReplace2;
  BOOL bRegexStartOrEndOfLine;

  if (!lstrlenA(lpefr->szFind))
    return /*EditFindReplaceDlg(hwnd,lpefr,TRUE)*/FALSE;

  // Show wait cursor...
  SendMessage(hwnd,SCI_SETCURSOR,SC_CURSORWAIT,0);

  lstrcpynA(szFind2,lpefr->szFind,COUNTOF(szFind2));
  if (lpefr->bTransformBS)
    TransformBackslashes(szFind2,(lpefr->fuFlags & SCFIND_REGEXP));

  if (lstrlenA(szFind2) == 0)
  {
    InfoBox(0,IDS_NOTFOUND,L"MsgNotFound");
    return FALSE;
  }

  bRegexStartOrEndOfLine =
    (lpefr->fuFlags & SCFIND_REGEXP &&
      (!lstrcmpA(szFind2,"$") || !lstrcmpA(szFind2,"^") || !lstrcmpA(szFind2,"^$")));

  if (lstrcmpA(lpefr->szReplace,"^c") == 0) {
    iReplaceMsg = SCI_REPLACETARGET;
    pszReplace2 = EditGetClipboardText(hwnd);
  }
  else {
    //lstrcpyA(szReplace2,lpefr->szReplace);
    pszReplace2 = StrDupA(lpefr->szReplace);
    if (lpefr->bTransformBS)
      TransformBackslashes(pszReplace2,(lpefr->fuFlags & SCFIND_REGEXP));
  }

  if (!pszReplace2)
    pszReplace2 = StrDupA("");

  ZeroMemory(&ttf,sizeof(ttf));

  ttf.chrg.cpMin = 0;
  ttf.chrg.cpMax = SendMessage(hwnd,SCI_GETLENGTH,0,0);
  ttf.lpstrText = szFind2;

  while ((iPos = SendMessage(hwnd,SCI_FINDTEXT,lpefr->fuFlags,(LPARAM)&ttf)) != -1)
  {
    int iReplacedLen;
    char ch;

    if (iCount == 0 && bRegexStartOrEndOfLine) {
      if (0 == SendMessage(hwnd,SCI_GETLINEENDPOSITION,0,0)) {
        iPos = 0;
        ttf.chrgText.cpMin = 0;
        ttf.chrgText.cpMax = 0;
      }
    }

    if (++iCount == 1)
      SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);

    SendMessage(hwnd,SCI_SETTARGETSTART,ttf.chrgText.cpMin,0);
    SendMessage(hwnd,SCI_SETTARGETEND,ttf.chrgText.cpMax,0);
    iReplacedLen = SendMessage(hwnd,iReplaceMsg,(WPARAM)-1,(LPARAM)pszReplace2);

    ttf.chrg.cpMin = ttf.chrgText.cpMin + iReplacedLen;
    ttf.chrg.cpMax = SendMessage(hwnd,SCI_GETLENGTH,0,0);

    if (ttf.chrg.cpMin == ttf.chrg.cpMax)
      break;

    ch = (char)SendMessage(hwnd,SCI_GETCHARAT,SendMessage(hwnd,SCI_GETTARGETEND,0,0),0);

    if (/*ch == '\r' || ch == '\n' || iReplacedLen == 0 || */
        ttf.chrgText.cpMin == ttf.chrgText.cpMax &&
        !(bRegexStartOrEndOfLine && iReplacedLen > 0))
      ttf.chrg.cpMin = SendMessage(hwnd,SCI_POSITIONAFTER,ttf.chrg.cpMin,0);
  }

  if (iCount)
    SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);

  // Remove wait cursor
  SendMessage(hwnd,SCI_SETCURSOR,SC_CURSORNORMAL,0);

  if (bShowInfo)
    MsgBox(MBINFO,IDS_REPLCOUNT,iCount);

  LocalFree(pszReplace2);
  return TRUE;

}


//=============================================================================
//
//  EditReplaceAllInSelection()
//
BOOL EditReplaceAllInSelection(HWND hwnd,LPCEDITFINDREPLACE lpefr,BOOL bShowInfo)
{

  struct TextToFind ttf;
  int iPos;
  int iCount = 0;
  int iReplaceMsg = (lpefr->fuFlags & SCFIND_REGEXP) ? SCI_REPLACETARGETRE : SCI_REPLACETARGET;
  BOOL fCancel = FALSE;
  char szFind2[512];
  char *pszReplace2;
  BOOL bRegexStartOrEndOfLine;

  if (SC_SEL_RECTANGLE == SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0))
  {
    MsgBox(MBINFO,IDS_SELRECT);
    return FALSE;
  }

  if (!lstrlenA(lpefr->szFind))
    return /*EditFindReplaceDlg(hwnd,lpefr,TRUE)*/FALSE;

  // Show wait cursor...
  SendMessage(hwnd,SCI_SETCURSOR,SC_CURSORWAIT,0);

  lstrcpynA(szFind2,lpefr->szFind,COUNTOF(szFind2));
  if (lpefr->bTransformBS)
    TransformBackslashes(szFind2,(lpefr->fuFlags & SCFIND_REGEXP));

  if (lstrlenA(szFind2) == 0)
  {
    InfoBox(0,IDS_NOTFOUND,L"MsgNotFound");
    return FALSE;
  }

  bRegexStartOrEndOfLine =
    (lpefr->fuFlags & SCFIND_REGEXP &&
      (!lstrcmpA(szFind2,"$") || !lstrcmpA(szFind2,"^") || !lstrcmpA(szFind2,"^$")));

  if (lstrcmpA(lpefr->szReplace,"^c") == 0) {
    iReplaceMsg = SCI_REPLACETARGET;
    pszReplace2 = EditGetClipboardText(hwnd);
  }
  else {
    //lstrcpyA(szReplace2,lpefr->szReplace);
    pszReplace2 = StrDupA(lpefr->szReplace);
    if (lpefr->bTransformBS)
      TransformBackslashes(pszReplace2,(lpefr->fuFlags & SCFIND_REGEXP));
  }

  if (!pszReplace2)
    pszReplace2 = StrDupA("");

  ZeroMemory(&ttf,sizeof(ttf));

  ttf.chrg.cpMin = SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0);
  ttf.chrg.cpMax = SendMessage(hwnd,SCI_GETLENGTH,0,0);
  ttf.lpstrText = szFind2;

  while ((iPos = SendMessage(hwnd,SCI_FINDTEXT,lpefr->fuFlags,(LPARAM)&ttf)) != -1 && !fCancel)
  {
    if (ttf.chrgText.cpMin >= SendMessage(hwnd,SCI_GETSELECTIONSTART,0,0) &&
        ttf.chrgText.cpMax <= SendMessage(hwnd,SCI_GETSELECTIONEND,0,0))
    {
      int iReplacedLen;
      char ch;

      if (ttf.chrg.cpMin == 0 && iCount == 0 && bRegexStartOrEndOfLine) {
        if (0 == SendMessage(hwnd,SCI_GETLINEENDPOSITION,0,0)) {
          iPos = 0;
          ttf.chrgText.cpMin = 0;
          ttf.chrgText.cpMax = 0;
        }
      }

      if (++iCount == 1)
        SendMessage(hwnd,SCI_BEGINUNDOACTION,0,0);

      SendMessage(hwnd,SCI_SETTARGETSTART,ttf.chrgText.cpMin,0);
      SendMessage(hwnd,SCI_SETTARGETEND,ttf.chrgText.cpMax,0);
      iReplacedLen = SendMessage(hwnd,iReplaceMsg,(WPARAM)-1,(LPARAM)pszReplace2);

      ttf.chrg.cpMin = ttf.chrgText.cpMin + iReplacedLen;
      ttf.chrg.cpMax = SendMessage(hwnd,SCI_GETLENGTH,0,0);

      if (ttf.chrg.cpMin == ttf.chrg.cpMax)
        fCancel = TRUE;

      ch = (char)SendMessage(hwnd,SCI_GETCHARAT,SendMessage(hwnd,SCI_GETTARGETEND,0,0),0);

      if (/*ch == '\r' || ch == '\n' || iReplacedLen == 0 || */
          ttf.chrgText.cpMin == ttf.chrgText.cpMax &&
          !(bRegexStartOrEndOfLine && iReplacedLen > 0))
        ttf.chrg.cpMin = SendMessage(hwnd,SCI_POSITIONAFTER,ttf.chrg.cpMin,0);
      }

    else
      // gone across selection, cancel
      fCancel = TRUE;
  }

  if (iCount)
    SendMessage(hwnd,SCI_ENDUNDOACTION,0,0);

  // Remove wait cursor
  SendMessage(hwnd,SCI_SETCURSOR,SC_CURSORNORMAL,0);

  if (bShowInfo)
    MsgBox(MBINFO,IDS_REPLCOUNT,iCount);

  LocalFree(pszReplace2);
  return TRUE;

}


//=============================================================================
//
//  EditLinenumDlgProc()
//
BOOL CALLBACK EditLinenumDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  switch(umsg)
  {

    case WM_INITDIALOG:
      {

        int iCurLine = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,
                         SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0),0)+1;

        SetDlgItemInt(hwnd,IDC_LINENUM,iCurLine,FALSE);
        SendDlgItemMessage(hwnd,IDC_LINENUM,EM_LIMITTEXT,15,0);

        SendDlgItemMessage(hwnd,IDC_COLNUM,EM_LIMITTEXT,15,0);

        CenterDlgInParent(hwnd);

      }
      return TRUE;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case IDOK: {

          BOOL fTranslated;
          BOOL fTranslated2;

          int iNewCol;

          int iNewLine = GetDlgItemInt(hwnd,IDC_LINENUM,&fTranslated,FALSE);
          int iMaxLine = SendMessage(hwndEdit,SCI_GETLINECOUNT,0,0);

          if (SendDlgItemMessage(hwnd,IDC_COLNUM,WM_GETTEXTLENGTH,0,0) > 0)
            iNewCol = GetDlgItemInt(hwnd,IDC_COLNUM,&fTranslated2,FALSE);
          else {
            iNewCol = 1;
            fTranslated2 = TRUE;
          }

          if (!fTranslated || !fTranslated2)
          {
            PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,(!fTranslated) ? IDC_LINENUM : IDC_COLNUM)),1);
            return TRUE;
          }

          if (iNewLine > 0 && iNewLine <= iMaxLine && iNewCol > 0)
          {
            //int iNewPos  = SendMessage(hwndEdit,SCI_POSITIONFROMLINE,(WPARAM)iNewLine-1,0);
            //int iLineEndPos = SendMessage(hwndEdit,SCI_GETLINEENDPOSITION,(WPARAM)iNewLine-1,0);

            //while (iNewCol-1 > SendMessage(hwndEdit,SCI_GETCOLUMN,(WPARAM)iNewPos,0))
            //{
            //  if (iNewPos >= iLineEndPos)
            //    break;

            //  iNewPos = SendMessage(hwndEdit,SCI_POSITIONAFTER,(WPARAM)iNewPos,0);
            //}

            //iNewPos = min(iNewPos,iLineEndPos);
            //SendMessage(hwndEdit,SCI_GOTOPOS,(WPARAM)iNewPos,0);
            //SendMessage(hwndEdit,SCI_CHOOSECARETX,0,0);

            EditJumpTo(hwndEdit,iNewLine,iNewCol);

            EndDialog(hwnd,IDOK);
          }

          else
            PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,(!(iNewLine > 0 && iNewLine <= iMaxLine)) ? IDC_LINENUM : IDC_COLNUM)),1);

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
//  EditLinenumDlg()
//
BOOL EditLinenumDlg(HWND hwnd)
{

  if (IDOK == DialogBoxParam(g_hInstance,MAKEINTRESOURCE(IDD_LINENUM),
                             GetParent(hwnd),EditLinenumDlgProc,(LPARAM)hwnd))
    return TRUE;

  else
    return FALSE;

}


//=============================================================================
//
//  EditModifyLinesDlg()
//
//  Controls: 100 Input
//            101 Input
//
typedef struct _modlinesdata {
  LPWSTR pwsz1;
  LPWSTR pwsz2;
} MODLINESDATA, *PMODLINESDATA;


BOOL CALLBACK EditModifyLinesDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{
  static PMODLINESDATA pdata;
  switch(umsg)
  {
    case WM_INITDIALOG:
      {
        pdata = (PMODLINESDATA)lParam;
        SetDlgItemTextW(hwnd,100,pdata->pwsz1);
        SendDlgItemMessage(hwnd,100,EM_LIMITTEXT,255,0);
        SetDlgItemTextW(hwnd,101,pdata->pwsz2);
        SendDlgItemMessage(hwnd,101,EM_LIMITTEXT,255,0);
        CenterDlgInParent(hwnd);
      }
      return TRUE;
    case WM_COMMAND:
      switch(LOWORD(wParam))
      {
        case IDOK: {
            GetDlgItemTextW(hwnd,100,pdata->pwsz1,256);
            GetDlgItemTextW(hwnd,101,pdata->pwsz2,256);
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
//  EditModifyLinesDlg()
//
BOOL EditModifyLinesDlg(HWND hwnd,LPWSTR pwsz1,LPWSTR pwsz2)
{

  int iResult;
  MODLINESDATA data = { pwsz1, pwsz2 };

  iResult = DialogBoxParamW(
              g_hInstance,
              MAKEINTRESOURCEW(IDD_MODIFYLINES),
              hwnd,
              EditModifyLinesDlgProc,
              (LPARAM)&data);

  return (iResult == IDOK) ? TRUE : FALSE;

}


//=============================================================================
//
//  EditEncloseSelectionDlgProc()
//
//  Controls: 100 Input
//            101 Input
//
typedef struct _encloseselectiondata {
  LPWSTR pwsz1;
  LPWSTR pwsz2;
} ENCLOSESELDATA, *PENCLOSESELDATA;


BOOL CALLBACK EditEncloseSelectionDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{
  static PENCLOSESELDATA pdata;
  switch(umsg)
  {
    case WM_INITDIALOG:
      {
        pdata = (PENCLOSESELDATA)lParam;
        SendDlgItemMessage(hwnd,100,EM_LIMITTEXT,255,0);
        SetDlgItemTextW(hwnd,100,pdata->pwsz1);
        SendDlgItemMessage(hwnd,101,EM_LIMITTEXT,255,0);
        SetDlgItemTextW(hwnd,101,pdata->pwsz2);
        CenterDlgInParent(hwnd);
      }
      return TRUE;
    case WM_COMMAND:
      switch(LOWORD(wParam))
      {
        case IDOK: {
            GetDlgItemTextW(hwnd,100,pdata->pwsz1,256);
            GetDlgItemTextW(hwnd,101,pdata->pwsz2,256);
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
//  EditEncloseSelectionDlg()
//
BOOL EditEncloseSelectionDlg(HWND hwnd,LPWSTR pwszOpen,LPWSTR pwszClose)
{

  int iResult;
  ENCLOSESELDATA data = { pwszOpen, pwszClose };

  iResult = DialogBoxParamW(
              g_hInstance,
              MAKEINTRESOURCEW(IDD_ENCLOSESELECTION),
              hwnd,
              EditEncloseSelectionDlgProc,
              (LPARAM)&data);

  return (iResult == IDOK) ? TRUE : FALSE;

}


//=============================================================================
//
//  EditInsertTagDlgProc()
//
//  Controls: 100 Input
//            101 Input
//
typedef struct _tagsdata {
  LPWSTR pwsz1;
  LPWSTR pwsz2;
} TAGSDATA, *PTAGSDATA;


BOOL CALLBACK EditInsertTagDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{
  static PTAGSDATA pdata;
  switch(umsg)
  {
    case WM_INITDIALOG:
      {
        pdata = (PTAGSDATA)lParam;
        SendDlgItemMessage(hwnd,100,EM_LIMITTEXT,254,0);
        SetDlgItemTextW(hwnd,100,L"<tag>");
        SendDlgItemMessage(hwnd,101,EM_LIMITTEXT,255,0);
        SetDlgItemTextW(hwnd,101,L"</tag>");
        SetFocus(GetDlgItem(hwnd,100));
        PostMessage(GetDlgItem(hwnd,100),EM_SETSEL,1,4);
        CenterDlgInParent(hwnd);
      }
      return FALSE;
    case WM_COMMAND:
      switch(LOWORD(wParam))
      {
        case 100: {
            if (HIWORD(wParam) == EN_CHANGE) {

              WCHAR wchBuf[256];
              WCHAR wchIns[256] = L"</";
              int  cchIns = 2;
              BOOL bClear = TRUE;

              GetDlgItemTextW(hwnd,100,wchBuf,256);
              if (lstrlen(wchBuf) >= 3) {

                if (wchBuf[0] == L'<') {

                  const WCHAR* pwCur = &wchBuf[1];

                  while (
                    *pwCur &&
                    *pwCur != L'<' &&
                    *pwCur != L'>' &&
                    *pwCur != L' ' &&
                    *pwCur != L'\t' &&
                    (StrChr(L":_-.",*pwCur) || IsCharAlphaNumericW(*pwCur)))

                      wchIns[cchIns++] = *pwCur++;

                  while (
                    *pwCur &&
                    *pwCur != L'>')

                      pwCur++;

                  if (*pwCur == L'>' && *(pwCur-1) != L'/') {
                    wchIns[cchIns++] = L'>';
                    wchIns[cchIns] = L'\0';

                    if (cchIns > 3 &&
                      lstrcmpi(wchIns,L"</base>") &&
                      lstrcmpi(wchIns,L"</bgsound>") &&
                      lstrcmpi(wchIns,L"</br>") &&
                      lstrcmpi(wchIns,L"</embed>") &&
                      lstrcmpi(wchIns,L"</hr>") &&
                      lstrcmpi(wchIns,L"</img>") &&
                      lstrcmpi(wchIns,L"</input>") &&
                      lstrcmpi(wchIns,L"</link>") &&
                      lstrcmpi(wchIns,L"</meta>")) {

                        SetDlgItemTextW(hwnd,101,wchIns);
                        bClear = FALSE;
                    }
                  }
                }
              }
              if (bClear)
                SetDlgItemTextW(hwnd,101,L"");
            }
          }
          break;
        case IDOK: {
            GetDlgItemTextW(hwnd,100,pdata->pwsz1,256);
            GetDlgItemTextW(hwnd,101,pdata->pwsz2,256);
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
//  EditInsertTagDlg()
//
BOOL EditInsertTagDlg(HWND hwnd,LPWSTR pwszOpen,LPWSTR pwszClose)
{

  int iResult;
  TAGSDATA data = { pwszOpen, pwszClose };

  iResult = DialogBoxParamW(
              g_hInstance,
              MAKEINTRESOURCEW(IDD_INSERTTAG),
              hwnd,
              EditInsertTagDlgProc,
              (LPARAM)&data);

  return (iResult == IDOK) ? TRUE : FALSE;

}


//=============================================================================
//
//  FileVars_Init()
//
extern int fNoFileVariables;

BOOL FileVars_Init(char *lpData,DWORD cbData,LPFILEVARS lpfv) {

  int i;
  char tch[256];

  ZeroMemory(lpfv,sizeof(FILEVARS));
  if (fNoFileVariables || !lpData || !cbData)
    return(TRUE);

  lstrcpynA(tch,lpData,min(cbData,COUNTOF(tch)));

  if (FileVars_ParseInt(tch,"tab-width",&i)) {
    lpfv->iTabWidth = max(min(i,32),1);
    lpfv->mask |= FV_TABWIDTH;
  }

  if (FileVars_ParseInt(tch,"c-basic-indent",&i)) {
    lpfv->iIndentWidth = max(min(i,32),0);
    lpfv->mask |= FV_INDENTWIDTH;
  }

  if (FileVars_ParseInt(tch,"indent-tabs-mode",&i)) {
    lpfv->bTabsAsSpaces = (i) ? FALSE : TRUE;
    lpfv->mask |= FV_TABSASSPACES;
  }

  if (FileVars_ParseInt(tch,"fill-column",&i)) {
    lpfv->iLongLinesLimit = max(min(i,999),0);
    lpfv->mask |= FV_LONGLINESLIMIT;
  }

  if (FileVars_ParseStr(tch,"encoding",lpfv->tchEncoding,COUNTOF(lpfv->tchEncoding)))
    lpfv->mask |= FV_ENCODING;
  else if (FileVars_ParseStr(tch,"charset",lpfv->tchEncoding,COUNTOF(lpfv->tchEncoding)))
    lpfv->mask |= FV_ENCODING;
  else if (FileVars_ParseStr(tch,"coding",lpfv->tchEncoding,COUNTOF(lpfv->tchEncoding)))
    lpfv->mask |= FV_ENCODING;

  if (FileVars_ParseStr(tch,"mode",lpfv->tchMode,COUNTOF(lpfv->tchMode)))
    lpfv->mask |= FV_MODE;

  if (lpfv->mask == 0 && cbData > COUNTOF(tch)) {

    lstrcpynA(tch,lpData+cbData-COUNTOF(tch),COUNTOF(tch));

    if (FileVars_ParseInt(tch,"tab-width",&i)) {
      lpfv->iTabWidth = max(min(i,32),1);
      lpfv->mask |= FV_TABWIDTH;
    }

    if (FileVars_ParseInt(tch,"c-basic-indent",&i)) {
      lpfv->iIndentWidth = max(min(i,32),0);
      lpfv->mask |= FV_INDENTWIDTH;
    }

    if (FileVars_ParseInt(tch,"indent-tabs-mode",&i)) {
      lpfv->bTabsAsSpaces = (i) ? FALSE : TRUE;
      lpfv->mask |= FV_TABSASSPACES;
    }

    if (FileVars_ParseInt(tch,"fill-column",&i)) {
      lpfv->iLongLinesLimit = max(min(i,999),0);
      lpfv->mask |= FV_LONGLINESLIMIT;
    }

    if (FileVars_ParseStr(tch,"encoding",lpfv->tchEncoding,COUNTOF(lpfv->tchEncoding)))
      lpfv->mask |= FV_ENCODING;
    else if (FileVars_ParseStr(tch,"charset",lpfv->tchEncoding,COUNTOF(lpfv->tchEncoding)))
      lpfv->mask |= FV_ENCODING;
    else if (FileVars_ParseStr(tch,"coding",lpfv->tchEncoding,COUNTOF(lpfv->tchEncoding)))
      lpfv->mask |= FV_ENCODING;

    if (FileVars_ParseStr(tch,"mode",lpfv->tchMode,COUNTOF(lpfv->tchMode)))
      lpfv->mask |= FV_MODE;
  }

  return(TRUE);
}


//=============================================================================
//
//  FileVars_Apply()
//
extern int iTabWidth;
extern int iTabWidthG;
extern int iIndentWidth;
extern int iIndentWidthG;
extern BOOL bTabsAsSpaces;
extern BOOL bTabsAsSpacesG;
extern int iLongLinesLimit;
extern int iLongLinesLimitG;
extern int iWrapCol;

BOOL FileVars_Apply(HWND hwnd,LPFILEVARS lpfv) {

  if (lpfv->mask & FV_TABWIDTH)
    iTabWidth = lpfv->iTabWidth;
  else
    iTabWidth = iTabWidthG;
  SendMessage(hwnd,SCI_SETTABWIDTH,iTabWidth,0);

  if (lpfv->mask & FV_INDENTWIDTH)
    iIndentWidth = lpfv->iIndentWidth;
  else
    iIndentWidth = iIndentWidthG;
  SendMessage(hwndEdit,SCI_SETINDENT,iIndentWidth,0);

  if (lpfv->mask & FV_TABSASSPACES)
    bTabsAsSpaces = lpfv->bTabsAsSpaces;
  else
    bTabsAsSpaces = bTabsAsSpacesG;
  SendMessage(hwndEdit,SCI_SETUSETABS,!bTabsAsSpaces,0);

  if (lpfv->mask & FV_LONGLINESLIMIT)
    iLongLinesLimit = lpfv->iLongLinesLimit;
  else
    iLongLinesLimit = iLongLinesLimitG;
  SendMessage(hwndEdit,SCI_SETEDGECOLUMN,iLongLinesLimit,0);

  iWrapCol = 0;

  return(TRUE);
}


//=============================================================================
//
//  FileVars_ParseInt()
//
BOOL FileVars_ParseInt(char* pszData,char* pszName,int* piValue) {

  char tch[32];
  char *pvStart = pszData;
  char chPrev;
  char *pvEnd;
  int  itok;

  while (pvStart = StrStrIA(pvStart,pszName)) {
    chPrev = (pvStart > pszData) ? *(pvStart-1) : 0;
    if (!IsCharAlphaNumericA(chPrev) && chPrev != '-')
      break;
    else
      pvStart += lstrlenA(pszName);
  }

  if (pvStart) {

    pvStart += lstrlenA(pszName);

    while (*pvStart == ' ')
      pvStart++;

    if (*pvStart != ':' && *pvStart != '=')
      return(FALSE);

    while (*pvStart && StrChrIA(":=\"' \t",*pvStart))
      pvStart++;

    lstrcpynA(tch,pvStart,COUNTOF(tch));

    pvEnd = tch;
    while (*pvEnd && IsCharAlphaNumericA(*pvEnd))
      pvEnd++;
    *pvEnd = 0;
    StrTrimA(tch," \t:=\"'");

    itok = sscanf(tch,"%i",piValue);
    if (itok == 1)
      return(TRUE);

    if (tch[0] == 't') {
      *piValue = 1;
      return(TRUE);
    }

    if (tch[0] == 'n' || tch[0] == 'f') {
      *piValue = 0;
      return(TRUE);
    }
  }
  return(FALSE);
}


//=============================================================================
//
//  FileVars_ParseStr()
//
BOOL FileVars_ParseStr(char* pszData,char* pszName,char* pszValue,int cchValue) {

  char tch[32];
  char *pvStart = pszData;
  char chPrev;
  char *pvEnd;
  BOOL bQuoted = FALSE;

  while (pvStart = StrStrIA(pvStart,pszName)) {
    chPrev = (pvStart > pszData) ? *(pvStart-1) : 0;
    if (!IsCharAlphaNumericA(chPrev) && chPrev != '-')
      break;
    else
     pvStart += lstrlenA(pszName);
  }

  if (pvStart) {

    pvStart += lstrlenA(pszName);

    while (*pvStart == ' ')
      pvStart++;

    if (*pvStart != ':' && *pvStart != '=')
      return(FALSE);

    while (*pvStart && StrChrIA(":=\"' \t",*pvStart)) {
      if (*pvStart == '\'' || *pvStart == '"')
        bQuoted = TRUE;
      pvStart++;
    }
    lstrcpynA(tch,pvStart,COUNTOF(tch));

    pvEnd = tch;
    while (*pvEnd && (IsCharAlphaNumericA(*pvEnd) || StrChrIA("+-/",*pvEnd) || (bQuoted && *pvEnd == ' ')))
      pvEnd++;
    *pvEnd = 0;
    StrTrimA(tch," \t:=\"'");

    lstrcpynA(pszValue,tch,cchValue);
    return(TRUE);
  }
  return(FALSE);
}


//=============================================================================
//
//  FileVars_IsUTF8()
//
BOOL FileVars_IsUTF8(LPFILEVARS lpfv) {
  if (lpfv->mask & FV_ENCODING) {
    if (lstrcmpiA(lpfv->tchEncoding,"utf-8") == 0 ||
        lstrcmpiA(lpfv->tchEncoding,"utf8") == 0)
      return(TRUE);
  }
  return(FALSE);
}


//=============================================================================
//
//  FileVars_IsANSI()
//
BOOL FileVars_IsANSI(LPFILEVARS lpfv) {
  if (lpfv->mask & FV_ENCODING) {
    if (lstrlenA(lpfv->tchEncoding) &&
        lstrcmpiA(lpfv->tchEncoding,"utf-8") != 0 &&
        lstrcmpiA(lpfv->tchEncoding,"utf8") != 0)
      return(TRUE);
  }
  return(FALSE);
}


//=============================================================================
//
//  SciInitThemes()
//
//WNDPROC pfnSciWndProc = NULL;
//
//FARPROC pfnOpenThemeData = NULL;
//FARPROC pfnCloseThemeData = NULL;
//FARPROC pfnDrawThemeBackground = NULL;
//FARPROC pfnGetThemeBackgroundContentRect = NULL;
//FARPROC pfnIsThemeActive = NULL;
//FARPROC pfnDrawThemeParentBackground = NULL;
//FARPROC pfnIsThemeBackgroundPartiallyTransparent = NULL;
//
//BOOL bThemesPresent = FALSE;
//extern BOOL bIsAppThemed;
//extern HMODULE hModUxTheme;
//
//void SciInitThemes(HWND hwnd)
//{
//  if (hModUxTheme) {
//
//    pfnOpenThemeData = GetProcAddress(hModUxTheme,"OpenThemeData");
//    pfnCloseThemeData = GetProcAddress(hModUxTheme,"CloseThemeData");
//    pfnDrawThemeBackground = GetProcAddress(hModUxTheme,"DrawThemeBackground");
//    pfnGetThemeBackgroundContentRect = GetProcAddress(hModUxTheme,"GetThemeBackgroundContentRect");
//    pfnIsThemeActive = GetProcAddress(hModUxTheme,"IsThemeActive");
//    pfnDrawThemeParentBackground = GetProcAddress(hModUxTheme,"DrawThemeParentBackground");
//    pfnIsThemeBackgroundPartiallyTransparent = GetProcAddress(hModUxTheme,"IsThemeBackgroundPartiallyTransparent");
//
//    pfnSciWndProc = (WNDPROC)SetWindowLongW(hwnd,GWL_WNDPROC,(LONG)&SciThemedWndProc);
//    bThemesPresent = TRUE;
//  }
//}
//
//
////=============================================================================
////
////  SciThemedWndProc()
////
//LRESULT CALLBACK SciThemedWndProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
//{
//  static RECT rcContent;
//
//  if (umsg == WM_NCCALCSIZE) {
//    if (wParam) {
//      LRESULT lresult = CallWindowProcW(pfnSciWndProc,hwnd,WM_NCCALCSIZE,wParam,lParam);
//      NCCALCSIZE_PARAMS *csp = (NCCALCSIZE_PARAMS*)lParam;
//
//      if (bThemesPresent && bIsAppThemed) {
//        HANDLE hTheme = (HANDLE)pfnOpenThemeData(hwnd,L"edit");
//        if(hTheme) {
//          BOOL bSuccess = FALSE;
//          RECT rcClient;
//
//          if(pfnGetThemeBackgroundContentRect(
//              hTheme,NULL,/*EP_EDITTEXT*/1,/*ETS_NORMAL*/1,&csp->rgrc[0],&rcClient) == S_OK) {
//            InflateRect(&rcClient,-1,-1);
//
//            rcContent.left = rcClient.left-csp->rgrc[0].left;
//            rcContent.top = rcClient.top-csp->rgrc[0].top;
//            rcContent.right = csp->rgrc[0].right-rcClient.right;
//            rcContent.bottom = csp->rgrc[0].bottom-rcClient.bottom;
//
//            CopyRect(&csp->rgrc[0],&rcClient);
//            bSuccess = TRUE;
//          }
//          pfnCloseThemeData(hTheme);
//
//          if (bSuccess)
//            return WVR_REDRAW;
//        }
//      }
//      return lresult;
//    }
//  }
//
//  else if (umsg == WM_NCPAINT) {
//    LRESULT lresult = CallWindowProcW(pfnSciWndProc,hwnd,WM_NCPAINT,wParam,lParam);
//    if(bThemesPresent && bIsAppThemed) {
//
//      HANDLE hTheme = (HANDLE)pfnOpenThemeData(hwnd,L"edit");
//      if(hTheme) {
//        RECT rcBorder;
//        RECT rcClient;
//        int nState;
//
//        HDC hdc = GetWindowDC(hwnd);
//
//        GetWindowRect(hwnd,&rcBorder);
//        OffsetRect(&rcBorder,-rcBorder.left,-rcBorder.top);
//
//        CopyRect(&rcClient,&rcBorder);
//        rcClient.left += rcContent.left;
//        rcClient.top += rcContent.top;
//        rcClient.right -= rcContent.right;
//        rcClient.bottom -= rcContent.bottom;
//
//        ExcludeClipRect(hdc,rcClient.left,rcClient.top,rcClient.right,rcClient.bottom);
//
//        if(pfnIsThemeBackgroundPartiallyTransparent(hTheme,/*EP_EDITTEXT*/1,/*ETS_NORMAL*/1))
//          pfnDrawThemeParentBackground(hwnd,hdc,&rcBorder);
//
//        /*
//        ETS_NORMAL = 1
//        ETS_HOT = 2
//        ETS_SELECTED = 3
//        ETS_DISABLED = 4
//        ETS_FOCUSED = 5
//        ETS_READONLY = 6
//        ETS_ASSIST = 7
//        */
//
//        if(!IsWindowEnabled(hwnd))
//          nState = /*ETS_DISABLED*/4;
//        else if (GetFocus() == hwnd)
//          nState = /*ETS_FOCUSED*/5;
//        else if(SendMessage(hwnd,SCI_GETREADONLY,0,0))
//          nState = /*ETS_READONLY*/6;
//        else
//          nState = /*ETS_NORMAL*/1;
//
//        pfnDrawThemeBackground(hTheme,hdc,/*EP_EDITTEXT*/1,nState,&rcBorder,NULL);
//        pfnCloseThemeData(hTheme);
//
//        ReleaseDC(hwnd,hdc);
//        return 0;
//      }
//    }
//    return lresult;
//  }
//
//  return CallWindowProcW(pfnSciWndProc,hwnd,umsg,wParam,lParam);
//}



///   End of Edit.c   \\\
