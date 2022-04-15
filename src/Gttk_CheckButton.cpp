// Copyright (c) 2004-2008 Georgios Petasis (petasis@iit.demokritos.gr)
// Copyright (c) 2022 rdbende (rdbende@gmail.com)

#include "Gttk_Utilities.h"
#include "Gttk_TkHeaders.h"
#include "Gttk_WidgetDefaults.h"

typedef struct {
} CheckButtonIndicatorElement;


static Ttk_ElementOptionSpec CheckButtonIndicatorElementOptions[] = {
    {NULL}
};

static void CheckButtonIndicatorElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTTK_WIDGET_CACHE_DEFINITION;
    gint size, spacing = CheckButtonHorizontalPadding;
    gint focus_width, focus_pad;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = Gttk_GetCheckButton(wc);
    GTTK_ENSURE_WIDGET_OK;
    Gttk_gtk_widget_style_get(widget,
           "indicator-size",    &size,
           "indicator-spacing", &spacing,
           "focus-line-width",  &focus_width,
           "focus-padding",     &focus_pad, NULL);
    *widthPtr = *heightPtr = size+spacing;
    size = focus_pad + focus_width;
    *paddingPtr = Ttk_MakePadding(spacing+size, size, spacing+size, size);
}

static void CheckButtonIndicatorElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    gint indicator_size, x, y;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = Gttk_GetCheckButton(wc);
    GTTK_ENSURE_WIDGET_OK;
    GTTK_DRAWABLE_FROM_WIDGET;
    style = Gttk_GetGtkWindowStyle(wc->gtkWindow);
    GTTK_DEFAULT_BACKGROUND;
    GTTK_STYLE_FROM_WIDGET;
    GTTK_WIDGET_SET_FOCUS(widget);
    Gttk_gtk_widget_style_get(widget,
           "indicator-size", &indicator_size, NULL);
    Gttk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTTK_SECTION_BUTTONS|GTTK_SECTION_ALL);
    // Gttk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    x = (b.width  - indicator_size) / 2;
    y = (b.height - indicator_size) / 2;
    Gttk_gtk_paint_check(style, gdkDrawable, gtkState, gtkShadow, NULL,
            widget, "checkbutton", x, y, indicator_size, indicator_size);
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
            0, 0, b.width, b.height, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec CheckButtonIndicatorElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(CheckButtonIndicatorElement),
    CheckButtonIndicatorElementOptions,
    CheckButtonIndicatorElementGeometry,
    CheckButtonIndicatorElementDraw
};

typedef struct {
} CheckButtonBorderElement;


static Ttk_ElementOptionSpec CheckButtonBorderElementOptions[] = {
    {NULL}
};

static void CheckButtonBorderElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTTK_WIDGET_CACHE_DEFINITION;
    gint focus_width;
    gint focus_pad;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = Gttk_GetCheckButton(wc);
    GTTK_ENSURE_WIDGET_OK;
    Gttk_gtk_widget_style_get(widget,
           "focus-line-width", &focus_width,
           "focus-padding",    &focus_pad, NULL);
    *paddingPtr = Ttk_UniformPadding(focus_width + focus_pad);
}

static void CheckButtonBorderElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = Gttk_GetCheckButton(wc);
    GTTK_ENSURE_WIDGET_OK;
    GTTK_DRAWABLE_FROM_WIDGET;
    GTTK_STYLE_BACKGROUND_DEFAULT;
    GTTK_DEFAULT_BACKGROUND;
    if (state & TTK_STATE_FOCUS) {
      gint border_width = ((GtkContainer*) widget)->border_width;
      gint focus_width;
      gint focus_pad;
      GTTK_STYLE_FROM_WIDGET;
      GTTK_WIDGET_SET_FOCUS(widget);
      Gttk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTTK_SECTION_BUTTONS|GTTK_SECTION_ALL);
      Gttk_gtk_widget_style_get(widget,
           "focus-line-width", &focus_width,
           "focus-padding",    &focus_pad, NULL);
      Gttk_gtk_paint_focus(style, gdkDrawable, gtkState, NULL, widget,
              "checkbutton", b.x + border_width, b.y + border_width,
               b.width - 2 * border_width, b.height - 2 * border_width);
    }
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                   0, 0, b.width, b.height, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec CheckButtonBorderElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(CheckButtonBorderElement),
    CheckButtonBorderElementOptions,
    CheckButtonBorderElementGeometry,
    CheckButtonBorderElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

/*
 * TTK_BEGIN_LAYOUT(CheckbuttonLayout)
 *      TTK_GROUP("Checkbutton.border", TTK_FILL_BOTH,
 *          TTK_GROUP("Checkbutton.padding", TTK_FILL_BOTH,
 *              TTK_NODE("Checkbutton.indicator", TTK_PACK_LEFT)
 *              TTK_GROUP("Checkbutton.focus", TTK_PACK_LEFT | TTK_STICK_W,
 *                  TTK_NODE("Checkbutton.label", TTK_FILL_BOTH))))
 * TTK_END_LAYOUT
 */
TTK_BEGIN_LAYOUT(CheckbuttonLayout)
  TTK_GROUP("Checkbutton.border", TTK_FILL_BOTH,
    TTK_GROUP("Checkbutton.padding", TTK_FILL_BOTH,
      TTK_NODE("Checkbutton.indicator", TTK_PACK_LEFT)
      TTK_NODE("Checkbutton.label", TTK_PACK_LEFT|TTK_STICK_W|TTK_FILL_BOTH)))
TTK_END_LAYOUT

int Gttk_Init_CheckButton(Tcl_Interp *interp,
                       Gttk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Checkbutton.border",
            &CheckButtonBorderElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Checkbutton.indicator",
            &CheckButtonIndicatorElementSpec, (void *) wc[0]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr, "TCheckbutton", CheckbuttonLayout);
    
    return TCL_OK;
}; /* Gttk_Init_CheckButton */
