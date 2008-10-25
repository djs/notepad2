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
#include "styles.h"
#include "resource.h"


KEYWORDLIST KeyWords_NULL = {
"", "", "", "", "", "", "", "", "" };


EDITLEXER lexDefault = { SCLEX_NULL, "Default Text", "txt;wtx;log;asc;doc;diz;nfo", "", &KeyWords_NULL, {
                /*  0 */ { STYLE_DEFAULT, "Default Style", "font:Lucida Console;size:10", "" },
                /*  1 */ { STYLE_LINENUMBER, "Margins and Line Numbers", "size:-2;fore:#FF0000", "" },
                /*  2 */ { STYLE_BRACELIGHT, "Matching Braces", "size:+1;fore:#FF0000;bold", "" },
                /*  3 */ { STYLE_BRACEBAD, "Matching Braces Error", "size:+1;fore:#000080;bold", "" },
                /*  4 */ { STYLE_CONTROLCHAR, "Control Characters (Unused)", "", "" },
                /*  5 */ { STYLE_INDENTGUIDE, "Indentation Guide (Color only)", "", "" },
                /*  6 */ { SCI_SETSELFORE+SCI_SETSELBACK, "Selected Text (Colors only)", "back:#0A246A;alpha:95;eolfilled", "" },
                /*  7 */ { SCI_SETWHITESPACEFORE+SCI_SETWHITESPACEBACK, "Whitespace (Colors only)", "fore:#FF0000", "" },
                /*  8 */ { SCI_SETCARETLINEBACK, "Current Line Background (Color only)", "back:#FFFF00;alpha:50", "" },
                /*  9 */ { SCI_SETCARETFORE, "Caret Foreground (Color only)", "", "" },
                /* 10 */ { SCI_SETCARETWIDTH, "Caret Width (Size from 1 to 3)", "", "" },
                /* 11 */ { SCI_SETEDGECOLOUR, "Long Line Marker (Colors only)", "", "" },

                /* 12 */ { STYLE_DEFAULT, "2nd Default Style", "font:Courier New;size:10", "" },
                /* 13 */ { STYLE_LINENUMBER, "2nd Margins and Line Numbers", "font:Tahoma;size:-2;fore:#FF0000", "" },
                /* 14 */ { STYLE_BRACELIGHT, "2nd Matching Braces", "fore:#FF0000;bold", "" },
                /* 15 */ { STYLE_BRACEBAD, "2nd Matching Braces Error", "fore:#000080;bold", "" },
                /* 16 */ { STYLE_CONTROLCHAR, "2nd Control Characters (Unused)", "", "" },
                /* 17 */ { STYLE_INDENTGUIDE, "2nd Indentation Guide (Color only)", "", "" },
                /* 18 */ { SCI_SETSELFORE+SCI_SETSELBACK, "2nd Selected Text (Colors only)", "eolfilled", "" },
                /* 19 */ { SCI_SETWHITESPACEFORE+SCI_SETWHITESPACEBACK, "2nd Whitespace (Colors only)", "fore:#FF0000", "" },
                /* 20 */ { SCI_SETCARETLINEBACK, "2nd Current Line Background (Color only)", "", "" },
                /* 21 */ { SCI_SETCARETFORE, "2nd Caret Foreground (Color only)", "", "" },
                /* 22 */ { SCI_SETCARETWIDTH, "2nd Caret Width (Size from 1 to 3)", "", "" },
                /* 23 */ { SCI_SETEDGECOLOUR, "2nd Long Line Marker (Colors only)", "", "" },
                         { -1, "", "", "" } } };


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
"__class__ __file__ __function__ __line__ __method__ __sleep __wakeup "
"abstract and argc argv array as break case catch cfunction class clone "
"const continue declare default define die do e_all e_error e_parse "
"e_warning echo else elseif empty enddeclare endfor endforeach endif "
"endswitch endwhile eval exception exit extends false final for foreach "
"function global http_cookie_vars http_env_vars http_get_vars "
"http_post_files http_post_vars http_server_vars if implements include "
"include_once interface isset list new not null old_function or parent "
"php_os php_self php_user_filter php_version print private protected "
"public require require_once return static stdclass switch this throw "
"true try unset use var virtual while xor",
"ELEMENT DOCTYPE ATTLIST ENTITY NOTATION",
"", "", "" };


EDITLEXER lexHTML = { SCLEX_HTML, "Web Source Code", "html;htm;asp;aspx;shtml;htd;xhtml;php;php3;phtml;htt;cfm;tpl;dtd;hta;htc", "", &KeyWords_HTML, {
                      { STYLE_DEFAULT, "Default", "", "" },
                      //{ SCE_H_DEFAULT, "Default", "", "" },
                      { SCE_H_TAG, "HTML Tag", "fore:#000080", "" },
                      { SCE_H_TAGUNKNOWN, "HTML Unknown Tag", "fore:#000080", "" },
                      { SCE_H_ATTRIBUTE, "HTML Attribute", "fore:#FF0000", "" },
                      { SCE_H_ATTRIBUTEUNKNOWN, "HTML Unknown Attribute", "fore:#FF0000", "" },
                      { SCE_H_VALUE, "HTML Value", "fore:#3A6EA5", "" },
                      { SCE_H_DOUBLESTRING, "HTML String double quoted", "fore:#3A6EA5", "" },
                      { SCE_H_SINGLESTRING, "HTML String single quoted", "fore:#3A6EA5", "" },
                      { SCE_H_OTHER, "HTML Other Inside Tag", "fore:#3A6EA5", "" },
                      { SCE_H_COMMENT, "HTML Comment", "fore:#808080", "" },
                      { SCE_H_ENTITY, "HTML Entity", "fore:#800080", "" },
                      { SCE_H_TAGEND, "XML Closing Tag", "fore:#000080", "" },
                      { SCE_H_XMLSTART, "XML Identifier Start", "bold;fore:#FF0000", "" },
                      { SCE_H_XMLEND, "XML Identifier End", "bold;fore:#FF0000", "" },
                      { SCE_H_ASP, "ASP Start Tag", "", "fore:#000080;bold" },
                      { SCE_H_ASPAT, "ASP Start Tag @", "fore:#000080;bold", "" },
                      //{ SCE_H_SCRIPT, "Script", "", "" },
                      { SCE_H_CDATA, "CDATA", "", "" },
                      //{ SCE_H_XCCOMMENT, "XC Comment", "", "" },
                      { SCE_H_QUESTION, "PHP Start Tag", "fore:#000080;bold", "" },
                      { SCE_HPHP_DEFAULT, "PHP Default", "", "" },
                      { SCE_HPHP_HSTRING, "PHP String", "fore:#008000", "" },
                      { SCE_HPHP_SIMPLESTRING, "PHP Simple String", "fore:#008000", "" },
                      { SCE_HPHP_WORD, "PHP Keyword", "fore:#800080", "" },
                      { SCE_HPHP_NUMBER, "PHP Number", "fore:#FF0000", "" },
                      { SCE_HPHP_VARIABLE, "PHP Variable", "italic;fore:#000080", "" },
                      { SCE_HPHP_HSTRING_VARIABLE, "PHP String Variable", "italic;fore:#000080", "" },
                      { SCE_HPHP_COMPLEX_VARIABLE, "PHP Complex Variable", "italic;fore:#000080", "" },
                      { SCE_HPHP_COMMENT, "PHP Comment", "fore:#FF8000", "" },
                      { SCE_HPHP_COMMENTLINE, "PHP Comment Line", "fore:#FF8000", "" },
                      { SCE_HPHP_OPERATOR, "PHP Operator", "", "" },
                      //{ SCE_HJ_START, "JS Start", "", "" },
                      { SCE_HJ_DEFAULT, "JS Default", "", "" },
                      { SCE_HJ_COMMENT, "JS Comment", "fore:#008000", "" },
                      { SCE_HJ_COMMENTLINE, "JS Comment Line", "fore:#008000", "" },
                      { SCE_HJ_COMMENTDOC, "JS Comment Doc", "fore:#008000", "" },
                      { SCE_HJ_NUMBER, "JS Number", "fore:#FF0000", "" },
                      { SCE_HJ_WORD, "JS Identifier", "", "" },
                      { SCE_HJ_KEYWORD, "JS Keyword", "fore:#0A246A;bold", "" },
                      { SCE_HJ_DOUBLESTRING, "JS Double String", "fore:#008000", "" },
                      { SCE_HJ_SINGLESTRING, "JS Single String", "fore:#008000", "" },
                      { SCE_HJ_STRINGEOL, "JS String EOL", "fore:#008000", "" },
                      { SCE_HJ_SYMBOLS, "JS Symbols", "", "" },
                      { SCE_HJ_REGEX, "JS Regex", "", "" },
                      //{ SCE_HJA_START, "ASP JS Start", "", "" },
                      { SCE_HJA_DEFAULT, "ASP JS Default", "", "" },
                      { SCE_HJA_COMMENT, "ASP JS Comment", "fore:#008000", "" },
                      { SCE_HJA_COMMENTLINE, "ASP JS Commentline", "fore:#008000", "" },
                      { SCE_HJA_COMMENTDOC, "ASP JS Comment Doc", "fore:#008000", "" },
                      { SCE_HJA_NUMBER, "ASP JS Number", "fore:#FF0000", "" },
                      { SCE_HJA_WORD, "ASP JS Identifier", "", "" },
                      { SCE_HJA_KEYWORD, "ASP JS Keyword", "fore:#0A246A;bold", "" },
                      { SCE_HJA_DOUBLESTRING, "ASP JS Double String", "fore:#008000", "" },
                      { SCE_HJA_SINGLESTRING, "ASP JS Single String", "fore:#008000", "" },
                      { SCE_HJA_STRINGEOL, "ASP JS String EOL", "fore:#008000", "" },
                      { SCE_HJA_SYMBOLS, "ASP JS Symbols", "", "" },
                      { SCE_HJA_REGEX, "ASP JS Regex", "", "" },
                      //{ SCE_HB_START, "VBS Start", "", "" },
                      { SCE_HB_DEFAULT, "VBS Default", "", "" },
                      { SCE_HB_COMMENTLINE, "VBS Comment Line", "fore:#008000", "" },
                      { SCE_HB_NUMBER, "VBS Number", "fore:#FF0000", "" },
                      { SCE_HB_WORD, "VBS Keyword", "fore:#800080;bold", "" },
                      { SCE_HB_STRING, "VBS String", "fore:#008000", "" },
                      { SCE_HB_STRINGEOL, "VBS String EOL", "fore:#008000", "" },
                      { SCE_HB_IDENTIFIER, "VBS Identifier", "", "" },
                      //{ SCE_HBA_START, "ASP VBS Start", "", "" },
                      { SCE_HBA_DEFAULT, "ASP VBS Default", "", "" },
                      { SCE_HBA_COMMENTLINE, "ASP VBS Comment Line", "fore:#008000", "" },
                      { SCE_HBA_NUMBER, "ASP VBS Number", "", "" },
                      { SCE_HBA_WORD, "ASP VBS Keyword", "fore:#800080;bold", "" },
                      { SCE_HBA_STRING, "ASP VBS String", "fore:#008000", "" },
                      { SCE_HBA_STRINGEOL, "ASP VBS String EOL", "fore:#008000", "" },
                      { SCE_HBA_IDENTIFIER, "ASP VBS Identifier", "", "" },
                      //{ SCE_HP_START, "Phyton Start", "", "" },
                      //{ SCE_HP_DEFAULT, "Phyton Default", "", "" },
                      //{ SCE_HP_COMMENTLINE, "Phyton Comment Line", "", "" },
                      //{ SCE_HP_NUMBER, "Phyton Number", "", "" },
                      //{ SCE_HP_STRING, "Phyton String", "", "" },
                      //{ SCE_HP_CHARACTER, "Phyton Character", "", "" },
                      //{ SCE_HP_WORD, "Phyton Keyword", "", "" },
                      //{ SCE_HP_TRIPLE, "Phyton Triple", "", "" },
                      //{ SCE_HP_TRIPLEDOUBLE, "Phyton Triple Double", "", "" },
                      //{ SCE_HP_CLASSNAME, "Phyton Class Name", "", "" },
                      //{ SCE_HP_DEFNAME, "Phyton Def Name", "", "" },
                      //{ SCE_HP_OPERATOR, "Phyton Operator", "", "" },
                      //{ SCE_HP_IDENTIFIER, "Phyton Identifier", "", "" },
                      //{ SCE_HPA_START, "ASP Phyton Start", "", "" },
                      //{ SCE_HPA_DEFAULT, "ASP Phyton Default", "", "" },
                      //{ SCE_HPA_COMMENTLINE, "ASP Phyton Comment Line", "", "" },
                      //{ SCE_HPA_NUMBER, "ASP Phyton Number", "", "" },
                      //{ SCE_HPA_STRING, "ASP Phyton String", "", "" },
                      //{ SCE_HPA_CHARACTER, "ASP Phyton Character", "", "" },
                      //{ SCE_HPA_WORD, "ASP Phyton Keyword", "", "" },
                      //{ SCE_HPA_TRIPLE, "ASP Phyton Triple", "", "" },
                      //{ SCE_HPA_TRIPLEDOUBLE, "ASP Phyton Triple Double", "", "" },
                      //{ SCE_HPA_CLASSNAME, "ASP Phyton Class Name", "", "" },
                      //{ SCE_HPA_DEFNAME, "ASP Phyton Def Name", "", "" },
                      //{ SCE_HPA_OPERATOR, "ASP Phyton Operator", "", "" },
                      //{ SCE_HPA_IDENTIFIER, "ASP Phyton Identifier", "", "" },
                      //{ SCE_H_SGML_DEFAULT, "SGML Default", "", "" },
                      //{ SCE_H_SGML_COMMAND, "SGML Command", "","" },
                      //{ SCE_H_SGML_1ST_PARAM, "SGML 1st Param", "","" },
                      //{ SCE_H_SGML_DOUBLESTRING, "SGML Double String", "", "" },
                      //{ SCE_H_SGML_SIMPLESTRING, "SGML Single Stirng", "", "" },
                      //{ SCE_H_SGML_ERROR, "SGML Error", "", "" },
                      //{ SCE_H_SGML_SPECIAL, "SGML Special", "", "" },
                      //{ SCE_H_SGML_ENTITY, "SGML Entity", "", "" },
                      //{ SCE_H_SGML_COMMENT, "SGML Comment", "", "" },
                      //{ SCE_H_SGML_1ST_PARAM_COMMENT, "SGML 1st Param Comment", "", "" },
                      //{ SCE_H_SGML_BLOCK_DEFAULT, "SGML Block Default", "", "" },
                      { -1, "", "", "" } } };


