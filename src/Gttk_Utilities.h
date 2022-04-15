// Copyright (c) 2004-2008 Georgios Petasis (petasis@iit.demokritos.gr)
// Copyright (c) 2022 rdbende (rdbende@gmail.com)

#include "Gttk_GtkHeaders.h"
#include "Gttk_Elements.h"
#include <string.h>

#define NO_GTK_STYLE_ENGINE {/*printf("NULL qApp\n");fflush(NULL);*/return;}

#define GTTK_ENSURE_GTK_STYLE_ENGINE_ACTIVE \
  if (!Gttk_GtkInitialised()) return;

#define GTTK_ENSURE_WIDGET_OK \
  if (!widget) return;

#define GTTK_ATTACH_STYLE_TO_WIDGET \
  style = Gttk_gtk_style_attach(style, widget->window);

#define GTTK_WIDGET_CACHE_DEFINITION \
  Gttk_WidgetCache *wc = (Gttk_WidgetCache *) clientData;

#define GTTK_ORIENTATION_DEFINITION \
  int orientation = wc->orientation;

#define GTTK_GTK_DRAWABLE_DEFINITIONS \
  Gttk_WidgetCache *wc = (Gttk_WidgetCache *) clientData; \
  GdkPixmap    *gdkDrawable = NULL; \
  GtkStyle     *style       = NULL; \
  GtkStateType  gtkState    = GTK_STATE_NORMAL; \
  GtkShadowType gtkShadow   = GTK_SHADOW_NONE;

#define GTTK_SETUP_GTK_DRAWABLE \
  GTTK_SETUP_GTK_DRAWABLE_PIXMAP_SIZE(b.width, b.height)

#define GTTK_SETUP_GTK_DRAWABLE_PIXMAP_SIZE(pw, ph) \
  if (!wc) return; \
  style = Gttk_GetGtkWindowStyle(wc->gtkWindow); \
  if (!style) return; \
  gdkDrawable = Gttk_gdk_pixmap_new(wc->gtkWindow->window, pw, ph, -1); \
  style = Gttk_GetGtkWindowStyle(wc->gtkWindow);

#define GTTK_STYLE_BACKGROUND_DEFAULT \
  if (wc && wc->gtkWindow) style = Gttk_GetGtkWindowStyle(wc->gtkWindow); \
  if (!style) return;

#define GTTK_STYLE_FROM_WIDGET \
  style = Gttk_GetGtkWindowStyle(widget); \
  if (!style) style = Gttk_GetGtkWindowStyle(wc->gtkWindow); \
  if (!style) return;

#define GTTK_DRAWABLE_FROM_WIDGET_SIZE(pw, ph) \
  gdkDrawable = Gttk_gdk_pixmap_new(widget->window, pw, ph, -1);

#define GTTK_DRAWABLE_FROM_WIDGET \
  GTTK_DRAWABLE_FROM_WIDGET_SIZE(b.width, b.height)

// #define GTTK_DRAWABLE_FROM_WIDGET \
//   gdkDrawable = gdk_pixmap_foreign_new(Tk_WindowId(tkwin)); \
//   GdkColormap gdkColormap = gdk_x11_colormap_foreign_new(gdkx_visual_get(Tk_Visual(tkwin)), Tk_Colormap(tkwin)); \
//   gdk_drawable_set_colormap(gdkDrawable, gdkColormap);

#define GTTK_DEFAULT_BACKGROUND_SIZE(pw, ph) \
  Gttk_gtk_style_apply_default_background(style, gdkDrawable, TRUE, \
               gtkState, NULL, 0, 0, pw, ph);

#define GTTK_DEFAULT_BACKGROUND \
  GTTK_DEFAULT_BACKGROUND_SIZE(b.width, b.height)

#define GTTK_CLEANUP_GTK_DRAWABLE \
  if (gdkDrawable) Gttk_g_object_unref(gdkDrawable);

#define GTTK_SETUP_STATE_SHADOW(statemap, shadowmap) \
    gtkState  = (GtkStateType) \
       Gttk_StateTableLookup(statemap,  state); \
    gtkShadow = (GtkShadowType) \
       Gttk_StateTableLookup(shadowmap, state);

#define GTTK_SETUP_WIDGET_SIZE(width, height) \
  Gttk_gtk_widget_set_size_request(widget, width, height);

#define GTTK_GET_WIDGET_SIZE(widthPtr, heightPtr)  \
  if (widget) { \
    GtkRequisition size; \
    Gttk_gtk_widget_size_request(widget, &size); \
    widthPtr  = size.width; \
    heightPtr = size.height; \
  }

