// Copyright (c) 2004-2008 Georgios Petasis (petasis@iit.demokritos.gr)
// Copyright (c) 2022 rdbende (rdbende@gmail.com)

#include "Gttk_Utilities.h"
#include "Gttk_TkHeaders.h"
#include "Gttk_WidgetDefaults.h"

#if 0
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable size_grip_statemap[] =
{
};
#endif

typedef struct {
} SizeGripElement;

static Ttk_ElementOptionSpec SizeGripElementOptions[] = {
    {NULL}
};

static void SizeGripElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTTK_WIDGET_CACHE_DEFINITION;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = Gttk_GetStatusBar(wc);
    *widthPtr  = 18;
    *heightPtr = 18;
    GTTK_ENSURE_WIDGET_OK;
    *paddingPtr = Ttk_MakePadding(0, 0, widget->style->xthickness,
                                        widget->style->ythickness);
}

static void SizeGripElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    /* GTTK_SETUP_GTK_DRAWABLE; */
    GtkWidget *widget = wc->gtkWindow;
    GTTK_ENSURE_WIDGET_OK;
    GTTK_STYLE_FROM_WIDGET;
    GTTK_DRAWABLE_FROM_WIDGET;
    // Gttk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
    //         GTTK_SECTION_ALL);
    GTTK_DEFAULT_BACKGROUND;
    // Gttk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    Gttk_gtk_paint_resize_grip(style, gdkDrawable, GTK_STATE_NORMAL, NULL, widget,
       "window", GDK_WINDOW_EDGE_SOUTH_EAST, 0, 0, b.width, b.height);
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}; /* SizeGripElementDraw */

static Ttk_ElementSpec SizeGripElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(SizeGripElement),
    SizeGripElementOptions,
    SizeGripElementGeometry,
    SizeGripElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

TTK_BEGIN_LAYOUT(SizegripLayout)
    TTK_NODE("Sizegrip.sizegrip", TTK_PACK_BOTTOM|TTK_STICK_S|TTK_STICK_E)
TTK_END_LAYOUT

int Gttk_Init_SizeGrip(Tcl_Interp *interp,
                       Gttk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "sizegrip",
            &SizeGripElementSpec, (void *) wc[0]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr, "TSizegrip", SizegripLayout);

    return TCL_OK;
}; /* Gttk_Init_SizeGrip */
