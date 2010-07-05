
DIR_O=obj
DIR_BIN=bin

CC=cl
RC=rc
LD=link

EXE=$(DIR_BIN)\notepad2.exe

CXXFLAGS=-Zi -W3 -EHsc -D_CRT_SECURE_NO_DEPRECATE=1 -D_UNICODE -DUNICODE -DSTATIC_BUILD -DSCI_LEXER -DBOOKMARK_EDITION
CXXDEBUG=-Od -MTd -DDEBUG
CXXNDEBUG=-O2 -MT -DNDEBUG -GL
NAME=-Fo
MANIFEST=-"manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
			processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'"
LDFLAGS=-OPT:REF -DEBUG $(MANIFEST)
LDDEBUG=
LDNDEBUG=-LTCG
LIBS=kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib shlwapi.lib comdlg32.lib comctl32.lib winspool.lib imm32.lib ole32.lib oleaut32.lib psapi.lib
NOLOGO=-nologo


!IFDEF DEBUG
CXXFLAGS=$(CXXFLAGS) $(CXXDEBUG) $(NOLOGO)
LDFLAGS=$(LDDEBUG) $(LDFLAGS) $(NOLOGO)
!ELSE
CXXFLAGS=$(CXXFLAGS) $(CXXNDEBUG) $(NOLOGO)
LDFLAGS=$(LDNDEBUG) $(LDFLAGS) $(NOLOGO)
!ENDIF

SCINTILLA_INCLUDEDIRS=-I "scintilla\include" -I "scintilla\src" -I "scintilla\win32"
INCLUDEDIRS=-Isrc -Iinc $(SCINTILLA_INCLUDEDIRS)
CXXFLAGS=$(CXXFLAGS) $(INCLUDEDIRS)


ALL:	$(EXE)

clean:
	-del /q $(DIR_O)\*.obj $(DIR_O)\*.pdb $(EXE) \
	$(DIR_O)\*.res $(DIR_BIN)\*.map $(DIR_BIN)\*.exp $(DIR_BIN)\*.pdb $(DIR_BIN)\*.lib $(DIR_BIN)\*.manifest



################################################################################
# Scintilla Definitions
################################################################################