KEYWORDLIST KeyWords_XML = {
"", "", "", "", "", "ELEMENT DOCTYPE ATTLIST ENTITY NOTATION", "", "", "" };


EDITLEXER lexXML = { SCLEX_XML, "XML Document", "xml;xsl;svg;xul;xsd;xslt;axl;rdf;vcproj;manifest", "", &KeyWords_XML, {
                     { STYLE_DEFAULT, "Default", "", "" },
                     //{ SCE_H_DEFAULT, "Default", "", "" },
                     { SCE_H_TAG, "XML Tag", "fore:#800080", "" },
                     { SCE_H_TAGEND, "XML Closing Tag", "fore:#800080", "" },
                     //{ SCE_H_TAGUNKNOWN, "XML Unknown Tag", "fore:#000080", "" },
                     { SCE_H_ATTRIBUTE, "XML Attribute", "fore:#FF0000", "" },
                     //{ SCE_H_ATTRIBUTEUNKNOWN, "XML Unknown Attribute", "fore:#FF0000", "" },
                     { SCE_H_VALUE, "XML Value", "fore:#3A6EA5", "" },
                     { SCE_H_DOUBLESTRING, "XML String double quoted", "fore:#3A6EA5", "" },
                     { SCE_H_SINGLESTRING, "XML String single quoted", "fore:#3A6EA5", "" },
                     { SCE_H_OTHER, "XML Other Inside Tag", "fore:#3A6EA5", "" },
                     { SCE_H_COMMENT, "XML Comment", "fore:#808080", "" },
                     { SCE_H_ENTITY, "XML Entity", "fore:#800080", "" },
                     { SCE_H_XMLSTART, "XML Identifier Start", "bold;fore:#FF0000", "" },
                     { SCE_H_XMLEND, "XML Identifier End", "bold;fore:#FF0000", "" },
                     //{ SCE_H_ASP, "ASP Start Tag", "", "fore:#000080;bold" },
                     //{ SCE_H_ASPAT, "ASP Start Tag @", "fore:#000080;bold", "" },
                     //{ SCE_H_SCRIPT, "Script", "", "" },
                     { SCE_H_CDATA, "CDATA", "fore:#008000", "" },
                     //{ SCE_H_XCCOMMENT, "XC Comment", "", "" },
                     //{ SCE_H_QUESTION, "PHP Start Tag", "fore:#000080;bold", "" },
                     //{ SCE_HPHP_DEFAULT, "PHP Default", "", "" },
                     //{ SCE_HPHP_HSTRING, "PHP String", "fore:#008000", "" },
                     //{ SCE_HPHP_SIMPLESTRING, "PHP Simple String", "fore:#008000", "" },
                     //{ SCE_HPHP_WORD, "PHP Keyword", "fore:#800080", "" },
                     //{ SCE_HPHP_NUMBER, "PHP Number", "fore:#FF0000", "" },
                     //{ SCE_HPHP_VARIABLE, "PHP Variable", "italic;fore:#000080", "" },
                     //{ SCE_HPHP_HSTRING_VARIABLE, "PHP String Variable", "italic;fore:#000080", "" },
                     //{ SCE_HPHP_COMPLEX_VARIABLE, "PHP Complex Variable", "italic;fore:#000080", "" },
                     //{ SCE_HPHP_COMMENT, "PHP Comment", "fore:#FF8000", "" },
                     //{ SCE_HPHP_COMMENTLINE, "PHP Comment Line", "fore:#FF8000", "" },
                     //{ SCE_HPHP_OPERATOR, "PHP Operator", "", "" },
                     //{ SCE_HJ_START, "JS Start", "", "" },
                     //{ SCE_HJ_DEFAULT, "JS Default", "", "" },
                     //{ SCE_HJ_COMMENT, "JS Comment", "fore:#008000", "" },
                     //{ SCE_HJ_COMMENTLINE, "JS Comment Line", "fore:#008000", "" },
                     //{ SCE_HJ_COMMENTDOC, "JS Comment Doc", "fore:#008000", "" },
                     //{ SCE_HJ_NUMBER, "JS Number", "fore:#FF0000", "" },
                     //{ SCE_HJ_WORD, "JS Identifier", "", "" },
                     //{ SCE_HJ_KEYWORD, "JS Keyword", "fore:#0A246A;bold", "" },
                     //{ SCE_HJ_DOUBLESTRING, "JS Double String", "fore:#008000", "" },
                     //{ SCE_HJ_SINGLESTRING, "JS Single String", "fore:#008000", "" },
                     //{ SCE_HJ_STRINGEOL, "JS String EOL", "fore:#008000", "" },
                     //{ SCE_HJ_SYMBOLS, "JS Symbols", "", "" },
                     //{ SCE_HJ_REGEX, "JS Regex", "", "" },
                     //{ SCE_HJA_START, "ASP JS Start", "", "" },
                     //{ SCE_HJA_DEFAULT, "ASP JS Default", "", "" },
                     //{ SCE_HJA_COMMENT, "ASP JS Comment", "fore:#008000", "" },
                     //{ SCE_HJA_COMMENTLINE, "ASP JS Commentline", "fore:#008000", "" },
                     //{ SCE_HJA_COMMENTDOC, "ASP JS Comment Doc", "fore:#008000", "" },
                     //{ SCE_HJA_NUMBER, "ASP JS Number", "fore:#FF0000", "" },
                     //{ SCE_HJA_WORD, "ASP JS Identifier", "", "" },
                     //{ SCE_HJA_KEYWORD, "ASP JS Keyword", "fore:#0A246A;bold", "" },
                     //{ SCE_HJA_DOUBLESTRING, "ASP JS Double String", "fore:#008000", "" },
                     //{ SCE_HJA_SINGLESTRING, "ASP JS Single String", "fore:#008000", "" },
                     //{ SCE_HJA_STRINGEOL, "ASP JS String EOL", "fore:#008000", "" },
                     //{ SCE_HJA_SYMBOLS, "ASP JS Symbols", "", "" },
                     //{ SCE_HJA_REGEX, "ASP JS Regex", "", "" },
                     //{ SCE_HB_START, "VBS Start", "", "" },
                     //{ SCE_HB_DEFAULT, "VBS Default", "", "" },
                     //{ SCE_HB_COMMENTLINE, "VBS Comment Line", "fore:#008000", "" },
                     //{ SCE_HB_NUMBER, "VBS Number", "fore:#FF0000", "" },
                     //{ SCE_HB_WORD, "VBS Keyword", "fore:#800080;bold", "" },
                     //{ SCE_HB_STRING, "VBS String", "fore:#008000", "" },
                     //{ SCE_HB_STRINGEOL, "VBS String EOL", "fore:#008000", "" },
                     //{ SCE_HB_IDENTIFIER, "VBS Identifier", "", "" },
                     //{ SCE_HBA_START, "ASP VBS Start", "", "" },
                     //{ SCE_HBA_DEFAULT, "ASP VBS Default", "", "" },
                     //{ SCE_HBA_COMMENTLINE, "ASP VBS Comment Line ", "fore:#008000", "" },
                     //{ SCE_HBA_NUMBER, "ASP VBS Number", "", "" },
                     //{ SCE_HBA_WORD, "ASP VBS Keyword", "fore:#800080;bold", "" },
                     //{ SCE_HBA_STRING, "ASP VBS String", "fore:#008000", "" },
                     //{ SCE_HBA_STRINGEOL, "ASP VBS String EOL", "fore:#008000", "" },
                     //{ SCE_HBA_IDENTIFIER, "ASP VBS Identifier", "", "" },
                     //{ SCE_HP_START, "Phyton Start", "", "" },
                     //{ SCE_HP_DEFAULT, "Phyton Default", "", "" },
                     //{ SCE_HP_COMMENTLINE, "Phyton Comment Line", "", "" },
                     //{ SCE_HP_NUMBER, "Phyton Number", "", "" },
                     //{ SCE_HP_STRING, "Phyton String", "", "" },
                     //{ SCE_HP_CHARACTER, "Phyton Character", "", "" },
                     //{ SCE_HP_WORD, "Phyton Keyword", "", "" },
                     //{ SCE_HP_TRIPLE, "Phyton Triple", "", "" },
                     //{ SCE_HP_TRIPLEDOUBLE, "Phyton Triple Double", "", "" },
                     //{ SCE_HP_CLASSNAME, "Phyton Class Name", "", "" },
                     //{ SCE_HP_DEFNAME, "Phyton Def Name", "", "" },
                     //{ SCE_HP_OPERATOR, "Phyton Operator", "", "" },
                     //{ SCE_HP_IDENTIFIER, "Phyton Identifier", "", "" },
                     //{ SCE_HPA_START, "ASP Phyton Start", "", "" },
                     //{ SCE_HPA_DEFAULT, "ASP Phyton Default", "", "" },
                     //{ SCE_HPA_COMMENTLINE, "ASP Phyton Comment Line", "", "" },
                     //{ SCE_HPA_NUMBER, "ASP Phyton Number", "", "" },
                     //{ SCE_HPA_STRING, "ASP Phyton String", "", "" },
                     //{ SCE_HPA_CHARACTER, "ASP Phyton Character", "", "" },
                     //{ SCE_HPA_WORD, "ASP Phyton Keyword", "", "" },
                     //{ SCE_HPA_TRIPLE, "ASP Phyton Triple", "", "" },
                     //{ SCE_HPA_TRIPLEDOUBLE, "ASP Phyton Triple Double", "", "" },
                     //{ SCE_HPA_CLASSNAME, "ASP Phyton Class Name", "", "" },
                     //{ SCE_HPA_DEFNAME, "ASP Phyton Def Name", "", "" },
                     //{ SCE_HPA_OPERATOR, "ASP Phyton Operator", "", "" },
                     //{ SCE_HPA_IDENTIFIER, "ASP Phyton Identifier", "", "" },
                     //{ SCE_H_SGML_DEFAULT, "SGML Default", "", "" },
                     //{ SCE_H_SGML_COMMAND, "SGML Command", "","" },
                     //{ SCE_H_SGML_1ST_PARAM, "SGML 1st Param", "","" },
                     //{ SCE_H_SGML_DOUBLESTRING, "SGML Double String", "", "" },
                     //{ SCE_H_SGML_SIMPLESTRING, "SGML Single Stirng", "", "" },
                     //{ SCE_H_SGML_ERROR, "SGML Error", "", "" },
                     //{ SCE_H_SGML_SPECIAL, "SGML Special", "", "" },
                     //{ SCE_H_SGML_ENTITY, "SGML Entity", "", "" },
                     //{ SCE_H_SGML_COMMENT, "SGML Comment", "", "" },
                     //{ SCE_H_SGML_1ST_PARAM_COMMENT, "SGML 1st Param Comment", "", "" },
                     //{ SCE_H_SGML_BLOCK_DEFAULT, "SGML Block Default", "", "" },
                     { -1, "", "", "" } } };


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


