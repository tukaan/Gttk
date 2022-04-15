// Copyright (c) 2004-2008 Georgios Petasis (petasis@iit.demokritos.gr)
// Copyright (c) 2022 rdbende (rdbende@gmail.com)

#ifndef _GTTK_GTKHEADERS
#define _GTTK_GTKHEADERS

#include <tk.h>
#include "Gttk_Symbols.h"

/* Convenience memory allocators
 */
#define Gttk_g_new0(struct_type, n_structs) \
    ((struct_type *) Gttk_g_malloc0 (((gsize) sizeof (struct_type)) * ((gsize) (n_structs))))

typedef struct Gttk_WidgetCache {
  Tk_Window      Gttk_tkwin;
  Display*       Gttk_MainDisplay;
  Tcl_Interp*    Gttk_MainInterp;
  int            orientation;
  GtkOrientation gtkOrientation;
  GdkDisplay*    gdkDisplay;
  GtkWidget*     gtkArrow;
  GtkWidget*     gtkNotebook;
  GtkWidget*     gtkEntry;
  GtkWidget*     gtkHScale;
  GtkWidget*     gtkVScale;
  GtkWidget*     gtkHProgressBar;
  GtkWidget*     gtkVProgressBar;
  GtkWidget*     gtkCombobox;
  GtkWidget*     gtkComboboxEntry;
  GtkWidget*     gtkHScrollBar;
  GtkWidget*     gtkVScrollBar;
  GtkWidget*     gtkButton;
  GtkWidget*     gtkCheckButton;
  GtkWidget*     gtkSpinButton;
  GtkWidget*     gtkToolBar;
  GtkWidget*     gtkToolButton;
  GtkWidget*     gtkTreeView;
  GtkWidget*     gtkTreeHeader;
  GtkWidget*     gtkFrame;
  GtkWidget*     gtkStatusBar;
  GtkWidget*     gtkHPaned;
  GtkWidget*     gtkVPaned;
  GtkWidget*     gtkWindow;
  GtkWidget*     protoLayout;
} Gttk_WidgetCache;

typedef struct {
    GtkStateType  state;    /* State value to return if this entry matches */
    GtkShadowType shadow;   /* Shadow value to return if this entry matches */
    unsigned int  value;    /* Value to return if this entry matches */
    unsigned int  onBits;   /* Bits which must be set */
    unsigned int  offBits;  /* Bits which must be cleared */
    unsigned int  section;  /* Table section to match */
} Gttk_StateTable;

#define GTTK_SECTION_ALL       (1<<0)
#define GTTK_SECTION_BUTTONS   (1<<1)
#define GTTK_SECTION_ENTRY     (1<<2)
#define GTTK_SECTION_SCROLLBAR (1<<3)
#define GTTK_SECTION_STEPPERS  (1<<4)
#define GTTK_SECTION_TROUGH    (1<<5)
#define GTTK_SECTION_SASH      (1<<6)
#define GTTK_SECTION_TABS      (1<<7)

#endif
