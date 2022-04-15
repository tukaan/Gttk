namespace eval ttk::theme::Gttk {
  variable PreviewInterp {}
  variable System
  variable StyleToRc

  proc loadLibraries {args} {
    if {![initialiseLibrary required]} {return}
    package require platform
    set libs {gdk gdk_pixbuf glib gobject gtk}
    switch -glob [::platform::generic] {
      *_64 {
        set paths {
          /usr/lib64 /usr/lib /opt/gnome/lib /usr/openwin/lib /usr/lib/x86_64-linux-gnu
        }
      }
      default {
        set paths {
          /usr/lib /opt/gnome/lib /usr/openwin/lib
        }
      }
    }
    set ext [info sharedlibextension]
    set sep :
    set prefixes {lib}
    set names {-x11-2.0 -2.0 20 -x11 {}}
    lappend libs gdk_pixbuf_xlib

    if {[info exists ::env(LD_LIBRARY_PATH)]} {
      foreach path [split $::env(LD_LIBRARY_PATH) $sep] {
        set path [string trim $path]
        if {[string length $path] && [file isdirectory $path]} {
          lappend paths $path
        }
      }
    }
    if {[info exists ::env(ProgramFiles)]} {
      foreach path [glob -nocomplain -dir $::env(ProgramFiles) *gtk*] {
        set path $path/bin
        if {[file isdirectory $path]} {
          lappend paths [file normalize $path]
        }
      }
    }
      set loaded 0
      foreach lib $libs {
	  set found 0
	  foreach name $names {
	      foreach prefix $prefixes {
		  foreach path $paths {
		      if {$found} {break}
		      if {![file isdirectory $path]} {continue}
		      foreach file [glob -nocomplain -dir $path -type f ${prefix}${lib}${name}$ext] {
			  if {$found} {break}
			  if {[file exists $file]} {
			      if {[catch {initialiseLibrary $lib $file} symbol]} {
				  puts " => ERROR while initialising library $lib from $file : $symbol"
			      } else {
				  incr loaded; set found 1
				  break
			      }
			  }
		      }
		  }
	      }
	  }
      }
      if {$loaded != [llength $libs]} {
      error "Not all symbols loaded!"
    }
  }

  proc getSystemInfo {} {
    global env
    variable System

    set System(HOME) {}
    foreach var {HOME} {
      if {[info exists env($var)]} {
        set System($var) [file normalize $env($var)]
        break
      }
    }

    set System(GTK_RC_FILES) {}
    if {[info exists env(GTK_RC_FILES)]} {
      set System(GTK_RC_FILES) [file normalize $env(GTK_RC_FILES)]
    } elseif {[info exists env(HOME)]} {
      foreach rc [glob -nocomplain -dir $System(HOME) -type f .gtkrc*] {
        lappend System(GTK_RC_FILES) $rc
      }
      set System(GTK_RC_FILES) [lsort -dictionary $System(GTK_RC_FILES)]
    }

    set System(TEMP) {}
    foreach var {TEMP TMP temp tmp} {
      if {[info exists env($var)]} {
        set System(TEMP) [file normalize $env($var)]
        break
      }
    } 
    if {![string length $System(TEMP)]} {
      foreach dir {/tmp} {
        if {[file isdirectory $dir] && [file writable $dir]} {
          set System(TEMP) $dir; break
        }
      }
    }

    set System(DEFAULT_FILES) [gtkDirectory default_files]
  };

  proc updateLayouts {} {
    ## Variable "theme" should be defined by the C part of the extension.
    variable theme
    variable GtkHScrollbar
    variable GtkVScrollbar
    if {![info exists theme]} {return}
    ttk::style theme use Gttk
    # puts "============================================================"
    # puts "Current Gtk Theme: [currentThemeName] ($theme)"
    # puts "Tab alignment:    [getStyleHint   -SH_TabBar_Alignment]"
    # puts "Tab base overlap: [getPixelMetric -PM_TabBarBaseOverlap]"
    # puts "Tab overlap:      [getPixelMetric -PM_TabBarTabOverlap]"
    # foreach sc {SC_ScrollBarAddLine SC_ScrollBarSubLine
    #             SC_ScrollBarAddPage SC_ScrollBarSubPage
    #             SC_ScrollBarFirst   SC_ScrollBarLast
    #             SC_ScrollBarSlider  SC_ScrollBarGroove} {
    #   foreach {x y w h} [getSubControlMetrics -$sc] {break}
    #   puts "$sc: x=$x, y=$y, w=$w, h=$h"
    # }
    ##
    ## Update the scrollbars layouts, according to information retrieved from
    ## the GTK style in use...
    ##
    ## has-backward-stepper:           Display the standard backward arrow
    ##                                 button.
    ## has-forward-stepper:            Display the standard forward arrow
    ##                                 button.
    ## has-secondary-backward-stepper: Display a second backward arrow button
    ##                                 on the opposite end of the scrollbar.
    ## has-secondary-forward-stepper:  Display a second forward arrow button
    ##                                 on the opposite end of the scrollbar.
    foreach widget {GtkHScrollbar GtkVScrollbar} {
      foreach p {has-backward-stepper has-forward-stepper
                 has-secondary-backward-stepper has-secondary-forward-stepper
                 trough-under-steppers trough-side-details} {
        set ${widget}($p) [widgetStyleProperty $widget $p boolean]
      }
      foreach p {arrow-displacement-x arrow-displacement-y
                 slider-width stepper-size stepper-spacing trough-border} {
        set ${widget}($p) [widgetStyleProperty $widget $p integer]
      }
    }
    # parray GtkHScrollbar
    # puts "============================================================"

    ##
    ## Dynamically create the scrollbars layouts...
    ##
    set Horizontal.Scrollbar.trough [list]
    if {$GtkHScrollbar(has-backward-stepper)} {
      lappend Horizontal.Scrollbar.trough \
              Horizontal.Scrollbar.leftarrow  -side left
    }
    if {$GtkHScrollbar(has-forward-stepper)} {
      lappend Horizontal.Scrollbar.trough \
              Horizontal.Scrollbar.rightarrow -side right
    }
    if {$GtkHScrollbar(has-secondary-backward-stepper)} {
      lappend Horizontal.Scrollbar.trough \
              Horizontal.Scrollbar.leftarrow -side right
    }
    if {$GtkHScrollbar(has-secondary-forward-stepper)} {
      lappend Horizontal.Scrollbar.trough \
              Horizontal.Scrollbar.rightarrow -side left
    }
    lappend Horizontal.Scrollbar.trough \
            Horizontal.Scrollbar.thumb -side left -expand true -sticky we
    ttk::style layout Horizontal.TScrollbar [list \
         Horizontal.Scrollbar.trough -children ${Horizontal.Scrollbar.trough}]

    set Vertical.Scrollbar.trough   [list]
    if {$GtkVScrollbar(has-backward-stepper)} {
      lappend Vertical.Scrollbar.trough \
              Vertical.Scrollbar.uparrow -side top
    }
    if {$GtkVScrollbar(has-forward-stepper)} {
      lappend Vertical.Scrollbar.trough \
              Vertical.Scrollbar.downarrow -side bottom
    }
    if {$GtkVScrollbar(has-secondary-backward-stepper)} {
      lappend Vertical.Scrollbar.trough \
              Vertical.Scrollbar.uparrow -side bottom
    }
    if {$GtkVScrollbar(has-secondary-forward-stepper)} {
      lappend Vertical.Scrollbar.trough \
              Vertical.Scrollbar.downarrow -side top
    }
    lappend Vertical.Scrollbar.trough \
            Vertical.Scrollbar.thumb -side top -expand true -sticky ns
    ttk::style layout Vertical.TScrollbar \
      [list Vertical.Scrollbar.trough -children ${Vertical.Scrollbar.trough}]
  }; # updateLayouts

  proc updateStyles {} {
    ttk::style theme settings Gttk {
      ttk::style configure . \
         -background       [currentThemeColour bg(NORMAL)] \
         -foreground       [currentThemeColour fg(NORMAL)] \
         -selectforeground [currentThemeColour fg(SELECTED)] \
         -selectbackground [currentThemeColour bg(SELECTED)] \
         ;
      # ttk::style map . -foreground [list \
      #    active          [currentThemeColour fg(ACTIVE)]      \
      #    disabled        [currentThemeColour fg(INSENSITIVE)] \
      #    focus           [currentThemeColour fg(PRELIGHT)]    \
      #    pressed         [currentThemeColour fg(ACTIVE)]      \
      #    selected        [currentThemeColour fg(SELECTED)]    \
      # ] -background [list \
      #    active          [currentThemeColour bg(ACTIVE)]      \
      #    disabled        [currentThemeColour bg(INSENSITIVE)] \
      #    pressed         [currentThemeColour bg(PRELIGHT)]    \
      #    pressed         [currentThemeColour bg(ACTIVE)]      \
      #    selected        [currentThemeColour bg(SELECTED)]    \
      # ]

      # ttk::style map TButton -foreground [list \
      # ] -background [list \
      # ]
      ttk::style configure TButton -anchor center -width -11 -padding {2}

      # ttk::style map TCheckbutton -foreground [list \
      # ] -background [list \
      # ]
      ttk::style configure TCheckbutton -padding {0 1 0 1}
      
      # ttk::style map TCombobox -foreground [list \
      # ] -background [list \
      # ]
      ttk::style configure TCombobox    -padding {1 2 1 1}
      
      # ttk::style map TEntry -foreground [list \
      # ] -background [list \
      # ] -selectforeground [list \
      # ] -selectbackground [list \
      # ]
      # ttk::style configure TEntry       -padding {3 4 3 3}
      
      ttk::style configure TLabelframe  -background [currentThemeColour \
                            bg(NORMAL)] -labeloutside false -padding 0
      
      # ttk::style map TMenubutton -foreground [list \
      # ] -background [list \
      # ] -selectforeground [list \
      # ] -selectbackground [list \
      # ]
      ttk::style configure TMenubutton  -width -11 -padding {3 2 3 2}

      ## Notebook widget properties:
      ## tab-pos:       Which side of the notebook holds the tabs
      ## tab-border:    Width of the border around the tab labels
      ## tab-hborder:   Width of the horizontal border of tab labels
      ## tab-vborder:   Width of the vertical border of tab labels
      ## homogeneous:   Whether tabs should have homogeneous sizes
      ## tab-overlap:   Size of tab overlap area
      ## tab-curvature: Size of tab curvature
      ## arrow-spacing: Scroll arrow spacing
      set tab_overlap   [widgetStyleProperty GtkNotebook tab-overlap   integer]
      set tab_curvature [widgetStyleProperty GtkNotebook tab-curvature integer]
      set homogeneous   [widgetProperty      GtkNotebook homogeneous   boolean]
      set tab_pos       [gtkEnum GtkPositionType [widgetProperty GtkNotebook tab-pos integer]]
      set tab_base_overlap 0
      # incr tab_overlap $tab_curvature
      # puts "tab_overlap   -> $tab_overlap"
      # puts "tab_curvature -> $tab_curvature"
      # puts "homogeneous   -> $homogeneous"
      # puts "tab_pos       -> $tab_pos"
      # puts "tab_overlap=$tab_overlap, tab_base_overlap=$tab_base_overlap"
      switch -exact $tab_pos {
        GTK_POS_TOP    {set tabposition nw}
        GTK_POS_BOTTOM {set tabposition sw}
        GTK_POS_LEFT   {set tabposition nw}
        GTK_POS_RIGHT  {set tabposition ne}
        default        {set tabposition nw}
      }
      # tabmargins {left top right bottom}
      ttk::style configure TNotebook -tabposition $tabposition -tabmargins \
        [list $tab_overlap 0 $tab_overlap $tab_base_overlap]
      ttk::style map TNotebook.Tab -expand [list selected \
        [list $tab_overlap 0 $tab_overlap $tab_base_overlap]]

      # ttk::style map TRadiobutton -foreground [list \
      # ] -background [list \
      # ]
      ttk::style configure TRadiobutton -padding {0 1 0 1}

      # ttk::style map Toolbutton -foreground [list \
      # ] -background [list \
      # ]
      ttk::style configure Toolbutton -anchor center -padding {2 2 2 2}

      ttk::style configure TPaned -background [currentThemeColour bg(NORMAL)]
      ttk::style configure Horizontal.Sash -background [currentThemeColour \
          bg(NORMAL)]
      ttk::style configure Vertical.Sash -background [currentThemeColour \
          bg(NORMAL)]
    }
  }

  proc availableGtkThemes {} {
    variable StyleToRc
    variable System
    set styles {}
    foreach dir [list $System(HOME)/.themes [gtkDirectory theme]] {
      foreach theme [glob -nocomplain -type d -directory $dir *] {
        if {[file exists $theme/gtk-2.0/gtkrc]} {
          set style [file tail $theme]
          lappend styles $style
          set StyleToRc($style) [file normalize $theme/gtk-2.0/gtkrc]
        }
      }
    }
    return $styles
  };

  proc setGtkTheme {theme} {
    variable StyleToRc
    variable System
    if {![info exists StyleToRc($theme)]} {
      error
    }

    ## In order to force a theme update, we need to create an rc file, and force GTK to load it...
    set rc [file normalize $System(TEMP)/gtkrc.Gttk-[pid]]
    set fd [open $rc w]
    puts $fd {# -- THEME AUTO-WRITTEN DO NOT EDIT --}
    puts $fd "include \"$StyleToRc($theme)\""
    puts $fd "include \"$System(HOME)/.gtkrc.mine\""
    puts $fd {# -- THEME AUTO-WRITTEN DO NOT EDIT --}
    close $fd

    ## Set it as the default file...
    set default_files [gtkDirectory default_files]
    gtkDirectory default_files [list $rc]
    gtk_method gtk_rc_reparse_all_for_settings
    gtk_method gtk_rc_reset_styles
    # puts $StyleToRc($theme)
    updateStyles
    updateLayouts
    event generate {} <<ThemeChanged>>
    file delete -force $rc
  }
  
  proc init {} {
    ## Update layouts on load...
    getSystemInfo
    availableGtkThemes
    updateLayouts
    updateStyles
  }
}
