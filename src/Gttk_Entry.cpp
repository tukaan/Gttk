// Copyright (c) 2004-2008 Georgios Petasis (petasis@iit.demokritos.gr)
// Copyright (c) 2022 rdbende (rdbende@gmail.com)

#include "Gttk_Utilities.h"
#include "Gttk_TkHeaders.h"
#include "Gttk_WidgetDefaults.h"

#if 0
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable entry_statemap[] =
{
};
#endif

typedef struct {
} EntryFieldElement;


static Ttk_ElementOptionSpec EntryFieldElementOptions[] = {
    {NULL}
};

static void EntryFieldElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTTK_WIDGET_CACHE_DEFINITION;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = Gttk_GetEntry(wc);
    GTTK_ENSURE_WIDGET_OK;
    int xt = widget->style->xthickness;
    int yt = widget->style->ythickness;
    *paddingPtr = Ttk_MakePadding(xt + EntryUniformPadding,
                                  yt + EntryUniformPadding,
                                  xt + EntryUniformPadding,
                                  yt + EntryUniformPadding);
}

static void EntryFieldElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    gboolean hasFrame = TRUE;
    GtkWidget *widget = Gttk_GetEntry(wc);
    GTTK_ENSURE_WIDGET_OK;
    GTTK_DRAWABLE_FROM_WIDGET;
    style = Gttk_GetGtkWindowStyle(wc->gtkWindow);
    GTTK_DEFAULT_BACKGROUND;
    if (hasFrame) {
      GTTK_STYLE_FROM_WIDGET;
      Gttk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
              GTTK_SECTION_ENTRY|GTTK_SECTION_ALL);
      Gttk_gtk_paint_flat_box(style, gdkDrawable, gtkState, gtkShadow, NULL, widget,
          "entry_bg", 0, 0, b.width, b.height);
      GTTK_WIDGET_SET_FOCUS(widget);
            Gttk_gtk_paint_shadow(style, gdkDrawable, gtkState, gtkShadow, NULL,
                          widget, "entry", 0, 0, b.width, b.height);
    }
    // Gttk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec EntryFieldElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(EntryFieldElement),
    EntryFieldElementOptions,
    EntryFieldElementGeometry,
    EntryFieldElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

int Gttk_Init_Entry(Tcl_Interp *interp,
                       Gttk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Entry.field",
            &EntryFieldElementSpec, (void *) wc[0]);
    
    /*
     * Register layouts:
     */

    return TCL_OK;
}; /* Gttk_Init_Entry */