OBJS=$(DIR_O)\Dialogs.obj \
	 $(DIR_O)\Dlapi.obj \
	 $(DIR_O)\Edit.obj \
	 $(DIR_O)\Helpers.obj \
	 $(DIR_O)\Notepad2.obj \
	 $(DIR_O)\Print.obj \
	 $(DIR_O)\Styles.obj \
	 $(DIR_O)\Notepad2.res \
	 scintilla\win32\ScintillaWinL.obj \
	scintilla\win32\AutoComplete.obj \
	scintilla\win32\CallTip.obj \
	scintilla\win32\CellBuffer.obj \
	scintilla\win32\CharClassify.obj \
	scintilla\win32\ContractionState.obj \
	scintilla\win32\Decoration.obj \
	scintilla\win32\Document.obj \
	scintilla\win32\DocumentAccessor.obj \
	scintilla\win32\Editor.obj \
	scintilla\win32\ExternalLexer.obj \
	scintilla\win32\Indicator.obj \
	scintilla\win32\KeyMap.obj \
	scintilla\win32\KeyWords.obj \
	scintilla\win32\LineMarker.obj \
	scintilla\win32\PerLine.obj \
	scintilla\win32\PlatWin.obj \
	scintilla\win32\PositionCache.obj \
	scintilla\win32\PropSet.obj \
	scintilla\win32\RESearch.obj \
	scintilla\win32\RunStyles.obj \
	scintilla\win32\ScintillaBaseL.obj \
	scintilla\win32\Selection.obj \
	scintilla\win32\Style.obj \
	scintilla\win32\StyleContext.obj \
	scintilla\win32\UniConversion.obj \
	scintilla\win32\ViewStyle.obj \
	scintilla\win32\XPM.obj \
	scintilla\win32\LexAbaqus.obj \
	scintilla\win32\LexAda.obj \
	scintilla\win32\LexAPDL.obj \
	scintilla\win32\LexAsm.obj \
	scintilla\win32\LexAsn1.obj \
	scintilla\win32\LexASY.obj \
	scintilla\win32\LexAU3.obj \
	scintilla\win32\LexAVE.obj \
	scintilla\win32\LexBaan.obj \
	scintilla\win32\LexBash.obj \
	scintilla\win32\LexBasic.obj \
	scintilla\win32\LexBullant.obj \
	scintilla\win32\LexCaml.obj \
	scintilla\win32\LexCLW.obj \
	scintilla\win32\LexCmake.obj \
	scintilla\win32\LexCOBOL.obj \
	scintilla\win32\LexConf.obj \
	scintilla\win32\LexCPP.obj \
	scintilla\win32\LexCrontab.obj \
	scintilla\win32\LexCsound.obj \
	scintilla\win32\LexCSS.obj \
	scintilla\win32\LexD.obj \
	scintilla\win32\LexEiffel.obj \
	scintilla\win32\LexErlang.obj \
	scintilla\win32\LexEScript.obj \
	scintilla\win32\LexFlagship.obj \
	scintilla\win32\LexForth.obj \
	scintilla\win32\LexFortran.obj \
	scintilla\win32\LexGAP.obj \
	scintilla\win32\LexGui4Cli.obj \
	scintilla\win32\LexHaskell.obj \
	scintilla\win32\LexHTML.obj \
	scintilla\win32\LexInno.obj \
	scintilla\win32\LexKix.obj \
	scintilla\win32\LexLisp.obj \
	scintilla\win32\LexLout.obj \
	scintilla\win32\LexLua.obj \
	scintilla\win32\LexMagik.obj \
	scintilla\win32\LexMarkdown.obj \
	scintilla\win32\LexMatlab.obj \
	scintilla\win32\LexMetapost.obj \
	scintilla\win32\LexMMIXAL.obj \
	scintilla\win32\LexMPT.obj \
	scintilla\win32\LexMSSQL.obj \
	scintilla\win32\LexMySQL.obj \
	scintilla\win32\LexNimrod.obj \
	scintilla\win32\LexNsis.obj \
	scintilla\win32\LexOpal.obj \
	scintilla\win32\LexOthers.obj \
	scintilla\win32\LexPascal.obj \
	scintilla\win32\LexPB.obj \
	scintilla\win32\LexPerl.obj \
	scintilla\win32\LexPLM.obj \
	scintilla\win32\LexPOV.obj \
	scintilla\win32\LexPowerPro.obj \
	scintilla\win32\LexPowerShell.obj \
	scintilla\win32\LexProgress.obj \
	scintilla\win32\LexPS.obj \
	scintilla\win32\LexPython.obj \
	scintilla\win32\LexR.obj \
	scintilla\win32\LexRebol.obj \
	scintilla\win32\LexRuby.obj \
	scintilla\win32\LexScriptol.obj \
	scintilla\win32\LexSmalltalk.obj \
	scintilla\win32\LexSML.obj \
	scintilla\win32\LexSorcus.obj \
	scintilla\win32\LexSpecman.obj \
	scintilla\win32\LexSpice.obj \
	scintilla\win32\LexSQL.obj \
	scintilla\win32\LexTACL.obj \
	scintilla\win32\LexTADS3.obj \
	scintilla\win32\LexTAL.obj \
	scintilla\win32\LexTCL.obj \
	scintilla\win32\LexTeX.obj \
	scintilla\win32\LexVB.obj \
	scintilla\win32\LexVerilog.obj \
	scintilla\win32\LexVHDL.obj \
	scintilla\win32\LexYAML.obj \

{src}.c{$(DIR_O)}.obj:
	$(CC) $(CXXFLAGS) -c $(NAME)$@ $<

{src}.cpp{$(DIR_O)}.obj:
	$(CC) $(CXXFLAGS) -c $(NAME)$@ $<


$(DIR_O)\Notepad2.res : src\Notepad2.rc
	$(RC) $(NOLOGO) $(INCLUDEDIRS) -fo$@ $**

$(EXE): $(OBJS)
	$(LD) $(LDFLAGS) -OUT:$@ $** $(LIBS)
	@if exist "$@.manifest" \
	    mt.exe -manifest "$@.manifest" -outputresource:$@;1
