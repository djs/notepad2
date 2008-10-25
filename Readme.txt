
=======================================================================
=                                                                     =
=                                                                     =
=   Notepad2 - light-weight Scintilla-based text editor for Windows   =
=                                                                     =
=                                                                     =
=                                                   Notepad2 2.1.19   =
=                                      (c) Florian Balmer 2004-2008   =
=                                       http://www.flos-freeware.ch   =
=                                                                     =
=                                                                     =
=======================================================================


Notepad2 Source Code

  This package contains the full source code of Notepad2 2.1.19 for
  Windows. Project files for Visual C++ .NET 7.0 are included. I think
  it's possible to build Notepad2 with other development tools, but I'm
  sorry I have no further information on this topic.

  To be able to rebuild Notepad2, the source code of the Scintilla
  editing component (available from http://www.scintilla.org) has to be
  unzipped to the "Scintilla" subdirectory of the Notepad2 source code
  directory.

  Notepad2 2.1.19 has been created with Scintilla 1.76. The following
  modification to the Scintilla source code is necessary:

  Scintilla/src/KeyWords.cxx:

      #define LINK_LEXER(lexer) extern LexerModule lexer; ...

    must be replaced with:

      #define LINK_LEXER(lexer) void(0)


Copyright

  See License.txt for details about distribution and modification.

  If you have any comments or questions, please drop me a note:
  florian.balmer@gmail.com

  (c) Florian Balmer 2004-2008
  http://www.flos-freeware.ch

###