EDITLEXER lexCSS = { SCLEX_CSS, "CSS Style Sheets", "css", "", &KeyWords_CSS, {
                     { STYLE_DEFAULT, "Default", "", "" },
                     //{ SCE_CSS_DEFAULT, "Default", "", "" },
                     { SCE_CSS_TAG, "Tag", "fore:#0A246A", "" },
                     { SCE_CSS_CLASS, "Class", "fore:#FF0000", "" },
                     { SCE_CSS_PSEUDOCLASS, "Pseudoclass", "fore:#FF0000", "" },
                     { SCE_CSS_UNKNOWN_PSEUDOCLASS, "Unknown Pseudoclass", "fore:#FF0000", "" },
                     { SCE_CSS_OPERATOR, "Operator", "", "" },
                     { SCE_CSS_IDENTIFIER, "Identifier", "fore:#3A6EA5", "" },
                     { SCE_CSS_UNKNOWN_IDENTIFIER, "Unknown Identifier", "", "" },
                     { SCE_CSS_IDENTIFIER2, "CSS 2 Keyword", "fore:#3A6EA5", "" },
                     { SCE_CSS_VALUE, "Value", "", "" },
                     { SCE_CSS_COMMENT, "Comment", "fore:#008000", "" },
                     { SCE_CSS_ID, "Id", "", "" },
                     { SCE_CSS_IMPORTANT, "Important", "", "" },
                     { SCE_CSS_DIRECTIVE, "Directive", "", "" },
                     { SCE_CSS_ATTRIBUTE, "Attribute", "italic", "" },
                     { SCE_CSS_DOUBLESTRING, "String double quoted", "fore:#008000", "" },
                     { SCE_CSS_SINGLESTRING, "String single quoted", "fore:#008000", "" },
                     { -1, "", "", "" } } };


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


EDITLEXER lexCPP = { SCLEX_CPP, "C/C++ Source Code", "c;cpp;cxx;cc;h;hpp;hxx;hh", "", &KeyWords_CPP, {
                     { STYLE_DEFAULT, "Default", "", "" },
                     //{ SCE_C_DEFAULT, "Default", "", "" },
                     { SCE_C_COMMENT, "Comment", "fore:#008000", "" },
                     { SCE_C_COMMENTLINE, "Comment Line", "fore:#008000", "" },
                     { SCE_C_COMMENTDOC, "Comment Doc", "fore:#008000", "" },
                     { SCE_C_COMMENTLINEDOC, "Comment Line Doc", "fore:#008000", "" },
                     { SCE_C_WORD, "Keyword", "fore:#0A246A;bold", "" },
                     { SCE_C_IDENTIFIER, "Identifier", "", "" },
                     { SCE_C_NUMBER, "Number", "fore:#FF0000", "" },
                     { SCE_C_STRING, "String", "fore:#008000", "" },
                     { SCE_C_STRINGEOL, "String not closed", "fore:#008000;back:#FFFF80;eolfilled", "" },
                     { SCE_C_CHARACTER, "Character", "fore:#008000", "" },
                     //{ SCE_C_UUID, "UUID", "", "" },
                     { SCE_C_OPERATOR, "Operator", "fore:#0A246A", "" },
                     { SCE_C_PREPROCESSOR, "Preprocessor", "fore:#FF8000", "" },
                     //{ SCE_C_VERBATIM, "Verbatim", "", "" },
                     //{ SCE_C_REGEX, "Regex", "", "" },
                     //{ SCE_C_WORD2, "Word 2", "", "" },
                     { SCE_C_COMMENTDOCKEYWORD, "Comment Doc Keyword", "fore:#008000", "" },
                     { SCE_C_COMMENTDOCKEYWORDERROR, "Comment Doc Keyword Error", "fore:#008000", "" },
                     //{ SCE_C_GLOBALCLASS, "Global Class", "", "" },
                     { -1, "", "", "" } } };


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


EDITLEXER lexCS = { SCLEX_CPP, "C# Source Code", "cs", "", &KeyWords_CS, {
                    { STYLE_DEFAULT, "Default", "", "" },
                    //{ SCE_C_DEFAULT, "Default", "", "" },
                    { SCE_C_COMMENT, "Comment", "fore:#008000", "" },
                    { SCE_C_COMMENTLINE, "Comment Line", "fore:#008000", "" },
                    { SCE_C_COMMENTDOC, "Comment Doc", "fore:#008000", "" },
                    { SCE_C_COMMENTLINEDOC, "Comment Line Doc", "fore:#008000", "" },
                    { SCE_C_WORD, "Keyword", "fore:#0A246A;bold", "" },
                    { SCE_C_IDENTIFIER, "Identifier", "", "" },
                    { SCE_C_NUMBER, "Number", "fore:#FF0000", "" },
                    { SCE_C_STRING, "String", "fore:#008000", "" },
                    { SCE_C_VERBATIM, "Verbatim String", "fore:#008000", "" },
                    { SCE_C_STRINGEOL, "String not closed", "fore:#008000;back:#FFFF80;eolfilled", "" },
                    { SCE_C_CHARACTER, "Character", "fore:#008000", "" },
                    //{ SCE_C_UUID, "UUID", "", "" },
                    { SCE_C_OPERATOR, "Operator", "fore:#0A246A", "" },
                    { SCE_C_PREPROCESSOR, "Preprocessor", "fore:#FF8000", "" },
                    //{ SCE_C_REGEX, "Regex", "", "" },
                    //{ SCE_C_WORD2, "Word 2", "", "" },
                    { SCE_C_COMMENTDOCKEYWORD, "Comment Doc Keyword", "fore:#008000", "" },
                    { SCE_C_COMMENTDOCKEYWORDERROR, "Comment Doc Keyword Error", "fore:#008000", "" },
                    //{ SCE_C_GLOBALCLASS, "Global Class", "", "" },
                    { -1, "", "", "" } } };


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


EDITLEXER lexRC = { SCLEX_CPP, "Resource Script", "rc;rc2;rct;dlg", "", &KeyWords_RC, {
                    { STYLE_DEFAULT, "Default", "", "" },
                    //{ SCE_C_DEFAULT, "Default", "", "" },
                    { SCE_C_COMMENT, "Comment", "fore:#008000", "" },
                    { SCE_C_COMMENTLINE, "Comment Line", "fore:#008000", "" },
                    { SCE_C_COMMENTDOC, "Comment Doc", "fore:#008000", "" },
                    { SCE_C_WORD, "Keyword", "fore:#0A246A;bold", "" },
                    { SCE_C_IDENTIFIER, "Identifier", "", "" },
                    { SCE_C_NUMBER, "Number", "fore:#FF0000", "" },
                    { SCE_C_STRING, "String", "fore:#008000", "" },
                    { SCE_C_STRINGEOL, "String not closed", "fore:#008000;back:#FFFF80;eolfilled", "" },
                    { SCE_C_CHARACTER, "Character", "fore:#008000", "" },
                    //{ SCE_C_UUID, "UUID", "", "" },
                    { SCE_C_OPERATOR, "Operator", "fore:#0A246A", "" },
                    { SCE_C_PREPROCESSOR, "Preprocessor", "fore:#FF8000", "" },
                    //{ SCE_C_VERBATIM, "Verbatim", "", "" },
                    //{ SCE_C_REGEX, "Regex", "", "" },
                    //{ SCE_C_COMMENTLINEDOC, "Comment Line Doc", "", "" },
                    //{ SCE_C_WORD2, "Word 2", "", "" },
                    //{ SCE_C_COMMENTDOCKEYWORD, "Comment Doc Keyword", "", "" },
                    //{ SCE_C_COMMENTDOCKEYWORDERROR, "Comment Doc Keyword Error", "", "" },
                    //{ SCE_C_GLOBALCLASS, "Global Class", "", "" },
                    { -1, "", "", "" } } };


KEYWORDLIST KeyWords_MAK = {
"", "", "", "", "", "", "", "", "" };


