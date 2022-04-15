package ifneeded ttk::theme::Gttk 0.7  [list apply { dir {
  package require Ttk

  source [file join $dir Gttk.tcl]
  load [file join $dir libGttk0.7[info sharedlibextension]] Gttkpkg
  
  package provide ttk::theme::Gttk 0.7

}} $dir]