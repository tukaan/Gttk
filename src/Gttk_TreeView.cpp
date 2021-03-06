// Copyright (c) 2004-2008 Georgios Petasis (petasis@iit.demokritos.gr)
// Copyright (c) 2022 rdbende (rdbende@gmail.com)

#include "Gttk_Utilities.h"
#include "Gttk_TkHeaders.h"
#include "Gttk_WidgetDefaults.h"

#define ITEM_LAYOUT		"Item"
#define CELL_LAYOUT		"Cell"
#define HEADING_LAYOUT		"Heading"
#define ROW_LAYOUT		"Row"

/*------------------------------------------------------------------------
 * +++ TreeView Field Element.
 */
 
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable treeview_field_statemap[] =
{
#ifdef GTTK_GTK_VERSION_3
    {QStyle::Style_Enabled|QStyle::Style_Down,      TTK_STATE_PRESSED, 0},
    {QStyle::Style_Enabled,                         TTK_STATE_ACTIVE, 0},
    {QStyle::Style_Enabled,                         0, 0}
#endif /* GTTK_GTK_VERSION_3 */
#ifdef GTTK_GTK_VERSION_4
    {QStyle::State_Enabled|QStyle::State_Sunken,    TTK_STATE_PRESSED, 0},
    {QStyle::State_Enabled,                         TTK_STATE_ACTIVE, 0},
    {QStyle::State_Enabled,                         0, 0}
#endif /* GTTK_GTK_VERSION_4 */
};

typedef struct {
} TreeViewFieldElement;


static Ttk_ElementOptionSpec TreeViewFieldElementOptions[] = {
    {NULL}
};

static void TreeViewFieldElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    if (!Gttk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    *paddingPtr = Ttk_UniformPadding(0);
}

static void TreeViewFieldElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    if (!Gttk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    NULL_PROXY_WIDGET(Gttk_QWidget_Widget);
    Tcl_MutexLock(&GttkMutex);
    QPixmap      pixmap(b.width, b.height);
    QPainter     painter(&pixmap);
    GTTK_PAINT_BACKGROUND_BASE(b.width, b.height);
    GTTK_SET_FOCUS(state);
#ifdef GTTK_GTK_VERSION_3
    QStyle::SFlags sflags =Gttk_StateTableLookup(treeview_field_statemap, state);
    sflags |= QStyle::Style_Horizontal;
    wc->Gttk_Style->drawPrimitive(QStyle::PE_GroupBoxFrame, &painter,
          QRect(0, 0, b.width, b.height), qApp->palette().active(), sflags,
          QStyleOption(/*lineWidth*/1, /*midLineWidth*/0,
                       /*frameShape*/QFrame::StyledPanel,
                       /*frameShadow*/QFrame::Sunken));
    wc->Gttk_Style->drawPrimitive(QStyle::PE_PanelLineEdit, &painter,
          QRect(0, 0, b.width, b.height), qApp->palette().active(), sflags,
          QStyleOption(1,1));
#endif /* GTTK_GTK_VERSION_3 */
#ifdef GTTK_GTK_VERSION_4
    QStyleOptionFrame option;
    option.rect = QRect(0, 0, b.width, b.height);
    option.lineWidth = 1;
    option.state |= 
      (QStyle::StateFlag) Gttk_StateTableLookup(treeview_field_statemap, state);
    wc->Gttk_Style->drawPrimitive(QStyle::PE_FrameGroupBox, &option,
                                    &painter);
#endif /* GTTK_GTK_VERSION_4 */
    GTTK_CLEAR_FOCUS(state);
    // printf("x=%d, y=%d, w=%d, h=%d\n", b.x, b.y, b.width, b.height);
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                                    0, 0, b.width, b.height, b.x, b.y);
    Tcl_MutexUnlock(&GttkMutex);
}

static Ttk_ElementSpec TreeViewFieldElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(TreeViewFieldElement),
    TreeViewFieldElementOptions,
    TreeViewFieldElementGeometry,
    TreeViewFieldElementDraw
};

/*------------------------------------------------------------------------
 * +++ TreeView Client Element.
 */
 
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable treeview_client_statemap[] =
{
#ifdef GTTK_GTK_VERSION_3
    {QStyle::Style_Enabled|QStyle::Style_Down,      TTK_STATE_PRESSED, 0},
    {QStyle::Style_Enabled,                         TTK_STATE_ACTIVE, 0},
    {QStyle::Style_Enabled,                         0, 0}
#endif /* GTTK_GTK_VERSION_3 */
#ifdef GTTK_GTK_VERSION_4
    {QStyle::State_Enabled|QStyle::State_Sunken,    TTK_STATE_PRESSED, 0},
    {QStyle::State_Enabled,                         TTK_STATE_ACTIVE, 0},
    {QStyle::State_Enabled,                         0, 0}
#endif /* GTTK_GTK_VERSION_4 */
};