EDITLEXER lexMAK = { SCLEX_MAKEFILE, "Makefiles", "mak;make;dsp", "", &KeyWords_MAK, {
                     { STYLE_DEFAULT, "Default", "", "" },
                     //{ SCE_MAKE_DEFAULT, "Default", "", "" },
                     { SCE_MAKE_COMMENT, "Comment", "fore:#008000", "" },
                     { SCE_MAKE_PREPROCESSOR, "Preprocessor", "fore:#FF8000", "" },
                     { SCE_MAKE_IDENTIFIER, "Identifier", "", "" },
                     { SCE_MAKE_OPERATOR, "Operator", "", "" },
                     { SCE_MAKE_TARGET, "Target", "", "" },
                     { SCE_MAKE_IDEOL, "ID EOL", "", "" },
                     { -1, "", "", "" } } };


KEYWORDLIST KeyWords_VBS = {
"and begin case call continue do each else elseif end erase error event exit false "
"for function get gosub goto if implement in load loop lset me mid new next not nothing "
"on or property raiseevent rem resume return rset select set stop sub then to true unload "
"until wend while with withevents attribute alias as boolean byref byte byval const compare "
"currency date declare dim double enum explicit friend global integer let lib long module object "
"option optional preserve private property public redim single static string type variant",
"", "", "", "", "", "", "", "" };


EDITLEXER lexVBS = { SCLEX_VBSCRIPT, "VBScript", "vbs;dsm", "", &KeyWords_VBS, {
                    { STYLE_DEFAULT, "Default", "", "" },
                    //{ SCE_B_DEFAULT, "Default", "", "" },
                    { SCE_B_COMMENT, "Comment", "fore:#008000", "" },
                    { SCE_B_NUMBER, "Number", "fore:#FF0000", "" },
                    { SCE_B_KEYWORD, "Keyword", "fore:#0A246A;bold", "" },
                    { SCE_B_STRING, "String", "fore:#008000", "" },
                    { SCE_B_STRINGEOL, "String not closed", "fore:#008000;back:#FFFF80;eolfilled", "" },
                    { SCE_B_OPERATOR, "Operator", "", "" },
                    { SCE_B_IDENTIFIER, "Identifier", "", "" },
                    //{ SCE_B_DATE, "Date", "", "" },
                    //{ SCE_B_KEYWORD2, "Keyword 2", "", "" },
                    //{ SCE_B_KEYWORD3, "Keyword 3", "", "" },
                    //{ SCE_B_KEYWORD4, "Keyword 4", "", "" },
                    { SCE_B_CONSTANT, "Constant", "", "" },
                    { SCE_B_PREPROCESSOR, "Preprocessor", "#fore:#808080", "" },
                    { SCE_B_ASM, "Assembler", "fore:#FF8000", "" },
                    { -1, "", "", "" } } };


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


EDITLEXER lexVB = { SCLEX_VB, "Visual Basic", "vb;bas;frm;cls;ctl;pag;dsr;dob", "", &KeyWords_VB, {
                    { STYLE_DEFAULT, "Default", "", "" },
                    //{ SCE_B_DEFAULT, "Default", "", "" },
                    { SCE_B_COMMENT, "Comment", "fore:#008000", "" },
                    { SCE_B_NUMBER, "Number", "fore:#FF0000", "" },
                    { SCE_B_KEYWORD, "Keyword", "fore:#0A246A;bold", "" },
                    { SCE_B_STRING, "String", "fore:#008000", "" },
                    { SCE_B_STRINGEOL, "String not closed", "fore:#008000;back:#FFFF80;eolfilled", "" },
                    { SCE_B_OPERATOR, "Operator", "", "" },
                    { SCE_B_IDENTIFIER, "Identifier", "", "" },
                    //{ SCE_B_DATE, "Date", "", "" },
                    //{ SCE_B_KEYWORD2, "Keyword 2", "", "" },
                    //{ SCE_B_KEYWORD3, "Keyword 3", "", "" },
                    //{ SCE_B_KEYWORD4, "Keyword 4", "", "" },
                    { SCE_B_CONSTANT, "Constant", "", "" },
                    { SCE_B_PREPROCESSOR, "Preprocessor", "#fore:#808080", "" },
                    { SCE_B_ASM, "Assembler", "fore:#FF8000", "" },
                    { -1, "", "", "" } } };


KEYWORDLIST KeyWords_JS = {
"abstract boolean break byte case catch char class const continue "
"debugger default delete do double else enum export extends final "
"finally float for function goto if implements import in instanceof int "
"interface long native new package private protected public return "
"short static super switch synchronized this throw throws transient try "
"typeof var void volatile while with",
"", "", "", "", "", "", "", "" };


EDITLEXER lexJS = { SCLEX_CPP, "JavaScript", "js;jse", "", &KeyWords_JS, {
                    { STYLE_DEFAULT, "Default", "", "" },
                    //{ SCE_C_DEFAULT, "Default", "", "" },
                    { SCE_C_COMMENT, "Comment", "fore:#008000", "" },
                    { SCE_C_COMMENTLINE, "Comment Line", "fore:#008000", "" },
                    { SCE_C_COMMENTDOC, "Comment Doc", "fore:#008000", "" },
                    { SCE_C_COMMENTLINEDOC, "Comment Line Doc", "fore:#008000", "" },
                    { SCE_C_WORD, "Keyword", "fore:#0A246A;bold", "" },
                    { SCE_C_IDENTIFIER, "Identifier", "", "" },
                    { SCE_C_NUMBER, "Number", "fore:#FF0000", "" },
                    { SCE_C_STRING, "String", "fore:#008000", "" },
                    { SCE_C_STRINGEOL, "String not closed", "fore:#008000;back:#FFFF80;eolfilled", "" },
                    { SCE_C_CHARACTER, "Character", "fore:#008000", "" },
                    //{ SCE_C_UUID, "UUID", "", "" },
                    { SCE_C_OPERATOR, "Operator", "fore:#0A246A", "" },
                    { SCE_C_PREPROCESSOR, "Preprocessor", "fore:#FF8000", "" },
                    //{ SCE_C_VERBATIM, "Verbatim", "", "" },
                    { SCE_C_REGEX, "Regular Expression", "fore:#008000", "" },
                    //{ SCE_C_WORD2, "Word 2", "", "" },
                    //{ SCE_C_COMMENTDOCKEYWORD, "Comment Doc Keyword", "", "" },
                    //{ SCE_C_COMMENTDOCKEYWORDERROR, "Comment Doc Keyword Error", "", "" },
                    //{ SCE_C_GLOBALCLASS, "Global Class", "", "" },
                    { -1, "", "", "" } } };


KEYWORDLIST KeyWords_JAVA = {
"abstract assert boolean break byte case catch char class const "
"continue default do double else extends final finally float for future "
"generic goto if implements import inner instanceof int interface long "
"native new null outer package private protected public rest return "
"short static super switch synchronized this throw throws transient try "
"var void volatile while",
"", "", "", "", "", "", "", "" };


EDITLEXER lexJAVA = { SCLEX_CPP, "Java Source Code", "java", "", &KeyWords_JAVA, {
                      { STYLE_DEFAULT, "Default", "", "" },
                      //{ SCE_C_DEFAULT, "Default", "", "" },
                      { SCE_C_COMMENT, "Comment", "fore:#008000", "" },
                      { SCE_C_COMMENTLINE, "Comment Line", "fore:#008000", "" },
                      { SCE_C_COMMENTDOC, "Comment Doc", "fore:#008000", "" },
                      { SCE_C_COMMENTLINEDOC, "Comment Line Doc", "fore:#008000", "" },
                      { SCE_C_WORD, "Keyword", "fore:#0A246A;bold", "" },
                      { SCE_C_IDENTIFIER, "Identifier", "", "" },
                      { SCE_C_NUMBER, "Number", "fore:#FF0000", "" },
                      { SCE_C_STRING, "String", "fore:#008000", "" },
                      { SCE_C_STRINGEOL, "String not closed", "fore:#008000;back:#FFFF80;eolfilled", "" },
                      { SCE_C_CHARACTER, "Character", "fore:#008000", "" },
                      //{ SCE_C_UUID, "UUID", "", "" },
                      { SCE_C_OPERATOR, "Operator", "fore:#0A246A", "" },
                      { SCE_C_PREPROCESSOR, "Preprocessor", "fore:#FF8000", "" },
                      //{ SCE_C_VERBATIM, "Verbatim", "", "" },
                      { SCE_C_REGEX, "Regular Expression", "fore:#008000", "" },
                      //{ SCE_C_WORD2, "Word 2", "", "" },
                      //{ SCE_C_COMMENTDOCKEYWORD, "Comment Doc Keyword", "", "" },
                      //{ SCE_C_COMMENTDOCKEYWORDERROR, "Comment Doc Keyword Error", "", "" },
                      //{ SCE_C_GLOBALCLASS, "Global Class", "", "" },
                      { -1, "", "", "" } } };


KEYWORDLIST KeyWords_PAS = {
"and array asm begin case cdecl class const constructor default destructor "
"div do downto else end end. except exit exports external far file finalization "
"finally for function goto if implementation in index inherited initialization inline "
"interface label library message mod near nil not object of on or out overload "
"override packed pascal private procedure program property protected public "
"published raise read record register repeat resourcestring safecall set shl shr "
"stdcall stored string then threadvar to try type unit until uses var virtual "
"while with write xor",
"write read default public protected private property published stored",
"", "", "", "", "", "", "" };


EDITLEXER lexPAS = { SCLEX_PASCAL, "Pascal Source Code", "pas;dpr;dpk;dfm;inc;pp", "", &KeyWords_PAS, {
                     { STYLE_DEFAULT, "Default", "", "" },
                     //{ 0, "Default", "", "" },
                     { SCE_C_COMMENT, "Comment", "fore:#008000", "" },
                     { SCE_C_COMMENTLINE, "Comment Line", "fore:#008000", "" },
                     { SCE_C_COMMENTDOC, "Comment Doc", "fore:#008000", "" },
                     { SCE_C_WORD, "Keyword", "fore:#800080;bold", "" },
                     { SCE_C_IDENTIFIER, "Identifier", "", "" },
                     //{ SCE_C_STRING, "String", "", "" },
                     //{ SCE_C_STRINGEOL, "String eol", "", "" },
                     { SCE_C_NUMBER, "Number", "fore:#FF0000", "" },
                     { SCE_C_CHARACTER, "String", "fore:#008000", "" },
                     { SCE_C_OPERATOR, "Operator", "fore:#0A246A", "" },
                     { SCE_C_PREPROCESSOR, "Preprocessor", "fore:#808080", "" },
                     { SCE_C_REGEX, "Inline Asm", "fore:#FF8000", "" },
                     //{ SCE_C_COMMENTDOCKEYWORD, "Comment Doc Keyword", "fore:#008000", "" },
                     //{ SCE_C_COMMENTDOCKEYWORDERROR, "Comment Doc Keyword Error", "fore:#008000", "" },
                     { -1, "", "", "" } } };


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


