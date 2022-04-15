// Copyright (c) 2004-2008 Georgios Petasis (petasis@iit.demokritos.gr)
// Copyright (c) 2022 rdbende (rdbende@gmail.com)

#include "Gttk_Utilities.h"
#include "Gttk_TkHeaders.h"
#include "Gttk_WidgetDefaults.h"

#if 0
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable scrollbar_statemap[] =
{
};
#endif

typedef struct {
} ScrollbarTroughElement;

static Ttk_ElementOptionSpec ScrollbarTroughElementOptions[] = {
    {NULL}
};

static void ScrollbarTroughElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTTK_WIDGET_CACHE_DEFINITION;
    gint trough_border = 0;
    int xt = 0, yt = 0;
    GtkWidget *widget = Gttk_GetScrollBar(wc);
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GTTK_ENSURE_WIDGET_OK;
    Gttk_gtk_widget_style_get(widget, "trough-border", &trough_border, NULL);
    xt = widget->style->xthickness;
    yt = widget->style->ythickness;
    *paddingPtr = Ttk_MakePadding(xt + trough_border - xt,
                                  yt + trough_border - yt,
                                  xt + trough_border + xt,
                                  yt + trough_border + yt);
}

static void ScrollbarTroughElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GTTK_SETUP_GTK_DRAWABLE;
    GtkWidget *widget = Gttk_GetScrollBar(wc);
    gboolean trough_side_details = FALSE;
    GTTK_ENSURE_WIDGET_OK;
    GTTK_STYLE_FROM_WIDGET;
    Gttk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTTK_SECTION_TROUGH|GTTK_SECTION_ALL);
    //GTTK_SETUP_WIDGET_SIZE(b.width, b.height);
    GTTK_WIDGET_SET_FOCUS(widget);
    Gttk_gtk_widget_style_get(widget, "trough-side-details",
                                 &trough_side_details, NULL);
    GTTK_DEFAULT_BACKGROUND;
    // Gttk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    if (trough_side_details) {
      int trough_change_pos_x = b.width, trough_change_pos_y = b.height;
      if (wc->gtkOrientation == GTK_ORIENTATION_HORIZONTAL)
        trough_change_pos_x = b.width / 2;
      else
        trough_change_pos_y = b.height / 2;
      Gttk_gtk_paint_box(style, gdkDrawable, gtkState, GTK_SHADOW_IN, NULL, widget,
          "trough-upper", 0, 0, trough_change_pos_x, trough_change_pos_y);
      if (wc->gtkOrientation == GTK_ORIENTATION_HORIZONTAL)
        trough_change_pos_y = 0;
      else
        trough_change_pos_x = 0;
      Gttk_gtk_paint_box(style, gdkDrawable, gtkState, GTK_SHADOW_IN, NULL, widget,
          "trough-lower", trough_change_pos_x, trough_change_pos_y,
          b.width-trough_change_pos_x, b.height-trough_change_pos_y);
    } else {
      Gttk_gtk_paint_box(style, gdkDrawable, gtkState, GTK_SHADOW_IN, NULL, widget,
          "trough", 0, 0, b.width, b.height);
    }
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec ScrollbarTroughElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ScrollbarTroughElement),
    ScrollbarTroughElementOptions,
    ScrollbarTroughElementGeometry,
    ScrollbarTroughElementDraw
};

typedef struct {
} ScrollbarThumbElement;

static Ttk_ElementOptionSpec ScrollbarThumbElementOptions[] = {
    {NULL}
};

static void ScrollbarThumbElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTTK_WIDGET_CACHE_DEFINITION;
    gint slider_width = 0;
    gint slider_min = ScrollbarThumbMinimumLen;
    GtkWidget *widget = Gttk_GetScrollBar(wc);
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GTTK_ENSURE_WIDGET_OK;
    Gttk_gtk_widget_style_get(widget, "slider-width", &slider_width,
                                  "min-slider-length", &slider_min, NULL);
    *heightPtr = *widthPtr = slider_min;
    if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
      *heightPtr = slider_width;
      *widthPtr  = slider_min;
    } else {
      *widthPtr  = slider_width;
      *heightPtr = slider_min;
    }
    *paddingPtr = Ttk_UniformPadding(0);
}

static void ScrollbarThumbElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GTTK_SETUP_GTK_DRAWABLE;
    GtkWidget *widget = Gttk_GetScrollBar(wc);
    GtkAdjustment *adj = NULL;
    GTTK_ENSURE_WIDGET_OK;
    GTTK_STYLE_FROM_WIDGET;
    // GTTK_SETUP_WIDGET_SIZE(b.width, b.height);
    GTTK_WIDGET_SET_FOCUS(widget);
    adj = Gttk_gtk_range_get_adjustment((GtkRange *) widget);
    Gttk_gtk_adjustment_set_value(adj,Gttk_ValueFromSlider(wc, tkwin, b));
    //    GTTK_DEFAULT_BACKGROUND;
      Gttk_gtk_paint_box(style, gdkDrawable, gtkState, GTK_SHADOW_IN, NULL, widget,
          "trough", 0, 0, b.width, b.height);

    Gttk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTTK_SECTION_SCROLLBAR|GTTK_SECTION_ALL);
    // Gttk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    Gttk_gtk_paint_slider(style, gdkDrawable, gtkState, gtkShadow, NULL, widget,
        "slider", 0, 0, b.width, b.height,
        wc->gtkOrientation);
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec ScrollbarThumbElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ScrollbarThumbElement),
    ScrollbarThumbElementOptions,
    ScrollbarThumbElementGeometry,
    ScrollbarThumbElementDraw
};

