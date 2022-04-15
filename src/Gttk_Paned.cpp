// Copyright (c) 2004-2008 Georgios Petasis (petasis@iit.demokritos.gr)
// Copyright (c) 2022 rdbende (rdbende@gmail.com)

#include "Gttk_Utilities.h"
#include "Gttk_TkHeaders.h"
#include "Gttk_WidgetDefaults.h"

#if 0
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable paned_statemap[] =
{
};
#endif

typedef struct {
} PanedSashGripElement;

static Ttk_ElementOptionSpec PanedSashGripElementOptions[] = {
    {NULL}
};

static void PanedSashGripElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTTK_WIDGET_CACHE_DEFINITION;
    GtkWidget *widget = Gttk_GetPaned(wc);
    gint size = 5;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GTTK_ENSURE_WIDGET_OK;
    Gttk_gtk_widget_style_get(widget, "handle-size", &size, NULL);
    if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
      *widthPtr  = size;
    } else {
      *heightPtr = size;
    }
    *paddingPtr = Ttk_UniformPadding(PanedUniformPadding);
}

static void PanedSashGripElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    /* GTTK_SETUP_GTK_DRAWABLE; */
    GtkWidget *widget = Gttk_GetPaned(wc);
    GTTK_ENSURE_WIDGET_OK;
    GTTK_STYLE_FROM_WIDGET;
    GTTK_DRAWABLE_FROM_WIDGET;
    Gttk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTTK_SECTION_SASH|GTTK_SECTION_ALL);
    GTTK_DEFAULT_BACKGROUND;
    Gttk_gtk_paint_handle(style, gdkDrawable, gtkState, GTK_SHADOW_NONE,
            NULL, widget, "paned", 0, 0, b.width, b.height,
            wc->gtkOrientation);
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}; /* PanedSashGripElementDraw */

static Ttk_ElementSpec PanedSashGripElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(PanedSashGripElement),
    PanedSashGripElementOptions,
    PanedSashGripElementGeometry,
    PanedSashGripElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

TTK_BEGIN_LAYOUT(HorizontalSashLayout)
    TTK_GROUP("Sash.hsash", TTK_FILL_BOTH,
        TTK_NODE("Sash.hgrip", TTK_FILL_BOTH))
TTK_END_LAYOUT

TTK_BEGIN_LAYOUT(VerticalSashLayout)
    TTK_GROUP("Sash.vsash", TTK_FILL_BOTH,
        TTK_NODE("Sash.vgrip", TTK_FILL_BOTH))
TTK_END_LAYOUT

int Gttk_Init_Paned(Tcl_Interp *interp,
                       Gttk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "hgrip",
            &PanedSashGripElementSpec,  (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "vgrip",
            &PanedSashGripElementSpec,  (void *) wc[1]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr, "Horizontal.Sash", HorizontalSashLayout);
    Ttk_RegisterLayout(themePtr, "Vertical.Sash", VerticalSashLayout);

    return TCL_OK;
}; /* Gttk_Init_Paned */