EDITLEXER lexASM = { SCLEX_ASM, "Assembly Script", "asm", "", &KeyWords_ASM, {
                     { STYLE_DEFAULT, "Default", "", "" },
                     //{ SCE_ASM_DEFAULT, "Default", "", "" },
                     { SCE_ASM_COMMENT, "Comment", "fore:#008000", "" },
                     //{ SCE_ASM_COMMENTBLOCK, "Comment Block (not used)", "", "" },
                     { SCE_ASM_NUMBER, "Number", "fore:#FF0000", "" },
                     { SCE_ASM_STRING, "String double quoted", "fore:#008000", "" },
                     { SCE_ASM_CHARACTER, "String single quoted", "fore:#008000", "" },
                     { SCE_ASM_STRINGEOL, "String not closed", "fore:#008000;back:#FFFF80;eolfilled", "" },
                     { SCE_ASM_OPERATOR, "Operator", "fore:#0A246A", "" },
                     { SCE_ASM_IDENTIFIER, "Identifier", "", "" },
                     { SCE_ASM_CPUINSTRUCTION, "CPU Instruction", "fore:#0A246A", "" },
                     { SCE_ASM_MATHINSTRUCTION, "FPU Instruction", "fore:#0A246A", "" },
                     { SCE_ASM_REGISTER, "Register", "fore:#FF8000", "" },
                     { SCE_ASM_DIRECTIVE, "Directive", "fore:#0A246A", "" },
                     { SCE_ASM_DIRECTIVEOPERAND, "Directive Operand", "fore:#0A246A", "" },
                     { SCE_ASM_EXTINSTRUCTION, "Extended Instruction", "fore:#0A246A", "" },
                     { -1, "", "", "" } } };



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


EDITLEXER lexPL = { SCLEX_PERL, "Perl Script", "pl;pm;cgi;pod", "", &KeyWords_PL, {
                    { STYLE_DEFAULT, "Default", "", "" },
                    //{ SCE_PL_DEFAULT, "Default", "", "" },
                    { SCE_PL_ERROR, "Error", "", "" },
                    { SCE_PL_COMMENTLINE, "Comment", "fore:#008000", "" },
                    { SCE_PL_NUMBER, "Number", "", "" },
                    { SCE_PL_WORD, "Keyword", "fore:#FF8000;bold", "" },
                    { SCE_PL_STRING, "String double quoted", "fore:#008000", "" },
                    { SCE_PL_CHARACTER, "String single quoted", "fore:#008000", "" },
                    //{ SCE_PL_PUNCTUATION, "Symbols / Punctuation (not used)", "", "" },
                    //{ SCE_PL_PREPROCESSOR, "Preprocessor (not used)", "", "" },
                    { SCE_PL_OPERATOR, "Operator", "", "" },
                    { SCE_PL_IDENTIFIER, "Identifier", "", "" },
                    { SCE_PL_POD, "POD at beginning of line", "", "" },
                    { SCE_PL_POD_VERB, "POD verbatim paragraphs", "", "" },
                    { SCE_PL_SCALAR, "Scalar $", "", "" },
                    { SCE_PL_ARRAY, "Array @", "", "" },
                    { SCE_PL_HASH, "Hash %", "", "" },
                    { SCE_PL_SYMBOLTABLE, "Symbol Table *", "", "" },
                    { SCE_PL_REGEX, "Regex /re/ or m{re}", "", "" },
                    { SCE_PL_REGSUBST, "Substitution s/re/or e/", "", "" },
                    //{ SCE_PL_LONGQUOTE, "Long Quote (qq, qr, qw, qx) (not used)", "", "" },
                    { SCE_PL_BACKTICKS, "Back Ticks", "", "" },
                    { SCE_PL_DATASECTION, "Data Section", "", "" },
                    { SCE_PL_HERE_DELIM, "Here-doc (Delimiter)", "", "" },
                    { SCE_PL_HERE_Q, "Here-doc (Single quoted, q)", "", "" },
                    { SCE_PL_HERE_QQ, "Here-doc (Double quoted, qq)", "", "" },
                    { SCE_PL_HERE_QX, "Here-doc (Back ticks, qx)", "", "" },
                    { SCE_PL_STRING_Q, "Single quoted string, generic (q)", "", "" },
                    { SCE_PL_STRING_QQ, "Double quoted string (qq)", "", "" },
                    { SCE_PL_STRING_QX, "Back ticks (qx)", "", "" },
                    { SCE_PL_STRING_QR, "Regex (qr)", "", "" },
                    { SCE_PL_STRING_QW, "Array (qw)", "", "" },
                    { -1, "", "", "" } } };


KEYWORDLIST KeyWords_INI = {
"", "", "", "", "", "", "", "", "" };


EDITLEXER lexINI = { SCLEX_PROPERTIES, "Configuration Files", "ini;inf;reg;cfg;properties;url;sif", "", &KeyWords_INI, {
                     { STYLE_DEFAULT, "Default", "", "" },
                     //{ SCE_PROPS_DEFAULT, "Default", "", "" },
                     { SCE_PROPS_COMMENT, "Comment", "fore:#008000", "" },
                     { SCE_PROPS_SECTION, "Section", "back:#FF8040;bold;eolfilled", "" },
                     { SCE_PROPS_ASSIGNMENT, "Assignment", "fore:#FF0000", "" },
                     { SCE_PROPS_DEFVAL, "Default Value", "fore:#FF0000", "" },
                     { -1, "", "", "" } } };


KEYWORDLIST KeyWords_BAT = {
"rem set if exist errorlevel for in do break call chcp cd chdir choice cls "
"country ctty date del erase dir echo exit goto loadfix loadhigh mkdir md "
"move path pause prompt rename ren rmdir rd shift time type ver verify vol "
"com con lpt nul echo.",
"", "", "", "", "", "", "", "" };


EDITLEXER lexBAT = { SCLEX_BATCH, "Batch Files", "bat;cmd", "", &KeyWords_BAT, {
                     { STYLE_DEFAULT, "Default", "", "" },
                     //{ SCE_BAT_DEFAULT, "Default", "", "" },
                     { SCE_BAT_COMMENT, "Comment", "fore:#008000", "" },
                     { SCE_BAT_WORD, "Keyword", "bold;fore:#0A246A", "" },
                     { SCE_BAT_LABEL, "Label", "fore:#800000;back:#FFFF80;eolfilled", "" },
                     { SCE_BAT_HIDE, "Hide", "", "" },
                     { SCE_BAT_COMMAND, "Command", "bold", "" },
                     { SCE_BAT_IDENTIFIER, "Identifier", "", "" },
                     { SCE_BAT_OPERATOR, "Operator", "", "" },
                     { -1, "", "", "" } } };


KEYWORDLIST KeyWords_DIFF = {
"", "", "", "", "", "", "", "", "" };


EDITLEXER lexDIFF = { SCLEX_DIFF, "Diff Files", "diff;patch", "", &KeyWords_DIFF, {
                      { STYLE_DEFAULT, "Default", "", "" },
                      //{ SCE_DIFF_DEFAULT, "Default", "", "" },
                      { SCE_DIFF_COMMENT, "Comment", "fore:#008000", "" },
                      { SCE_DIFF_COMMAND, "Command", "bold;fore:#0A246A", "" },
                      { SCE_DIFF_HEADER, "Source and Destination Files", "fore:#800000;back:#FFFF80;eolfilled", "" },
                      { SCE_DIFF_POSITION, "Position Setting", "fore:#FF0000", "" },
                      { SCE_DIFF_ADDED, "Line Addition", "back:#80FF80;eolfilled", "" },
                      { SCE_DIFF_DELETED, "Line Removal", "back:#FF8080;eolfilled", "" },
                      { -1, "", "", "" } } };


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


EDITLEXER lexSQL = { SCLEX_SQL, "SQL Query", "sql", "", &KeyWords_SQL, {
                     //{ 0, "Default", "", "" },
                     { STYLE_DEFAULT, "Default", "", "" },
                     { 1, "Comment", "fore:#008000", "" },
                     { 2, "Line Comment", "fore:#008000", "" },
                     { 3, "Doc Comment", "fore:#008000", "" },
                     { 4, "Number", "fore:#FF0000", "" },
                     { 5, "Keyword", "fore:#800080;bold", "" },
                     { 6, "String double quoted", "fore:#008000", "" },
                     { 7, "String single quoted", "fore:#008000", "" },
                     //{ 12, "String not closed", "fore:#008000;back:#FFFF80;eolfilled", "" },
                     { 10, "Operator", "", "" },
                     { 11, "Identifier", "", "" },
                     //{ 8, "Symbols", "", "" },
                     //{ 9, "Preprocessor", "fore:#FF8000", "" },
                     { -1, "", "", "" } } };


KEYWORDLIST KeyWords_PY = {
"and as assert break class continue def del elif else except "
"exec False finally for from global if import in is lambda None "
"not or pass print raise return True try with while yield",
"", "", "", "", "", "", "", "" };


EDITLEXER lexPY = { SCLEX_PYTHON, "Python Script", "py;pyw", "", &KeyWords_PY, {
                    { STYLE_DEFAULT, "Default", "", "" },
                    //{ SCE_P_DEFAULT, "Default", "", "" },
                    { SCE_P_COMMENTLINE, "Comment", "fore:#007F00", "" },
                    { SCE_P_COMMENTBLOCK, "Comment Block", "fore:#007F00", "" },
                    { SCE_P_WORD, "Keyword", "fore:#00007F;bold", "" },
                    { SCE_P_IDENTIFIER, "Identifier", "", "" },
                    { SCE_P_NUMBER, "Number", "fore:#007F7F", "" },
                    { SCE_P_OPERATOR, "Operator", "bold", "" },
                    { SCE_P_STRING, "String double quoted", "fore:#FF8000", "" },
                    { SCE_P_CHARACTER, "String single quoted", "fore:#FF8000", "" },
                    { SCE_P_STRINGEOL, "String not closed", "fore:#FF8000", "" },
                    { SCE_P_TRIPLEDOUBLE, "String triple double quotes", "fore:#FF8000", "" },
                    { SCE_P_TRIPLE, "String triple single quotes", "fore:#FF8000", "" },
                    { SCE_P_CLASSNAME, "Class name", "fore:#0000FF;bold", "" },
                    { SCE_P_DEFNAME, "Function name", "fore:#007F7F;bold", "" },
                    { -1, "", "", "" } } };


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


EDITLEXER lexCONF = { SCLEX_CONF, "Apache Config Files", "conf;htaccess", "", &KeyWords_CONF, {
                      { STYLE_DEFAULT, "Default", "", "" },
                      //{ SCE_DIFF_DEFAULT, "Default", "", "" },
                      { SCE_CONF_COMMENT, "Comment", "fore:#007F00", "" },
                      { SCE_CONF_NUMBER, "Number", "fore:#007F7F", "" },
                      { SCE_CONF_OPERATOR, "Operator", "bold", "" },
                      { SCE_CONF_STRING, "String", "fore:#7F007F", "" },
                      { SCE_CONF_IDENTIFIER, "Identifier", "bold;fore:#00007F", "" },
                      { SCE_CONF_DIRECTIVE, "Runtime Directive", "bold;fore:#0A246A", "" },
                      { SCE_CONF_PARAMETER, "Runtime Directive Parameter", "fore:#000000;back:#FFE0FF", "" },
                      { SCE_CONF_IP, "IP Address", "fore:#007F7F;bold", "" },
                      { SCE_CONF_EXTENSION, "Extension", "fore:#000000;back:#FFE0FF", "" },
                      { -1, "", "", "" } } };


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


