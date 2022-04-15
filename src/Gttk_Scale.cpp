// Copyright (c) 2004-2008 Georgios Petasis (petasis@iit.demokritos.gr)
// Copyright (c) 2022 rdbende (rdbende@gmail.com)

#include "Gttk_Utilities.h"
#include "Gttk_TkHeaders.h"
#include "Gttk_WidgetDefaults.h"

#if 0
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable scale_statemap[] =
{
};
#endif

typedef struct {
} ScaleTroughElement;

static Ttk_ElementOptionSpec ScaleTroughElementOptions[] = {
    {NULL}
};

static void ScaleTroughElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTTK_WIDGET_CACHE_DEFINITION;
    gint trough_border = 0;
    GtkWidget *widget = Gttk_GetScale(wc);
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GTTK_ENSURE_WIDGET_OK;
    Gttk_gtk_widget_style_get(widget, "trough-border", &trough_border, NULL);
    int xt = widget->style->xthickness;
    int yt = widget->style->ythickness;
    *paddingPtr = Ttk_MakePadding(xt + trough_border,
                                  yt + trough_border,
                                  xt + trough_border,
                                  yt + trough_border);
}

static void ScaleTroughElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = Gttk_GetScale(wc);
    gboolean trough_side_details = FALSE;
    GTTK_ENSURE_WIDGET_OK;
    GTTK_DRAWABLE_FROM_WIDGET;
    GTTK_STYLE_BACKGROUND_DEFAULT;
    GTTK_DEFAULT_BACKGROUND;
    GTTK_STYLE_FROM_WIDGET;
    Gttk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTTK_SECTION_TROUGH|GTTK_SECTION_ALL);
    GTTK_WIDGET_SET_FOCUS(widget);
    Gttk_gtk_widget_style_get(widget, "trough-side-details",
                                 &trough_side_details, NULL);
    // Gttk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    if (trough_side_details) {
      Gttk_gtk_paint_box(style, gdkDrawable, gtkState, GTK_SHADOW_IN, NULL,
              widget, "trough-upper", 0, 0,
              b.width, b.height);
    } else {
      Gttk_gtk_paint_box(style, gdkDrawable, gtkState, GTK_SHADOW_IN, NULL,
              widget, "trough", 0, 0, b.width, b.height);
    }
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec ScaleTroughElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ScaleTroughElement),
    ScaleTroughElementOptions,
    ScaleTroughElementGeometry,
    ScaleTroughElementDraw
};

typedef struct {
    Tcl_Obj *lengthObj;  /* the length of the slider (if a flat style) */
    Tcl_Obj *widthObj;   /* the width of the slider (height if horizontal) */
    Tcl_Obj *orientObj;  /* orientation of overall slider */
} ScaleSliderElement;

static Ttk_ElementOptionSpec ScaleSliderElementOptions[] = {
    {NULL}
};

static void ScaleSliderElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTTK_WIDGET_CACHE_DEFINITION;
    gint slider_len = 0, slider_width = ScaleThumbMinimumLen;
    GtkWidget *widget = Gttk_GetScale(wc);
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GTTK_ENSURE_WIDGET_OK;
    Gttk_gtk_widget_style_get(widget,
           "slider-length", &slider_len,
           "slider-width",  &slider_width, NULL);
    *heightPtr = *widthPtr = slider_width;
    if (wc->orientation == TTK_ORIENT_HORIZONTAL) {
      *heightPtr = slider_width;
      *widthPtr  = slider_len;
    } else {
      *widthPtr  = slider_width;
      *heightPtr = slider_len;
    }
    *paddingPtr = Ttk_UniformPadding(0);
}

static void ScaleSliderElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = Gttk_GetScale(wc);
    GtkAdjustment *adj = NULL;
    GTTK_ENSURE_WIDGET_OK;
    GTTK_DRAWABLE_FROM_WIDGET;
    GTTK_STYLE_BACKGROUND_DEFAULT;
    GTTK_DEFAULT_BACKGROUND;
    GTTK_STYLE_FROM_WIDGET;
    GTTK_WIDGET_SET_FOCUS(widget);
    adj = Gttk_gtk_range_get_adjustment((GtkRange *) widget);
    Gttk_gtk_adjustment_set_value(adj,
            Gttk_ValueFromSlider(wc, tkwin, b));
    Gttk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTTK_SECTION_SCROLLBAR|GTTK_SECTION_ALL);
    // Gttk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    Gttk_gtk_paint_slider(style, gdkDrawable, gtkState, gtkShadow, NULL,
            widget,
            (wc->orientation == TTK_ORIENT_HORIZONTAL) ? "hscale" : "vscale",
            0, 0, b.width, b.height, wc->gtkOrientation);
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec ScaleSliderElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ScaleSliderElement),
    ScaleSliderElementOptions,
    ScaleSliderElementGeometry,
    ScaleSliderElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

TTK_BEGIN_LAYOUT(VerticalScaleLayout)
        TTK_GROUP("Vertical.Scale.trough", TTK_FILL_BOTH,
            TTK_NODE("Vertical.Scale.slider", TTK_PACK_TOP) )
TTK_END_LAYOUT

TTK_BEGIN_LAYOUT(HorizontalScaleLayout)
        TTK_GROUP("Horizontal.Scale.trough", TTK_FILL_BOTH,
            TTK_NODE("Horizontal.Scale.slider", TTK_PACK_LEFT) )
TTK_END_LAYOUT

int Gttk_Init_Scale(Tcl_Interp *interp,
                       Gttk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Horizontal.Scale.trough",
            &ScaleTroughElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Vertical.Scale.trough",
            &ScaleTroughElementSpec, (void *) wc[1]);
    Ttk_RegisterElement(interp, themePtr, "Horizontal.Scale.slider",
            &ScaleSliderElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Vertical.Scale.slider",
            &ScaleSliderElementSpec, (void *) wc[1]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr,
            "Horizontal.TScale", HorizontalScaleLayout);
    Ttk_RegisterLayout(themePtr,
            "Vertical.TScale", VerticalScaleLayout);

    return TCL_OK;
}; /* Gttk_Init_Scale */
