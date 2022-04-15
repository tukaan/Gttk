// Copyright (c) 2004-2008 Georgios Petasis (petasis@iit.demokritos.gr)
// Copyright (c) 2022 rdbende (rdbende@gmail.com)

#include "Gttk_Utilities.h"
#include "Gttk_TkHeaders.h"
#include "Gttk_WidgetDefaults.h"

#if 0
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable notebook_statemap[] =
{
};
#endif

typedef struct {
} NotebookTabElement;


static Ttk_ElementOptionSpec NotebookTabElementOptions[] = {
    {NULL}
};

static void NotebookTabElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTTK_WIDGET_CACHE_DEFINITION;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = Gttk_GetNotebook(wc);
    gint focus_width = 1, tab_curvature = 1;
    GTTK_ENSURE_WIDGET_OK;
    Gttk_gtk_widget_style_get(widget, "focus-line-width", &focus_width,
                                 "tab-curvature",    &tab_curvature, NULL);
    *paddingPtr = Ttk_UniformPadding(tab_curvature + focus_width +
                                     ((GtkNotebook *)widget)->tab_hborder);
    // *paddingPtr = Ttk_MakePadding(7, 6, 7, 6);
}

#define TAB_BASE_OVERLAP_MAX 10
static void NotebookTabElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    int height_with_overlap = b.height + TAB_BASE_OVERLAP_MAX;
    int dh = 0;
    /* GTTK_SETUP_GTK_DRAWABLE_PIXMAP_SIZE(b.width, height_with_overlap); */
    /* GtkWidget *widget = Gttk_GetNotebook(wc); */
    /* 10 Aug 2009: Notebook widgets usually have a lighter background.
     * To avoid multiple background colours inside the widget,
     *  use a frame for drawing instead of a notebook gtk widget. */
    GtkWidget *widget = Gttk_GetFrame(wc);
    GTTK_ENSURE_WIDGET_OK;
    GTTK_STYLE_FROM_WIDGET;
    GTTK_DRAWABLE_FROM_WIDGET_SIZE(b.width, height_with_overlap);
    Gttk_gtk_style_apply_default_background(style, gdkDrawable, TRUE,
            gtkState, NULL, 0, 0, b.width, height_with_overlap);

    if (state & TTK_STATE_SELECTED) {
      dh = 1 /*widget->style->ythickness - 1*/;
    }

    if (state & TTK_STATE_USER1 && state & TTK_STATE_USER2) {
      /* This is the only tab in the widget! */
    } else if (state & TTK_STATE_USER1) {
      /* This is the first tab! */
    } else if (state & TTK_STATE_USER2) {
      /* This is the last tab! */
    }
    GTTK_DEFAULT_BACKGROUND;
    Gttk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTTK_SECTION_TABS|GTTK_SECTION_ALL);
    // Gttk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    Gttk_gtk_paint_extension(style, gdkDrawable, gtkState, gtkShadow, NULL,
       widget, (char *) "tab", 0, 0, b.width, b.height + dh, GTK_POS_BOTTOM);
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height + dh, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec NotebookTabElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(NotebookTabElement),
    NotebookTabElementOptions,
    NotebookTabElementGeometry,
    NotebookTabElementDraw
};

typedef struct {
} NotebookClientElement;


static Ttk_ElementOptionSpec NotebookClientElementOptions[] = {
    {NULL}
};

static void NotebookClientElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    int tabBarBaseHeight = 0;
    *paddingPtr = Ttk_MakePadding(NotebookClientUniformPadding,
           NotebookClientUniformPadding + tabBarBaseHeight,
           NotebookClientUniformPadding, NotebookClientUniformPadding);
}

static void NotebookClientElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    /* GTTK_SETUP_GTK_DRAWABLE; */
    /* GtkWidget *widget = Gttk_GetNotebook(wc); */
    /* 10 Aug 2009: Notebook widgets usually have a lighter background.
     * To avoid multiple background colours inside the widget,
     *  use a frame for drawing instead of a notebook gtk widget. */
    GtkWidget *widget = Gttk_GetFrame(wc);
    GTTK_ENSURE_WIDGET_OK;
    GTTK_STYLE_FROM_WIDGET;
    GTTK_DRAWABLE_FROM_WIDGET;
    // Gttk_gtk_paint_box_gap(style, gdkDrawable,
    //      GTK_STATE_NORMAL,GTK_SHADOW_OUT,
    //      NULL, widget, (char *) "notebook", 0, 0, b.width, b.height,
    //      GTK_POS_TOP, 0, 0);
    Gttk_gtk_paint_box(style, gdkDrawable, GTK_STATE_NORMAL, GTK_SHADOW_OUT,
         NULL, widget, (char *) "notebook", 0, 0, b.width, b.height);
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec NotebookClientElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(NotebookClientElement),
    NotebookClientElementOptions,
    NotebookClientElementGeometry,
    NotebookClientElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

int Gttk_Init_Notebook(Tcl_Interp *interp,
                         Gttk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Notebook.tab",
                &NotebookTabElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Notebook.client",
                &NotebookClientElementSpec, (void *) wc[0]);
    
    /*
     * Register layouts:
     */

    return TCL_OK;
}; /* Gttk_Init_Notebook */