typedef struct {
} TreeViewClientElement;


static Ttk_ElementOptionSpec TreeViewClientElementOptions[] = {
    {NULL}
};

static void TreeViewClientElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    if (!Gttk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    *paddingPtr = Ttk_UniformPadding(0);
}

static void TreeViewClientElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    if (!Gttk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    NULL_PROXY_WIDGET(Gttk_QWidget_Widget);
    Tcl_MutexLock(&GttkMutex);
    QPixmap      pixmap(b.width, b.height);
    QPainter     painter(&pixmap);
    GTTK_PAINT_BACKGROUND_BASE(b.width, b.height);
#if 0
#ifdef GTTK_GTK_VERSION_3
    QStyle::SFlags sflags =
                   Gttk_StateTableLookup(treeview_client_statemap, state);
    sflags |= QStyle::Style_Horizontal;
    wc->Gttk_Style->drawPrimitive(QStyle::PE_GroupBoxFrame, &painter,
          QRect(0, 0, b.width, b.height), qApp->palette().active(), sflags,
          QStyleOption(/*lineWidth*/1, /*midLineWidth*/0,
                       /*frameShape*/QFrame::StyledPanel,
                       /*frameShadow*/QFrame::Sunken));
#endif /* GTTK_GTK_VERSION_3 */
#ifdef GTTK_GTK_VERSION_4
    QStyleOptionFrame option;
    option.rect = QRect(0, 0, b.width, b.height);
    option.lineWidth = 1;
    option.state |= (QStyle::StateFlag) 
                    Gttk_StateTableLookup(treeview_client_statemap, state);
    wc->Gttk_Style->drawPrimitive(QStyle::PE_FrameGroupBox, &option,
                                    &painter);
#endif /* GTTK_GTK_VERSION_4 */
#endif
    // printf("x=%d, y=%d, w=%d, h=%d\n", b.x, b.y, b.width, b.height);
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                                    0, 0, b.width, b.height, b.x, b.y);
    Tcl_MutexUnlock(&GttkMutex);
}

static Ttk_ElementSpec TreeViewClientElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(TreeViewClientElement),
    TreeViewClientElementOptions,
    TreeViewClientElementGeometry,
    TreeViewClientElementDraw
};

/*------------------------------------------------------------------------
 * +++ TreeView Header Border Element.
 */
 
/*
 * Map between Tk/Tile & Gtk/GNOME state flags.
 */
static Ttk_StateTable treeheading_border_statemap[] =
{
#ifdef GTTK_GTK_VERSION_3
    {QStyle::Style_Enabled|QStyle::Style_Down,      TTK_STATE_PRESSED, 0},
    {QStyle::Style_Enabled,                         TTK_STATE_ACTIVE, 0},
    {QStyle::Style_Enabled,                         0, 0}
#endif /* GTTK_GTK_VERSION_3 */
#ifdef GTTK_GTK_VERSION_4
    {QStyle::State_Enabled|QStyle::State_Sunken,    TTK_STATE_PRESSED, 0},
    {QStyle::State_Enabled,                         TTK_STATE_ACTIVE, 0},
    {QStyle::State_Enabled,                         0, 0}
#endif /* GTTK_GTK_VERSION_4 */
};

typedef struct {
} TreeHeadingBorderElement;


static Ttk_ElementOptionSpec TreeHeadingBorderElementOptions[] = {
    {NULL}
};

