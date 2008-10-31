/******************************************************************************
*
*
* Notepad2
*
* SciWrap.cpp
*   C-Wrapper around Scintilla C++ functions
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
#define PLAT_WIN 1
#include "scintilla.h"


bool Scintilla_RegisterClasses(void *hInstance);
bool Scintilla_ReleaseResources();


extern "C" int SciWrap_RegisterClasses(HINSTANCE hInstance) {
  return (int)Scintilla_RegisterClasses(hInstance);
}


extern "C" int SciWrap_ReleaseResources() {
  return (int)Scintilla_ReleaseResources();
}


// End of SciWrap.cpp
