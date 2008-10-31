/******************************************************************************
*
*
* Notepad2
*
* Styles.c
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
#define _WIN32_WINNT 0x501
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <stdio.h>
#include "dialogs.h"
#include "helpers.h"
#include "notepad2.h"
#include "scintilla.h"
#include "scilexer.h"
#include "edit.h"
#include "styles.h"
#include "resource.h"


KEYWORDLIST KeyWords_NULL = {
"", "", "", "", "", "", "", "", "" };


EDITLEXER lexDefault = { SCLEX_NULL, L"Default Text", L"txt; text; wtx; log; asc; doc; diz; nfo", L"", &KeyWords_NULL, {
                /*  0 */ { STYLE_DEFAULT, L"Default Style", L"font:Lucida Console; size:10", L"" },
                /*  1 */ { STYLE_LINENUMBER, L"Margins and Line Numbers", L"size:-2; fore:#FF0000", L"" },
                /*  2 */ { STYLE_BRACELIGHT, L"Matching Braces", L"size:+1; fore:#FF0000; bold", L"" },
                /*  3 */ { STYLE_BRACEBAD, L"Matching Braces Error", L"size:+1; fore:#000080; bold", L"" },
                /*  4 */ { STYLE_CONTROLCHAR, L"Control Characters (Unused)", L"", L"" },
                /*  5 */ { STYLE_INDENTGUIDE, L"Indentation Guide (Color only)", L"", L"" },
                /*  6 */ { SCI_SETSELFORE+SCI_SETSELBACK, L"Selected Text (Colors only)", L"back:#0A246A; alpha:95; eolfilled", L"" },
                /*  7 */ { SCI_SETWHITESPACEFORE+SCI_SETWHITESPACEBACK, L"Whitespace (Colors only)", L"fore:#FF0000", L"" },
                /*  8 */ { SCI_SETCARETLINEBACK, L"Current Line Background (Color only)", L"back:#FFFF00; alpha:50", L"" },
                /*  9 */ { SCI_SETCARETFORE, L"Caret Foreground (Color only)", L"", L"" },
                /* 10 */ { SCI_SETCARETWIDTH, L"Caret Width (Size from 1 to 3)", L"", L"" },
                /* 11 */ { SCI_SETEDGECOLOUR, L"Long Line Marker (Colors only)", L"", L"" },

                /* 12 */ { STYLE_DEFAULT, L"2nd Default Style", L"font:Courier New; size:10", L"" },
                /* 13 */ { STYLE_LINENUMBER, L"2nd Margins and Line Numbers", L"font:Tahoma; size:-2; fore:#FF0000", L"" },
                /* 14 */ { STYLE_BRACELIGHT, L"2nd Matching Braces", L"fore:#FF0000; bold", L"" },
                /* 15 */ { STYLE_BRACEBAD, L"2nd Matching Braces Error", L"fore:#000080; bold", L"" },
                /* 16 */ { STYLE_CONTROLCHAR, L"2nd Control Characters (Unused)", L"", L"" },
                /* 17 */ { STYLE_INDENTGUIDE, L"2nd Indentation Guide (Color only)", L"", L"" },
                /* 18 */ { SCI_SETSELFORE+SCI_SETSELBACK, L"2nd Selected Text (Colors only)", L"eolfilled", L"" },
                /* 19 */ { SCI_SETWHITESPACEFORE+SCI_SETWHITESPACEBACK, L"2nd Whitespace (Colors only)", L"fore:#FF0000", L"" },
                /* 20 */ { SCI_SETCARETLINEBACK, L"2nd Current Line Background (Color only)", L"", L"" },
                /* 21 */ { SCI_SETCARETFORE, L"2nd Caret Foreground (Color only)", L"", L"" },
                /* 22 */ { SCI_SETCARETWIDTH, L"2nd Caret Width (Size from 1 to 3)", L"", L"" },
                /* 23 */ { SCI_SETEDGECOLOUR, L"2nd Long Line Marker (Colors only)", L"", L"" },
                         { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_HTML = {
"a abbr acronym address applet area b base basefont bdo big blockquote body bordercolor br button caption "
"center cite code col colgroup dd del dfn dir div dl dt em fieldset font form frame frameset h1 h2 "
"h3 h4 h5 h6 head hr html i iframe img input ins isindex kbd label legend li link map menu meta "
"noframes noscript object ol optgroup option p param pre q s samp script select small span strike "
"strong style sub sup table tbody td textarea tfoot th thead title tr tt u ul var xml xmlns abbr "
"accept-charset accept accesskey action align alink alt archive axis background bgcolor border "
"cellpadding cellspacing char charoff charset checked cite class classid clear codebase codetype "
"color cols colspan compact content coords data datafld dataformatas datapagesize datasrc "
"datetime declare defer dir disabled enctype event face for frame frameborder headers height href "
"hreflang hspace http-equiv id ismap label lang language leftmargin link longdesc marginwidth "
"marginheight maxlength media method multiple name nohref noresize noshade nowrap object "
"onblur onchange onclick ondblclick onfocus onkeydown onkeypress onkeyup onload onmousedown "
"onmousemove onmouseover onmouseout onmouseup onreset onselect onsubmit onunload profile "
"prompt readonly rel rev rows rowspan rules scheme scope scrolling selected shape size span src standby "
"start style summary tabindex target text title topmargin type usemap valign value valuetype "
"version vlink vspace width text password checkbox radio submit reset file hidden image public "
"!doctype",
"abstract boolean break byte case catch char class const continue debugger default delete do "
"double else enum export extends final finally float for function goto if implements import in "
"instanceof int interface long native new package private protected public return short static "
"super switch synchronized this throw throws transient try typeof var void volatile while with",
"and begin case call continue do each else elseif end erase error event exit false for function get "
"gosub goto if implement in load loop lset me mid new next not nothing on or property raiseevent "
"rem resume return rset select set stop sub then to true unload until wend while with withevents "
"attribute alias as boolean byref byte byval const compare currency date declare dim double enum "
"explicit friend global integer let lib long module object option optional preserve private property "
"public redim single static string type variant",
"and assert break class continue def del elif else except exec finally for from global if import in is "
"lambda None not or pass print raise return try while yield",
"__callstatic __class__ __dir__ __file__ __function__ __get __isset __line__ __method__ "
"__namespace__ __set __sleep __unset __wakeup abstract and argc argv array as break case catch "
"cfunction class clone const continue declare default define die do e_all e_error e_fatal "
"e_notice e_parse e_strict e_warning echo else elseif empty enddeclare endfor endforeach endif "
"endswitch endwhile eval exception exit extends false final for foreach function global goto "
"http_cookie_vars http_env_vars http_get_vars http_post_files http_post_vars http_server_vars if "
"implements include include_once instanceof interface isset list namespace new not null "
"old_function or parent php_self print private protected public require require_once return "
"static stdclass switch this throw true try unset use var virtual while xor",
"ELEMENT DOCTYPE ATTLIST ENTITY NOTATION",
"", "", "" };


EDITLEXER lexHTML = { SCLEX_HTML, L"Web Source Code", L"html; htm; asp; aspx; shtml; htd; xhtml; php; php3; phtml; htt; cfm; tpl; dtd; hta; htc", L"", &KeyWords_HTML, {
                      { STYLE_DEFAULT, L"Default", L"", L"" },
                      //{ SCE_H_DEFAULT, L"Default", L"", L"" },
                      { SCE_H_TAG, L"HTML Tag", L"fore:#000080", L"" },
                      { SCE_H_TAGUNKNOWN, L"HTML Unknown Tag", L"fore:#000080", L"" },
                      { SCE_H_ATTRIBUTE, L"HTML Attribute", L"fore:#FF0000", L"" },
                      { SCE_H_ATTRIBUTEUNKNOWN, L"HTML Unknown Attribute", L"fore:#FF0000", L"" },
                      { SCE_H_VALUE, L"HTML Value", L"fore:#3A6EA5", L"" },
                      { SCE_H_DOUBLESTRING, L"HTML String double quoted", L"fore:#3A6EA5", L"" },
                      { SCE_H_SINGLESTRING, L"HTML String single quoted", L"fore:#3A6EA5", L"" },
                      { SCE_H_OTHER, L"HTML Other Inside Tag", L"fore:#3A6EA5", L"" },
                      { SCE_H_COMMENT, L"HTML Comment", L"fore:#808080", L"" },
                      { SCE_H_ENTITY, L"HTML Entity", L"fore:#800080", L"" },
                      { SCE_H_TAGEND, L"XML Closing Tag", L"fore:#000080", L"" },
                      { SCE_H_XMLSTART, L"XML Identifier Start", L"bold; fore:#FF0000", L"" },
                      { SCE_H_XMLEND, L"XML Identifier End", L"bold; fore:#FF0000", L"" },
                      { SCE_H_ASP, L"ASP Start Tag", L"", L"fore:#000080; bold" },
                      { SCE_H_ASPAT, L"ASP Start Tag @", L"fore:#000080; bold", L"" },
                      //{ SCE_H_SCRIPT, L"Script", L"", L"" },
                      { SCE_H_CDATA, L"CDATA", L"", L"" },
                      //{ SCE_H_XCCOMMENT, L"XC Comment", L"", L"" },
                      { SCE_H_QUESTION, L"PHP Start Tag", L"fore:#000080; bold", L"" },
                      { SCE_HPHP_DEFAULT, L"PHP Default", L"", L"" },
                      { SCE_HPHP_HSTRING, L"PHP String", L"fore:#008000", L"" },
                      { SCE_HPHP_SIMPLESTRING, L"PHP Simple String", L"fore:#008000", L"" },
                      { SCE_HPHP_WORD, L"PHP Keyword", L"fore:#800080", L"" },
                      { SCE_HPHP_NUMBER, L"PHP Number", L"fore:#FF0000", L"" },
                      { SCE_HPHP_VARIABLE, L"PHP Variable", L"italic; fore:#000080", L"" },
                      { SCE_HPHP_HSTRING_VARIABLE, L"PHP String Variable", L"italic; fore:#000080", L"" },
                      { SCE_HPHP_COMPLEX_VARIABLE, L"PHP Complex Variable", L"italic; fore:#000080", L"" },
                      { SCE_HPHP_COMMENT, L"PHP Comment", L"fore:#FF8000", L"" },
                      { SCE_HPHP_COMMENTLINE, L"PHP Comment Line", L"fore:#FF8000", L"" },
                      { SCE_HPHP_OPERATOR, L"PHP Operator", L"", L"" },
                      //{ SCE_HJ_START, L"JS Start", L"", L"" },
                      { SCE_HJ_DEFAULT, L"JS Default", L"", L"" },
                      { SCE_HJ_COMMENT, L"JS Comment", L"fore:#008000", L"" },
                      { SCE_HJ_COMMENTLINE, L"JS Comment Line", L"fore:#008000", L"" },
                      { SCE_HJ_COMMENTDOC, L"JS Comment Doc", L"fore:#008000", L"" },
                      { SCE_HJ_NUMBER, L"JS Number", L"fore:#FF0000", L"" },
                      { SCE_HJ_WORD, L"JS Identifier", L"", L"" },
                      { SCE_HJ_KEYWORD, L"JS Keyword", L"fore:#0A246A; bold", L"" },
                      { SCE_HJ_DOUBLESTRING, L"JS Double String", L"fore:#008000", L"" },
                      { SCE_HJ_SINGLESTRING, L"JS Single String", L"fore:#008000", L"" },
                      { SCE_HJ_STRINGEOL, L"JS String EOL", L"fore:#008000", L"" },
                      { SCE_HJ_SYMBOLS, L"JS Symbols", L"", L"" },
                      { SCE_HJ_REGEX, L"JS Regex", L"", L"" },
                      //{ SCE_HJA_START, L"ASP JS Start", L"", L"" },
                      { SCE_HJA_DEFAULT, L"ASP JS Default", L"", L"" },
                      { SCE_HJA_COMMENT, L"ASP JS Comment", L"fore:#008000", L"" },
                      { SCE_HJA_COMMENTLINE, L"ASP JS Commentline", L"fore:#008000", L"" },
                      { SCE_HJA_COMMENTDOC, L"ASP JS Comment Doc", L"fore:#008000", L"" },
                      { SCE_HJA_NUMBER, L"ASP JS Number", L"fore:#FF0000", L"" },
                      { SCE_HJA_WORD, L"ASP JS Identifier", L"", L"" },
                      { SCE_HJA_KEYWORD, L"ASP JS Keyword", L"fore:#0A246A; bold", L"" },
                      { SCE_HJA_DOUBLESTRING, L"ASP JS Double String", L"fore:#008000", L"" },
                      { SCE_HJA_SINGLESTRING, L"ASP JS Single String", L"fore:#008000", L"" },
                      { SCE_HJA_STRINGEOL, L"ASP JS String EOL", L"fore:#008000", L"" },
                      { SCE_HJA_SYMBOLS, L"ASP JS Symbols", L"", L"" },
                      { SCE_HJA_REGEX, L"ASP JS Regex", L"", L"" },
                      //{ SCE_HB_START, L"VBS Start", L"", L"" },
                      { SCE_HB_DEFAULT, L"VBS Default", L"", L"" },
                      { SCE_HB_COMMENTLINE, L"VBS Comment Line", L"fore:#008000", L"" },
                      { SCE_HB_NUMBER, L"VBS Number", L"fore:#FF0000", L"" },
                      { SCE_HB_WORD, L"VBS Keyword", L"fore:#800080; bold", L"" },
                      { SCE_HB_STRING, L"VBS String", L"fore:#008000", L"" },
                      { SCE_HB_STRINGEOL, L"VBS String EOL", L"fore:#008000", L"" },
                      { SCE_HB_IDENTIFIER, L"VBS Identifier", L"", L"" },
                      //{ SCE_HBA_START, L"ASP VBS Start", L"", L"" },
                      { SCE_HBA_DEFAULT, L"ASP VBS Default", L"", L"" },
                      { SCE_HBA_COMMENTLINE, L"ASP VBS Comment Line", L"fore:#008000", L"" },
                      { SCE_HBA_NUMBER, L"ASP VBS Number", L"", L"" },
                      { SCE_HBA_WORD, L"ASP VBS Keyword", L"fore:#800080; bold", L"" },
                      { SCE_HBA_STRING, L"ASP VBS String", L"fore:#008000", L"" },
                      { SCE_HBA_STRINGEOL, L"ASP VBS String EOL", L"fore:#008000", L"" },
                      { SCE_HBA_IDENTIFIER, L"ASP VBS Identifier", L"", L"" },
                      //{ SCE_HP_START, L"Phyton Start", L"", L"" },
                      //{ SCE_HP_DEFAULT, L"Phyton Default", L"", L"" },
                      //{ SCE_HP_COMMENTLINE, L"Phyton Comment Line", L"", L"" },
                      //{ SCE_HP_NUMBER, L"Phyton Number", L"", L"" },
                      //{ SCE_HP_STRING, L"Phyton String", L"", L"" },
                      //{ SCE_HP_CHARACTER, L"Phyton Character", L"", L"" },
                      //{ SCE_HP_WORD, L"Phyton Keyword", L"", L"" },
                      //{ SCE_HP_TRIPLE, L"Phyton Triple", L"", L"" },
                      //{ SCE_HP_TRIPLEDOUBLE, L"Phyton Triple Double", L"", L"" },
                      //{ SCE_HP_CLASSNAME, L"Phyton Class Name", L"", L"" },
                      //{ SCE_HP_DEFNAME, L"Phyton Def Name", L"", L"" },
                      //{ SCE_HP_OPERATOR, L"Phyton Operator", L"", L"" },
                      //{ SCE_HP_IDENTIFIER, L"Phyton Identifier", L"", L"" },
                      //{ SCE_HPA_START, L"ASP Phyton Start", L"", L"" },
                      //{ SCE_HPA_DEFAULT, L"ASP Phyton Default", L"", L"" },
                      //{ SCE_HPA_COMMENTLINE, L"ASP Phyton Comment Line", L"", L"" },
                      //{ SCE_HPA_NUMBER, L"ASP Phyton Number", L"", L"" },
                      //{ SCE_HPA_STRING, L"ASP Phyton String", L"", L"" },
                      //{ SCE_HPA_CHARACTER, L"ASP Phyton Character", L"", L"" },
                      //{ SCE_HPA_WORD, L"ASP Phyton Keyword", L"", L"" },
                      //{ SCE_HPA_TRIPLE, L"ASP Phyton Triple", L"", L"" },
                      //{ SCE_HPA_TRIPLEDOUBLE, L"ASP Phyton Triple Double", L"", L"" },
                      //{ SCE_HPA_CLASSNAME, L"ASP Phyton Class Name", L"", L"" },
                      //{ SCE_HPA_DEFNAME, L"ASP Phyton Def Name", L"", L"" },
                      //{ SCE_HPA_OPERATOR, L"ASP Phyton Operator", L"", L"" },
                      //{ SCE_HPA_IDENTIFIER, L"ASP Phyton Identifier", L"", L"" },
                      //{ SCE_H_SGML_DEFAULT, L"SGML Default", L"", L"" },
                      //{ SCE_H_SGML_COMMAND, L"SGML Command", L"",L"" },
                      //{ SCE_H_SGML_1ST_PARAM, L"SGML 1st Param", L"",L"" },
                      //{ SCE_H_SGML_DOUBLESTRING, L"SGML Double String", L"", L"" },
                      //{ SCE_H_SGML_SIMPLESTRING, L"SGML Single Stirng", L"", L"" },
                      //{ SCE_H_SGML_ERROR, L"SGML Error", L"", L"" },
                      //{ SCE_H_SGML_SPECIAL, L"SGML Special", L"", L"" },
                      //{ SCE_H_SGML_ENTITY, L"SGML Entity", L"", L"" },
                      //{ SCE_H_SGML_COMMENT, L"SGML Comment", L"", L"" },
                      //{ SCE_H_SGML_1ST_PARAM_COMMENT, L"SGML 1st Param Comment", L"", L"" },
                      //{ SCE_H_SGML_BLOCK_DEFAULT, L"SGML Block Default", L"", L"" },
                      { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_XML = {
"", "", "", "", "", "ELEMENT DOCTYPE ATTLIST ENTITY NOTATION", "", "", "" };


EDITLEXER lexXML = { SCLEX_XML, L"XML Document", L"xml; xsl; rss; svg; xul; xsd; xslt; axl; rdf; vcproj; manifest", L"", &KeyWords_XML, {
                     { STYLE_DEFAULT, L"Default", L"", L"" },
                     //{ SCE_H_DEFAULT, L"Default", L"", L"" },
                     { SCE_H_TAG, L"XML Tag", L"fore:#800080", L"" },
                     { SCE_H_TAGEND, L"XML Closing Tag", L"fore:#800080", L"" },
                     //{ SCE_H_TAGUNKNOWN, L"XML Unknown Tag", L"fore:#000080", L"" },
                     { SCE_H_ATTRIBUTE, L"XML Attribute", L"fore:#FF0000", L"" },
                     //{ SCE_H_ATTRIBUTEUNKNOWN, L"XML Unknown Attribute", L"fore:#FF0000", L"" },
                     { SCE_H_VALUE, L"XML Value", L"fore:#3A6EA5", L"" },
                     { SCE_H_DOUBLESTRING, L"XML String double quoted", L"fore:#3A6EA5", L"" },
                     { SCE_H_SINGLESTRING, L"XML String single quoted", L"fore:#3A6EA5", L"" },
                     { SCE_H_OTHER, L"XML Other Inside Tag", L"fore:#3A6EA5", L"" },
                     { SCE_H_COMMENT, L"XML Comment", L"fore:#808080", L"" },
                     { SCE_H_ENTITY, L"XML Entity", L"fore:#800080", L"" },
                     { SCE_H_XMLSTART, L"XML Identifier Start", L"bold; fore:#FF0000", L"" },
                     { SCE_H_XMLEND, L"XML Identifier End", L"bold; fore:#FF0000", L"" },
                     //{ SCE_H_ASP, L"ASP Start Tag", L"", "fore:#000080; bold" },
                     //{ SCE_H_ASPAT, L"ASP Start Tag @", L"fore:#000080; bold", L"" },
                     //{ SCE_H_SCRIPT, L"Script", L"", L"" },
                     { SCE_H_CDATA, L"CDATA", L"fore:#008000", L"" },
                     //{ SCE_H_XCCOMMENT, L"XC Comment", L"", L"" },
                     //{ SCE_H_QUESTION, L"PHP Start Tag", L"fore:#000080; bold", L"" },
                     //{ SCE_HPHP_DEFAULT, L"PHP Default", L"", L"" },
                     //{ SCE_HPHP_HSTRING, L"PHP String", L"fore:#008000", L"" },
                     //{ SCE_HPHP_SIMPLESTRING, L"PHP Simple String", L"fore:#008000", L"" },
                     //{ SCE_HPHP_WORD, L"PHP Keyword", L"fore:#800080", L"" },
                     //{ SCE_HPHP_NUMBER, L"PHP Number", L"fore:#FF0000", L"" },
                     //{ SCE_HPHP_VARIABLE, L"PHP Variable", L"italic; fore:#000080", L"" },
                     //{ SCE_HPHP_HSTRING_VARIABLE, L"PHP String Variable", L"italic; fore:#000080", L"" },
                     //{ SCE_HPHP_COMPLEX_VARIABLE, L"PHP Complex Variable", L"italic; fore:#000080", L"" },
                     //{ SCE_HPHP_COMMENT, L"PHP Comment", L"fore:#FF8000", L"" },
                     //{ SCE_HPHP_COMMENTLINE, L"PHP Comment Line", L"fore:#FF8000", L"" },
                     //{ SCE_HPHP_OPERATOR, L"PHP Operator", L"", L"" },
                     //{ SCE_HJ_START, L"JS Start", L"", L"" },
                     //{ SCE_HJ_DEFAULT, L"JS Default", L"", L"" },
                     //{ SCE_HJ_COMMENT, L"JS Comment", L"fore:#008000", L"" },
                     //{ SCE_HJ_COMMENTLINE, L"JS Comment Line", L"fore:#008000", L"" },
                     //{ SCE_HJ_COMMENTDOC, L"JS Comment Doc", L"fore:#008000", L"" },
                     //{ SCE_HJ_NUMBER, L"JS Number", L"fore:#FF0000", L"" },
                     //{ SCE_HJ_WORD, L"JS Identifier", L"", L"" },
                     //{ SCE_HJ_KEYWORD, L"JS Keyword", L"fore:#0A246A; bold", L"" },
                     //{ SCE_HJ_DOUBLESTRING, L"JS Double String", L"fore:#008000", L"" },
                     //{ SCE_HJ_SINGLESTRING, L"JS Single String", L"fore:#008000", L"" },
                     //{ SCE_HJ_STRINGEOL, L"JS String EOL", L"fore:#008000", L"" },
                     //{ SCE_HJ_SYMBOLS, L"JS Symbols", L"", L"" },
                     //{ SCE_HJ_REGEX, L"JS Regex", L"", L"" },
                     //{ SCE_HJA_START, L"ASP JS Start", L"", L"" },
                     //{ SCE_HJA_DEFAULT, L"ASP JS Default", L"", L"" },
                     //{ SCE_HJA_COMMENT, L"ASP JS Comment", L"fore:#008000", L"" },
                     //{ SCE_HJA_COMMENTLINE, L"ASP JS Commentline", L"fore:#008000", L"" },
                     //{ SCE_HJA_COMMENTDOC, L"ASP JS Comment Doc", L"fore:#008000", L"" },
                     //{ SCE_HJA_NUMBER, L"ASP JS Number", L"fore:#FF0000", L"" },
                     //{ SCE_HJA_WORD, L"ASP JS Identifier", L"", L"" },
                     //{ SCE_HJA_KEYWORD, L"ASP JS Keyword", L"fore:#0A246A; bold", L"" },
                     //{ SCE_HJA_DOUBLESTRING, L"ASP JS Double String", L"fore:#008000", L"" },
                     //{ SCE_HJA_SINGLESTRING, L"ASP JS Single String", L"fore:#008000", L"" },
                     //{ SCE_HJA_STRINGEOL, L"ASP JS String EOL", L"fore:#008000", L"" },
                     //{ SCE_HJA_SYMBOLS, L"ASP JS Symbols", L"", L"" },
                     //{ SCE_HJA_REGEX, L"ASP JS Regex", L"", L"" },
                     //{ SCE_HB_START, L"VBS Start", L"", L"" },
                     //{ SCE_HB_DEFAULT, L"VBS Default", L"", L"" },
                     //{ SCE_HB_COMMENTLINE, L"VBS Comment Line", L"fore:#008000", L"" },
                     //{ SCE_HB_NUMBER, L"VBS Number", L"fore:#FF0000", L"" },
                     //{ SCE_HB_WORD, L"VBS Keyword", L"fore:#800080; bold", L"" },
                     //{ SCE_HB_STRING, L"VBS String", L"fore:#008000", L"" },
                     //{ SCE_HB_STRINGEOL, L"VBS String EOL", L"fore:#008000", L"" },
                     //{ SCE_HB_IDENTIFIER, L"VBS Identifier", L"", L"" },
                     //{ SCE_HBA_START, L"ASP VBS Start", L"", L"" },
                     //{ SCE_HBA_DEFAULT, L"ASP VBS Default", L"", L"" },
                     //{ SCE_HBA_COMMENTLINE, L"ASP VBS Comment Line ", L"fore:#008000", L"" },
                     //{ SCE_HBA_NUMBER, L"ASP VBS Number", L"", L"" },
                     //{ SCE_HBA_WORD, L"ASP VBS Keyword", L"fore:#800080; bold", L"" },
                     //{ SCE_HBA_STRING, L"ASP VBS String", L"fore:#008000", L"" },
                     //{ SCE_HBA_STRINGEOL, L"ASP VBS String EOL", L"fore:#008000", L"" },
                     //{ SCE_HBA_IDENTIFIER, L"ASP VBS Identifier", L"", L"" },
                     //{ SCE_HP_START, L"Phyton Start", L"", L"" },
                     //{ SCE_HP_DEFAULT, L"Phyton Default", L"", L"" },
                     //{ SCE_HP_COMMENTLINE, L"Phyton Comment Line", L"", L"" },
                     //{ SCE_HP_NUMBER, L"Phyton Number", L"", L"" },
                     //{ SCE_HP_STRING, L"Phyton String", L"", L"" },
                     //{ SCE_HP_CHARACTER, L"Phyton Character", L"", L"" },
                     //{ SCE_HP_WORD, L"Phyton Keyword", L"", L"" },
                     //{ SCE_HP_TRIPLE, L"Phyton Triple", L"", L"" },
                     //{ SCE_HP_TRIPLEDOUBLE, L"Phyton Triple Double", L"", L"" },
                     //{ SCE_HP_CLASSNAME, L"Phyton Class Name", L"", L"" },
                     //{ SCE_HP_DEFNAME, L"Phyton Def Name", L"", L"" },
                     //{ SCE_HP_OPERATOR, L"Phyton Operator", L"", L"" },
                     //{ SCE_HP_IDENTIFIER, L"Phyton Identifier", L"", L"" },
                     //{ SCE_HPA_START, L"ASP Phyton Start", L"", L"" },
                     //{ SCE_HPA_DEFAULT, L"ASP Phyton Default", L"", L"" },
                     //{ SCE_HPA_COMMENTLINE, L"ASP Phyton Comment Line", L"", L"" },
                     //{ SCE_HPA_NUMBER, L"ASP Phyton Number", L"", L"" },
                     //{ SCE_HPA_STRING, L"ASP Phyton String", L"", L"" },
                     //{ SCE_HPA_CHARACTER, L"ASP Phyton Character", L"", L"" },
                     //{ SCE_HPA_WORD, L"ASP Phyton Keyword", L"", L"" },
                     //{ SCE_HPA_TRIPLE, L"ASP Phyton Triple", L"", L"" },
                     //{ SCE_HPA_TRIPLEDOUBLE, L"ASP Phyton Triple Double", L"", L"" },
                     //{ SCE_HPA_CLASSNAME, L"ASP Phyton Class Name", L"", L"" },
                     //{ SCE_HPA_DEFNAME, L"ASP Phyton Def Name", L"", L"" },
                     //{ SCE_HPA_OPERATOR, L"ASP Phyton Operator", L"", L"" },
                     //{ SCE_HPA_IDENTIFIER, L"ASP Phyton Identifier", L"", L"" },
                     //{ SCE_H_SGML_DEFAULT, L"SGML Default", L"", L"" },
                     //{ SCE_H_SGML_COMMAND, L"SGML Command", L"",L"" },
                     //{ SCE_H_SGML_1ST_PARAM, L"SGML 1st Param", L"",L"" },
                     //{ SCE_H_SGML_DOUBLESTRING, L"SGML Double String", L"", L"" },
                     //{ SCE_H_SGML_SIMPLESTRING, L"SGML Single Stirng", L"", L"" },
                     //{ SCE_H_SGML_ERROR, L"SGML Error", L"", L"" },
                     //{ SCE_H_SGML_SPECIAL, L"SGML Special", L"", L"" },
                     //{ SCE_H_SGML_ENTITY, L"SGML Entity", L"", L"" },
                     //{ SCE_H_SGML_COMMENT, L"SGML Comment", L"", L"" },
                     //{ SCE_H_SGML_1ST_PARAM_COMMENT, L"SGML 1st Param Comment", L"", L"" },
                     //{ SCE_H_SGML_BLOCK_DEFAULT, L"SGML Block Default", L"", L"" },
                     { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_CSS = {
"background background-attachment background-color background-image "
"background-position background-repeat border border-bottom "
"border-bottom-width border-color border-left border-left-width border-right "
"border-right-width border-style border-top border-top-width border-width "
"clear color display float font font-family font-size font-style font-variant "
"font-weight height letter-spacing line-height list-style list-style-image "
"list-style-position list-style-type margin margin-bottom margin-left "
"margin-right margin-top padding padding-bottom padding-left padding-right "
"padding-top text-align text-decoration text-indent text-transform "
"vertical-align white-space width word-spacing",
"first-letter first-line link active visited first-child "
"focus hover lang before after left right first",
"ascent azimuth baseline bbox border-bottom-color border-bottom-style "
"border-collapse border-color border-left-color border-left-style "
"border-right-color border-right-style border-spacing border-style "
"border-top-color border-top-style bottom cap-height caption-side centerline "
"clip content counter-increment counter-reset cue cue-after cue-before cursor "
"definition-src descent direction elevation empty-cells font-size-adjust "
"font-stretch left marker-offset marks mathline max-height max-width "
"min-height min-width orphans outline outline-color outline-style "
"outline-width overflow page page-break-after page-break-before "
"page-break-inside panose-1 pause pause-after pause-before pitch pitch-range "
"play-during position quotes richness right size slope speak speak-header "
"speak-numeral speak-punctuation speech-rate src stemh stemv stress "
"table-layout text-shadow top topline unicode-bidi unicode-range units-per-em "
"visibility voice-family volume widows widths x-height z-index",
"", "", "", "", "", "" };


EDITLEXER lexCSS = { SCLEX_CSS, L"CSS Style Sheets", L"css", L"", &KeyWords_CSS, {
                     { STYLE_DEFAULT, L"Default", L"", L"" },
                     //{ SCE_CSS_DEFAULT, L"Default", L"", L"" },
                     { SCE_CSS_TAG, L"Tag", L"fore:#0A246A", L"" },
                     { SCE_CSS_CLASS, L"Class", L"fore:#FF0000", L"" },
                     { SCE_CSS_PSEUDOCLASS, L"Pseudoclass", L"fore:#FF0000", L"" },
                     { SCE_CSS_UNKNOWN_PSEUDOCLASS, L"Unknown Pseudoclass", L"fore:#FF0000", L"" },
                     { SCE_CSS_OPERATOR, L"Operator", L"", L"" },
                     { SCE_CSS_IDENTIFIER, L"Identifier", L"fore:#3A6EA5", L"" },
                     { SCE_CSS_UNKNOWN_IDENTIFIER, L"Unknown Identifier", L"", L"" },
                     { SCE_CSS_IDENTIFIER2, L"CSS 2 Keyword", L"fore:#3A6EA5", L"" },
                     { SCE_CSS_VALUE, L"Value", L"", L"" },
                     { SCE_CSS_COMMENT, L"Comment", L"fore:#008000", L"" },
                     { SCE_CSS_ID, L"Id", L"", L"" },
                     { SCE_CSS_IMPORTANT, L"Important", L"", L"" },
                     { SCE_CSS_DIRECTIVE, L"Directive", L"", L"" },
                     { SCE_CSS_ATTRIBUTE, L"Attribute", L"italic", L"" },
                     { SCE_CSS_DOUBLESTRING, L"String double quoted", L"fore:#008000", L"" },
                     { SCE_CSS_SINGLESTRING, L"String single quoted", L"fore:#008000", L"" },
                     { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_CPP = {
"and and_eq asm auto bitand bitor bool break case catch char class "
"compl const const_cast continue default delete do double "
"dynamic_cast else enum explicit export extern false float for friend "
"goto if inline int long mutable namespace new not not_eq operator or "
"or_eq private protected public register reinterpret_cast return short "
"signed sizeof static static_cast struct switch template this throw true "
"try typedef typeid typename union unsigned using virtual void volatile "
"wchar_t while xor xor_eq",
"",
"a addindex addtogroup anchor arg attention author b brief bug c class code "
"date def defgroup deprecated dontinclude e em endcode endhtmlonly endif "
"endlatexonly endlink endverbatim enum example exception f$ f[ f] file fn "
"hideinitializer htmlinclude htmlonly if image include ingroup internal invariant "
"interface latexonly li line link mainpage name namespace nosubgrouping note "
"overload p page par param post pre ref relates remarks return retval sa "
"section see showinitializer since skip skipline struct subsection test throw "
"todo typedef union until var verbatim verbinclude version warning "
"weakgroup $ @ \\ & < > # { }",
"", "", "", "", "", "" };


EDITLEXER lexCPP = { SCLEX_CPP, L"C/C++ Source Code", L"c; cpp; cxx; cc; h; hpp; hxx; hh", L"", &KeyWords_CPP, {
                     { STYLE_DEFAULT, L"Default", L"", L"" },
                     //{ SCE_C_DEFAULT, L"Default", L"", L"" },
                     { SCE_C_COMMENT, L"Comment", L"fore:#008000", L"" },
                     { SCE_C_COMMENTLINE, L"Comment Line", L"fore:#008000", L"" },
                     { SCE_C_COMMENTDOC, L"Comment Doc", L"fore:#008000", L"" },
                     { SCE_C_COMMENTLINEDOC, L"Comment Line Doc", L"fore:#008000", L"" },
                     { SCE_C_WORD, L"Keyword", L"fore:#0A246A; bold", L"" },
                     { SCE_C_IDENTIFIER, L"Identifier", L"", L"" },
                     { SCE_C_NUMBER, L"Number", L"fore:#FF0000", L"" },
                     { SCE_C_STRING, L"String", L"fore:#008000", L"" },
                     { SCE_C_STRINGEOL, L"String not closed", L"fore:#008000; back:#FFFF80; eolfilled", L"" },
                     { SCE_C_CHARACTER, L"Character", L"fore:#008000", L"" },
                     //{ SCE_C_UUID, L"UUID", L"", L"" },
                     { SCE_C_OPERATOR, L"Operator", L"fore:#0A246A", L"" },
                     { SCE_C_PREPROCESSOR, L"Preprocessor", L"fore:#FF8000", L"" },
                     //{ SCE_C_VERBATIM, L"Verbatim", L"", L"" },
                     //{ SCE_C_REGEX, L"Regex", L"", L"" },
                     //{ SCE_C_WORD2, L"Word 2", L"", L"" },
                     { SCE_C_COMMENTDOCKEYWORD, L"Comment Doc Keyword", L"fore:#008000", L"" },
                     { SCE_C_COMMENTDOCKEYWORDERROR, L"Comment Doc Keyword Error", L"fore:#008000", L"" },
                     //{ SCE_C_GLOBALCLASS, L"Global Class", L"", L"" },
                     { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_CS = {
"abstract as base bool break byte case catch char checked class const "
"continue decimal default delegate do double else enum event explicit "
"extern false finally fixed float for foreach goto if implicit in int interface "
"internal is lock long namespace new null object operator out override "
"params private protected public readonly ref return sbyte sealed short "
"sizeof stackalloc static string struct switch this throw true try typeof "
"uint ulong unchecked unsafe ushort using virtual void while",
"",
"a addindex addtogroup anchor arg attention author b brief bug c class code "
"date def defgroup deprecated dontinclude e em endcode endhtmlonly endif "
"endlatexonly endlink endverbatim enum example exception f$ f[ f] file fn "
"hideinitializer htmlinclude htmlonly if image include ingroup internal invariant "
"interface latexonly li line link mainpage name namespace nosubgrouping note "
"overload p page par param post pre ref relates remarks return retval sa "
"section see showinitializer since skip skipline struct subsection test throw todo "
"typedef union until var verbatim verbinclude version warning weakgroup $ @ "
"\\ & < > # { }",
"", "", "", "", "", "" };


EDITLEXER lexCS = { SCLEX_CPP, L"C# Source Code", L"cs", L"", &KeyWords_CS, {
                    { STYLE_DEFAULT, L"Default", L"", L"" },
                    //{ SCE_C_DEFAULT, L"Default", L"", L"" },
                    { SCE_C_COMMENT, L"Comment", L"fore:#008000", L"" },
                    { SCE_C_COMMENTLINE, L"Comment Line", L"fore:#008000", L"" },
                    { SCE_C_COMMENTDOC, L"Comment Doc", L"fore:#008000", L"" },
                    { SCE_C_COMMENTLINEDOC, L"Comment Line Doc", L"fore:#008000", L"" },
                    { SCE_C_WORD, L"Keyword", L"fore:#0A246A; bold", L"" },
                    { SCE_C_IDENTIFIER, L"Identifier", L"", L"" },
                    { SCE_C_NUMBER, L"Number", L"fore:#FF0000", L"" },
                    { SCE_C_STRING, L"String", L"fore:#008000", L"" },
                    { SCE_C_VERBATIM, L"Verbatim String", L"fore:#008000", L"" },
                    { SCE_C_STRINGEOL, L"String not closed", L"fore:#008000; back:#FFFF80; eolfilled", L"" },
                    { SCE_C_CHARACTER, L"Character", L"fore:#008000", L"" },
                    //{ SCE_C_UUID, L"UUID", L"", L"" },
                    { SCE_C_OPERATOR, L"Operator", L"fore:#0A246A", L"" },
                    { SCE_C_PREPROCESSOR, L"Preprocessor", L"fore:#FF8000", L"" },
                    //{ SCE_C_REGEX, L"Regex", L"", L"" },
                    //{ SCE_C_WORD2, L"Word 2", L"", L"" },
                    { SCE_C_COMMENTDOCKEYWORD, L"Comment Doc Keyword", L"fore:#008000", L"" },
                    { SCE_C_COMMENTDOCKEYWORDERROR, L"Comment Doc Keyword Error", L"fore:#008000", L"" },
                    //{ SCE_C_GLOBALCLASS, L"Global Class", L"", L"" },
                    { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_RC = {
"ACCELERATORS ALT AUTO3STATE AUTOCHECKBOX AUTORADIOBUTTON "
"BEGIN BITMAP BLOCK BUTTON CAPTION CHARACTERISTICS CHECKBOX "
"CLASS COMBOBOX CONTROL CTEXT CURSOR DEFPUSHBUTTON DIALOG "
"DIALOGEX DISCARDABLE EDITTEXT END EXSTYLE FONT GROUPBOX "
"ICON LANGUAGE LISTBOX LTEXT MENU MENUEX MENUITEM "
"MESSAGETABLE POPUP PUSHBUTTON RADIOBUTTON RCDATA RTEXT "
"SCROLLBAR SEPARATOR SHIFT STATE3 STRINGTABLE STYLE "
"TEXTINCLUDE VALUE VERSION VERSIONINFO VIRTKEY",
"", "", "", "", "", "", "", "" };


EDITLEXER lexRC = { SCLEX_CPP, L"Resource Script", L"rc; rc2; rct; dlg", L"", &KeyWords_RC, {
                    { STYLE_DEFAULT, L"Default", L"", L"" },
                    //{ SCE_C_DEFAULT, L"Default", L"", L"" },
                    { SCE_C_COMMENT, L"Comment", L"fore:#008000", L"" },
                    { SCE_C_COMMENTLINE, L"Comment Line", L"fore:#008000", L"" },
                    { SCE_C_COMMENTDOC, L"Comment Doc", L"fore:#008000", L"" },
                    { SCE_C_WORD, L"Keyword", L"fore:#0A246A; bold", L"" },
                    { SCE_C_IDENTIFIER, L"Identifier", L"", L"" },
                    { SCE_C_NUMBER, L"Number", L"fore:#FF0000", L"" },
                    { SCE_C_STRING, L"String", L"fore:#008000", L"" },
                    { SCE_C_STRINGEOL, L"String not closed", L"fore:#008000; back:#FFFF80; eolfilled", L"" },
                    { SCE_C_CHARACTER, L"Character", L"fore:#008000", L"" },
                    //{ SCE_C_UUID, L"UUID", L"", L"" },
                    { SCE_C_OPERATOR, L"Operator", L"fore:#0A246A", L"" },
                    { SCE_C_PREPROCESSOR, L"Preprocessor", L"fore:#FF8000", L"" },
                    //{ SCE_C_VERBATIM, L"Verbatim", L"", L"" },
                    //{ SCE_C_REGEX, L"Regex", L"", L"" },
                    //{ SCE_C_COMMENTLINEDOC, L"Comment Line Doc", L"", L"" },
                    //{ SCE_C_WORD2, L"Word 2", L"", L"" },
                    //{ SCE_C_COMMENTDOCKEYWORD, L"Comment Doc Keyword", L"", L"" },
                    //{ SCE_C_COMMENTDOCKEYWORDERROR, L"Comment Doc Keyword Error", L"", L"" },
                    //{ SCE_C_GLOBALCLASS, L"Global Class", L"", L"" },
                    { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_MAK = {
"", "", "", "", "", "", "", "", "" };


EDITLEXER lexMAK = { SCLEX_MAKEFILE, L"Makefiles", L"mak; make; dsp", L"", &KeyWords_MAK, {
                     { STYLE_DEFAULT, L"Default", L"", L"" },
                     //{ SCE_MAKE_DEFAULT, L"Default", L"", L"" },
                     { SCE_MAKE_COMMENT, L"Comment", L"fore:#008000", L"" },
                     { SCE_MAKE_PREPROCESSOR, L"Preprocessor", L"fore:#FF8000", L"" },
                     { SCE_MAKE_IDENTIFIER, L"Identifier", L"", L"" },
                     { SCE_MAKE_OPERATOR, L"Operator", L"", L"" },
                     { SCE_MAKE_TARGET, L"Target", L"", L"" },
                     { SCE_MAKE_IDEOL, L"ID EOL", L"", L"" },
                     { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_VBS = {
"and begin case call continue do each else elseif end erase error event exit false "
"for function get gosub goto if implement in load loop lset me mid new next not nothing "
"on or property raiseevent rem resume return rset select set stop sub then to true unload "
"until wend while with withevents attribute alias as boolean byref byte byval const compare "
"currency date declare dim double enum explicit friend global integer let lib long module object "
"option optional preserve private property public redim single static string type variant",
"", "", "", "", "", "", "", "" };


EDITLEXER lexVBS = { SCLEX_VBSCRIPT, L"VBScript", L"vbs; dsm", L"", &KeyWords_VBS, {
                    { STYLE_DEFAULT, L"Default", L"", L"" },
                    //{ SCE_B_DEFAULT, L"Default", L"", L"" },
                    { SCE_B_COMMENT, L"Comment", L"fore:#008000", L"" },
                    { SCE_B_NUMBER, L"Number", L"fore:#FF0000", L"" },
                    { SCE_B_KEYWORD, L"Keyword", L"fore:#0A246A; bold", L"" },
                    { SCE_B_STRING, L"String", L"fore:#008000", L"" },
                    { SCE_B_STRINGEOL, L"String not closed", L"fore:#008000; back:#FFFF80; eolfilled", L"" },
                    { SCE_B_OPERATOR, L"Operator", L"", L"" },
                    { SCE_B_IDENTIFIER, L"Identifier", L"", L"" },
                    //{ SCE_B_DATE, L"Date", L"", L"" },
                    //{ SCE_B_KEYWORD2, L"Keyword 2", L"", L"" },
                    //{ SCE_B_KEYWORD3, L"Keyword 3", L"", L"" },
                    //{ SCE_B_KEYWORD4, L"Keyword 4", L"", L"" },
                    { SCE_B_CONSTANT, L"Constant", L"", L"" },
                    { SCE_B_PREPROCESSOR, L"Preprocessor", L"#fore:#808080", L"" },
                    { SCE_B_ASM, L"Assembler", L"fore:#FF8000", L"" },
                    { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_VB = {
"addhandler addressof andalso alias and ansi as assembly auto boolean byref "
"byte byval call case catch cbool cbyte cchar cdate cdec cdbl char cint class "
"clng cobj const cshort csng cstr ctype date decimal declare default delegate "
"dim do double each else elseif end enum erase error event exit false finally for "
"friend function get gettype goto  handles if implements imports in inherits integer "
"interface is let lib like long loop me mod module mustinherit mustoverride "
"mybase myclass namespace new next not nothing notinheritable notoverridable "
"object on option optional or orelse overloads overridable overrides paramarray "
"preserve private property protected public raiseevent readonly redim rem "
"removehandler resume return select set shadows shared short single static step "
"stop string structure sub synclock then throw to true try typeof unicode until "
"variant when while with withevents writeonly xor",
"", "", "", "", "", "", "", "" };


EDITLEXER lexVB = { SCLEX_VB, L"Visual Basic", L"vb; bas; frm; cls; ctl; pag; dsr; dob", L"", &KeyWords_VB, {
                    { STYLE_DEFAULT, L"Default", L"", L"" },
                    //{ SCE_B_DEFAULT, L"Default", L"", L"" },
                    { SCE_B_COMMENT, L"Comment", L"fore:#008000", L"" },
                    { SCE_B_NUMBER, L"Number", L"fore:#FF0000", L"" },
                    { SCE_B_KEYWORD, L"Keyword", L"fore:#0A246A; bold", L"" },
                    { SCE_B_STRING, L"String", L"fore:#008000", L"" },
                    { SCE_B_STRINGEOL, L"String not closed", L"fore:#008000; back:#FFFF80; eolfilled", L"" },
                    { SCE_B_OPERATOR, L"Operator", L"", L"" },
                    { SCE_B_IDENTIFIER, L"Identifier", L"", L"" },
                    //{ SCE_B_DATE, L"Date", L"", L"" },
                    //{ SCE_B_KEYWORD2, L"Keyword 2", L"", L"" },
                    //{ SCE_B_KEYWORD3, L"Keyword 3", L"", L"" },
                    //{ SCE_B_KEYWORD4, L"Keyword 4", L"", L"" },
                    { SCE_B_CONSTANT, L"Constant", L"", L"" },
                    { SCE_B_PREPROCESSOR, L"Preprocessor", L"#fore:#808080", L"" },
                    { SCE_B_ASM, L"Assembler", L"fore:#FF8000", L"" },
                    { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_JS = {
"abstract boolean break byte case catch char class const continue "
"debugger default delete do double else enum export extends final "
"finally float for function goto if implements import in instanceof int "
"interface long native new package private protected public return "
"short static super switch synchronized this throw throws transient try "
"typeof var void volatile while with",
"", "", "", "", "", "", "", "" };


EDITLEXER lexJS = { SCLEX_CPP, L"JavaScript", L"js; jse; jsm; json", L"", &KeyWords_JS, {
                    { STYLE_DEFAULT, L"Default", L"", L"" },
                    //{ SCE_C_DEFAULT, L"Default", L"", L"" },
                    { SCE_C_COMMENT, L"Comment", L"fore:#008000", L"" },
                    { SCE_C_COMMENTLINE, L"Comment Line", L"fore:#008000", L"" },
                    { SCE_C_COMMENTDOC, L"Comment Doc", L"fore:#008000", L"" },
                    { SCE_C_COMMENTLINEDOC, L"Comment Line Doc", L"fore:#008000", L"" },
                    { SCE_C_WORD, L"Keyword", L"fore:#0A246A; bold", L"" },
                    { SCE_C_IDENTIFIER, L"Identifier", L"", L"" },
                    { SCE_C_NUMBER, L"Number", L"fore:#FF0000", L"" },
                    { SCE_C_STRING, L"String", L"fore:#008000", L"" },
                    { SCE_C_STRINGEOL, L"String not closed", L"fore:#008000; back:#FFFF80; eolfilled", L"" },
                    { SCE_C_CHARACTER, L"Character", L"fore:#008000", L"" },
                    //{ SCE_C_UUID, L"UUID", L"", L"" },
                    { SCE_C_OPERATOR, L"Operator", L"fore:#0A246A", L"" },
                    { SCE_C_PREPROCESSOR, L"Preprocessor", L"fore:#FF8000", L"" },
                    //{ SCE_C_VERBATIM, L"Verbatim", L"", L"" },
                    { SCE_C_REGEX, L"Regular Expression", L"fore:#008000", L"" },
                    //{ SCE_C_WORD2, L"Word 2", L"", L"" },
                    //{ SCE_C_COMMENTDOCKEYWORD, L"Comment Doc Keyword", L"", L"" },
                    //{ SCE_C_COMMENTDOCKEYWORDERROR, L"Comment Doc Keyword Error", L"", L"" },
                    //{ SCE_C_GLOBALCLASS, L"Global Class", L"", L"" },
                    { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_JAVA = {
"abstract assert boolean break byte case catch char class const "
"continue default do double else extends final finally float for future "
"generic goto if implements import inner instanceof int interface long "
"native new null outer package private protected public rest return "
"short static super switch synchronized this throw throws transient try "
"var void volatile while",
"", "", "", "", "", "", "", "" };


EDITLEXER lexJAVA = { SCLEX_CPP, L"Java Source Code", L"java", L"", &KeyWords_JAVA, {
                      { STYLE_DEFAULT, L"Default", L"", L"" },
                      //{ SCE_C_DEFAULT, L"Default", L"", L"" },
                      { SCE_C_COMMENT, L"Comment", L"fore:#008000", L"" },
                      { SCE_C_COMMENTLINE, L"Comment Line", L"fore:#008000", L"" },
                      { SCE_C_COMMENTDOC, L"Comment Doc", L"fore:#008000", L"" },
                      { SCE_C_COMMENTLINEDOC, L"Comment Line Doc", L"fore:#008000", L"" },
                      { SCE_C_WORD, L"Keyword", L"fore:#0A246A; bold", L"" },
                      { SCE_C_IDENTIFIER, L"Identifier", L"", L"" },
                      { SCE_C_NUMBER, L"Number", L"fore:#FF0000", L"" },
                      { SCE_C_STRING, L"String", L"fore:#008000", L"" },
                      { SCE_C_STRINGEOL, L"String not closed", L"fore:#008000; back:#FFFF80; eolfilled", L"" },
                      { SCE_C_CHARACTER, L"Character", L"fore:#008000", L"" },
                      //{ SCE_C_UUID, L"UUID", L"", L"" },
                      { SCE_C_OPERATOR, L"Operator", L"fore:#0A246A", L"" },
                      { SCE_C_PREPROCESSOR, L"Preprocessor", L"fore:#FF8000", L"" },
                      //{ SCE_C_VERBATIM, L"Verbatim", L"", L"" },
                      { SCE_C_REGEX, L"Regular Expression", L"fore:#008000", L"" },
                      //{ SCE_C_WORD2, L"Word 2", L"", L"" },
                      //{ SCE_C_COMMENTDOCKEYWORD, L"Comment Doc Keyword", L"", L"" },
                      //{ SCE_C_COMMENTDOCKEYWORDERROR, L"Comment Doc Keyword Error", L"", L"" },
                      //{ SCE_C_GLOBALCLASS, L"Global Class", L"", L"" },
                      { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_PAS = {
"absolute abstract alias and array as asm assembler begin break case cdecl class const constructor continue cppdecl default "
"destructor dispose div do downto else end end. except exit export exports external false far far16 file finalization finally for "
"forward function goto if implementation in index inherited initialization inline interface is label library local message mod "
"name near new nil nostackframe not object of oldfpccall on operator or out overload override packed pascal private procedure "
"program property protected public published raise read record register reintroduce repeat resourcestring safecall self set shl "
"shr softfloat stdcall stored string then threadvar to true try type unit until uses var virtual while with write xor",
"", "", "", "", "", "", "", "" };


EDITLEXER lexPAS = { SCLEX_PASCAL, L"Pascal Source Code", L"pas; dpr; dpk; dfm; inc; pp", L"", &KeyWords_PAS, {
                     { STYLE_DEFAULT, L"Default", L"", L"" },
                     //{ 0, L"Default", L"", L"" },
                     { SCE_C_COMMENT, L"Comment", L"fore:#008000", L"" },
                     { SCE_C_COMMENTLINE, L"Comment Line", L"fore:#008000", L"" },
                     { SCE_C_COMMENTDOC, L"Comment Doc", L"fore:#008000", L"" },
                     { SCE_C_WORD, L"Keyword", L"fore:#800080; bold", L"" },
                     { SCE_C_IDENTIFIER, L"Identifier", L"", L"" },
                     //{ SCE_C_STRING, L"String", L"", L"" },
                     //{ SCE_C_STRINGEOL, L"String eol", L"", L"" },
                     { SCE_C_NUMBER, L"Number", L"fore:#FF0000", L"" },
                     { SCE_C_CHARACTER, L"String", L"fore:#008000", L"" },
                     { SCE_C_OPERATOR, L"Operator", L"fore:#0A246A", L"" },
                     { SCE_C_PREPROCESSOR, L"Preprocessor", L"fore:#808080", L"" },
                     { SCE_C_REGEX, L"Inline Asm", L"fore:#FF8000", L"" },
                     //{ SCE_C_COMMENTDOCKEYWORD, L"Comment Doc Keyword", L"fore:#008000", L"" },
                     //{ SCE_C_COMMENTDOCKEYWORDERROR, L"Comment Doc Keyword Error", L"fore:#008000", L"" },
                     { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_ASM = {
"aaa aad aam aas adc add and call cbw clc cld cli cmc cmp cmps cmpsb "
"cmpsw cwd daa das dec div esc hlt idiv imul in inc int into iret ja jae jb jbe "
"jc jcxz je jg jge jl jle jmp jna jnae jnb jnbe jnc jne jng jnge jnl jnle jno jnp "
"jns jnz jo jp jpe jpo js jz lahf lds lea les lods lodsb lodsw loop loope loopew "
"loopne loopnew loopnz loopnzw loopw loopz loopzw mov movs movsb "
"movsw mul neg nop not or out pop popf push pushf rcl rcr ret retf retn rol "
"ror sahf sal sar sbb scas scasb scasw shl shr stc std sti stos stosb stosw "
"sub test wait xchg xlat xlatb xor bound enter ins insb insw leave outs "
"outsb outsw popa pusha pushw arpl lar lsl sgdt sidt sldt smsw str verr "
"verw clts lgdt lidt lldt lmsw ltr bsf bsr bt btc btr bts cdq cmpsd cwde insd "
"iretd iretdf iretf jecxz lfs lgs lodsd loopd looped loopned loopnzd loopzd "
"lss movsd movsx movzx outsd popad popfd pushad pushd pushfd scasd "
"seta setae setb setbe setc sete setg setge setl setle setna setnae "
"setnb setnbe setnc setne setng setnge setnl setnle setno setnp setns "
"setnz seto setp setpe setpo sets setz shld shrd stosd bswap cmpxchg "
"invd invlpg wbinvd xadd lock rep repe repne repnz repz cflush cpuid emms "
"femms cmovo cmovno cmovb cmovc cmovnae cmovae cmovnb cmovnc "
"cmove cmovz cmovne cmovnz cmovbe cmovna cmova cmovnbe cmovs "
"cmovns cmovp cmovpe cmovnp cmovpo cmovl cmovnge cmovge cmovnl "
"cmovle cmovng cmovg cmovnle cmpxchg486 cmpxchg8b loadall loadall286 "
"ibts icebp int1 int3 int01 int03 iretw popaw popfw pushaw pushfw rdmsr "
"rdpmc rdshr rdtsc rsdc rsldt rsm rsts salc smi smint smintold svdc svldt "
"svts syscall sysenter sysexit sysret ud0 ud1 ud2 umov xbts wrmsr wrshr",
"f2xm1 fabs fadd faddp fbld fbstp fchs fclex fcom fcomp fcompp fdecstp "
"fdisi fdiv fdivp fdivr fdivrp feni ffree fiadd ficom ficomp fidiv fidivr fild fimul "
"fincstp finit fist fistp fisub fisubr fld fld1 fldcw fldenv fldenvw fldl2e fldl2t "
"fldlg2 fldln2 fldpi fldz fmul fmulp fnclex fndisi fneni fninit fnop fnsave "
"fnsavew fnstcw fnstenv fnstenvw fnstsw fpatan fprem fptan frndint frstor "
"frstorw fsave fsavew fscale fsqrt fst fstcw fstenv fstenvw fstp fstsw fsub "
"fsubp fsubr fsubrp ftst fwait fxam fxch fxtract fyl2x fyl2xp1 fsetpm fcos "
"fldenvd fnsaved fnstenvd fprem1 frstord fsaved fsin fsincos fstenvd fucom "
"fucomp fucompp fcomi fcomip ffreep fcmovb fcmove fcmovbe fcmovu "
"fcmovnb fcmovne fcmovnbe fcmovnu",
"ah al ax bh bl bp bx ch cl cr0 cr2 cr3 cr4 cs cx dh di dl dr0 dr1 dr2 dr3 dr6 "
"dr7 ds dx eax ebp ebx ecx edi edx es esi esp fs gs si sp ss st tr3 tr4 tr5 "
"tr6 tr7 st0 st1 st2 st3 st4 st5 st6 st7 mm0 mm1 mm2 mm3 mm4 mm5 "
"mm6 mm7 xmm0 xmm1 xmm2 xmm3 xmm4 xmm5 xmm6 xmm7",
".186 .286 .286c .286p .287 .386 .386c .386p .387 .486 .486p .8086 .8087 "
".alpha .break .code .const .continue .cref .data .data? .dosseg .else "
".elseif .endif .endw .err .err1 .err2 .errb .errdef .errdif .errdifi .erre .erridn "
".erridni .errnb .errndef .errnz .exit .fardata .fardata? .if .lall .lfcond .list "
".listall .listif .listmacro .listmacroall .model .no87 .nocref .nolist .nolistif "
".nolistmacro .radix .repeat .sall .seq .sfcond .stack .startup .tfcond .type "
".until .untilcxz .while .xall .xcref .xlist alias align assume catstr comm "
"comment db dd df dosseg dq dt dup dw echo else elseif elseif1 elseif2 "
"elseifb elseifdef elseifdif elseifdifi elseife elseifidn elseifidni elseifnb "
"elseifndef end endif endm endp ends eq equ even exitm extern externdef "
"extrn for forc ge goto group gt high highword if if1 if2 ifb ifdef ifdif ifdifi ife "
"ifidn ifidni ifnb ifndef include includelib instr invoke irp irpc label le length "
"lengthof local low lowword lroffset lt macro mask mod .msfloat name ne "
"offset opattr option org %out page popcontext proc proto ptr public "
"purge pushcontext record repeat rept seg segment short size sizeof "
"sizestr struc struct substr subtitle subttl textequ this title type typedef "
"union while width db dw dd dq dt resb resw resd resq rest incbin equ "
"times %define %idefine %xdefine %xidefine %undef %assign %iassign "
"%strlen %substr %macro %imacro %endmacro %rotate .nolist %if %elif "
"%else %endif %ifdef %ifndef %elifdef %elifndef %ifmacro %ifnmacro "
"%elifmacro %elifnmacro %ifctk %ifnctk %elifctk %elifnctk %ifidn %ifnidn "
"%elifidn %elifnidn %ifidni %ifnidni %elifidni %elifnidni %ifid %ifnid %elifid "
"%elifnid %ifstr %ifnstr %elifstr %elifnstr %ifnum %ifnnum %elifnum "
"%elifnnum %error %rep %endrep %exitrep %include %push %pop %repl "
"struct endstruc istruc at iend align alignb %arg %stacksize %local %line "
"bits use16 use32 section absolute extern global common cpu org section "
"group import export",
"$ ? @b @f addr basic byte c carry? dword far far16 fortran fword near "
"near16 overflow? parity? pascal qword real4 real8 real10 sbyte sdword "
"sign? stdcall sword syscall tbyte vararg word zero? flat near32 far32 abs "
"all assumes at casemap common compact cpu dotname emulator epilogue "
"error export expr16 expr32 farstack flat forceframe huge language large "
"listing ljmp loadds m510 medium memory nearstack nodotname noemulator "
"nokeyword noljmp nom510 none nonunique nooldmacros nooldstructs "
"noreadonly noscoped nosignextend nothing notpublic oldmacros oldstructs "
"os_dos para private prologue radix readonly req scoped setif2 smallstack "
"tiny use16 use32 uses # nasm directives, mostly complete, does not parse "
"properly a16 a32 o16 o32 byte word dword nosplit $ $$ seq wrt flat large "
"small .text .data .bss near far %0 %1 %2 %3 %4 %5 %6 %7 %8 %9",
"addpd addps addsd addss andpd andps andnpd andnps cmpeqpd cmpltpd "
"cmplepd cmpunordpd cmpnepd cmpnltpd cmpnlepd cmpordpd cmpeqps "
"cmpltps cmpleps cmpunordps cmpneps cmpnltps cmpnleps cmpordps "
"cmpeqsd cmpltsd cmplesd cmpunordsd cmpnesd cmpnltsd cmpnlesd "
"cmpordsd cmpeqss cmpltss cmpless cmpunordss cmpness cmpnltss "
"cmpnless cmpordss comisd comiss cvtdq2pd cvtdq2ps cvtpd2dq cvtpd2pi "
"cvtpd2ps cvtpi2pd cvtpi2ps cvtps2dq cvtps2pd cvtps2pi cvtss2sd "
"cvtss2si cvtsd2si cvtsd2ss cvtsi2sd cvtsi2ss cvttpd2dq cvttpd2pi "
"cvttps2dq cvttps2pi cvttsd2si cvttss2si divpd divps divsd divss fxrstor "
"fxsave ldmxscr lfence mfence maskmovdqu maskmovdq maxpd maxps "
"paxsd maxss minpd minps minsd minss movapd movaps movdq2q movdqa "
"movdqu movhlps movhpd movhps movd movq movlhps movlpd movlps "
"movmskpd movmskps movntdq movnti movntpd movntps movntq "
"movq2dq movsd movss movupd movups mulpd mulps mulsd mulss orpd "
"orps packssdw packsswb packuswb paddb paddsb paddw paddsw paddd "
"paddsiw paddq paddusb paddusw pand pandn pause paveb pavgb pavgw "
"pavgusb pdistib pextrw pcmpeqb pcmpeqw pcmpeqd pcmpgtb pcmpgtw "
"pcmpgtd pf2id pf2iw pfacc pfadd pfcmpeq pfcmpge pfcmpgt pfmax pfmin "
"pfmul pmachriw pmaddwd pmagw pmaxsw pmaxub pminsw pminub "
"pmovmskb pmulhrwc pmulhriw pmulhrwa pmulhuw pmulhw pmullw pmuludq "
"pmvzb pmvnzb pmvlzb pmvgezb pfnacc pfpnacc por prefetch prefetchw "
"prefetchnta prefetcht0 prefetcht1 prefetcht2 pfrcp pfrcpit1 pfrcpit2 "
"pfrsqit1 pfrsqrt pfsub pfsubr pi2fd pf2iw pinsrw psadbw pshufd pshufhw "
"pshuflw pshufw psllw pslld psllq pslldq psraw psrad psrlw psrld psrlq psrldq "
"psubb psubw psubd psubq psubsb psubsw psubusb psubusw psubsiw "
"pswapd punpckhbw punpckhwd punpckhdq punpckhqdq punpcklbw "
"punpcklwd punpckldq punpcklqdq pxor rcpps rcpss rsqrtps rsqrtss sfence "
"shufpd shufps sqrtpd sqrtps sqrtsd sqrtss stmxcsr subpd subps subsd "
"subss ucomisd ucomiss unpckhpd unpckhps unpcklpd unpcklps xorpd xorps",
"", "", "" };


EDITLEXER lexASM = { SCLEX_ASM, L"Assembly Script", L"asm", L"", &KeyWords_ASM, {
                     { STYLE_DEFAULT, L"Default", L"", L"" },
                     //{ SCE_ASM_DEFAULT, L"Default", L"", L"" },
                     { SCE_ASM_COMMENT, L"Comment", L"fore:#008000", L"" },
                     //{ SCE_ASM_COMMENTBLOCK, L"Comment Block (not used)", L"", L"" },
                     { SCE_ASM_NUMBER, L"Number", L"fore:#FF0000", L"" },
                     { SCE_ASM_STRING, L"String double quoted", L"fore:#008000", L"" },
                     { SCE_ASM_CHARACTER, L"String single quoted", L"fore:#008000", L"" },
                     { SCE_ASM_STRINGEOL, L"String not closed", L"fore:#008000; back:#FFFF80; eolfilled", L"" },
                     { SCE_ASM_OPERATOR, L"Operator", L"fore:#0A246A", L"" },
                     { SCE_ASM_IDENTIFIER, L"Identifier", L"", L"" },
                     { SCE_ASM_CPUINSTRUCTION, L"CPU Instruction", L"fore:#0A246A", L"" },
                     { SCE_ASM_MATHINSTRUCTION, L"FPU Instruction", L"fore:#0A246A", L"" },
                     { SCE_ASM_REGISTER, L"Register", L"fore:#FF8000", L"" },
                     { SCE_ASM_DIRECTIVE, L"Directive", L"fore:#0A246A", L"" },
                     { SCE_ASM_DIRECTIVEOPERAND, L"Directive Operand", L"fore:#0A246A", L"" },
                     { SCE_ASM_EXTINSTRUCTION, L"Extended Instruction", L"fore:#0A246A", L"" },
                     { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_PL = {
"NULL __FILE__ __LINE__ __PACKAGE__ __DATA__ __END__ AUTOLOAD "
"BEGIN CORE DESTROY END EQ GE GT INIT LE LT NE CHECK abs accept alarm "
"and atan2 bind binmode bless caller chdir chmod chomp chop chown chr "
"chroot close closedir cmp connect continue cos crypt dbmclose dbmopen "
"defined delete die do dump each else elsif endgrent endhostent endnetent "
"endprotoent endpwent endservent eof eq eval exec exists exit exp fcntl "
"fileno flock for foreach fork format formline ge getc getgrent getgrgid "
"getgrnam gethostbyaddr gethostbyname gethostent getlogin getnetbyaddr "
"getnetbyname getnetent getpeername getpgrp getppid getpriority "
"getprotobyname getprotobynumber getprotoent getpwent getpwnam "
"getpwuid getservbyname getservbyport getservent getsockname "
"getsockopt glob gmtime goto grep gt hex if index int ioctl join keys kill last lc "
"lcfirst le length link listen local localtime lock log lstat lt m map mkdir msgctl "
"msgget msgrcv msgsnd my ne next no not oct open opendir or ord our pack "
"package pipe pop pos print printf prototype push q qq qr quotemeta qu qw "
"qx rand read readdir readline readlink readpipe recv redo ref rename require "
"reset return reverse rewinddir rindex rmdir s scalar seek seekdir select "
"semctl semget semop send setgrent sethostent setnetent setpgrp "
"setpriority setprotoent setpwent setservent setsockopt shift shmctl shmget "
"shmread shmwrite shutdown sin sleep socket socketpair sort splice split "
"sprintf sqrt srand stat study sub substr symlink syscall sysopen sysread "
"sysseek system syswrite tell telldir tie tied time times tr truncate uc ucfirst "
"umask undef unless unlink unpack unshift untie until use utime values vec "
"wait waitpid wantarray warn while write x xor y",
"", "", "", "", "", "", "", "" };


EDITLEXER lexPL = { SCLEX_PERL, L"Perl Script", L"pl; pm; cgi; pod", L"", &KeyWords_PL, {
                    { STYLE_DEFAULT, L"Default", L"", L"" },
                    //{ SCE_PL_DEFAULT, L"Default", L"", L"" },
                    { SCE_PL_ERROR, L"Error", L"", L"" },
                    { SCE_PL_COMMENTLINE, L"Comment", L"fore:#008000", L"" },
                    { SCE_PL_NUMBER, L"Number", L"", L"" },
                    { SCE_PL_WORD, L"Keyword", L"fore:#FF8000; bold", L"" },
                    { SCE_PL_STRING, L"String double quoted", L"fore:#008000", L"" },
                    { SCE_PL_CHARACTER, L"String single quoted", L"fore:#008000", L"" },
                    //{ SCE_PL_PUNCTUATION, L"Symbols / Punctuation (not used)", L"", L"" },
                    //{ SCE_PL_PREPROCESSOR, L"Preprocessor (not used)", L"", L"" },
                    { SCE_PL_OPERATOR, L"Operator", L"", L"" },
                    { SCE_PL_IDENTIFIER, L"Identifier", L"", L"" },
                    { SCE_PL_POD, L"POD at beginning of line", L"", L"" },
                    { SCE_PL_POD_VERB, L"POD verbatim paragraphs", L"", L"" },
                    { SCE_PL_SCALAR, L"Scalar $", L"", L"" },
                    { SCE_PL_ARRAY, L"Array @", L"", L"" },
                    { SCE_PL_HASH, L"Hash %", L"", L"" },
                    { SCE_PL_SYMBOLTABLE, L"Symbol Table *", L"", L"" },
                    { SCE_PL_REGEX, L"Regex /re/ or m{re}", L"", L"" },
                    { SCE_PL_REGSUBST, L"Substitution s/re/or e/", L"", L"" },
                    //{ SCE_PL_LONGQUOTE, L"Long Quote (qq, qr, qw, qx) (not used)", L"", L"" },
                    { SCE_PL_BACKTICKS, L"Back Ticks", L"", L"" },
                    { SCE_PL_DATASECTION, L"Data Section", L"", L"" },
                    { SCE_PL_HERE_DELIM, L"Here-doc (Delimiter)", L"", L"" },
                    { SCE_PL_HERE_Q, L"Here-doc (Single quoted, q)", L"", L"" },
                    { SCE_PL_HERE_QQ, L"Here-doc (Double quoted, qq)", L"", L"" },
                    { SCE_PL_HERE_QX, L"Here-doc (Back ticks, qx)", L"", L"" },
                    { SCE_PL_STRING_Q, L"Single quoted string, generic (q)", L"", L"" },
                    { SCE_PL_STRING_QQ, L"Double quoted string (qq)", L"", L"" },
                    { SCE_PL_STRING_QX, L"Back ticks (qx)", L"", L"" },
                    { SCE_PL_STRING_QR, L"Regex (qr)", L"", L"" },
                    { SCE_PL_STRING_QW, L"Array (qw)", L"", L"" },
                    { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_INI = {
"", "", "", "", "", "", "", "", "" };


EDITLEXER lexINI = { SCLEX_PROPERTIES, L"Configuration Files", L"ini; inf; reg; cfg; properties; oem; sif; url", L"", &KeyWords_INI, {
                     { STYLE_DEFAULT, L"Default", L"", L"" },
                     //{ SCE_PROPS_DEFAULT, L"Default", L"", L"" },
                     { SCE_PROPS_COMMENT, L"Comment", L"fore:#008000", L"" },
                     { SCE_PROPS_SECTION, L"Section", L"back:#FF8040; bold; eolfilled", L"" },
                     { SCE_PROPS_ASSIGNMENT, L"Assignment", L"fore:#FF0000", L"" },
                     { SCE_PROPS_DEFVAL, L"Default Value", L"fore:#FF0000", L"" },
                     { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_BAT = {
"rem set if exist errorlevel for in do break call chcp cd chdir choice cls "
"country ctty date del erase dir echo exit goto loadfix loadhigh mkdir md "
"move path pause prompt rename ren rmdir rd shift time type ver verify vol "
"com con lpt nul echo.",
"", "", "", "", "", "", "", "" };


EDITLEXER lexBAT = { SCLEX_BATCH, L"Batch Files", L"bat; cmd", L"", &KeyWords_BAT, {
                     { STYLE_DEFAULT, L"Default", L"", L"" },
                     //{ SCE_BAT_DEFAULT, L"Default", L"", L"" },
                     { SCE_BAT_COMMENT, L"Comment", L"fore:#008000", L"" },
                     { SCE_BAT_WORD, L"Keyword", L"bold; fore:#0A246A", L"" },
                     { SCE_BAT_LABEL, L"Label", L"fore:#800000; back:#FFFF80; eolfilled", L"" },
                     { SCE_BAT_HIDE, L"Hide", L"", L"" },
                     { SCE_BAT_COMMAND, L"Command", L"bold", L"" },
                     { SCE_BAT_IDENTIFIER, L"Identifier", L"", L"" },
                     { SCE_BAT_OPERATOR, L"Operator", L"", L"" },
                     { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_DIFF = {
"", "", "", "", "", "", "", "", "" };


EDITLEXER lexDIFF = { SCLEX_DIFF, L"Diff Files", L"diff; patch", L"", &KeyWords_DIFF, {
                      { STYLE_DEFAULT, L"Default", L"", L"" },
                      //{ SCE_DIFF_DEFAULT, L"Default", L"", L"" },
                      { SCE_DIFF_COMMENT, L"Comment", L"fore:#008000", L"" },
                      { SCE_DIFF_COMMAND, L"Command", L"bold; fore:#0A246A", L"" },
                      { SCE_DIFF_HEADER, L"Source and Destination Files", L"fore:#800000; back:#FFFF80; eolfilled", L"" },
                      { SCE_DIFF_POSITION, L"Position Setting", L"fore:#FF0000", L"" },
                      { SCE_DIFF_ADDED, L"Line Addition", L"back:#80FF80; eolfilled", L"" },
                      { SCE_DIFF_DELETED, L"Line Removal", L"back:#FF8080; eolfilled", L"" },
                      { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_SQL = {
"abort accessible action add after all alter analyze and as asc asensitive attach autoincrement "
"before begin between bigint binary bit blob both by call cascade case cast change char character "
"check collate column commit condition conflict constraint continue convert create cross current_date "
"current_time current_timestamp current_user cursor database databases date day_hour day_microsecond "
"day_minute day_second dec decimal declare default deferrable deferred delayed delete desc describe "
"detach deterministic distinct distinctrow div double drop dual each else elseif enclosed end enum "
"escape escaped except exclusive exists exit explain fail false fetch float float4 float8 for force "
"foreign from full fulltext glob grant group having high_priority hour_microsecond hour_minute "
"hour_second if ignore immediate in index infile initially inner inout insensitive insert instead int "
"int1 int2 int3 int4 int8 integer intersect interval into is isnull iterate join key keys kill "
"leading leave left like limit linear lines load localtime localtimestamp lock long longblob longtext "
"loop low_priority master_ssl_verify_server_cert match mediumblob mediumint mediumtext middleint "
"minute_microsecond minute_second mod modifies natural no no_write_to_binlog not notnull null numeric "
"of offset on optimize option optionally or order out outer outfile plan pragma precision primary "
"procedure purge query raise range read read_only read_write reads real references regexp reindex "
"release rename repeat replace require restrict return revoke right rlike rollback row rowid schema "
"schemas second_microsecond select sensitive separator set show smallint spatial specific sql "
"sql_big_result sql_calc_found_rows sql_small_result sqlexception sqlstate sqlwarning ssl starting "
"straight_join table temp temporary terminated text then time timestamp tinyblob tinyint tinytext to "
"trailing transaction trigger true undo union unique unlock unsigned update usage use using utc_date "
"utc_time utc_timestamp vacuum values varbinary varchar varcharacter varying view virtual when where "
"while with write xor year_month zerofill",
"", "", "", "", "", "", "", "" };


EDITLEXER lexSQL = { SCLEX_SQL, L"SQL Query", L"sql", L"", &KeyWords_SQL, {
                     //{ 0, L"Default", L"", L"" },
                     { STYLE_DEFAULT, L"Default", L"", L"" },
                     { 1, L"Comment", L"fore:#008000", L"" },
                     { 2, L"Line Comment", L"fore:#008000", L"" },
                     { 3, L"Doc Comment", L"fore:#008000", L"" },
                     { 4, L"Number", L"fore:#FF0000", L"" },
                     { 5, L"Keyword", L"fore:#800080; bold", L"" },
                     { 6, L"String double quoted", L"fore:#008000", L"" },
                     { 7, L"String single quoted", L"fore:#008000", L"" },
                     //{ 12, L"String not closed", L"fore:#008000; back:#FFFF80; eolfilled", L"" },
                     { 10, L"Operator", L"", L"" },
                     { 11, L"Identifier", L"", L"" },
                     //{ 8, L"Symbols", L"", L"" },
                     //{ 9, L"Preprocessor", L"fore:#FF8000", L"" },
                     { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_PY = {
"and as assert break class continue def del elif else except "
"exec False finally for from global if import in is lambda None "
"not or pass print raise return True try with while yield",
"", "", "", "", "", "", "", "" };


EDITLEXER lexPY = { SCLEX_PYTHON, L"Python Script", L"py; pyw", L"", &KeyWords_PY, {
                    { STYLE_DEFAULT, L"Default", L"", L"" },
                    //{ SCE_P_DEFAULT, L"Default", L"", L"" },
                    { SCE_P_COMMENTLINE, L"Comment", L"fore:#007F00", L"" },
                    { SCE_P_COMMENTBLOCK, L"Comment Block", L"fore:#007F00", L"" },
                    { SCE_P_WORD, L"Keyword", L"fore:#00007F; bold", L"" },
                    { SCE_P_IDENTIFIER, L"Identifier", L"", L"" },
                    { SCE_P_NUMBER, L"Number", L"fore:#007F7F", L"" },
                    { SCE_P_OPERATOR, L"Operator", L"bold", L"" },
                    { SCE_P_STRING, L"String double quoted", L"fore:#FF8000", L"" },
                    { SCE_P_CHARACTER, L"String single quoted", L"fore:#FF8000", L"" },
                    { SCE_P_STRINGEOL, L"String not closed", L"fore:#FF8000", L"" },
                    { SCE_P_TRIPLEDOUBLE, L"String triple double quotes", L"fore:#FF8000", L"" },
                    { SCE_P_TRIPLE, L"String triple single quotes", L"fore:#FF8000", L"" },
                    { SCE_P_CLASSNAME, L"Class name", L"fore:#0000FF; bold", L"" },
                    { SCE_P_DEFNAME, L"Function name", L"fore:#007F7F; bold", L"" },
                    { -1, L"", L"", L"" } } };


KEYWORDLIST KeyWords_CONF = {
"acceptmutex acceptpathinfo accessconfig accessfilename action addalt addaltbyencoding addaltbytype addcharset adddefaultcharset "
"adddescription addencoding addhandler addicon addiconbyencoding addiconbytype addinputfilter addlanguage addmodule addmoduleinfo "
"addoutputfilter addoutputfilterbytype addtype agentlog alias aliasmatch all allow allowconnect allowencodedslashes allowoverride "
"anonymous anonymous_authoritative anonymous_logemail anonymous_mustgiveemail anonymous_nouserid anonymous_verifyemail "
"assignuserid authauthoritative authdbauthoritative authdbgroupfile authdbmauthoritative authdbmgroupfile authdbmtype "
"authdbmuserfile authdbuserfile authdigestalgorithm authdigestdomain authdigestfile authdigestgroupfile authdigestnccheck "
"authdigestnonceformat authdigestnoncelifetime authdigestqop authdigestshmemsize authgroupfile authldapauthoritative "
"authldapbinddn authldapbindpassword authldapcharsetconfig authldapcomparednonserver authldapdereferencealiases authldapenabled "
"authldapfrontpagehack authldapgroupattribute authldapgroupattributeisdn authldapremoteuserisdn authldapurl authname authtype "
"authuserfile bindaddress browsermatch browsermatchnocase bs2000account bufferedlogs cachedefaultexpire cachedirlength "
"cachedirlevels cachedisable cacheenable cacheexpirycheck cachefile cacheforcecompletion cachegcclean cachegcdaily "
"cachegcinterval cachegcmemusage cachegcunused cacheignorecachecontrol cacheignoreheaders cacheignorenolastmod "
"cachelastmodifiedfactor cachemaxexpire cachemaxfilesize cacheminfilesize cachenegotiateddocs cacheroot cachesize cachetimemargin "
"cgimapextension charsetdefault charsetoptions charsetsourceenc checkspelling childperuserid clearmodulelist contentdigest "
"cookiedomain cookieexpires cookielog cookiename cookiestyle cookietracking coredumpdirectory customlog dav davdepthinfinity "
"davlockdb davmintimeout defaulticon defaultlanguage defaulttype define deflatebuffersize deflatecompressionlevel "
"deflatefilternote deflatememlevel deflatewindowsize deny directory directoryindex directorymatch directoryslash documentroot "
"dumpioinput dumpiooutput enableexceptionhook enablemmap enablesendfile errordocument errorlog example expiresactive "
"expiresbytype expiresdefault extendedstatus extfilterdefine extfilteroptions fancyindexing fileetag files filesmatch "
"forcelanguagepriority forcetype forensiclog from group header headername hostnamelookups identitycheck ifdefine ifmodule "
"imapbase imapdefault imapmenu include indexignore indexoptions indexorderdefault isapiappendlogtoerrors isapiappendlogtoquery "
"isapicachefile isapifakeasync isapilognotsupported isapireadaheadbuffer keepalive keepalivetimeout languagepriority "
"ldapcacheentries ldapcachettl ldapconnectiontimeout ldapopcacheentries ldapopcachettl ldapsharedcachefile ldapsharedcachesize "
"ldaptrustedca ldaptrustedcatype limit limitexcept limitinternalrecursion limitrequestbody limitrequestfields "
"limitrequestfieldsize limitrequestline limitxmlrequestbody listen listenbacklog loadfile loadmodule location locationmatch "
"lockfile logformat loglevel maxclients maxkeepaliverequests maxmemfree maxrequestsperchild maxrequestsperthread maxspareservers "
"maxsparethreads maxthreads maxthreadsperchild mcachemaxobjectcount mcachemaxobjectsize mcachemaxstreamingbuffer "
"mcacheminobjectsize mcacheremovalalgorithm mcachesize metadir metafiles metasuffix mimemagicfile minspareservers minsparethreads "
"mmapfile modmimeusepathinfo multiviewsmatch namevirtualhost nocache noproxy numservers nwssltrustedcerts nwsslupgradeable "
"options order passenv pidfile port protocolecho proxy proxybadheader proxyblock proxydomain proxyerroroverride proxyiobuffersize "
"proxymatch proxymaxforwards proxypass proxypassreverse proxypreservehost proxyreceivebuffersize proxyremote proxyremotematch "
"proxyrequests proxytimeout proxyvia qsc readmename redirect redirectmatch redirectpermanent redirecttemp refererignore "
"refererlog removecharset removeencoding removehandler removeinputfilter removelanguage removeoutputfilter removetype "
"requestheader require resourceconfig rewritebase rewritecond rewriteengine rewritelock rewritelog rewriteloglevel rewritemap "
"rewriteoptions rewriterule rlimitcpu rlimitmem rlimitnproc satisfy scoreboardfile script scriptalias scriptaliasmatch "
"scriptinterpretersource scriptlog scriptlogbuffer scriptloglength scriptsock securelisten sendbuffersize serveradmin serveralias "
"serverlimit servername serverpath serverroot serversignature servertokens servertype setenv setenvif setenvifnocase sethandler "
"setinputfilter setoutputfilter singlelisten ssiendtag ssierrormsg ssistarttag ssitimeformat ssiundefinedecho "
"sslcacertificatefile sslcacertificatepath sslcarevocationfile sslcarevocationpath sslcertificatechainfile sslcertificatefile "
"sslcertificatekeyfile sslciphersuite sslengine sslmutex ssloptions sslpassphrasedialog sslprotocol sslproxycacertificatefile "
"sslproxycacertificatepath sslproxycarevocationfile sslproxycarevocationpath sslproxyciphersuite sslproxyengine "
"sslproxymachinecertificatefile sslproxymachinecertificatepath sslproxyprotocol sslproxyverify sslproxyverifydepth sslrandomseed "
"sslrequire sslrequiressl sslsessioncache sslsessioncachetimeout sslusername sslverifyclient sslverifydepth startservers "
"startthreads suexecusergroup threadlimit threadsperchild threadstacksize timeout transferlog typesconfig unsetenv "
"usecanonicalname user userdir virtualdocumentroot virtualdocumentrootip virtualhost virtualscriptalias virtualscriptaliasip "
"win32disableacceptex xbithack",
"on off standalone inetd force-response-1.0 downgrade-1.0 nokeepalive indexes includes followsymlinks none x-compress x-gzip",
"", "", "", "", "", "", "" };


EDITLEXER lexCONF = { SCLEX_CONF, L"Apache Config Files", L"conf; htaccess", L"", &KeyWords_CONF, {
                      { STYLE_DEFAULT, L"Default", L"", L"" },
                      //{ SCE_DIFF_DEFAULT, L"Default", L"", L"" },
                      { SCE_CONF_COMMENT, L"Comment", L"fore:#007F00", L"" },
                      { SCE_CONF_NUMBER, L"Number", L"fore:#007F7F", L"" },
                      { SCE_CONF_OPERATOR, L"Operator", L"bold", L"" },
                      { SCE_CONF_STRING, L"String", L"fore:#7F007F", L"" },
                      { SCE_CONF_IDENTIFIER, L"Identifier", L"bold; fore:#00007F", L"" },
                      { SCE_CONF_DIRECTIVE, L"Runtime Directive", L"bold; fore:#0A246A", L"" },
                      { SCE_CONF_PARAMETER, L"Runtime Directive Parameter", L"fore:#000000; back:#FFE0FF", L"" },
                      { SCE_CONF_IP, L"IP Address", L"fore:#007F7F; bold", L"" },
                      { SCE_CONF_EXTENSION, L"Extension", L"fore:#000000; back:#FFE0FF", L"" },
                      { -1, L"", L"", L"" } } };


// This array holds all the lexers...
PEDITLEXER pLexArray[NUMLEXERS] =
{
  &lexDefault,
  &lexHTML,
  &lexXML,
  &lexCSS,
  &lexJS,
  &lexVBS,
  &lexCPP,
  &lexCS,
  &lexRC,
  &lexMAK,
  &lexJAVA,
  &lexVB,
  &lexPAS,
  &lexASM,
  &lexSQL,
  &lexPL,
  &lexPY,
  &lexINI,
  &lexCONF,
  &lexBAT,
  &lexDIFF
};


// Currently used lexer
PEDITLEXER pLexCurrent = &lexDefault;
COLORREF crCustom[16];
BOOL bUse2ndDefaultStyle;
BOOL fStylesModified = FALSE;
int iBaseFontSize = 10;
int iDefaultLexer;
BOOL bAutoSelect;
int cxStyleSelectDlg;
int cyStyleSelectDlg;
extern int  iDefaultCodePage;
extern int  iDefaultCharSet;
extern BOOL bHiliteCurrentLine;


//=============================================================================
//
//  Style_Load()
//
void Style_Load()
{
  int i,iLexer;
  WCHAR tch[32];
  WCHAR *pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);
  int   cchIniSection = LocalSize(pIniSection)/sizeof(WCHAR);

  // Custom colors
  crCustom[0] = RGB(0,0,128);                     crCustom[8]  = RGB(255,255,226);
  crCustom[1] = GetSysColor(COLOR_WINDOWTEXT);    crCustom[9]  = GetSysColor(COLOR_WINDOW);
  crCustom[2] = GetSysColor(COLOR_INFOTEXT);      crCustom[10] = GetSysColor(COLOR_INFOBK);
  crCustom[3] = GetSysColor(COLOR_HIGHLIGHTTEXT); crCustom[11] = GetSysColor(COLOR_HIGHLIGHT);
  crCustom[4] = GetSysColor(COLOR_ACTIVECAPTION); crCustom[12] = GetSysColor(COLOR_DESKTOP);
  crCustom[5] = GetSysColor(COLOR_3DFACE);        crCustom[13] = GetSysColor(COLOR_3DHILIGHT);
  crCustom[6] = GetSysColor(COLOR_3DLIGHT);       crCustom[14] = GetSysColor(COLOR_3DSHADOW);
  crCustom[7] = GetSysColor(COLOR_APPWORKSPACE);  crCustom[15] = GetSysColor(COLOR_SCROLLBAR);

  LoadIniSection(L"Custom Colors",pIniSection,cchIniSection);
  for (i = 0; i < 16; i++) {
    wsprintf(tch,L"%02i",i+1);
    crCustom[i] = IniSectionGetInt(pIniSection,tch,crCustom[i]);
  }

  LoadIniSection(L"Styles",pIniSection,cchIniSection);
  // 2nd default
  bUse2ndDefaultStyle = (IniSectionGetInt(pIniSection,L"Use2ndDefaultStyle",0)) ? 1 : 0;

  // default scheme
  iDefaultLexer = IniSectionGetInt(pIniSection,L"DefaultScheme",0);
  iDefaultLexer = min(max(iDefaultLexer,0),NUMLEXERS-1);

  // auto select
  bAutoSelect = (IniSectionGetInt(pIniSection,L"AutoSelect",1)) ? 1 : 0;

  // scheme select dlg dimensions
  cxStyleSelectDlg = IniSectionGetInt(pIniSection,L"SelectDlgSizeX",304);
  cxStyleSelectDlg = max(cxStyleSelectDlg,0);

  cyStyleSelectDlg = IniSectionGetInt(pIniSection,L"SelectDlgSizeY",0);
  cyStyleSelectDlg = max(cyStyleSelectDlg,324);

  for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
    LoadIniSection(pLexArray[iLexer]->pszName,pIniSection,cchIniSection);
    if (!IniSectionGetString(pIniSection,L"FileNameExtensions",pLexArray[iLexer]->pszDefExt,
          pLexArray[iLexer]->szExtensions,COUNTOF(pLexArray[iLexer]->szExtensions)))
      lstrcpyn(pLexArray[iLexer]->szExtensions,pLexArray[iLexer]->pszDefExt,
        COUNTOF(pLexArray[iLexer]->szExtensions));
    i = 0;
    while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
      IniSectionGetString(pIniSection,pLexArray[iLexer]->Styles[i].pszName,
        pLexArray[iLexer]->Styles[i].pszDefault,
        pLexArray[iLexer]->Styles[i].szValue,
        COUNTOF(pLexArray[iLexer]->Styles[i].szValue));
      i++;
    }
  }
  LocalFree(pIniSection);
}


//=============================================================================
//
//  Style_Save()
//
void Style_Save()
{
  int i,iLexer;
  WCHAR tch[32];
  WCHAR *pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);
  int   cchIniSection = LocalSize(pIniSection)/sizeof(WCHAR);

  // Custom colors
  for (i = 0; i < 16; i++) {
    wsprintf(tch,L"%02i",i+1);
    IniSectionSetInt(pIniSection,tch,crCustom[i]);
  }
  SaveIniSection(L"Custom Colors",pIniSection);
  ZeroMemory(pIniSection,cchIniSection);

  // auto select
  IniSectionSetInt(pIniSection,L"Use2ndDefaultStyle",bUse2ndDefaultStyle);

  // default scheme
  IniSectionSetInt(pIniSection,L"DefaultScheme",iDefaultLexer);

  // auto select
  IniSectionSetInt(pIniSection,L"AutoSelect",bAutoSelect);

  // scheme select dlg dimensions
  IniSectionSetInt(pIniSection,L"SelectDlgSizeX",cxStyleSelectDlg);
  IniSectionSetInt(pIniSection,L"SelectDlgSizeY",cyStyleSelectDlg);

  SaveIniSection(L"Styles",pIniSection);

  if (!fStylesModified) {
    LocalFree(pIniSection);
    return;
  }

  ZeroMemory(pIniSection,cchIniSection);
  for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
    IniSectionSetString(pIniSection,L"FileNameExtensions",pLexArray[iLexer]->szExtensions);
    i = 0;
    while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
      IniSectionSetString(pIniSection,pLexArray[iLexer]->Styles[i].pszName,pLexArray[iLexer]->Styles[i].szValue);
      i++;
    }
    SaveIniSection(pLexArray[iLexer]->pszName,pIniSection);
    ZeroMemory(pIniSection,cchIniSection);
  }
  LocalFree(pIniSection);
}


//=============================================================================
//
//  Style_Import()
//
BOOL Style_Import(HWND hwnd)
{
  WCHAR szFile[MAX_PATH * 2] = L"";
  WCHAR szFilter[256];
  OPENFILENAME ofn;

  ZeroMemory(&ofn,sizeof(OPENFILENAME));
  GetString(IDS_FILTER_INI,szFilter,COUNTOF(szFilter));
  PrepareFilterStr(szFilter);

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = szFilter;
  ofn.lpstrFile = szFile;
  ofn.lpstrDefExt = L"ini";
  ofn.nMaxFile = COUNTOF(szFile);
  ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_DONTADDTORECENT
            | OFN_PATHMUSTEXIST | OFN_SHAREAWARE /*| OFN_NODEREFERENCELINKS*/;

  if (GetOpenFileName(&ofn)) {

    int i,iLexer;
    WCHAR *pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);
    int   cchIniSection = LocalSize(pIniSection)/sizeof(WCHAR);

    for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
      if (GetPrivateProfileSection(pLexArray[iLexer]->pszName,pIniSection,cchIniSection,szFile)) {
        if (!IniSectionGetString(pIniSection,L"FileNameExtensions",pLexArray[iLexer]->pszDefExt,
              pLexArray[iLexer]->szExtensions,COUNTOF(pLexArray[iLexer]->szExtensions)))
          lstrcpyn(pLexArray[iLexer]->szExtensions,pLexArray[iLexer]->pszDefExt,
            COUNTOF(pLexArray[iLexer]->szExtensions));
        i = 0;
        while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
          IniSectionGetString(pIniSection,pLexArray[iLexer]->Styles[i].pszName,
            pLexArray[iLexer]->Styles[i].pszDefault,
            pLexArray[iLexer]->Styles[i].szValue,
            COUNTOF(pLexArray[iLexer]->Styles[i].szValue));
          i++;
        }
      }
    }
    LocalFree(pIniSection);
    return(TRUE);
  }
  else
    return(FALSE);
}

//=============================================================================
//
//  Style_Export()
//
BOOL Style_Export(HWND hwnd)
{
  WCHAR szFile[MAX_PATH * 2] = L"";
  WCHAR szFilter[256];
  OPENFILENAME ofn;

  ZeroMemory(&ofn,sizeof(OPENFILENAME));
  GetString(IDS_FILTER_INI,szFilter,COUNTOF(szFilter));
  PrepareFilterStr(szFilter);

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = szFilter;
  ofn.lpstrFile = szFile;
  ofn.lpstrDefExt = L"ini";
  ofn.nMaxFile = COUNTOF(szFile);
  ofn.Flags = /*OFN_FILEMUSTEXIST |*/ OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_DONTADDTORECENT
            | OFN_PATHMUSTEXIST | OFN_SHAREAWARE /*| OFN_NODEREFERENCELINKS*/ | OFN_OVERWRITEPROMPT;

  if (GetSaveFileName(&ofn)) {

    int i,iLexer;
    WCHAR *pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);
    int   cchIniSection = LocalSize(pIniSection)/sizeof(WCHAR);

    for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
      IniSectionSetString(pIniSection,L"FileNameExtensions",pLexArray[iLexer]->szExtensions);
      i = 0;
      while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
        IniSectionSetString(pIniSection,pLexArray[iLexer]->Styles[i].pszName,pLexArray[iLexer]->Styles[i].szValue);
        i++;
      }
      WritePrivateProfileSection(pLexArray[iLexer]->pszName,pIniSection,szFile);
      ZeroMemory(pIniSection,cchIniSection);
    }
    LocalFree(pIniSection);
    return(TRUE);
  }
  else
    return(FALSE);
}


//=============================================================================
//
//  Style_SetLexer()
//
void Style_SetLexer(HWND hwnd,PEDITLEXER pLexNew)
{
  int i;
  //WCHAR *p;
  int rgb;
  int iValue;
  int iIdx;

  // Select default if NULL is specified
  if (!pLexNew)
    pLexNew = pLexArray[iDefaultLexer];

  // Clear
  SendMessage(hwnd,SCI_CLEARDOCUMENTSTYLE,0,0);

  // Lexer
  SendMessage(hwnd,SCI_SETLEXER,pLexNew->iLexer,0);

  if (pLexNew->iLexer == SCLEX_HTML || pLexNew->iLexer == SCLEX_XML)
    SendMessage(hwnd,SCI_SETSTYLEBITS,7,0);
  else
    SendMessage(hwnd,SCI_SETSTYLEBITS,5,0);

  // Add KeyWord Lists
  for (i = 0; i < 9; i++) {
    SendMessage(hwnd,SCI_SETKEYWORDS,i,(LPARAM)pLexNew->pKeyWords->pszKeyWords[i]); }

  // Use 2nd default style
  iIdx = (bUse2ndDefaultStyle) ? 12 : 0;

  // Default Values are always set
  SendMessage(hwnd,SCI_STYLERESETDEFAULT,0,0);
  SendMessage(hwnd,SCI_STYLESETCHARACTERSET,STYLE_DEFAULT,(LPARAM)DEFAULT_CHARSET);
  iBaseFontSize = 10;
  Style_SetStyles(hwnd,lexDefault.Styles[0+iIdx].iStyle,lexDefault.Styles[0+iIdx].szValue);  // default
  Style_StrGetSize(lexDefault.Styles[0+iIdx].szValue,&iBaseFontSize);                        // base size

  // Auto-select codepage according to charset
  //Style_SetACPfromCharSet(hwnd);

  if (!Style_StrGetColor(TRUE,lexDefault.Styles[0+iIdx].szValue,&iValue))
    SendMessage(hwnd,SCI_STYLESETFORE,STYLE_DEFAULT,(LPARAM)GetSysColor(COLOR_WINDOWTEXT));   // default text color
  if (!Style_StrGetColor(FALSE,lexDefault.Styles[0+iIdx].szValue,&iValue))
    SendMessage(hwnd,SCI_STYLESETBACK,STYLE_DEFAULT,(LPARAM)GetSysColor(COLOR_WINDOW));       // default window color

  if (pLexNew->iLexer != SCLEX_NULL) {
    Style_SetStyles(hwnd,pLexNew->Styles[0].iStyle,pLexNew->Styles[0].szValue); } // lexer default
  SendMessage(hwnd,SCI_STYLECLEARALL,0,0);

  Style_SetStyles(hwnd,lexDefault.Styles[1+iIdx].iStyle,lexDefault.Styles[1+iIdx].szValue); // linenumber
  Style_SetStyles(hwnd,lexDefault.Styles[2+iIdx].iStyle,lexDefault.Styles[2+iIdx].szValue); // brace light
  Style_SetStyles(hwnd,lexDefault.Styles[3+iIdx].iStyle,lexDefault.Styles[3+iIdx].szValue); // brace bad
  Style_SetStyles(hwnd,lexDefault.Styles[4+iIdx].iStyle,lexDefault.Styles[4+iIdx].szValue); // control WCHAR
  Style_SetStyles(hwnd,lexDefault.Styles[5+iIdx].iStyle,lexDefault.Styles[5+iIdx].szValue); // indent guide

  // More default values...
  if (Style_StrGetColor(TRUE,lexDefault.Styles[6+iIdx].szValue,&rgb)) // selection fore
    SendMessage(hwnd,SCI_SETSELFORE,TRUE,rgb);
  else
    SendMessage(hwnd,SCI_SETSELFORE,0,0);

  if (Style_StrGetColor(FALSE,lexDefault.Styles[6+iIdx].szValue,&iValue)) // selection back
    SendMessage(hwnd,SCI_SETSELBACK,TRUE,iValue);
  else
    SendMessage(hwnd,SCI_SETSELBACK,TRUE,RGB(0xC0,0xC0,0xC0)); // use a default value...

  if (Style_StrGetAlpha(lexDefault.Styles[6+iIdx].szValue,&iValue)) // selection alpha
    SendMessage(hwnd,SCI_SETSELALPHA,iValue,0);
  else
    SendMessage(hwnd,SCI_SETSELALPHA,SC_ALPHA_NOALPHA,0);

  if (StrStrI(lexDefault.Styles[6+iIdx].szValue,L"eolfilled")) // selection eolfilled
    SendMessage(hwnd,SCI_SETSELEOLFILLED,1,0);
  else
    SendMessage(hwnd,SCI_SETSELEOLFILLED,0,0);

  if (Style_StrGetColor(TRUE,lexDefault.Styles[7+iIdx].szValue,&rgb)) // whitespace fore
    SendMessage(hwnd,SCI_SETWHITESPACEFORE,TRUE,rgb);
  else
    SendMessage(hwnd,SCI_SETWHITESPACEFORE,0,0);

  if (Style_StrGetColor(FALSE,lexDefault.Styles[7+iIdx].szValue,&rgb)) // whitespace back
    SendMessage(hwnd,SCI_SETWHITESPACEBACK,TRUE,rgb);
  else
    SendMessage(hwnd,SCI_SETWHITESPACEBACK,0,0);    // use a default value...

  if (bHiliteCurrentLine) {

    if (Style_StrGetColor(FALSE,lexDefault.Styles[8+iIdx].szValue,&rgb)) // caret line back
    {
      SendMessage(hwnd,SCI_SETCARETLINEVISIBLE,TRUE,0);
      SendMessage(hwnd,SCI_SETCARETLINEBACK,rgb,0);

      if (Style_StrGetAlpha(lexDefault.Styles[8+iIdx].szValue,&iValue))
        SendMessage(hwnd,SCI_SETCARETLINEBACKALPHA,iValue,0);
      else
        SendMessage(hwnd,SCI_SETCARETLINEBACKALPHA,SC_ALPHA_NOALPHA,0);
    }
    else
      SendMessage(hwnd,SCI_SETCARETLINEVISIBLE,FALSE,0);
  }
  else
    SendMessage(hwnd,SCI_SETCARETLINEVISIBLE,FALSE,0);

  if (Style_StrGetColor(TRUE,lexDefault.Styles[9+iIdx].szValue,&rgb)) // caret fore
    SendMessage(hwnd,SCI_SETCARETFORE,rgb,0);
  else
    SendMessage(hwnd,SCI_SETCARETFORE,GetSysColor(COLOR_WINDOWTEXT),0); // default value

  // caret style and width
  if (StrStr(lexDefault.Styles[10+iIdx].szValue,L"block"))
    SendMessage(hwnd,SCI_SETCARETSTYLE,CARETSTYLE_BLOCK,0);
  else {
    iValue = 1;
    Style_StrGetSize(lexDefault.Styles[10+iIdx].szValue,&iValue);
    iValue = max(min(iValue,3),1);
    wsprintf(lexDefault.Styles[10+iIdx].szValue,L"size:%i",iValue);
    SendMessage(hwnd,SCI_SETCARETSTYLE,CARETSTYLE_LINE,0);
    SendMessage(hwnd,SCI_SETCARETWIDTH,iValue,0);
  }

  if (SendMessage(hwnd,SCI_GETEDGEMODE,0,0) == EDGE_LINE) {
    if (Style_StrGetColor(TRUE,lexDefault.Styles[11+iIdx].szValue,&rgb)) // edge fore
      SendMessage(hwnd,SCI_SETEDGECOLOUR,rgb,0);
    else
      SendMessage(hwnd,SCI_SETEDGECOLOUR,GetSysColor(COLOR_3DLIGHT),0);
    }
  else {
    if (Style_StrGetColor(FALSE,lexDefault.Styles[11+iIdx].szValue,&rgb)) // edge back
      SendMessage(hwnd,SCI_SETEDGECOLOUR,rgb,0);
    else
      SendMessage(hwnd,SCI_SETEDGECOLOUR,GetSysColor(COLOR_3DLIGHT),0);
    }

  if (SendMessage(hwnd,SCI_GETINDENTATIONGUIDES,0,0) != SC_IV_NONE)
    Style_SetIndentGuides(hwnd,TRUE);

  if (pLexNew->iLexer != SCLEX_NULL)
  {
    i = 1;
    while (pLexNew->Styles[i].iStyle != -1) {
      Style_SetStyles(hwnd,pLexNew->Styles[i].iStyle,pLexNew->Styles[i].szValue);
      i++;
    }
  }

  SendMessage(hwnd,SCI_COLOURISE,0,(LPARAM)-1);

  // Save current lexer
  pLexCurrent = pLexNew;
}


//=============================================================================
//
//  Style_SetACPfromCharSet()
//
//void Style_SetACPfromCharSet(HWND hwnd)
//{
//  int iValue;
//
//  // Use 2nd default style
//  int iIdx = (bUse2ndDefaultStyle) ? 12 : 0;
//
//  if (bAutoSelACP) {
//    iValue = iDefaultCharSet;
//    Style_StrGetCharSet(lexDefault.Styles[0+iIdx].szValue,&iValue);
//    iDefaultCodePage = CodePageFromCharSet(iValue);
//    if (SendMessage(hwnd,SCI_GETCODEPAGE,0,0) != SC_CP_UTF8)
//      SendMessage(hwnd,SCI_SETCODEPAGE,(WPARAM)iDefaultCodePage,0); }
//}


//=============================================================================
//
//  Style_SetLongLineColors()
//
void Style_SetLongLineColors(HWND hwnd)
{
  int rgb;

  // Use 2nd default style
  int iIdx = (bUse2ndDefaultStyle) ? 12 : 0;

  if (SendMessage(hwnd,SCI_GETEDGEMODE,0,0) == EDGE_LINE) {
    if (Style_StrGetColor(TRUE,lexDefault.Styles[11+iIdx].szValue,&rgb)) // edge fore
      SendMessage(hwnd,SCI_SETEDGECOLOUR,rgb,0);
    else
      SendMessage(hwnd,SCI_SETEDGECOLOUR,GetSysColor(COLOR_3DLIGHT),0); }
  else {
    if (Style_StrGetColor(FALSE,lexDefault.Styles[11+iIdx].szValue,&rgb)) // edge back
      SendMessage(hwnd,SCI_SETEDGECOLOUR,rgb,0);
    else
      SendMessage(hwnd,SCI_SETEDGECOLOUR,GetSysColor(COLOR_3DLIGHT),0); }
}


//=============================================================================
//
//  Style_SetCurrentLineBackground()
//
void Style_SetCurrentLineBackground(HWND hwnd)
{
  int rgb, iValue;

  // Use 2nd default style
  int iIdx = (bUse2ndDefaultStyle) ? 12 : 0;

  if (bHiliteCurrentLine) {

    if (Style_StrGetColor(FALSE,lexDefault.Styles[8+iIdx].szValue,&rgb)) // caret line back
    {
      SendMessage(hwnd,SCI_SETCARETLINEVISIBLE,TRUE,0);
      SendMessage(hwnd,SCI_SETCARETLINEBACK,rgb,0);

      if (Style_StrGetAlpha(lexDefault.Styles[8+iIdx].szValue,&iValue))
        SendMessage(hwnd,SCI_SETCARETLINEBACKALPHA,iValue,0);
      else
        SendMessage(hwnd,SCI_SETCARETLINEBACKALPHA,SC_ALPHA_NOALPHA,0);
    }
    else
      SendMessage(hwnd,SCI_SETCARETLINEVISIBLE,FALSE,0);
  }
  else
    SendMessage(hwnd,SCI_SETCARETLINEVISIBLE,FALSE,0);
}


//=============================================================================
//
//  Style_SetLexerFromFile()
//
extern int fNoHTMLGuess;
extern FILEVARS fvCurFile;

void Style_SetLexerFromFile(HWND hwnd,LPCWSTR lpszFile,BOOL bSupersedeAutoSelect)
{
  int i;
  LPWSTR lpszExt;
  WCHAR  tch[256+16];
  WCHAR  *p1,*p2;
  BOOL  bFound = FALSE;
  PEDITLEXER pLexNew = pLexArray[iDefaultLexer];

  lpszExt = PathFindExtension(lpszFile);
  if ((bSupersedeAutoSelect || bAutoSelect) && /* bAutoSelect == FALSE skips lexer search */
      (lpszFile && lstrlen(lpszFile) > 0 && *lpszExt) || (fvCurFile.mask & FV_MODE))
  {
    WCHAR wchMode[32];
    if (!bSupersedeAutoSelect &&
        (fvCurFile.mask & FV_MODE) &&
        fvCurFile.tchMode[0]) {
      UINT cp = SendMessage(hwnd,SCI_GETCODEPAGE,0,0);
      MultiByteToWideChar(cp,0,fvCurFile.tchMode,-1,wchMode,COUNTOF(wchMode));
      lpszExt = wchMode;
    }

    // Skip the . (dot)
    if (*lpszExt == L'.')
      lpszExt++;

    // check associated extensions
    for (i = 0; i < NUMLEXERS; i++) {
      if (bFound)
        break;

      ZeroMemory(tch,sizeof(WCHAR)*COUNTOF(tch));
      lstrcpy(tch,pLexArray[i]->szExtensions);
      p1 = tch;
      while (*p1 && !bFound)
      {
        if (p2 = StrChr(p1,L';'))
          *p2 = L'\0';
        else
          p2 = StrEnd(p1);
        StrTrim(p1,L" .");
        if (lstrcmpi(p1,lpszExt) == 0) {
          pLexNew = pLexArray[i];
          bFound = TRUE;
        }
        p1 = p2+1;
      }
    }
  }

  // check lexer names
  if (!bFound && lpszFile && lstrlen(lpszFile) > 0) {

    if (!bSupersedeAutoSelect &&
        (fvCurFile.mask & FV_MODE) &&
        fvCurFile.tchMode[0]) {
      UINT cp = SendMessage(hwnd,SCI_GETCODEPAGE,0,0);
      MultiByteToWideChar(cp,0,fvCurFile.tchMode,-1,tch,COUNTOF(tch));
    }

    else {
      lstrcpy(tch,lpszFile);
      StrTrim(tch,L" .");
    }

    if (lstrcmpi(tch,L"source") &&
        lstrcmpi(tch,L"code") &&
        lstrcmpi(tch,L"source code") &&
        lstrcmpi(tch,L"script") &&
        lstrcmpi(tch,L"files")) {

      for (i = 0; i < NUMLEXERS; i++) {
        if (bFound)
          break;

        if (StrStrI(pLexArray[i]->pszName,tch)) {
          pLexNew = pLexArray[i];
          bFound = TRUE;
        }
      }
    }
  }

  if (!bFound && (bSupersedeAutoSelect || bAutoSelect) &&
       lstrcmpi(PathFindFileName(lpszFile),L"makefile") == 0) {
    pLexNew = &lexMAK;
    bFound = TRUE;
  }

  if (!bFound && (bSupersedeAutoSelect || bAutoSelect) && !fNoHTMLGuess) {
    char tchText[256];
    SendMessage(hwnd,SCI_GETTEXT,(WPARAM)COUNTOF(tchText)-1,(LPARAM)tchText);
    StrTrimA(tchText," \t\n\r");
    if (tchText[0] == '<') {
      pLexNew = &lexHTML;
      bFound = TRUE;
    }
  }

  // Apply the new lexer
  Style_SetLexer(hwnd,pLexNew);
}


//=============================================================================
//
//  Style_SetDefaultLexer()
//
void Style_SetDefaultLexer(HWND hwnd)
{
  Style_SetLexer(hwnd,pLexArray[0]);
}


//=============================================================================
//
//  Style_SetHTMLLexer()
//
void Style_SetHTMLLexer(HWND hwnd)
{
  Style_SetLexer(hwnd,pLexArray[1]);
}


//=============================================================================
//
//  Style_SetXMLLexer()
//
void Style_SetXMLLexer(HWND hwnd)
{
  Style_SetLexer(hwnd,pLexArray[2]);
}


//=============================================================================
//
//  Style_SetLexerFromID()
//
void Style_SetLexerFromID(HWND hwnd,int id)
{
  if (id >= 0 && id < NUMLEXERS) {
    Style_SetLexer(hwnd,pLexArray[id]);
  }
}


//=============================================================================
//
//  Style_ToggleUse2ndDefault()
//
void Style_ToggleUse2ndDefault(HWND hwnd)
{
  bUse2ndDefaultStyle = (bUse2ndDefaultStyle) ? 0 : 1;
  Style_SetLexer(hwnd,pLexCurrent);
}


//=============================================================================
//
//  Style_SetDefaultFont()
//
void Style_SetDefaultFont(HWND hwnd)
{
  int iIdx = (bUse2ndDefaultStyle) ? 12 : 0;
  if (Style_SelectFont(hwnd,
        lexDefault.Styles[0+iIdx].szValue,
        COUNTOF(lexDefault.Styles[0+iIdx].szValue),
        TRUE)) {
    fStylesModified = TRUE;
    Style_SetLexer(hwnd,pLexCurrent);
  }
}


//=============================================================================
//
//  Style_GetUse2ndDefault()
//
BOOL Style_GetUse2ndDefault(HWND hwnd)
{
  return (bUse2ndDefaultStyle);
}


//=============================================================================
//
//  Style_SetIndentGuides()
//
extern int flagSimpleIndentGuides;

void Style_SetIndentGuides(HWND hwnd,BOOL bShow)
{
  int iIndentView = SC_IV_NONE;
  if (bShow) {
    if (!flagSimpleIndentGuides) {
      if (SendMessage(hwnd,SCI_GETLEXER,0,0) == SCLEX_PYTHON)
        iIndentView = SC_IV_LOOKFORWARD;
      else
        iIndentView = SC_IV_LOOKBOTH;
    }
    else
      iIndentView = SC_IV_REAL;
  }
  SendMessage(hwnd,SCI_SETINDENTATIONGUIDES,iIndentView,0);
}


//=============================================================================
//
//  Style_GetFileOpenDlgFilter()
//
extern WCHAR tchFileDlgFilters[5*1024];

BOOL Style_GetOpenDlgFilterStr(LPWSTR lpszFilter,int cchFilter)
{
  if (lstrlen(tchFileDlgFilters) == 0)
    GetString(IDS_FILTER_ALL,lpszFilter,cchFilter);
  else {
    lstrcpyn(lpszFilter,tchFileDlgFilters,cchFilter-2);
    lstrcat(lpszFilter,L"||");
  }
  PrepareFilterStr(lpszFilter);
  return TRUE;
}


//=============================================================================
//
//  Style_StrGetFont()
//
BOOL Style_StrGetFont(LPCWSTR lpszStyle,LPWSTR lpszFont,int cchFont)
{
  WCHAR tch[256];
  WCHAR *p;

  if (p = StrStrI(lpszStyle,L"font:"))
  {
    lstrcpy(tch,p + lstrlen(L"font:"));
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    lstrcpyn(lpszFont,tch,cchFont);
    return TRUE;
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetCharSet()
//
BOOL Style_StrGetCharSet(LPCWSTR lpszStyle,int *i)
{
  WCHAR tch[256];
  WCHAR *p;
  int  iValue;
  int  itok;

  if (p = StrStrI(lpszStyle,L"charset:"))
  {
    lstrcpy(tch,p + lstrlen(L"charset:"));
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    itok = swscanf(tch,L"%i",&iValue);
    if (itok == 1)
    {
      *i = iValue;
      return TRUE;
    }
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetSize()
//
BOOL Style_StrGetSize(LPCWSTR lpszStyle,int *i)
{
  WCHAR tch[256];
  WCHAR *p;
  int  iValue;
  int  iSign = 0;
  int  itok;

  if (p = StrStrI(lpszStyle,L"size:"))
  {
    lstrcpy(tch,p + lstrlen(L"size:"));
    if (tch[0] == L'+')
    {
      iSign = 1;
      tch[0] = L' ';
    }
    else if (tch[0] == L'-')
    {
      iSign = -1;
      tch[0] = L' ';
    }
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    itok = swscanf(tch,L"%i",&iValue);
    if (itok == 1)
    {
      if (iSign == 0)
        *i = iValue;
      else
        *i = max(0,iBaseFontSize + iValue * iSign); // size must be +
      return TRUE;
    }
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetSizeStr()
//
BOOL Style_StrGetSizeStr(LPCWSTR lpszStyle,LPWSTR lpszSize,int cchSize)
{
  WCHAR tch[256];
  WCHAR *p;

  if (p = StrStrI(lpszStyle,L"size:"))
  {
    lstrcpy(tch,p + lstrlen(L"size:"));
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    lstrcpyn(lpszSize,tch,cchSize);
    return TRUE;
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetColor()
//
BOOL Style_StrGetColor(BOOL bFore,LPCWSTR lpszStyle,int *rgb)
{
  WCHAR tch[256];
  WCHAR *p;
  int  iValue;
  int  itok;
  WCHAR *pItem = (bFore) ? L"fore:" : L"back:";

  if (p = StrStrI(lpszStyle,pItem))
  {
    lstrcpy(tch,p + lstrlen(pItem));
    if (tch[0] == L'#')
      tch[0] = L' ';
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    itok = swscanf(tch,L"%x",&iValue);
    if (itok == 1)
    {
      *rgb = RGB((iValue&0xFF0000) >> 16,(iValue&0xFF00) >> 8,iValue&0xFF);
      return TRUE;
    }
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetCase()
//
BOOL Style_StrGetCase(LPCWSTR lpszStyle,int *i)
{
  WCHAR tch[256];
  WCHAR *p;

  if (p = StrStrI(lpszStyle,L"case:"))
  {
    lstrcpy(tch,p + lstrlen(L"case:"));
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    if (tch[0] == L'u' || tch[0] == L'U') {
      *i = SC_CASE_UPPER;
      return TRUE; }
    else if (tch[0] == L'l' || tch[0] == L'L') {
      *i = SC_CASE_LOWER;
      return TRUE; }
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetAlpha()
//
BOOL Style_StrGetAlpha(LPCWSTR lpszStyle,int *i)
{
  WCHAR tch[256];
  WCHAR *p;
  int  iValue;
  int  itok;

  if (p = StrStrI(lpszStyle,L"alpha:"))
  {
    lstrcpy(tch,p + lstrlen(L"alpha:"));
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    itok = swscanf(tch,L"%i",&iValue);
    if (itok == 1)
    {
      *i = min(max(SC_ALPHA_TRANSPARENT,iValue),SC_ALPHA_OPAQUE);
      return TRUE;
    }
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetAttribute()
//
/*BOOL Style_StrGetAttribute(LPCWSTR lpszStyle,LPCWSTR lpszAttribute)
{
  return (StrStrI(lpszStyle,lpszAttribute) == NULL) ? FALSE : TRUE;
}*/


//=============================================================================
//
//  Style_StrSetAttribute()
//
/*BOOL Style_StrSetAttribute(LPWSTR lpszStyle,int cchStyle,LPCWSTR lpszAttribute,BOOL bSet)
{
  WCHAR szNewStyle[512];
  int  iValue;
  WCHAR tch[32];

  // Rebuild style string
  lstrcpy(szNewStyle,L"");
  if (Style_StrGetFont(lpszStyle,tch,COUNTOF(tch)))
  {
    lstrcat(szNewStyle,L"font:");
    lstrcat(szNewStyle,tch);
  }

  if (Style_StrGetCharSet(lpszStyle,&iValue))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    wsprintf(tch,L"charset:%i",iValue);
    lstrcat(szNewStyle,tch);
  }

  if (Style_StrGetSizeStr(lpszStyle,tch,COUNTOF(tch)))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"size:");
    lstrcat(szNewStyle,tch);
  }

  if (lstrcmpi(lpszAttribute,L"bold") == 0)
  {
    if (bSet)
    {
      if (lstrlen(szNewStyle))
        lstrcat(szNewStyle,L"; ");
      lstrcat(szNewStyle,L"bold");
    }
  }
  else if (StrStrI(lpszStyle,L"bold"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"bold");
  }

  if (lstrcmpi(lpszAttribute,L"italic") == 0)
  {
    if (bSet)
    {
      if (lstrlen(szNewStyle))
        lstrcat(szNewStyle,L"; ");
      lstrcat(szNewStyle,L"italic");
    }
  }
  else if (StrStrI(lpszStyle,L"italic"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"italic");
  }

  if (lstrcmpi(lpszAttribute,L"underline") == 0)
  {
    if (bSet)
    {
      if (lstrlen(szNewStyle))
        lstrcat(szNewStyle,L"; ");
      lstrcat(szNewStyle,L"underline");
    }
  }
  else if (StrStrI(lpszStyle,L"underline"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"underline");
  }

  // save colors
  if (Style_StrGetColor(TRUE,lpszStyle,&iValue))
  {
    wsprintf(tch,L"fore:#%02X%02X%02X",
      (int)iValue&0xFF,
      (int)(iValue&0xFF00) >> 8,
      (int)(iValue&0xFF0000) >> 16);
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,tch);
  }
  if (Style_StrGetColor(FALSE,lpszStyle,&iValue))
  {
    wsprintf(tch,L"back:#%02X%02X%02X",
      (int)iValue&0xFF,
      (int)(iValue&0xFF00) >> 8,
      (int)(iValue&0xFF0000) >> 16);
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,tch);
  }

  if (lstrcmpi(lpszAttribute,L"eolfilled") == 0)
  {
    if (bSet)
    {
      if (lstrlen(szNewStyle))
        lstrcat(szNewStyle,L"; ");
      lstrcat(szNewStyle,L"eolfilled");
    }
  }
  else if (StrStrI(lpszStyle,L"eolfilled"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"eolfilled");
  }

  if (Style_StrGetCase(lpszStyle,&iValue)) {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"case:");
    lstrcat(szNewStyle,(iValue == SC_CASE_UPPER) ? L"u" : L""); }

  lstrcpyn(lpszStyle,szNewStyle,cchStyle);
  return TRUE;
}*/


//=============================================================================
//
//  Style_SelectFont()
//
BOOL Style_SelectFont(HWND hwnd,LPWSTR lpszStyle,int cchStyle,BOOL bDefaultStyle)
{
  CHOOSEFONT cf;
  LOGFONT lf;
  WCHAR szNewStyle[512];
  int  iValue;
  WCHAR tch[32];
  HDC hdc;

  ZeroMemory(&cf,sizeof(CHOOSEFONT));
  ZeroMemory(&lf,sizeof(LOGFONT));

  // Map lpszStyle to LOGFONT
  if (Style_StrGetFont(lpszStyle,tch,COUNTOF(tch)))
    lstrcpyn(lf.lfFaceName,tch,COUNTOF(lf.lfFaceName));
  if (Style_StrGetCharSet(lpszStyle,&iValue))
    lf.lfCharSet = iValue;
  if (Style_StrGetSize(lpszStyle,&iValue)) {
    hdc = GetDC(hwnd);
    lf.lfHeight = -MulDiv(iValue,GetDeviceCaps(hdc,LOGPIXELSY),72);
    ReleaseDC(hwnd,hdc);
  }
  lf.lfWeight = (StrStrI(lpszStyle,L"bold")) ? FW_BOLD : FW_NORMAL;
  lf.lfItalic = (StrStrI(lpszStyle,L"italic")) ? 1 : 0;

  // Init cf
  cf.lStructSize = sizeof(CHOOSEFONT);
  cf.hwndOwner = hwnd;
  cf.lpLogFont = &lf;
  cf.Flags = CF_INITTOLOGFONTSTRUCT /*| CF_NOSCRIPTSEL*/ | CF_SCREENFONTS;

  //// MonoSpace
  //if (bMonoSpace)
  //  cf.Flags |= CF_FIXEDPITCHONLY;

  if (!ChooseFont(&cf) || !lstrlen(lf.lfFaceName))
    return FALSE;

  // Map back to lpszStyle
  lstrcpy(szNewStyle,L"font:");
  lstrcat(szNewStyle,lf.lfFaceName);
  if (bDefaultStyle &&
      lf.lfCharSet != DEFAULT_CHARSET &&
      lf.lfCharSet != iDefaultCharSet) {
    lstrcat(szNewStyle,L"; charset:");
    wsprintf(tch,L"%i",lf.lfCharSet);
    lstrcat(szNewStyle,tch);
  }
  lstrcat(szNewStyle,L"; size:");
  wsprintf(tch,L"%i",cf.iPointSize/10);
  lstrcat(szNewStyle,tch);
  if (cf.nFontType & BOLD_FONTTYPE)
    lstrcat(szNewStyle,L"; bold");
  if (cf.nFontType & ITALIC_FONTTYPE)
    lstrcat(szNewStyle,L"; italic");

  if (StrStrI(lpszStyle,L"underline"))
    lstrcat(szNewStyle,L"; underline");

  // save colors
  if (Style_StrGetColor(TRUE,lpszStyle,&iValue))
  {
    wsprintf(tch,L"; fore:#%02X%02X%02X",
      (int)iValue&0xFF,
      (int)(iValue&0xFF00) >> 8,
      (int)(iValue&0xFF0000) >> 16);
    lstrcat(szNewStyle,tch);
  }
  if (Style_StrGetColor(FALSE,lpszStyle,&iValue))
  {
    wsprintf(tch,L"; back:#%02X%02X%02X",
      (int)iValue&0xFF,
      (int)(iValue&0xFF00) >> 8,
      (int)(iValue&0xFF0000) >> 16);
    lstrcat(szNewStyle,tch);
  }

  if (StrStrI(lpszStyle,L"eolfilled"))
    lstrcat(szNewStyle,L"; eolfilled");

  if (Style_StrGetCase(lpszStyle,&iValue)) {
    lstrcat(szNewStyle,L"; case:");
    lstrcat(szNewStyle,(iValue == SC_CASE_UPPER) ? L"u" : L""); }

  if (Style_StrGetAlpha(lpszStyle,&iValue)) {
    lstrcat(szNewStyle,L"; alpha:");
    wsprintf(tch,L"%i",iValue);
    lstrcat(szNewStyle,tch); }

  lstrcpyn(lpszStyle,szNewStyle,cchStyle);
  return TRUE;
}


//=============================================================================
//
//  Style_SelectColor()
//
BOOL Style_SelectColor(HWND hwnd,BOOL bFore,LPWSTR lpszStyle,int cchStyle)
{
  CHOOSECOLOR cc;
  WCHAR szNewStyle[512];
  int  iRGBResult;
  int  iValue;
  WCHAR tch[32];

  ZeroMemory(&cc,sizeof(CHOOSECOLOR));

  iRGBResult = (bFore) ? GetSysColor(COLOR_WINDOWTEXT) : GetSysColor(COLOR_WINDOW);
  Style_StrGetColor(bFore,lpszStyle,&iRGBResult);

  cc.lStructSize = sizeof(CHOOSECOLOR);
  cc.hwndOwner = hwnd;
  cc.rgbResult = iRGBResult;
  cc.lpCustColors = crCustom;
  cc.Flags = CC_RGBINIT | CC_SOLIDCOLOR;

  if (!ChooseColor(&cc))
    return FALSE;

  iRGBResult = cc.rgbResult;

  // Rebuild style string
  lstrcpy(szNewStyle,L"");
  if (Style_StrGetFont(lpszStyle,tch,COUNTOF(tch)))
  {
    lstrcat(szNewStyle,L"font:");
    lstrcat(szNewStyle,tch);
  }
  if (Style_StrGetCharSet(lpszStyle,&iValue))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    wsprintf(tch,L"charset:%i",iValue);
    lstrcat(szNewStyle,tch);
  }
  if (Style_StrGetSizeStr(lpszStyle,tch,COUNTOF(tch)))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"size:");
    lstrcat(szNewStyle,tch);
  }

  if (StrStrI(lpszStyle,L"bold"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"bold");
  }
  if (StrStrI(lpszStyle,L"italic"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"italic");
  }
  if (StrStrI(lpszStyle,L"underline"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"underline");
  }

  if (bFore)
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    wsprintf(tch,L"fore:#%02X%02X%02X",
      (int)iRGBResult&0xFF,
      (int)(iRGBResult&0xFF00) >> 8,
      (int)(iRGBResult&0xFF0000) >> 16);
    lstrcat(szNewStyle,tch);
    if (Style_StrGetColor(FALSE,lpszStyle,&iValue))
    {
      wsprintf(tch,L"; back:#%02X%02X%02X",
        (int)iValue&0xFF,
        (int)(iValue&0xFF00) >> 8,
        (int)(iValue&0xFF0000) >> 16);
      lstrcat(szNewStyle,tch);
    }
  }
  else
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    if (Style_StrGetColor(TRUE,lpszStyle,&iValue))
    {
      wsprintf(tch,L"fore:#%02X%02X%02X; ",
        (int)iValue&0xFF,
        (int)(iValue&0xFF00) >> 8,
        (int)(iValue&0xFF0000) >> 16);
      lstrcat(szNewStyle,tch);
    }
    wsprintf(tch,L"back:#%02X%02X%02X",
      (int)iRGBResult&0xFF,
      (int)(iRGBResult&0xFF00) >> 8,
      (int)(iRGBResult&0xFF0000) >> 16);
    lstrcat(szNewStyle,tch);
  }

  if (StrStrI(lpszStyle,L"eolfilled"))
    lstrcat(szNewStyle,L"; eolfilled");

  if (Style_StrGetCase(lpszStyle,&iValue)) {
    lstrcat(szNewStyle,L"; case:");
    lstrcat(szNewStyle,(iValue == SC_CASE_UPPER) ? L"u" : L""); }

  if (Style_StrGetAlpha(lpszStyle,&iValue)) {
    lstrcat(szNewStyle,L"; alpha:");
    wsprintf(tch,L"%i",iValue);
    lstrcat(szNewStyle,tch); }

  lstrcpyn(lpszStyle,szNewStyle,cchStyle);
  return TRUE;
}


//=============================================================================
//
//  Style_SetStyles()
//
void Style_SetStyles(HWND hwnd,int iStyle,LPCWSTR lpszStyle)
{

  WCHAR tch[256];
  WCHAR *p;
  int  iValue;

  // Font
  if (Style_StrGetFont(lpszStyle,tch,COUNTOF(tch))) {
    char mch[256];
    WideCharToMultiByte(CP_ACP,0,tch,-1,mch,COUNTOF(mch),NULL,NULL);
    SendMessage(hwnd,SCI_STYLESETFONT,iStyle,(LPARAM)mch);
  }

  // Size
  if (Style_StrGetSize(lpszStyle,&iValue))
    SendMessage(hwnd,SCI_STYLESETSIZE,iStyle,(LPARAM)iValue);

  // Fore
  if (Style_StrGetColor(TRUE,lpszStyle,&iValue))
    SendMessage(hwnd,SCI_STYLESETFORE,iStyle,(LPARAM)iValue);

  // Back
  if (Style_StrGetColor(FALSE,lpszStyle,&iValue))
    SendMessage(hwnd,SCI_STYLESETBACK,iStyle,(LPARAM)iValue);

  // Bold
  if (p = StrStrI(lpszStyle,L"bold"))
    SendMessage(hwnd,SCI_STYLESETBOLD,iStyle,(LPARAM)TRUE);
  else
    SendMessage(hwnd,SCI_STYLESETBOLD,iStyle,(LPARAM)FALSE);

  // Italic
  if (p = StrStrI(lpszStyle,L"italic"))
    SendMessage(hwnd,SCI_STYLESETITALIC,iStyle,(LPARAM)TRUE);
  else
    SendMessage(hwnd,SCI_STYLESETITALIC,iStyle,(LPARAM)FALSE);

  // Underline
  if (p = StrStrI(lpszStyle,L"underline"))
    SendMessage(hwnd,SCI_STYLESETUNDERLINE,iStyle,(LPARAM)TRUE);
  else
    SendMessage(hwnd,SCI_STYLESETUNDERLINE,iStyle,(LPARAM)FALSE);

  // EOL Filled
  if (p = StrStrI(lpszStyle,L"eolfilled"))
    SendMessage(hwnd,SCI_STYLESETEOLFILLED,iStyle,(LPARAM)TRUE);
  else
    SendMessage(hwnd,SCI_STYLESETEOLFILLED,iStyle,(LPARAM)FALSE);

  // Case
  if (Style_StrGetCase(lpszStyle,&iValue))
    SendMessage(hwnd,SCI_STYLESETCASE,iStyle,(LPARAM)iValue);

  // Character Set
  if (Style_StrGetCharSet(lpszStyle,&iValue))
    SendMessage(hwnd,SCI_STYLESETCHARACTERSET,iStyle,(LPARAM)iValue);

}


//=============================================================================
//
//  Style_GetCurrentLexerName()
//
LPCWSTR Style_GetCurrentLexerName()
{
  return pLexCurrent->pszName;
}


//=============================================================================
//
//  Style_GetLexerIconId()
//
int Style_GetLexerIconId(PEDITLEXER plex)
{
  WCHAR *p;
  WCHAR *pszExtensions;
  WCHAR *pszFile;

  SHFILEINFO shfi;

  if (lstrlen(plex->szExtensions))
    pszExtensions = plex->szExtensions;
  else
    pszExtensions = plex->pszDefExt;

  pszFile = GlobalAlloc(GPTR,sizeof(WCHAR)*(lstrlen(pszExtensions) + lstrlen(L"*.txt") + 16));
  lstrcpy(pszFile,L"*.");
  lstrcat(pszFile,pszExtensions);
  if (p = StrChr(pszFile,L';'))
    *p = L'\0';

  // check for ; at beginning
  if (lstrlen(pszFile) < 3)
    lstrcat(pszFile,L"txt");

  SHGetFileInfo(pszFile,FILE_ATTRIBUTE_NORMAL,&shfi,sizeof(SHFILEINFO),
    SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES);

  GlobalFree(pszFile);

  return (shfi.iIcon);
}


//=============================================================================
//
//  Style_AddLexerToTreeView()
//
void Style_AddLexerToTreeView(HWND hwnd,PEDITLEXER plex)
{
  //int i = 0;

  //LVITEM lvi;
  //ZeroMemory(&lvi,sizeof(LVITEM));

  //lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
  //lvi.pszText = plex->pszName;
  //lvi.iImage = Style_GetLexerIconId(plex);
  //lvi.lParam = 0;
  //lvi.iItem = ListView_GetItemCount(hwnd);
  //ListView_InsertItem(hwnd,&lvi);

  //lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_INDENT;
  //lvi.iImage = -1;
  //lvi.iIndent = 1;

  //while (plex->Styles[i].iStyle != -1)
  //{
  //  lvi.pszText = plex->Styles[i].pszName;
  //  lvi.lParam = (LPARAM)(&plex->Styles[i]);
  //  lvi.iItem = ListView_GetItemCount(hwnd);
  //  ListView_InsertItem(hwnd,&lvi);
  //  i++;
  //}

  int i = 0;

  HTREEITEM hTreeNode;

  TVINSERTSTRUCT tvis;
  ZeroMemory(&tvis,sizeof(TVINSERTSTRUCT));

  tvis.hInsertAfter = TVI_LAST;

  tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
  tvis.item.pszText = plex->pszName;
  tvis.item.iImage = Style_GetLexerIconId(plex);
  tvis.item.iSelectedImage = tvis.item.iImage;
  tvis.item.lParam = (LPARAM)plex;

  hTreeNode = (HTREEITEM)TreeView_InsertItem(hwnd,&tvis);

  tvis.hParent = hTreeNode;

  tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
  //tvis.item.iImage = -1;
  //tvis.item.iSelectedImage = -1;

  while (plex->Styles[i].iStyle != -1)
  {
    tvis.item.pszText = plex->Styles[i].pszName;
    tvis.item.lParam = (LPARAM)(&plex->Styles[i]);
    TreeView_InsertItem(hwnd,&tvis);
    i++;
  }

}


//=============================================================================
//
//  Style_AddLexerToListView()
//
void Style_AddLexerToListView(HWND hwnd,PEDITLEXER plex)
{
  LVITEM lvi;
  ZeroMemory(&lvi,sizeof(LVITEM));

  lvi.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT;
  lvi.iItem = ListView_GetItemCount(hwnd);
  lvi.pszText = plex->pszName;
  lvi.iImage = Style_GetLexerIconId(plex);
  lvi.lParam = (LPARAM)plex;

  ListView_InsertItem(hwnd,&lvi);
}


//=============================================================================
//
//  Style_ConfigDlgProc()
//
BOOL CALLBACK Style_ConfigDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  static HWND hwndTV;
  static BOOL fDragging;
  static PEDITLEXER pCurrentLexer;
  static PEDITSTYLE pCurrentStyle;
  static HFONT hFontTitle;
  static HBRUSH hbrFore;
  static HBRUSH hbrBack;

  switch(umsg)
  {

    case WM_INITDIALOG:
      {
        int i;
        SHFILEINFO shfi;
        LOGFONT lf;

        hwndTV = GetDlgItem(hwnd,IDC_STYLELIST);
        fDragging = FALSE;

        TreeView_SetImageList(hwndTV,
          (HIMAGELIST)SHGetFileInfo(L"C:\\",0,&shfi,sizeof(SHFILEINFO),
          SHGFI_SMALLICON | SHGFI_SYSICONINDEX),TVSIL_NORMAL);

        // Add lexers
        for (i = 0; i < NUMLEXERS; i++)
          Style_AddLexerToTreeView(hwndTV,pLexArray[i]);

        pCurrentLexer = 0;
        pCurrentStyle = 0;

        //TreeView_Expand(hwndTV,TreeView_GetRoot(hwndTV),TVE_EXPAND);
        TreeView_Select(hwndTV,TreeView_GetRoot(hwndTV),TVGN_CARET);

        SendDlgItemMessage(hwnd,IDC_STYLEEDIT,EM_LIMITTEXT,COUNTOF(lexDefault.Styles[0].szValue)-1,0);

        MakeBitmapButton(hwnd,IDC_PREVSTYLE,g_hInstance,IDB_PREV);
        MakeBitmapButton(hwnd,IDC_NEXTSTYLE,g_hInstance,IDB_NEXT);

        // Setup title font
        if (hFontTitle == NULL) {
          if (NULL == (hFontTitle = (HFONT)SendDlgItemMessage(hwnd,IDC_TITLE,WM_GETFONT,0,0)))
            hFontTitle = GetStockObject(DEFAULT_GUI_FONT);
          GetObject(hFontTitle,sizeof(LOGFONT),&lf);
          lf.lfHeight = (LONG) (lf.lfHeight * 1.2);
          lf.lfWeight = FW_BOLD;
          hFontTitle = CreateFontIndirect(&lf); }

        CenterDlgInParent(hwnd);
      }
      return TRUE;


    case WM_DESTROY:
      DeleteBitmapButton(hwnd,IDC_STYLEFORE);
      DeleteBitmapButton(hwnd,IDC_STYLEBACK);
      DeleteBitmapButton(hwnd,IDC_PREVSTYLE);
      DeleteBitmapButton(hwnd,IDC_NEXTSTYLE);
      return FALSE;


    case WM_CTLCOLORSTATIC:
      {
        DWORD dwId = GetWindowLong((HWND)lParam,GWL_ID);
        HDC hdc = (HDC)wParam;

        if (dwId == IDC_TITLE) {
          SetBkMode(hdc,TRANSPARENT);
          SetTextColor(hdc,GetSysColor(COLOR_BTNTEXT));
          SelectObject(hdc,hFontTitle);
          return(LONG)GetSysColorBrush(COLOR_BTNFACE);
        }
      }
      break;


    case WM_NOTIFY:

      if (((LPNMHDR)(lParam))->idFrom == IDC_STYLELIST)
      {
        LPNMTREEVIEW lpnmtv = (LPNMTREEVIEW)lParam;

        switch (lpnmtv->hdr.code)
        {

          case TVN_SELCHANGED:
            {
              if (pCurrentStyle)
                GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue,COUNTOF(pCurrentStyle->szValue));

              else if (pCurrentLexer) {
                if (!GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions,COUNTOF(pCurrentLexer->szExtensions)))
                  lstrcpy(pCurrentLexer->szExtensions,pCurrentLexer->pszDefExt); }

              // a lexer has been selected
              if (!TreeView_GetParent(hwndTV,lpnmtv->itemNew.hItem))
              {
                WCHAR wch[128];

                GetString(IDS_STYLELABEL,wch,COUNTOF(wch));
                if (StrChr(wch,L'|')) *StrChr(wch,L'|') = 0;

                pCurrentStyle = 0;
                if (pCurrentLexer = (PEDITLEXER)lpnmtv->itemNew.lParam)
                {
                  SetDlgItemText(hwnd,IDC_STYLELABEL,wch);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEEDIT),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFONT),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFORE),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEBACK),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEDEFAULT),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEBOLD),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEITALIC),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEUNDERLINE),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEEOLFILLED),FALSE);
                  //CheckDlgButton(hwnd,IDC_STYLEBOLD,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEITALIC,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,BST_UNCHECKED);
                  SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions);
                }
                else
                {
                  SetDlgItemText(hwnd,IDC_STYLELABEL,L"");
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEEDIT),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFONT),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFORE),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEBACK),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEDEFAULT),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEBOLD),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEITALIC),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEUNDERLINE),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEEOLFILLED),FALSE);
                  //CheckDlgButton(hwnd,IDC_STYLEBOLD,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEITALIC,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,BST_UNCHECKED);
                  SetDlgItemText(hwnd,IDC_STYLEEDIT,L"");
                }
              }

              // a style has been selected
              else
              {
                WCHAR wch[128];

                GetString(IDS_STYLELABEL,wch,COUNTOF(wch));
                if (StrChr(wch,L'|')) *StrChr(wch,L'|') = 0;


                pCurrentLexer = 0;
                if (pCurrentStyle = (PEDITSTYLE)lpnmtv->itemNew.lParam)
                {
                  SetDlgItemText(hwnd,IDC_STYLELABEL,StrEnd(wch)+1);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEEDIT),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFONT),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFORE),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEBACK),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEDEFAULT),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEBOLD),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEITALIC),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEUNDERLINE),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEEOLFILLED),TRUE);
                  //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(pCurrentStyle->szValue,L"bold") ? BST_CHECKED : BST_UNCHECKED));
                  //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(pCurrentStyle->szValue,L"italic") ? BST_CHECKED : BST_UNCHECKED));
                  //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(pCurrentStyle->szValue,L"underline") ? BST_CHECKED : BST_UNCHECKED));
                  //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(pCurrentStyle->szValue,L"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
                  SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue);
                }
                else
                {
                  SetDlgItemText(hwnd,IDC_STYLELABEL,L"");
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEEDIT),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFONT),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFORE),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEBACK),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEDEFAULT),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEBOLD),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEITALIC),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEUNDERLINE),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEEOLFILLED),FALSE);
                  //CheckDlgButton(hwnd,IDC_STYLEBOLD,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEITALIC,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,BST_UNCHECKED);
                  SetDlgItemText(hwnd,IDC_STYLEEDIT,L"");
                }
              }
            }
            break;

          case TVN_BEGINDRAG:
            {
              //HIMAGELIST himl;

              //if (pCurrentStyle)
              //  GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue,COUNTOF(pCurrentStyle->szValue));
              TreeView_Select(hwndTV,lpnmtv->itemNew.hItem,TVGN_CARET);

              //himl = TreeView_CreateDragImage(hwndTV,lpnmtv->itemNew.hItem);
              //ImageList_BeginDrag(himl,0,0,0);
              //ImageList_DragEnter(hwndTV,lpnmtv->ptDrag.x,lpnmtv->ptDrag.y);
              if (pCurrentStyle)
                DestroyCursor(SetCursor(LoadCursor(g_hInstance,MAKEINTRESOURCE(IDC_COPY))));
              else
                DestroyCursor(SetCursor(LoadCursor(NULL,IDC_NO)));

              SetCapture(hwnd);
              fDragging = TRUE;
            }

        }
      }

      break;


    case WM_MOUSEMOVE:
      {
        HTREEITEM htiTarget;
        TVHITTESTINFO tvht;

        if (fDragging && pCurrentStyle)
        {
          LONG xCur = LOWORD(lParam);
          LONG yCur = HIWORD(lParam);

          //ImageList_DragMove(xCur,yCur);
          //ImageList_DragShowNolock(FALSE);

          tvht.pt.x = xCur;
          tvht.pt.y = yCur;

          //ClientToScreen(hwnd,&tvht.pt);
          //ScreenToClient(hwndTV,&tvht.pt);
          MapWindowPoints(hwnd,hwndTV,&tvht.pt,1);

          if ((htiTarget = TreeView_HitTest(hwndTV,&tvht)) != NULL &&
               TreeView_GetParent(hwndTV,htiTarget) != NULL)
          {
            TreeView_SelectDropTarget(hwndTV,htiTarget);
            //TreeView_Expand(hwndTV,htiTarget,TVE_EXPAND);
            TreeView_EnsureVisible(hwndTV,htiTarget);
          }
          else
            TreeView_SelectDropTarget(hwndTV,NULL);

          //ImageList_DragShowNolock(TRUE);
        }
      }
      break;


    case WM_LBUTTONUP:
      {
        if (fDragging)
        {
          HTREEITEM htiTarget;

          //ImageList_EndDrag();

          if (htiTarget = TreeView_GetDropHilight(hwndTV))
          {
            WCHAR tchCopy[256];
            TreeView_SelectDropTarget(hwndTV,NULL);
            GetDlgItemText(hwnd,IDC_STYLEEDIT,tchCopy,COUNTOF(tchCopy));
            TreeView_Select(hwndTV,htiTarget,TVGN_CARET);

            // after select, this is new current item
            if (pCurrentStyle)
            {
              lstrcpy(pCurrentStyle->szValue,tchCopy);
              SetDlgItemText(hwnd,IDC_STYLEEDIT,tchCopy);
              //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(tchCopy,L"bold") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(tchCopy,L"italic") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(tchCopy,L"underline") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(tchCopy,L"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
            }
          }
          ReleaseCapture();
          DestroyCursor(SetCursor(LoadCursor(NULL,IDC_ARROW)));
          fDragging = FALSE;
        }
      }
      break;


    case WM_CANCELMODE:
      {
        if (fDragging)
        {
          //ImageList_EndDrag();
          TreeView_SelectDropTarget(hwndTV,NULL);
          ReleaseCapture();
          DestroyCursor(SetCursor(LoadCursor(NULL,IDC_ARROW)));
          fDragging = FALSE;
        }
      }
      break;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case IDC_PREVSTYLE:
          if (TreeView_GetSelection(hwndTV))
            TreeView_Select(hwndTV,TreeView_GetPrevVisible(hwndTV,
              TreeView_GetSelection(hwndTV)),TVGN_CARET);
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        case IDC_NEXTSTYLE:
          if (TreeView_GetSelection(hwndTV))
            TreeView_Select(hwndTV,TreeView_GetNextVisible(hwndTV,
              TreeView_GetSelection(hwndTV)),TVGN_CARET);
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        case IDC_STYLEFONT:
          if (pCurrentStyle)
          {
            WCHAR tch[256];
            GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
            if (Style_SelectFont(hwnd,tch,COUNTOF(tch),
                  lstrcmpi(pCurrentStyle->pszName,L"Default Style") == 0 ||
                  lstrcmpi(pCurrentStyle->pszName,L"2nd Default Style") == 0)) {
              SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
              //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(tch,L"bold") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(tch,L"italic") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(tch,L"underline") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(tch,L"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
            }
          }
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        case IDC_STYLEFORE:
          if (pCurrentStyle)
          {
            WCHAR tch[256];
            GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
            if (Style_SelectColor(hwnd,TRUE,tch,COUNTOF(tch))) {
              SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
              //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(tch,L"bold") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(tch,L"italic") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(tch,L"underline") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(tch,L"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
            }
          }
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        case IDC_STYLEBACK:
          if (pCurrentStyle)
          {
            WCHAR tch[256];
            GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
            if (Style_SelectColor(hwnd,FALSE,tch,COUNTOF(tch))) {
              SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
              //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(tch,L"bold") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(tch,L"italic") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(tch,L"underline") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(tch,L"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
            }
          }
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        case IDC_STYLEDEFAULT:
          if (pCurrentStyle)
          {
            lstrcpy(pCurrentStyle->szValue,pCurrentStyle->pszDefault);
            SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue);
            //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(pCurrentStyle->szValue,L"bold") ? BST_CHECKED : BST_UNCHECKED));
            //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(pCurrentStyle->szValue,L"italic") ? BST_CHECKED : BST_UNCHECKED));
            //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(pCurrentStyle->szValue,L"underline") ? BST_CHECKED : BST_UNCHECKED));
            //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(pCurrentStyle->szValue,L"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
          }
          else if (pCurrentLexer) {
            lstrcpy(pCurrentLexer->szExtensions,pCurrentLexer->pszDefExt);
            SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions);
          }
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        //case IDC_STYLEBOLD:
        //  if (pCurrentStyle)
        //  {
        //    WCHAR tch[256];
        //    GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
        //    Style_StrSetAttribute(tch,COUNTOF(tch),L"bold",IsDlgButtonChecked(hwnd,IDC_STYLEBOLD));
        //    SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
        //  }
        //  break;

        //case IDC_STYLEITALIC:
        //  if (pCurrentStyle)
        //  {
        //    WCHAR tch[256];
        //    GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
        //    Style_StrSetAttribute(tch,COUNTOF(tch),L"italic",IsDlgButtonChecked(hwnd,IDC_STYLEITALIC));
        //    SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
        //  }
        //  break;

        //case IDC_STYLEUNDERLINE:
        //  if (pCurrentStyle)
        //  {
        //    WCHAR tch[256];
        //    GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
        //    Style_StrSetAttribute(tch,COUNTOF(tch),L"underline",IsDlgButtonChecked(hwnd,IDC_STYLEUNDERLINE));
        //    SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
        //  }
        //  break;

        //case IDC_STYLEEOLFILLED:
        //  if (pCurrentStyle)
        //  {
        //    WCHAR tch[256];
        //    GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
        //    Style_StrSetAttribute(tch,COUNTOF(tch),L"eolfilled",IsDlgButtonChecked(hwnd,IDC_STYLEEOLFILLED));
        //    SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
        //  }
        //  break;

        case IDC_STYLEEDIT:
          {
            if (HIWORD(wParam) == EN_CHANGE) {
              int cr;
              WCHAR tch[256];

              GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));

              cr = -1;
              Style_StrGetColor(TRUE,tch,&cr);
              MakeColorPickButton(hwnd,IDC_STYLEFORE,g_hInstance,cr);

              cr = -1;
              Style_StrGetColor(FALSE,tch,&cr);
              MakeColorPickButton(hwnd,IDC_STYLEBACK,g_hInstance,cr);
            }
          }
          break;

        case IDC_IMPORT:
          {
            HWND hwndTV = GetDlgItem(hwnd,IDC_STYLELIST);

            if (pCurrentStyle)
              GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue,COUNTOF(pCurrentStyle->szValue));
            else if (pCurrentLexer) {
              if (!GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions,COUNTOF(pCurrentLexer->szExtensions)))
                lstrcpy(pCurrentLexer->szExtensions,pCurrentLexer->pszDefExt); }

            if (Style_Import(hwnd)) {

              if (pCurrentStyle)
                SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue);
              else if (pCurrentLexer)
                SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions);

              TreeView_Select(hwndTV,TreeView_GetRoot(hwndTV),TVGN_CARET);

              MsgBox(0,IDS_IMPORT_OK);
            }
          }
          break;

        case IDC_EXPORT:
          {
            if (pCurrentStyle)
              GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue,COUNTOF(pCurrentStyle->szValue));
            else if (pCurrentLexer) {
              if (!GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions,COUNTOF(pCurrentLexer->szExtensions)))
                lstrcpy(pCurrentLexer->szExtensions,pCurrentLexer->pszDefExt); }

            if (Style_Export(hwnd))
              MsgBox(0,IDS_EXPORT_OK);
          }
          break;

        case IDC_PREVIEW:
          {
            // Hack from outside
            extern HWND hwndEdit;
            extern void UpdateLineNumerWidth();

            if (pCurrentStyle)
              GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue,COUNTOF(pCurrentStyle->szValue));
            else if (pCurrentLexer) {
              if (!GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions,COUNTOF(pCurrentLexer->szExtensions)))
                lstrcpy(pCurrentLexer->szExtensions,pCurrentLexer->pszDefExt); }

            Style_SetLexer(hwndEdit,pLexCurrent);
            UpdateLineNumberWidth();
          }
          break;

        case IDOK:
          if (pCurrentStyle)
            GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue,COUNTOF(pCurrentStyle->szValue));
          else if (pCurrentLexer) {
            if (!GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions,COUNTOF(pCurrentLexer->szExtensions)))
              lstrcpy(pCurrentLexer->szExtensions,pCurrentLexer->pszDefExt); }
          EndDialog(hwnd,IDOK);
          break;

        case IDCANCEL:
          if (fDragging)
            SendMessage(hwnd,WM_CANCELMODE,0,0);
          else
            EndDialog(hwnd,IDCANCEL);
          break;

      }

      return TRUE;

  }

  return FALSE;

}