typedef struct {
} ScrollbarUpArrowElement;

static Ttk_ElementOptionSpec ScrollbarUpArrowElementOptions[] = {
    {NULL}
};

static void ScrollbarUpArrowElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTTK_WIDGET_CACHE_DEFINITION;
    gint stepper_size = 0, slider_width = 0;
    GtkWidget *widget = Gttk_GetScrollBar(wc);
    int xt = 0, yt = 0, trough_border = 0;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GTTK_ENSURE_WIDGET_OK;
    Gttk_gtk_widget_style_get(widget, "stepper-size", &stepper_size,
                                  "slider-width", &slider_width, NULL);
    if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
      *widthPtr  = stepper_size;
      *heightPtr = slider_width;
    } else {
      *widthPtr  = slider_width;
      *heightPtr = stepper_size;
    }
    xt = widget->style->xthickness;
    yt = widget->style->ythickness;
    *paddingPtr = Ttk_MakePadding(trough_border - xt,
                                  trough_border - yt,
                                  trough_border + xt,
                                  trough_border + yt);
}

static void ScrollbarUpArrowElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GTTK_SETUP_GTK_DRAWABLE;
    GtkWidget *widget = Gttk_GetScrollBar(wc);
    int horizontal = wc->orientation == TTK_ORIENT_HORIZONTAL;
    gfloat arrow_scaling;
    gint arrow_x;
    gint arrow_y;
    gint arrow_width;
    gint arrow_height;
    GTTK_ENSURE_WIDGET_OK;
    GTTK_STYLE_FROM_WIDGET;
    Gttk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTTK_SECTION_STEPPERS|GTTK_SECTION_ALL);
    // GTTK_SETUP_WIDGET_SIZE(b.width, b.height);
    GTTK_WIDGET_SET_FOCUS(widget);
    // GTTK_DEFAULT_BACKGROUND;
    // Gttk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    Gttk_gtk_paint_box(style, gdkDrawable, gtkState, GTK_SHADOW_IN, NULL,
        widget, horizontal ? "hscrollbar":"vscrollbar",
        0, 0, b.width, b.height);
    /* Draw the arrow, according to the desired scaling! */
    Gttk_gtk_widget_style_get(widget, "arrow-scaling", &arrow_scaling, NULL);
    if (arrow_scaling == 0.0) arrow_scaling = 1.0;
    arrow_width  = b.width  * arrow_scaling;
    arrow_height = b.height * arrow_scaling;
    arrow_x =  (b.width  - arrow_width) / 2;
    arrow_y =  (b.height - arrow_height) / 2;

    Gttk_gtk_paint_arrow(style, gdkDrawable, gtkState, gtkShadow, NULL,
        widget, horizontal ? "hscrollbar":"vscrollbar",
        horizontal? GTK_ARROW_LEFT : GTK_ARROW_UP, FALSE,
        arrow_x, arrow_y, arrow_width, arrow_height);
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec ScrollbarUpArrowElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ScrollbarUpArrowElement),
    ScrollbarUpArrowElementOptions,
    ScrollbarUpArrowElementGeometry,
    ScrollbarUpArrowElementDraw
};

typedef struct {
} ScrollbarDownArrowElement;

static Ttk_ElementOptionSpec ScrollbarDownArrowElementOptions[] = {
    {NULL}
};

static void ScrollbarDownArrowElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    ScrollbarUpArrowElementGeometry(clientData, elementRecord, tkwin,
                                    widthPtr, heightPtr, paddingPtr);
}

static void ScrollbarDownArrowElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GTTK_SETUP_GTK_DRAWABLE;
    GtkWidget *widget = Gttk_GetScrollBar(wc);
    int horizontal = wc->orientation == TTK_ORIENT_HORIZONTAL;
    GTTK_ENSURE_WIDGET_OK;
    gfloat arrow_scaling;
    gint arrow_x;
    gint arrow_y;
    gint arrow_width;
    gint arrow_height;
    GTTK_STYLE_FROM_WIDGET;
    Gttk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTTK_SECTION_STEPPERS|GTTK_SECTION_ALL);
    // GTTK_SETUP_WIDGET_SIZE(b.width, b.height);
    GTTK_WIDGET_SET_FOCUS(widget);
    // GTTK_DEFAULT_BACKGROUND;
    // Gttk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    Gttk_gtk_paint_box(style, gdkDrawable, gtkState, GTK_SHADOW_IN, NULL,
        widget, horizontal ? "hscrollbar":"vscrollbar",
        0, 0, b.width, b.height);
    /* Draw the arrow, according to the desired scaling! */
    Gttk_gtk_widget_style_get(widget, "arrow-scaling", &arrow_scaling, NULL);
    if (arrow_scaling == 0.0) arrow_scaling = 1.0;
    arrow_width  = b.width  * arrow_scaling;
    arrow_height = b.height * arrow_scaling;
    arrow_x =  (b.width  - arrow_width) / 2;
    arrow_y =  (b.height - arrow_height) / 2;

    Gttk_gtk_paint_arrow(style, gdkDrawable, gtkState, gtkShadow, NULL,
        widget, horizontal ? "hscrollbar":"vscrollbar",
        horizontal? GTK_ARROW_RIGHT : GTK_ARROW_DOWN, TRUE,
        arrow_x, arrow_y, arrow_width, arrow_height);
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec ScrollbarDownArrowElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ScrollbarDownArrowElement),
    ScrollbarDownArrowElementOptions,
    ScrollbarDownArrowElementGeometry,
    ScrollbarDownArrowElementDraw
};