// Custom colors
COLORREF crCustom[16];


// use 2nd default style
BOOL bUse2ndDefaultStyle;


// base font size used for relative values
int iBaseFontSize = 10;


// default lexer
int iDefaultLexer;


// automatically select lexer
BOOL bAutoSelect;


// scheme select dlg dimensions
int cxStyleSelectDlg;
int cyStyleSelectDlg;


// default codepage and character set
extern int  iDefaultCodePage;
extern int  iDefaultCharSet;


// highlight current line
extern BOOL bHiliteCurrentLine;


//=============================================================================
//
//  Style_Load()
//
void Style_Load()
{
  int i,iLexer;
  char tch[32];
  char *pIniSection = LocalAlloc(LPTR,32*1024);
  int   cIniSection = LocalSize(pIniSection);

  // Custom colors
  crCustom[0] = RGB(0,0,128);                     crCustom[8]  = RGB(255,255,226);
  crCustom[1] = GetSysColor(COLOR_WINDOWTEXT);    crCustom[9]  = GetSysColor(COLOR_WINDOW);
  crCustom[2] = GetSysColor(COLOR_INFOTEXT);      crCustom[10] = GetSysColor(COLOR_INFOBK);
  crCustom[3] = GetSysColor(COLOR_HIGHLIGHTTEXT); crCustom[11] = GetSysColor(COLOR_HIGHLIGHT);
  crCustom[4] = GetSysColor(COLOR_ACTIVECAPTION); crCustom[12] = GetSysColor(COLOR_DESKTOP);
  crCustom[5] = GetSysColor(COLOR_3DFACE);        crCustom[13] = GetSysColor(COLOR_3DHILIGHT);
  crCustom[6] = GetSysColor(COLOR_3DLIGHT);       crCustom[14] = GetSysColor(COLOR_3DSHADOW);
  crCustom[7] = GetSysColor(COLOR_APPWORKSPACE);  crCustom[15] = GetSysColor(COLOR_SCROLLBAR);

  LoadIniSection("Custom Colors",pIniSection,cIniSection);
  for (i = 0; i < 16; i++) {
    wsprintf(tch,"%02i",i+1);
    crCustom[i] = IniSectionGetInt(pIniSection,tch,crCustom[i]);
  }

  LoadIniSection("Styles",pIniSection,cIniSection);
  // 2nd default
  bUse2ndDefaultStyle = (IniSectionGetInt(pIniSection,"Use2ndDefaultStyle",0)) ? 1 : 0;

  // default scheme
  iDefaultLexer = IniSectionGetInt(pIniSection,"DefaultScheme",0);
  iDefaultLexer = min(max(iDefaultLexer,0),NUMLEXERS-1);

  // auto select
  bAutoSelect = (IniSectionGetInt(pIniSection,"AutoSelect",1)) ? 1 : 0;

  // scheme select dlg dimensions
  cxStyleSelectDlg = IniSectionGetInt(pIniSection,"SelectDlgSizeX",304);
  cxStyleSelectDlg = max(cxStyleSelectDlg,0);

  cyStyleSelectDlg = IniSectionGetInt(pIniSection,"SelectDlgSizeY",0);
  cyStyleSelectDlg = max(cyStyleSelectDlg,324);

  for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
    LoadIniSection(pLexArray[iLexer]->pszName,pIniSection,cIniSection);
    if (!IniSectionGetString(pIniSection,"FileNameExtensions",pLexArray[iLexer]->pszDefExt,
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
  char tch[32];
  char *pIniSection = LocalAlloc(LPTR,32*1024);
  int   cIniSection = LocalSize(pIniSection);

  // Custom colors
  for (i = 0; i < 16; i++) {
    wsprintf(tch,"%02i",i+1);
    IniSectionSetInt(pIniSection,tch,crCustom[i]);
  }
  SaveIniSection("Custom Colors",pIniSection);
  ZeroMemory(pIniSection,cIniSection);

  // auto select
  IniSectionSetInt(pIniSection,"Use2ndDefaultStyle",bUse2ndDefaultStyle);

  // default scheme
  IniSectionSetInt(pIniSection,"DefaultScheme",iDefaultLexer);

  // auto select
  IniSectionSetInt(pIniSection,"AutoSelect",bAutoSelect);

  // scheme select dlg dimensions
  IniSectionSetInt(pIniSection,"SelectDlgSizeX",cxStyleSelectDlg);
  IniSectionSetInt(pIniSection,"SelectDlgSizeY",cyStyleSelectDlg);

  SaveIniSection("Styles",pIniSection);
  ZeroMemory(pIniSection,cIniSection);

  for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
    IniSectionSetString(pIniSection,"FileNameExtensions",pLexArray[iLexer]->szExtensions);
    i = 0;
    while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
      IniSectionSetString(pIniSection,pLexArray[iLexer]->Styles[i].pszName,pLexArray[iLexer]->Styles[i].szValue);
      i++;
    }
    SaveIniSection(pLexArray[iLexer]->pszName,pIniSection);
    ZeroMemory(pIniSection,cIniSection);
  }
  LocalFree(pIniSection);
}