//=============================================================================
//
//  Style_ConfigDlg()
//
void Style_ConfigDlg(HWND hwnd)
{

  WCHAR *StyleBackup[1024];
  int c,cItems,i,iLexer;

  // Backup Styles
  c = 0;
  for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
    StyleBackup[c++] = StrDup(pLexArray[iLexer]->szExtensions);
    i = 0;
    while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
      StyleBackup[c++] = StrDup(pLexArray[iLexer]->Styles[i].szValue);
      i++;
    }
  }
  cItems = c;

  if (IDCANCEL == DialogBoxParam(g_hInstance,
                    MAKEINTRESOURCE(IDD_STYLECONFIG),
                    GetParent(hwnd),Style_ConfigDlgProc,
                    (LPARAM)&StyleBackup))
  {
    // Restore Styles
    c = 0;
    for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
      lstrcpy(pLexArray[iLexer]->szExtensions,StyleBackup[c++]);
      i = 0;
      while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
        lstrcpy(pLexArray[iLexer]->Styles[i].szValue,StyleBackup[c++]);
        i++;
      }
    }
  }

  else
    fStylesModified = TRUE;

  for (c = 0; c < cItems; c++)
    LocalFree(StyleBackup[c]);

  // Apply new (or previous) Styles
  Style_SetLexer(hwnd,pLexCurrent);
}


