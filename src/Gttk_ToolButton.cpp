// Copyright (c) 2004-2008 Georgios Petasis (petasis@iit.demokritos.gr)
// Copyright (c) 2022 rdbende (rdbende@gmail.com)

#include "Gttk_Utilities.h"
#include "Gttk_TkHeaders.h"
#include "Gttk_WidgetDefaults.h"

typedef struct {
    Tcl_Obj        *defaultStateObj;
} ToolButtonElement;

static Ttk_ElementOptionSpec ToolButtonElementOptions[] = {
    { (char *) "-default", TK_OPTION_ANY, 
        Tk_Offset(ToolButtonElement, defaultStateObj), (char *) "normal" },
    {NULL}
};

static void ToolButtonElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    GTTK_WIDGET_CACHE_DEFINITION;
    gint focus_width;
    gint focus_pad;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *widget = Gttk_GetButton(wc);
    GTTK_ENSURE_WIDGET_OK;
    Gttk_gtk_widget_style_get(widget,
           "focus-line-width", &focus_width,
           "focus-padding",    &focus_pad, NULL);
    *paddingPtr = Ttk_UniformPadding(PushButtonUniformPadding +
                                     focus_width + focus_pad);
}

static void ToolButtonElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    GTTK_GTK_DRAWABLE_DEFINITIONS;
    gint width, height;
    gint x, y;
    gint border_width;
    GtkBorder default_border = { 1, 1, 1, 1 };
    GtkBorder default_outside_border = { 0, 0, 0, 0 };
    GtkBorder *tmp_border, *tmp_outside_border;
    GtkReliefStyle button_relief = GTK_RELIEF_NORMAL;
    gboolean interior_focus;
    gint focus_width;
    gint focus_pad;
    GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE;
    GtkWidget *toolbar = Gttk_GetToolBar(wc);
    GtkWidget *widget  = Gttk_GetToolButton(wc);
    ToolButtonElement *bd = (ToolButtonElement *) elementRecord;
    GtkButton *button  = (GtkButton *) widget;
    GTTK_ENSURE_WIDGET_OK;
    GTTK_STYLE_FROM_WIDGET;
    GTTK_DRAWABLE_FROM_WIDGET;
    GTTK_DEFAULT_BACKGROUND;
    Gttk_StateShadowTableLookup(NULL, state, gtkState, gtkShadow,
            GTTK_SECTION_BUTTONS | GTTK_SECTION_ALL);
    GTTK_WIDGET_SET_FOCUS(widget);
    GTTK_WIDGET_SET_DEFAULT(widget, bd->defaultStateObj);
    // Gttk_StateInfo(state, gtkState, gtkShadow, tkwin, widget);
    /* Make the button relief to what suggested by the toolbar... */
    Gttk_gtk_widget_style_get(toolbar, "button-relief", &button_relief,NULL);
    if (button_relief != Gttk_gtk_button_get_relief(button)) {
      Gttk_gtk_button_set_relief(button, button_relief);
    }

    /*
     * The following was taken from GTK+ button drawing code.
     */
    border_width = ((GtkContainer*) widget)->border_width;
    Gttk_gtk_widget_style_get(widget,
           "default-border",         &tmp_border,
           "default-outside-border", &tmp_outside_border,
           "interior-focus",         &interior_focus, NULL);
    if (tmp_border) {
      default_border = *tmp_border;
      Gttk_gtk_border_free(tmp_border);
    }
    if (tmp_outside_border) {
      default_outside_border = *tmp_outside_border;
      Gttk_gtk_border_free(tmp_outside_border);
    }

    Gttk_gtk_widget_style_get(widget,
           "focus-line-width", &focus_width,
           "focus-padding",    &focus_pad, NULL); 
        
    x = b.x + border_width;
    y = b.y + border_width;
    width  = b.width  - border_width * 2;
    height = b.height - border_width * 2;

    if (has_default && button->relief == GTK_RELIEF_NORMAL) {
      Gttk_gtk_paint_box(style, gdkDrawable, GTK_STATE_NORMAL, GTK_SHADOW_IN,
              NULL, widget, "buttondefault", x, y, width, height);
      x += default_border.left;
      y += default_border.top;
      width -= default_border.left + default_border.right;
      height -= default_border.top + default_border.bottom;
    } else {
      x += default_outside_border.left;
      y += default_outside_border.top;
      width -= default_outside_border.left + default_outside_border.right;
      height -= default_outside_border.top + default_outside_border.bottom;
    }
     
    if (!interior_focus && (state & TTK_STATE_FOCUS)) {
      x += focus_width + focus_pad;
      y += focus_width + focus_pad;
      width -= 2 * (focus_width + focus_pad);
      height -= 2 * (focus_width + focus_pad);
    }

    if (button->relief != GTK_RELIEF_NONE || (state & TTK_STATE_PRESSED) ||
        (state & TTK_STATE_ACTIVE) ) {
      Gttk_gtk_paint_box(style, gdkDrawable, gtkState, gtkShadow, NULL,
             widget, "button", x, y, width, height);
    }
     
    if (state & TTK_STATE_FOCUS) {
      gint child_displacement_x;
      gint child_displacement_y;
      gboolean displace_focus;
      
      Gttk_gtk_widget_style_get(widget,
              "child-displacement-y", &child_displacement_y,
              "child-displacement-x", &child_displacement_x,
              "displace-focus",       &displace_focus, NULL);

      if (interior_focus) {
        x += widget->style->xthickness + focus_pad;
        y += widget->style->ythickness + focus_pad;
        width -= 2 * (widget->style->xthickness + focus_pad);
        height -=  2 * (widget->style->ythickness + focus_pad);
      } else {
        x -= focus_width + focus_pad;
        y -= focus_width + focus_pad;
        width += 2 * (focus_width + focus_pad);
        height += 2 * (focus_width + focus_pad);
      }

      if ((state & TTK_STATE_PRESSED) && displace_focus) {
        x += child_displacement_x;
        y += child_displacement_y;
      }

      Gttk_gtk_paint_focus(style, gdkDrawable, gtkState, NULL, widget,
              "button", x, y, width, height);
    }

    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                  0, 0, b.width, b.height, b.x, b.y);
    GTTK_CLEANUP_GTK_DRAWABLE;
}

static Ttk_ElementSpec ToolButtonElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(ToolButtonElement),
    ToolButtonElementOptions,
    ToolButtonElementGeometry,
    ToolButtonElementDraw
};

/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

TTK_BEGIN_LAYOUT(ToolbuttonLayout)
    TTK_GROUP("Toolbutton.border", TTK_FILL_BOTH,
	    TTK_GROUP("Toolbutton.padding", TTK_FILL_BOTH,
		TTK_NODE("Toolbutton.label", TTK_FILL_BOTH)))
TTK_END_LAYOUT

int Gttk_Init_ToolButton(Tcl_Interp *interp,
                       Gttk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Toolbutton.border",
            &ToolButtonElementSpec, (void *) wc[0]);

    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr, "TToolbutton", ToolbuttonLayout);
    return TCL_OK;
}; /* Gttk_Init_ToolButton */