//=============================================================================
//
//  Style_Import()
//
BOOL Style_Import(HWND hwnd)
{
  char szFile[MAX_PATH * 2] = "";
  char szFilter[256];
  OPENFILENAME ofn;

  ZeroMemory(&ofn,sizeof(OPENFILENAME));
  GetString(IDS_FILTER_INI,szFilter,COUNTOF(szFilter));
  PrepareFilterStr(szFilter);

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = szFilter;
  ofn.lpstrFile = szFile;
  ofn.lpstrDefExt = "ini";
  ofn.nMaxFile = COUNTOF(szFile);
  ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | /*OFN_DONTADDTORECENT*/0x02000000
            | OFN_PATHMUSTEXIST | OFN_SHAREAWARE /*| OFN_NODEREFERENCELINKS*/;

  if (GetOpenFileName(&ofn)) {

    int i,iLexer;
    char *pIniSection = LocalAlloc(LPTR,32*1024);
    int   cIniSection = LocalSize(pIniSection);

    for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
      if (GetPrivateProfileSection(pLexArray[iLexer]->pszName,pIniSection,cIniSection,szFile)) {
        if (!IniSectionGetString(pIniSection,"FileNameExtensions",pLexArray[iLexer]->pszDefExt,
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
  char szFile[MAX_PATH * 2] = "";
  char szFilter[256];
  OPENFILENAME ofn;

  ZeroMemory(&ofn,sizeof(OPENFILENAME));
  GetString(IDS_FILTER_INI,szFilter,COUNTOF(szFilter));
  PrepareFilterStr(szFilter);

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = szFilter;
  ofn.lpstrFile = szFile;
  ofn.lpstrDefExt = "ini";
  ofn.nMaxFile = COUNTOF(szFile);
  ofn.Flags = /*OFN_FILEMUSTEXIST |*/ OFN_HIDEREADONLY | OFN_NOCHANGEDIR | /*OFN_DONTADDTORECENT*/0x02000000
            | OFN_PATHMUSTEXIST | OFN_SHAREAWARE /*| OFN_NODEREFERENCELINKS*/ | OFN_OVERWRITEPROMPT;

  if (GetSaveFileName(&ofn)) {

    int i,iLexer;
    char *pIniSection = LocalAlloc(LPTR,32*1024);
    int   cIniSection = LocalSize(pIniSection);

    for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
      IniSectionSetString(pIniSection,"FileNameExtensions",pLexArray[iLexer]->szExtensions);
      i = 0;
      while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
        IniSectionSetString(pIniSection,pLexArray[iLexer]->Styles[i].pszName,pLexArray[iLexer]->Styles[i].szValue);
        i++;
      }
      WritePrivateProfileSection(pLexArray[iLexer]->pszName,pIniSection,szFile);
      ZeroMemory(pIniSection,cIniSection);
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
  //char *p;
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
  Style_SetStyles(hwnd,lexDefault.Styles[4+iIdx].iStyle,lexDefault.Styles[4+iIdx].szValue); // control char
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

  if (StrStrI(lexDefault.Styles[6+iIdx].szValue,"eolfilled")) // selection eolfilled
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
  if (StrStr(lexDefault.Styles[10+iIdx].szValue,"block"))
    SendMessage(hwnd,SCI_SETCARETSTYLE,CARETSTYLE_BLOCK,0);
  else {
    iValue = 1;
    Style_StrGetSize(lexDefault.Styles[10+iIdx].szValue,&iValue);
    iValue = max(min(iValue,3),1);
    wsprintf(lexDefault.Styles[10+iIdx].szValue,"size:%i",iValue);
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
void Style_SetLexerFromFile(HWND hwnd,LPCSTR lpszFile)
{
  int i;
  LPSTR lpszExt;
  char  tch[256+16];
  char  *p1,*p2;
  BOOL  bFound = FALSE;
  PEDITLEXER pLexNew = pLexArray[iDefaultLexer];

  lpszExt = PathFindExtension(lpszFile);

  if (bAutoSelect &&  /* bAutoSelect == FALSE skips lexer search */
      lpszFile && lstrlen(lpszFile) > 0 &&
      *lpszExt)
  {
    // Skip the . (dot)
    lpszExt++;

    // Check Lexers
    for (i = 0; i < NUMLEXERS; i++)
    {
      if (bFound)
        break;

      ZeroMemory(tch,COUNTOF(tch));
      lstrcpy(tch,pLexArray[i]->szExtensions);
      p1 = tch;
      while (*p1 && !bFound)
      {
        if (p2 = StrChr(p1,';'))
          *p2 = '\0';
        else
          p2 = StrEnd(p1);
        if (lstrcmpi(p1,lpszExt) == 0)
        {
          pLexNew = pLexArray[i];
          bFound = TRUE;
        }
        p1 = p2+1;
      }
    }
  }

  else if (bAutoSelect && lstrcmpi(PathFindFileName(lpszFile),"makefile") == 0)
    pLexNew = &lexMAK;

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
        COUNTOF(lexDefault.Styles[0+iIdx].szValue)))
    Style_SetLexer(hwnd,pLexCurrent);
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
extern char tchFileDlgFilters[5*1024];

BOOL Style_GetOpenDlgFilterStr(LPSTR lpszFilter,int cchFilter)
{
  if (lstrlen(tchFileDlgFilters) == 0)
    GetString(IDS_FILTER_ALL,lpszFilter,cchFilter);
  else
    lstrcpyn(lpszFilter,tchFileDlgFilters,cchFilter);
  PrepareFilterStr(lpszFilter);
  return TRUE;
}


//=============================================================================
//
//  Style_StrGetFont()
//
BOOL Style_StrGetFont(LPCSTR lpszStyle,LPSTR lpszFont,int cchFont)
{
  char tch[256];
  char *p;

  if (p = StrStrI(lpszStyle,"font:"))
  {
    lstrcpy(tch,p + lstrlen("font:"));
    if (p = StrChr(tch,';'))
      *p = '\0';
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
BOOL Style_StrGetCharSet(LPCSTR lpszStyle,int *i)
{
  char tch[256];
  char *p;
  int  iValue;
  int  itok;

  if (p = StrStrI(lpszStyle,"charset:"))
  {
    lstrcpy(tch,p + lstrlen("charset:"));
    if (p = StrChr(tch,';'))
      *p = '\0';
    TrimString(tch);
    itok = sscanf(tch,"%i",&iValue);
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
BOOL Style_StrGetSize(LPCSTR lpszStyle,int *i)
{
  char tch[256];
  char *p;
  int  iValue;
  int  iSign = 0;
  int  itok;

  if (p = StrStrI(lpszStyle,"size:"))
  {
    lstrcpy(tch,p + lstrlen("size:"));
    if (tch[0] == '+')
    {
      iSign = 1;
      tch[0] = ' ';
    }
    else if (tch[0] == '-')
    {
      iSign = -1;
      tch[0] = ' ';
    }
    if (p = StrChr(tch,';'))
      *p = '\0';
    TrimString(tch);
    itok = sscanf(tch,"%i",&iValue);
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
BOOL Style_StrGetSizeStr(LPCSTR lpszStyle,LPSTR lpszSize,int cchSize)
{
  char tch[256];
  char *p;

  if (p = StrStrI(lpszStyle,"size:"))
  {
    lstrcpy(tch,p + lstrlen("size:"));
    if (p = StrChr(tch,';'))
      *p = '\0';
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
BOOL Style_StrGetColor(BOOL bFore,LPCSTR lpszStyle,int *rgb)
{
  char tch[256];
  char *p;
  int  iValue;
  int  itok;
  char *pItem = (bFore) ? "fore:" : "back:";

  if (p = StrStrI(lpszStyle,pItem))
  {
    lstrcpy(tch,p + lstrlen(pItem));
    if (tch[0] == '#')
      tch[0] = ' ';
    if (p = StrChr(tch,';'))
      *p = '\0';
    TrimString(tch);
    itok = sscanf(tch,"%x",&iValue);
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
BOOL Style_StrGetCase(LPCSTR lpszStyle,int *i)
{
  char tch[256];
  char *p;

  if (p = StrStrI(lpszStyle,"case:"))
  {
    lstrcpy(tch,p + lstrlen("case:"));
    if (p = StrChr(tch,';'))
      *p = '\0';
    TrimString(tch);
    if (tch[0] == 'u' || tch[0] == 'U') {
      *i = SC_CASE_UPPER;
      return TRUE; }
    else if (tch[0] == 'l' || tch[0] == 'L') {
      *i = SC_CASE_LOWER;
      return TRUE; }
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetAlpha()
//
BOOL Style_StrGetAlpha(LPCSTR lpszStyle,int *i)
{
  char tch[256];
  char *p;
  int  iValue;
  int  itok;

  if (p = StrStrI(lpszStyle,"alpha:"))
  {
    lstrcpy(tch,p + lstrlen("alpha:"));
    if (p = StrChr(tch,';'))
      *p = '\0';
    TrimString(tch);
    itok = sscanf(tch,"%i",&iValue);
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
/*BOOL Style_StrGetAttribute(LPCSTR lpszStyle,LPCSTR lpszAttribute)
{
  return (StrStrI(lpszStyle,lpszAttribute) == NULL) ? FALSE : TRUE;
}*/


//=============================================================================
//
//  Style_StrSetAttribute()
//
/*BOOL Style_StrSetAttribute(LPSTR lpszStyle,int cchStyle,LPCSTR lpszAttribute,BOOL bSet)
{
  char szNewStyle[512];
  int  iValue;
  char tch[32];

  // Rebuild style string
  lstrcpy(szNewStyle,"");
  if (Style_StrGetFont(lpszStyle,tch,COUNTOF(tch)))
  {
    lstrcat(szNewStyle,"font:");
    lstrcat(szNewStyle,tch);
  }

  if (Style_StrGetCharSet(lpszStyle,&iValue))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    wsprintf(tch,"charset:%i",iValue);
    lstrcat(szNewStyle,tch);
  }

  if (Style_StrGetSizeStr(lpszStyle,tch,COUNTOF(tch)))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    lstrcat(szNewStyle,"size:");
    lstrcat(szNewStyle,tch);
  }

  if (lstrcmpi(lpszAttribute,"bold") == 0)
  {
    if (bSet)
    {
      if (lstrlen(szNewStyle))
        lstrcat(szNewStyle,";");
      lstrcat(szNewStyle,"bold");
    }
  }
  else if (StrStrI(lpszStyle,"bold"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    lstrcat(szNewStyle,"bold");
  }

  if (lstrcmpi(lpszAttribute,"italic") == 0)
  {
    if (bSet)
    {
      if (lstrlen(szNewStyle))
        lstrcat(szNewStyle,";");
      lstrcat(szNewStyle,"italic");
    }
  }
  else if (StrStrI(lpszStyle,"italic"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    lstrcat(szNewStyle,"italic");
  }

  if (lstrcmpi(lpszAttribute,"underline") == 0)
  {
    if (bSet)
    {
      if (lstrlen(szNewStyle))
        lstrcat(szNewStyle,";");
      lstrcat(szNewStyle,"underline");
    }
  }
  else if (StrStrI(lpszStyle,"underline"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    lstrcat(szNewStyle,"underline");
  }

  // save colors
  if (Style_StrGetColor(TRUE,lpszStyle,&iValue))
  {
    wsprintf(tch,"fore:#%02X%02X%02X",
      (int)iValue&0xFF,
      (int)(iValue&0xFF00) >> 8,
      (int)(iValue&0xFF0000) >> 16);
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    lstrcat(szNewStyle,tch);
  }
  if (Style_StrGetColor(FALSE,lpszStyle,&iValue))
  {
    wsprintf(tch,"back:#%02X%02X%02X",
      (int)iValue&0xFF,
      (int)(iValue&0xFF00) >> 8,
      (int)(iValue&0xFF0000) >> 16);
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    lstrcat(szNewStyle,tch);
  }

  if (lstrcmpi(lpszAttribute,"eolfilled") == 0)
  {
    if (bSet)
    {
      if (lstrlen(szNewStyle))
        lstrcat(szNewStyle,";");
      lstrcat(szNewStyle,"eolfilled");
    }
  }
  else if (StrStrI(lpszStyle,"eolfilled"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    lstrcat(szNewStyle,"eolfilled");
  }

  if (Style_StrGetCase(lpszStyle,&iValue)) {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    lstrcat(szNewStyle,"case:");
    lstrcat(szNewStyle,(iValue == SC_CASE_UPPER) ? "u" : "l"); }

  lstrcpyn(lpszStyle,szNewStyle,cchStyle);
  return TRUE;
}*/


//=============================================================================
//
//  Style_SelectFont()
//
BOOL Style_SelectFont(HWND hwnd,LPSTR lpszStyle,int cchStyle)
{
  CHOOSEFONT cf;
  LOGFONT lf;
  char szNewStyle[512];
  int  iValue;
  char tch[32];
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
  lf.lfWeight = (StrStrI(lpszStyle,"bold")) ? FW_BOLD : FW_NORMAL;
  lf.lfItalic = (StrStrI(lpszStyle,"italic")) ? 1 : 0;

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
  lstrcpy(szNewStyle,"font:");
  lstrcat(szNewStyle,lf.lfFaceName);
  if (lf.lfCharSet != DEFAULT_CHARSET && lf.lfCharSet != iDefaultCharSet) {
    lstrcat(szNewStyle,";charset:");
    wsprintf(tch,"%i",lf.lfCharSet);
    lstrcat(szNewStyle,tch);
  }
  lstrcat(szNewStyle,";size:");
  wsprintf(tch,"%i",cf.iPointSize/10);
  lstrcat(szNewStyle,tch);
  if (cf.nFontType & BOLD_FONTTYPE)
    lstrcat(szNewStyle,";bold");
  if (cf.nFontType & ITALIC_FONTTYPE)
    lstrcat(szNewStyle,";italic");

  if (StrStrI(lpszStyle,"underline"))
    lstrcat(szNewStyle,";underline");

  // save colors
  if (Style_StrGetColor(TRUE,lpszStyle,&iValue))
  {
    wsprintf(tch,";fore:#%02X%02X%02X",
      (int)iValue&0xFF,
      (int)(iValue&0xFF00) >> 8,
      (int)(iValue&0xFF0000) >> 16);
    lstrcat(szNewStyle,tch);
  }
  if (Style_StrGetColor(FALSE,lpszStyle,&iValue))
  {
    wsprintf(tch,";back:#%02X%02X%02X",
      (int)iValue&0xFF,
      (int)(iValue&0xFF00) >> 8,
      (int)(iValue&0xFF0000) >> 16);
    lstrcat(szNewStyle,tch);
  }

  if (StrStrI(lpszStyle,"eolfilled"))
    lstrcat(szNewStyle,";eolfilled");

  if (Style_StrGetCase(lpszStyle,&iValue)) {
    lstrcat(szNewStyle,";case:");
    lstrcat(szNewStyle,(iValue == SC_CASE_UPPER) ? "u" : "l"); }

  if (Style_StrGetAlpha(lpszStyle,&iValue)) {
    lstrcat(szNewStyle,";alpha:");
    wsprintf(tch,"%i",iValue);
    lstrcat(szNewStyle,tch); }

  lstrcpyn(lpszStyle,szNewStyle,cchStyle);
  return TRUE;
}


//=============================================================================
//
//  Style_SelectColor()
//
BOOL Style_SelectColor(HWND hwnd,BOOL bFore,LPSTR lpszStyle,int cchStyle)
{
  CHOOSECOLOR cc;
  char szNewStyle[512];
  int  iRGBResult;
  int  iValue;
  char tch[32];

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
  lstrcpy(szNewStyle,"");
  if (Style_StrGetFont(lpszStyle,tch,COUNTOF(tch)))
  {
    lstrcat(szNewStyle,"font:");
    lstrcat(szNewStyle,tch);
  }
  if (Style_StrGetCharSet(lpszStyle,&iValue))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    wsprintf(tch,"charset:%i",iValue);
    lstrcat(szNewStyle,tch);
  }
  if (Style_StrGetSizeStr(lpszStyle,tch,COUNTOF(tch)))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    lstrcat(szNewStyle,"size:");
    lstrcat(szNewStyle,tch);
  }

  if (StrStrI(lpszStyle,"bold"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    lstrcat(szNewStyle,"bold");
  }
  if (StrStrI(lpszStyle,"italic"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    lstrcat(szNewStyle,"italic");
  }
  if (StrStrI(lpszStyle,"underline"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    lstrcat(szNewStyle,"underline");
  }

  if (bFore)
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    wsprintf(tch,"fore:#%02X%02X%02X",
      (int)iRGBResult&0xFF,
      (int)(iRGBResult&0xFF00) >> 8,
      (int)(iRGBResult&0xFF0000) >> 16);
    lstrcat(szNewStyle,tch);
    if (Style_StrGetColor(FALSE,lpszStyle,&iValue))
    {
      wsprintf(tch,";back:#%02X%02X%02X",
        (int)iValue&0xFF,
        (int)(iValue&0xFF00) >> 8,
        (int)(iValue&0xFF0000) >> 16);
      lstrcat(szNewStyle,tch);
    }
  }
  else
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,";");
    if (Style_StrGetColor(TRUE,lpszStyle,&iValue))
    {
      wsprintf(tch,"fore:#%02X%02X%02X;",
        (int)iValue&0xFF,
        (int)(iValue&0xFF00) >> 8,
        (int)(iValue&0xFF0000) >> 16);
      lstrcat(szNewStyle,tch);
    }
    wsprintf(tch,"back:#%02X%02X%02X",
      (int)iRGBResult&0xFF,
      (int)(iRGBResult&0xFF00) >> 8,
      (int)(iRGBResult&0xFF0000) >> 16);
    lstrcat(szNewStyle,tch);
  }

  if (StrStrI(lpszStyle,"eolfilled"))
    lstrcat(szNewStyle,";eolfilled");

  if (Style_StrGetCase(lpszStyle,&iValue)) {
    lstrcat(szNewStyle,";case:");
    lstrcat(szNewStyle,(iValue == SC_CASE_UPPER) ? "u" : "l"); }

  if (Style_StrGetAlpha(lpszStyle,&iValue)) {
    lstrcat(szNewStyle,";alpha:");
    wsprintf(tch,"%i",iValue);
    lstrcat(szNewStyle,tch); }

  lstrcpyn(lpszStyle,szNewStyle,cchStyle);
  return TRUE;
}


//=============================================================================
//
//  Style_SetStyles()
//
void Style_SetStyles(HWND hwnd,int iStyle,LPCSTR lpszStyle)
{

  char tch[256];
  char *p;
  int  iValue;

  // Font
  if (Style_StrGetFont(lpszStyle,tch,COUNTOF(tch)))
    SendMessage(hwnd,SCI_STYLESETFONT,iStyle,(LPARAM)tch);

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
  if (p = StrStrI(lpszStyle,"bold"))
    SendMessage(hwnd,SCI_STYLESETBOLD,iStyle,(LPARAM)TRUE);
  else
    SendMessage(hwnd,SCI_STYLESETBOLD,iStyle,(LPARAM)FALSE);

  // Italic
  if (p = StrStrI(lpszStyle,"italic"))
    SendMessage(hwnd,SCI_STYLESETITALIC,iStyle,(LPARAM)TRUE);
  else
    SendMessage(hwnd,SCI_STYLESETITALIC,iStyle,(LPARAM)FALSE);

  // Underline
  if (p = StrStrI(lpszStyle,"underline"))
    SendMessage(hwnd,SCI_STYLESETUNDERLINE,iStyle,(LPARAM)TRUE);
  else
    SendMessage(hwnd,SCI_STYLESETUNDERLINE,iStyle,(LPARAM)FALSE);

  // EOL Filled
  if (p = StrStrI(lpszStyle,"eolfilled"))
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
LPCSTR Style_GetCurrentLexerName()
{
  return pLexCurrent->pszName;
}


//=============================================================================
//
//  Style_GetLexerIconId()
//
int Style_GetLexerIconId(PEDITLEXER plex)
{
  char *p;
  char *pszExtensions;
  char *pszFile;

  SHFILEINFO shfi;

  if (lstrlen(plex->szExtensions))
    pszExtensions = plex->szExtensions;
  else
    pszExtensions = plex->pszDefExt;

  pszFile = GlobalAlloc(GPTR,lstrlen(pszExtensions) + lstrlen("*.txt") + 16);
  lstrcpy(pszFile,"*.");
  lstrcat(pszFile,pszExtensions);
  if (p = StrChr(pszFile,';'))
    *p = '\0';

  // check for ; at beginning
  if (lstrlen(pszFile) < 3)
    lstrcat(pszFile,"txt");

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
          (HIMAGELIST)SHGetFileInfo("C:\\",0,&shfi,sizeof(SHFILEINFO),
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


    case WM_CTLCOLORSTATIC:
      {
        DWORD dwId = GetWindowLong((HWND)lParam,GWL_ID);
        HDC hdc = (HDC)wParam;

        if (dwId != IDC_TITLE)
          break;

        SetBkMode(hdc,TRANSPARENT);
        SetTextColor(hdc,GetSysColor(COLOR_BTNTEXT));
        SelectObject(hdc,hFontTitle);
        return(LONG)GetSysColorBrush(COLOR_BTNFACE);
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
                pCurrentStyle = 0;
                if (pCurrentLexer = (PEDITLEXER)lpnmtv->itemNew.lParam)
                {
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEEDIT),TRUE);
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
                  SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions);
                }
                else
                {
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
                  SetDlgItemText(hwnd,IDC_STYLEEDIT,"");
                }
              }

              // a style has been selected
              else
              {
                pCurrentLexer = 0;
                if (pCurrentStyle = (PEDITSTYLE)lpnmtv->itemNew.lParam)
                {
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEEDIT),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFONT),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFORE),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEBACK),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEDEFAULT),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEBOLD),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEITALIC),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEUNDERLINE),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEEOLFILLED),TRUE);
                  //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(pCurrentStyle->szValue,"bold") ? BST_CHECKED : BST_UNCHECKED));
                  //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(pCurrentStyle->szValue,"italic") ? BST_CHECKED : BST_UNCHECKED));
                  //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(pCurrentStyle->szValue,"underline") ? BST_CHECKED : BST_UNCHECKED));
                  //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(pCurrentStyle->szValue,"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
                  SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue);
                }
                else
                {
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEEDIT),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFONT),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFORE),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEBACK),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEBOLD),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEITALIC),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEUNDERLINE),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEEOLFILLED),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEDEFAULT),FALSE);
                  //CheckDlgButton(hwnd,IDC_STYLEBOLD,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEITALIC,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,BST_UNCHECKED);
                  SetDlgItemText(hwnd,IDC_STYLEEDIT,"");
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
            char tchCopy[256];
            TreeView_SelectDropTarget(hwndTV,NULL);
            GetDlgItemText(hwnd,IDC_STYLEEDIT,tchCopy,COUNTOF(tchCopy));
            TreeView_Select(hwndTV,htiTarget,TVGN_CARET);

            // after select, this is new current item
            if (pCurrentStyle)
            {
              lstrcpy(pCurrentStyle->szValue,tchCopy);
              SetDlgItemText(hwnd,IDC_STYLEEDIT,tchCopy);
              //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(tchCopy,"bold") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(tchCopy,"italic") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(tchCopy,"underline") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(tchCopy,"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
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
            char tch[256];
            GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
            if (Style_SelectFont(hwnd,tch,COUNTOF(tch))) {
              SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
              //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(tch,"bold") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(tch,"italic") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(tch,"underline") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(tch,"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
            }
          }
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        case IDC_STYLEFORE:
          if (pCurrentStyle)
          {
            char tch[256];
            GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
            if (Style_SelectColor(hwnd,TRUE,tch,COUNTOF(tch))) {
              SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
              //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(tch,"bold") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(tch,"italic") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(tch,"underline") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(tch,"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
            }
          }
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        case IDC_STYLEBACK:
          if (pCurrentStyle)
          {
            char tch[256];
            GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
            if (Style_SelectColor(hwnd,FALSE,tch,COUNTOF(tch))) {
              SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
              //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(tch,"bold") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(tch,"italic") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(tch,"underline") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(tch,"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
            }
          }
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        case IDC_STYLEDEFAULT:
          if (pCurrentStyle)
          {
            lstrcpy(pCurrentStyle->szValue,pCurrentStyle->pszDefault);
            SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue);
            //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(pCurrentStyle->szValue,"bold") ? BST_CHECKED : BST_UNCHECKED));
            //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(pCurrentStyle->szValue,"italic") ? BST_CHECKED : BST_UNCHECKED));
            //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(pCurrentStyle->szValue,"underline") ? BST_CHECKED : BST_UNCHECKED));
            //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(pCurrentStyle->szValue,"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
          }
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        //case IDC_STYLEBOLD:
        //  if (pCurrentStyle)
        //  {
        //    char tch[256];
        //    GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
        //    Style_StrSetAttribute(tch,COUNTOF(tch),"bold",IsDlgButtonChecked(hwnd,IDC_STYLEBOLD));
        //    SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
        //  }
        //  break;

        //case IDC_STYLEITALIC:
        //  if (pCurrentStyle)
        //  {
        //    char tch[256];
        //    GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
        //    Style_StrSetAttribute(tch,COUNTOF(tch),"italic",IsDlgButtonChecked(hwnd,IDC_STYLEITALIC));
        //    SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
        //  }
        //  break;

        //case IDC_STYLEUNDERLINE:
        //  if (pCurrentStyle)
        //  {
        //    char tch[256];
        //    GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
        //    Style_StrSetAttribute(tch,COUNTOF(tch),"underline",IsDlgButtonChecked(hwnd,IDC_STYLEUNDERLINE));
        //    SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
        //  }
        //  break;

        //case IDC_STYLEEOLFILLED:
        //  if (pCurrentStyle)
        //  {
        //    char tch[256];
        //    GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
        //    Style_StrSetAttribute(tch,COUNTOF(tch),"eolfilled",IsDlgButtonChecked(hwnd,IDC_STYLEEOLFILLED));
        //    SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
        //  }
        //  break;

        case IDC_IMPORT:
          {
            HWND hwndTV = GetDlgItem(hwnd,IDC_STYLELIST);

            if (pCurrentStyle)
              GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue,COUNTOF(pCurrentStyle->szValue));
            else if (pCurrentLexer) {
              if (!GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions,COUNTOF(pCurrentLexer->szExtensions)))
                lstrcpy(pCurrentLexer->szExtensions,pCurrentLexer->pszDefExt); }

            if (Style_Import(hwnd)) {
              TreeView_Select(hwndTV,TreeView_GetRoot(hwndTV),TVGN_CARET);

              if (pCurrentStyle)
                SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue);
              else if (pCurrentLexer)
                SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions);

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

  char StyleBackup[512][128];
  int c,i,iLexer;

  // Backup the Styles
  c = 0;
  for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
    lstrcpy(StyleBackup[c++],pLexArray[iLexer]->szExtensions);
    i = 0;
    while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
      lstrcpy(StyleBackup[c++],pLexArray[iLexer]->Styles[i].szValue);
      i++;
    }
  }

  if (IDCANCEL == DialogBoxParam(g_hInstance,
                    MAKEINTRESOURCE(IDD_STYLECONFIG),
                    GetParent(hwnd),Style_ConfigDlgProc,
                    (LPARAM)&StyleBackup))
  {
    // Restore the Styles
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
        LVCOLUMN lvc = { LVCF_FMT|LVCF_TEXT, LVCFMT_LEFT, 0, "", -1, 0, 0, 0 };

        RECT rc;
        char tch[MAX_PATH];
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
          (HIMAGELIST)SHGetFileInfo("C:\\",0,&shfi,sizeof(SHFILEINFO),SHGFI_SMALLICON | SHGFI_SYSICONINDEX),
          LVSIL_SMALL);

        ListView_SetImageList(hwndLV,
          (HIMAGELIST)SHGetFileInfo("C:\\",0,&shfi,sizeof(SHFILEINFO),SHGFI_LARGEICON | SHGFI_SYSICONINDEX),
          LVSIL_NORMAL);

        ListView_SetExtendedListViewStyle(hwndLV,/*LVS_EX_FULLROWSELECT|*//*LVS_EX_DOUBLEBUFFER*/0x00010000|LVS_EX_LABELTIP);
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