#define GTTK_WIDGET_SETUP_DEFAULT(obj) \
  int defaultState  = TTK_BUTTON_DEFAULT_DISABLED; \
  int has_default = (defaultState == TTK_BUTTON_DEFAULT_ACTIVE); \
  /*Ttk_GetButtonDefaultStateFromObj(NULL, obj, &defaultState);*/

#ifdef GTTK_LOAD_GTK_DYNAMICALLY

#define GTTK_WIDGET_SET_FOCUS(widget)

#define GTTK_WIDGET_SET_DEFAULT(widget, obj) \
  int defaultState  = TTK_BUTTON_DEFAULT_DISABLED; \
  int has_default = (defaultState == TTK_BUTTON_DEFAULT_ACTIVE);

#else /* GTTK_LOAD_GTK_DYNAMICALLY */

#define GTTK_WIDGET_SET_FOCUS(widget) \
  if (state & TTK_STATE_FOCUS) { \
    GTK_WIDGET_SET_FLAGS(widget,   GTK_HAS_FOCUS); \
  } else { \
    GTK_WIDGET_UNSET_FLAGS(widget, GTK_HAS_FOCUS); \
  }

#define GTTK_WIDGET_SET_DEFAULT(widget, obj) \
  int defaultState  = TTK_BUTTON_DEFAULT_DISABLED; \
  int has_default = (defaultState == TTK_BUTTON_DEFAULT_ACTIVE); \
  /*Ttk_GetButtonDefaultStateFromObj(NULL, obj, &defaultState);*/ \
  if (has_default) { \
    GTK_WIDGET_SET_FLAGS(widget,   GTK_HAS_DEFAULT); \
  } else { \
    GTK_WIDGET_UNSET_FLAGS(widget, GTK_HAS_DEFAULT); \
  } 
#endif /* GTTK_LOAD_GTK_DYNAMICALLY */

#define GTTK_DEBUG_PRINT_BOX \
  printf("x=%d, y=%d, w=%d, h=%d\n", b.x, b.y, b.width, b.height); \
  fflush(0);

#define GTTK_DEBUG_PRINT_TK_WIDGET \
  printf("Widget: %s,p=%p\n", Tk_PathName(tkwin), tkwin); \
  fflush(0);

#define GTTK_GTKBORDER_TO_PADDING(border) \
  Ttk_MakePadding(border.left, border.top, border.right, border.bottom)

TCL_DECLARE_MUTEX(GttkMutex);

/* Global Symbols */

/* Helper Functions */
extern int        Gttk_GtkInitialised(void);
extern GtkWidget *Gttk_GetGtkWindow(void);
extern GtkStyle  *Gttk_GetGtkWindowStyle(GtkWidget *gtkWindow);
extern GtkStyle  *Gttk_GetGtkStyle(void);

extern unsigned int Gttk_StateTableLookup(Ttk_StateTable *, unsigned int);
extern void Gttk_CopyGtkPixmapOnToDrawable(GdkPixmap *, Drawable, Tk_Window,
                                            int, int, int, int, int, int);
extern void Gttk_StateInfo(int, GtkStateType,
                    GtkShadowType, Tk_Window, GtkWidget *widget = NULL);

extern GtkWidget *Gttk_GetArrow(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetButton(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetCheckButton(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetRadioButton(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetToolBar(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetToolButton(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetFrame(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetEntry(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetCombobox(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetComboboxEntry(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetHScrollBar(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetVScrollBar(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetScrollBar(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetHScale(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetVScale(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetScale(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetHProgressBar(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetVProgressBar(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetProgressBar(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetStatusBar(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetPaned(Gttk_WidgetCache* wc);
extern GtkWidget *Gttk_GetNotebook(Gttk_WidgetCache* wc);
#if 0
extern void Gttk_StoreStyleNameLowers(Gttk_WidgetCache *wc);
extern bool Gttk_ThemeIs(Gttk_WidgetCache *wc, const char* name);
extern void Gttk_SetFocus(bool focus);
#endif

extern unsigned int Gttk_StateShadowTableLookup(Gttk_StateTable*,
       unsigned int, GtkStateType&, GtkShadowType&,
       unsigned int section = GTTK_SECTION_ALL);
extern double Gttk_ValueFromSlider(Gttk_WidgetCache *wc, Tk_Window tkwin,
                               Ttk_Box b);