static void TreeHeadingBorderElementGeometry(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    int *widthPtr, int *heightPtr, Ttk_Padding *paddingPtr)
{
    if (!Gttk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    *paddingPtr = Ttk_UniformPadding(TreeViewUniformPadding);
}

static void TreeHeadingBorderElementDraw(
    void *clientData, void *elementRecord, Tk_Window tkwin,
    Drawable d, Ttk_Box b, unsigned state)
{
    if (!Gttk_GtkInitialised()) NO_GTK_STYLE_ENGINE;
    NULL_PROXY_WIDGET(Gttk_QWidget_Widget);
    Tcl_MutexLock(&GttkMutex);
    QPixmap      pixmap(b.width, b.height);
    QPainter     painter(&pixmap);
    GTTK_PAINT_BACKGROUND(b.width, b.height);
#ifdef GTTK_GTK_VERSION_3
    QStyle::SFlags sflags = Gttk_StateTableLookup(treeheading_border_statemap,
                                                 state);
    sflags |= QStyle::Style_Horizontal;
    wc->Gttk_Style->drawPrimitive(QStyle::PE_HeaderSection, &painter,
          QRect(0, 0, b.width, b.height), qApp->palette().active(), sflags);
#endif /* GTTK_GTK_VERSION_3 */
#ifdef GTTK_GTK_VERSION_4
    QStyleOptionHeader option;
    option.rect = QRect(0, 0, b.width, b.height);
    option.state |= 
      (QStyle::StateFlag) Gttk_StateTableLookup(treeview_client_statemap, state);
    wc->Gttk_Style->drawControl(QStyle::CE_HeaderSection, &option,
                                  &painter);
#endif /* GTTK_GTK_VERSION_4 */
    // printf("x=%d, y=%d, w=%d, h=%d\n", b.x, b.y, b.width, b.height);
    Gttk_CopyGtkPixmapOnToDrawable(gdkDrawable, d, tkwin,
                                    0, 0, b.width, b.height, b.x, b.y);
    Tcl_MutexUnlock(&GttkMutex);
}

static Ttk_ElementSpec TreeHeadingBorderElementSpec = {
    TK_STYLE_VERSION_2,
    sizeof(TreeHeadingBorderElement),
    TreeHeadingBorderElementOptions,
    TreeHeadingBorderElementGeometry,
    TreeHeadingBorderElementDraw
};


/*------------------------------------------------------------------------
 * +++ Widget layout.
 */

#if 0
TTK_BEGIN_LAYOUT(TreeviewLayout)
    TTK_GROUP("Treeview.field", TTK_FILL_BOTH|TTK_BORDER,
	TTK_GROUP("Treeview.padding", TTK_FILL_BOTH,
	    TTK_NODE("Treeview.client", TTK_FILL_BOTH)))
TTK_END_LAYOUT
#endif

TTK_BEGIN_LAYOUT(ItemLayout)
    TTK_GROUP("Treeitem.padding", TTK_FILL_BOTH,
	TTK_NODE("Treeitem.indicator", TTK_PACK_LEFT)
	TTK_NODE("Treeitem.image", TTK_PACK_LEFT)
	TTK_GROUP("Treeitem.focus", TTK_PACK_LEFT,
	    TTK_NODE("Treeitem.text", TTK_PACK_LEFT)))
TTK_END_LAYOUT

TTK_BEGIN_LAYOUT(CellLayout)
    TTK_GROUP("Treedata.padding", TTK_FILL_BOTH,
	TTK_NODE("Treeitem.label", TTK_FILL_BOTH))
TTK_END_LAYOUT

TTK_BEGIN_LAYOUT(HeadingLayout)
    TTK_NODE("Treeheading.cell", TTK_FILL_BOTH)
    TTK_GROUP("Treeheading.border", TTK_FILL_BOTH,
	TTK_NODE("Treeheading.image", TTK_PACK_RIGHT)
	TTK_NODE("Treeheading.text", 0))
TTK_END_LAYOUT

TTK_BEGIN_LAYOUT(RowLayout)
    TTK_NODE("Treeitem.row", TTK_FILL_BOTH)
TTK_END_LAYOUT

int Gttk_Init_TreeView(Tcl_Interp *interp,
                       Gttk_WidgetCache **wc, Ttk_Theme themePtr)
{
    /*
     * Register elements:
     */
    Ttk_RegisterElement(interp, themePtr, "Treeview.field",
            &TreeViewFieldElementSpec, (void *) wc[0]);
    //Ttk_RegisterElement(interp, themePtr, "Treeview.client",
    //        &TreeViewClientElementSpec, (void *) wc[0]);
    Ttk_RegisterElement(interp, themePtr, "Treeheading.border",
            &TreeHeadingBorderElementSpec, (void *) wc[0]);
    
    /*
     * Register layouts:
     */
    Ttk_RegisterLayout(themePtr, ITEM_LAYOUT, ItemLayout);
    Ttk_RegisterLayout(themePtr, CELL_LAYOUT, CellLayout);
    Ttk_RegisterLayout(themePtr, HEADING_LAYOUT, HeadingLayout);
    Ttk_RegisterLayout(themePtr, ROW_LAYOUT, RowLayout);

    return TCL_OK;
}; /* Gttk_Init_TreeView */
