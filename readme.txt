Notepad2 Unified Edition
================================================================================

Dan Savilonis <djs@n-cube.org>
http://github.com/djs/notepad2

Notepad2 originally (c) Florian Balmer 2004-2010 (www.flos-freeware.ch)
Code folding and other modifications (c) Kai Liu 2009 (code.kliu.org/misc/notepad2/)



Building
================================================================================

Requirements
--------------------------------------------------------------------------------

 * Microsoft Windows SDK
 * NSIS 2.46

Instructions
--------------------------------------------------------------------------------

To build Notepad2, you must first build the modified Scintilla library:

    $ cd scintilla
    $ nmake

Then, build Notepad2:

    $ cd ..
    $ nmake

To build the installer, run:

    $ makensis nsis/notepad2.nsi

Notepad2 should build as a 32-bit or 64-bit executable, depending on the SDK
build environment used.

