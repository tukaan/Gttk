# Gttk

Gttk is a theme extension for Tcl/Tk, that uses Gtk 2 to draw widgets. Thus a Tk/Ttk application can look kinda native on Gnome or XFCE desktops.

**DO NOT USE** this yourself! This project is a `git submodule` in [Tukaan](https://github.com/tukaan/tukaan), that uses it internally.

Source code is licensed under the terms of the MIT license. Binaries are licensed under GNU LGPL v2.1.


## Build
A Tcl/Tk installation is required for build.
Building is only possible on Linux. (You shouldn't want to use Gtk+ themes on Windows)

```bash
cmake -DCMAKE_INSTALL_PREFIX=`tclsh <<< 'puts [lindex $auto_path end]'`
make
````


## Credits
The original license of the TileGtk project.

```
LICENSE ("MIT-style")

Copyright 2008-2012 Georgios Petasis
Copyright 2012 Cheer Xiao

The following terms apply to all files associated with this software
unless explicitly disclaimed in individual files.  

The author(s) hereby grant permission to use, copy, modify, distribute,
and license this software and its documentation for any purpose, provided
that existing copyright notices are retained in all copies and that this
notice is included in any distributions. No written agreement,
license, or royalty fee is required for any of the authorized uses.
Modifications to this software may be copyrighted by their authors
and need not follow the licensing terms described here, provided that
the new terms are clearly indicated on the first page of each file where
they apply.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS for a PARTICULAR PURPOSE.  IN NO EVENT
shall the AUTHORS of THIS SOFTWARE be LIABLE to ANY PARTY for
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, or CONSEQUENTIAL DAMAGES
arising out of the USE of THIS SOFTWARE and its DOCUMENTATION.
```