typedef struct {
} ScrollbarEmptyArrowElement;
static Ttk_ElementOptionSpec ScrollbarEmptyArrowElementOptions[] = {
    {NULL}
};

static void ScrollbarUpSubArrowElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    ScrollbarUpArrowElementDraw(clientData, elementRecord, tkwin, d, b, state);
}

static void ScrollbarDownSubArrowElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned stte)
{
    ScrollbarDownArrowElementDraw(clientData, elementRecord, tkwin, d, b, stte);
}

static Ttk_ElementSpec ScrollbarUpSubArrowElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ScrollbarEmptyArrowElement),
    ScrollbarEmptyArrowElementOptions,
    ScrollbarUpArrowElementGeometry,
    ScrollbarUpSubArrowElementDraw
};

static Ttk_ElementSpec ScrollbarDownSubArrowElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ScrollbarEmptyArrowElement),
    ScrollbarEmptyArrowElementOptions,
    ScrollbarUpArrowElementGeometry,
    ScrollbarDownSubArrowElementDraw
};


/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

// TTK_BEGIN_LAYOUT(HorizontalScrollbarLayout)
//     TTK_GROUP("Horizontal.Scrollbar.trough", TTK_FILL_X,
//         TTK_NODE("Horizontal.Scrollbar.leftarrow", TTK_PACK_LEFT)
//         TTK_NODE("Horizontal.Scrollbar.rightarrow", TTK_PACK_RIGHT)
//         TTK_NODE("Horizontal.Scrollbar.thumb", TTK_FILL_BOTH))
// TTK_END_LAYOUT
// 
// TTK_BEGIN_LAYOUT(VerticalScrollbarLayout)
//     TTK_GROUP("Vertical.Scrollbar.trough", TTK_FILL_Y,
//         TTK_NODE("Vertical.Scrollbar.uparrow", TTK_PACK_TOP)
//         TTK_NODE("Vertical.Scrollbar.downarrow", TTK_PACK_BOTTOM)
//         TTK_NODE("Vertical.Scrollbar.thumb", TTK_FILL_BOTH))
// TTK_END_LAYOUT

int Gttk_Init_Scrollbar(Tcl_Interp *interp,
                       Gttk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Horizontal.Scrollbar.trough",
      &ScrollbarTroughElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Vertical.Scrollbar.trough",
      &ScrollbarTroughElementSpec, (void *) wc[1]);
    Ttk_RegisterElement(interp, themePtr, "Horizontal.Scrollbar.thumb",
      &ScrollbarThumbElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Vertical.Scrollbar.thumb",
      &ScrollbarThumbElementSpec, (void *) wc[1]);
    Ttk_RegisterElement(interp, themePtr, "Horizontal.Scrollbar.leftarrow",
      &ScrollbarUpArrowElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Vertical.Scrollbar.uparrow",
      &ScrollbarUpArrowElementSpec, (void *) wc[1]);
    Ttk_RegisterElement(interp, themePtr, "Horizontal.Scrollbar.rightarrow",
      &ScrollbarDownArrowElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Vertical.Scrollbar.downarrow",
      &ScrollbarDownArrowElementSpec, (void *) wc[1]);
    Ttk_RegisterElement(interp, themePtr, "Horizontal.Scrollbar.subleftarrow",
      &ScrollbarUpSubArrowElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Vertical.Scrollbar.subuparrow",
      &ScrollbarUpSubArrowElementSpec, (void *) wc[1]);
    Ttk_RegisterElement(interp, themePtr, "Horizontal.Scrollbar.subrightarrow",
      &ScrollbarDownSubArrowElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Vertical.Scrollbar.subdownarrow",
      &ScrollbarDownSubArrowElementSpec, (void *) wc[1]);
    Ttk_RegisterElement(interp, themePtr, "Scrollbar.grip",
      &ScrollbarTroughElementSpec, NULL);
    
    /*
     * Register layouts:
     */
    //Ttk_RegisterLayout(themePtr, "Vertical.TScrollbar",
    //        VerticalScrollbarLayout);
    //Ttk_RegisterLayout(themePtr, "Horizontal.TScrollbar",
    //        HorizontalScrollbarLayout);

    return TCL_OK;
}; /* Gttk_Init_Scrollbar */