//=============================================================================
//
//  Style_SelectLexerDlgProc()
//
BOOL CALLBACK Style_SelectLexerDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  static int cxClient;
  static int cyClient;
  static int mmiPtMaxY;
  static int mmiPtMinX;

  static HWND hwndLV;
  static int  iInternalDefault;

  switch(umsg)
  {

    case WM_INITDIALOG:
      {
        int i;
        int lvItems;
        LVITEM lvi;
        SHFILEINFO shfi;
        LVCOLUMN lvc = { LVCF_FMT|LVCF_TEXT, LVCFMT_LEFT, 0, L"", -1, 0, 0, 0 };

        RECT rc;
        WCHAR tch[MAX_PATH];
        int cGrip;

        GetClientRect(hwnd,&rc);
        cxClient = rc.right - rc.left;
        cyClient = rc.bottom - rc.top;

        GetWindowRect(hwnd,&rc);
        mmiPtMaxY = rc.bottom-rc.top;
        mmiPtMinX = rc.right-rc.left;

        if (cxStyleSelectDlg < (rc.right-rc.left))
          cxStyleSelectDlg = rc.right-rc.left;
        if (cyStyleSelectDlg < (rc.bottom-rc.top))
          cyStyleSelectDlg = rc.bottom-rc.top;
        SetWindowPos(hwnd,NULL,rc.left,rc.top,cxStyleSelectDlg,cyStyleSelectDlg,SWP_NOZORDER);

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

        hwndLV = GetDlgItem(hwnd,IDC_STYLELIST);

        ListView_SetImageList(hwndLV,
          (HIMAGELIST)SHGetFileInfo(L"C:\\",0,&shfi,sizeof(SHFILEINFO),SHGFI_SMALLICON | SHGFI_SYSICONINDEX),
          LVSIL_SMALL);

        ListView_SetImageList(hwndLV,
          (HIMAGELIST)SHGetFileInfo(L"C:\\",0,&shfi,sizeof(SHFILEINFO),SHGFI_LARGEICON | SHGFI_SYSICONINDEX),
          LVSIL_NORMAL);

        ListView_SetExtendedListViewStyle(hwndLV,/*LVS_EX_FULLROWSELECT|*/LVS_EX_DOUBLEBUFFER|LVS_EX_LABELTIP);
        ListView_InsertColumn(hwndLV,0,&lvc);

        // Add lexers
        for (i = 0; i < NUMLEXERS; i++)
          Style_AddLexerToListView(hwndLV,pLexArray[i]);

        ListView_SetColumnWidth(hwndLV,0,LVSCW_AUTOSIZE_USEHEADER);

        // Select current lexer
        lvItems = ListView_GetItemCount(hwndLV);
        lvi.mask = LVIF_PARAM;
        for (i = 0; i < lvItems; i++) {
          lvi.iItem = i;
          ListView_GetItem(hwndLV,&lvi);;
          if (lstrcmp(((PEDITLEXER)lvi.lParam)->pszName,pLexCurrent->pszName) == 0) {
            ListView_SetItemState(hwndLV,i,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
            ListView_EnsureVisible(hwndLV,i,FALSE);
            if (iDefaultLexer == i) {
              CheckDlgButton(hwnd,IDC_DEFAULTSCHEME,BST_CHECKED); } } }

        iInternalDefault = iDefaultLexer;

        if (bAutoSelect)
          CheckDlgButton(hwnd,IDC_AUTOSELECT,BST_CHECKED);

        CenterDlgInParent(hwnd);
      }
      return TRUE;


    case WM_DESTROY:
      {
        RECT rc;

        GetWindowRect(hwnd,&rc);
        cxStyleSelectDlg = rc.right-rc.left;
        cyStyleSelectDlg = rc.bottom-rc.top;
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

        GetWindowRect(GetDlgItem(hwnd,IDC_STYLELIST),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_STYLELIST),NULL,0,0,rc.right-rc.left+dxClient,rc.bottom-rc.top+dyClient,SWP_NOZORDER|SWP_NOMOVE);
        ListView_SetColumnWidth(GetDlgItem(hwnd,IDC_STYLELIST),0,LVSCW_AUTOSIZE_USEHEADER);
        InvalidateRect(GetDlgItem(hwnd,IDC_STYLELIST),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDC_AUTOSELECT),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_AUTOSELECT),NULL,rc.left,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDC_AUTOSELECT),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDC_DEFAULTSCHEME),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_DEFAULTSCHEME),NULL,rc.left,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDC_DEFAULTSCHEME),NULL,TRUE);
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
      if (((LPNMHDR)(lParam))->idFrom == IDC_STYLELIST) {

      switch (((LPNMHDR)(lParam))->code) {

        case NM_DBLCLK:
          SendMessage(hwnd,WM_COMMAND,MAKELONG(IDOK,1),0);
          break;

        case LVN_ITEMCHANGED:
        case LVN_DELETEITEM: {
          int i = ListView_GetNextItem(hwndLV,-1,LVNI_ALL | LVNI_SELECTED);
          if (iInternalDefault == i)
            CheckDlgButton(hwnd,IDC_DEFAULTSCHEME,BST_CHECKED);
          else
            CheckDlgButton(hwnd,IDC_DEFAULTSCHEME,BST_UNCHECKED);
          EnableWindow(GetDlgItem(hwnd,IDC_DEFAULTSCHEME),i != -1);
          EnableWindow(GetDlgItem(hwnd,IDOK),i != -1); }
          break; } } }

      return TRUE;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case IDC_DEFAULTSCHEME:
          if (IsDlgButtonChecked(hwnd,IDC_DEFAULTSCHEME) == BST_CHECKED)
            iInternalDefault = ListView_GetNextItem(hwndLV,-1,LVNI_ALL | LVNI_SELECTED);
          else
            iInternalDefault = 0;
          break;


        case IDOK:
          {
            LVITEM lvi;

            lvi.mask = LVIF_PARAM;
            lvi.iItem = ListView_GetNextItem(hwndLV,-1,LVNI_ALL | LVNI_SELECTED);
            if (ListView_GetItem(hwndLV,&lvi)) {
              pLexCurrent = (PEDITLEXER)lvi.lParam;
              iDefaultLexer = iInternalDefault;
              bAutoSelect = (IsDlgButtonChecked(hwnd,IDC_AUTOSELECT) == BST_CHECKED) ? 1 : 0;
              EndDialog(hwnd,IDOK); }
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
//  Style_SelectLexerDlg()
//
void Style_SelectLexerDlg(HWND hwnd)
{
  if (IDOK == DialogBoxParam(g_hInstance,
                MAKEINTRESOURCE(IDD_STYLESELECT),
                GetParent(hwnd),Style_SelectLexerDlgProc,0))

    Style_SetLexer(hwnd,pLexCurrent);
}


// End of Styles.c
