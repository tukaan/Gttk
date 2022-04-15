// Copyright (c) 2004-2008 Georgios Petasis (petasis@iit.demokritos.gr)
// Copyright (c) 2022 rdbende (rdbende@gmail.com)

#ifdef GTTK_ENABLE_GNOME
#include <gnome.h>
GnomeProgram *my_app = NULL;
#endif /* GTTK_ENABLE_GNOME */

#include "Gttk_Utilities.h"
#include "Gttk_TkHeaders.h"
#include <string.h>
gboolean   Gttk_GtkInitialisedFlag = FALSE;
static int Gttk_xlib_rgb_initialised = 0;
GtkWidget *Gttk_GtkWindow = NULL;



/* In the following variable we store the XErrorHandler, before we install our
 * own, which filters out some XErrors... */
static int (*Gttk_TkXErrorHandler)(Display *displayPtr,
                                     XErrorEvent *errorPtr);
static int (*Gttk_GtkXErrorHandler)(Display *displayPtr,
                                     XErrorEvent *errorPtr);
static int Gttk_XErrorHandler(Display *displayPtr, XErrorEvent *errorPtr);

static int  Gttk_XEventHandler(ClientData clientdata, XEvent *eventPtr);

/*
 * Gttk_InterpDeleteProc:
 * This function will be called when the interpreter gets deleted. It must free
 * all allocated interp-specific memory segments.
 */
static void Gttk_InterpDeleteProc(ClientData clientData, Tcl_Interp *interp) {
  Gttk_WidgetCache **wc_array = (Gttk_WidgetCache **) clientData;
  Gttk_WidgetCache *wc = wc_array[0];
  if (wc && wc->gtkWindow) {
    /*This will destroy also ALL children!*/
    Gttk_gtk_widget_destroy(wc->gtkWindow);
  }
  // printf("Tk_DeleteGenericHandler: %p\n", interp); fflush(NULL);
  Tk_DeleteGenericHandler(&Gttk_XEventHandler, (ClientData) interp);
  Tcl_Free((char *) wc_array[0]);
  Tcl_Free((char *) wc_array[1]);
  Tcl_Free((char *) wc_array);
}; /* Gttk_InterpDeleteProc */

Gttk_WidgetCache **Gttk_CreateGtkApp(Tcl_Interp *interp) {
  /*
   * The first step is to initialise the gtk library. This must be done once
   * in the application lifetime.
   */
  Tcl_MutexLock(&GttkMutex);
  if (!Gttk_GtkInitialisedFlag) {
#ifdef GTTK_ENABLE_GNOME
    gchar **remaining_args = NULL;
    GOptionEntry option_entries[] = {
      {G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &remaining_args,
       "Special option that collects any remaining arguments for us"},
      { NULL }
    };
    GOptionContext *option_context;
#endif /* GTTK_ENABLE_GNOME */
    int argc = 1;
    char **argv = Gttk_g_new0(char*, 2);
    argv[0] = (char *) Tcl_GetNameOfExecutable();

#ifdef GTTK_INSTALL_XERROR_HANDLER
    Gttk_TkXErrorHandler = XSetErrorHandler(Gttk_XErrorHandler);
#endif /* GTTK_INSTALL_XERROR_HANDLER */

#ifdef GTTK_ENABLE_GNOME
    option_context = Gttk_g_option_context_new("tile-gtk");
    Gttk_g_option_context_add_main_entries(option_context,
                                              option_entries, NULL);
    /* We assume PACKAGE and VERSION are set to the program name and version
     * number respectively. Also, assume that 'option_entries' is a global
     * array of GOptionEntry structures.
     */
    my_app = gnome_program_init(PACKAGE_NAME, PACKAGE_VERSION,
                                LIBGNOMEUI_MODULE, argc, argv,
                                GNOME_PARAM_GOPTION_CONTEXT, option_context,
                                GNOME_PARAM_NONE);
    if (my_app) Gttk_GtkInitialisedFlag = TRUE;
    if (remaining_args != NULL) {
      Gttk_g_strfreev(remaining_args);
      remaining_args = NULL;
    }
#else  /* GTTK_ENABLE_GNOME */
    Gttk_gtk_disable_setlocale();
    Gttk_GtkInitialisedFlag = Gttk_gtk_init_check(&argc, &argv);
#endif /* GTTK_ENABLE_GNOME */
    Gttk_g_free(argv);
    if (!Gttk_GtkInitialisedFlag) {
      Tcl_MutexUnlock(&GttkMutex);
      return NULL;
    }
    /* Initialise Gttk_GtkWindow... */
    Gttk_GtkWindow = Gttk_gtk_window_new(GTK_WINDOW_POPUP);
    Gttk_gtk_widget_realize(Gttk_GtkWindow);
#ifdef GTTK_INSTALL_XERROR_HANDLER
    /*
     * GTK+ xerror handler will terminate the application.
     * Just get rid of that...
     */
    Gttk_GtkXErrorHandler = XSetErrorHandler(Gttk_XErrorHandler);
#endif /* GTTK_INSTALL_XERROR_HANDLER */

#ifdef GTTK_SYNCHRONIZE
    XSynchronize(Tk_Display(Tk_MainWindow(interp)), true);
#endif /* GTTK_SYNCHRONIZE */
  }
  Tcl_MutexUnlock(&GttkMutex);

  /*
   * Allocate the widget cache. We keep a widget cache per interpreter.
   * Each cache is an array of two elements, one for each orientation.
   */

  Gttk_WidgetCache **wc_array = (Gttk_WidgetCache **) 
                           Tcl_Alloc(sizeof(Gttk_WidgetCache*)*2);
  wc_array[0] = (Gttk_WidgetCache *) 
                           Tcl_Alloc(sizeof(Gttk_WidgetCache));
  wc_array[1] = (Gttk_WidgetCache *) 
                           Tcl_Alloc(sizeof(Gttk_WidgetCache));
  Tcl_SetAssocData(interp, "GttkTtk_gtk_widget_cache",
                   &Gttk_InterpDeleteProc, (ClientData) wc_array);
  Gttk_WidgetCache *wc = wc_array[0];
  memset(wc, 0, sizeof(Gttk_WidgetCache));
  /*
   * Initialise the widget cache.
   */
  wc->Gttk_MainInterp  = interp;
  wc->Gttk_tkwin       = Tk_MainWindow(interp);
  if (wc->Gttk_tkwin != NULL && wc->Gttk_MainDisplay == None) {
    Tk_MakeWindowExist(wc->Gttk_tkwin);
    wc->Gttk_MainDisplay = Tk_Display(wc->Gttk_tkwin);
  }
  if (wc->Gttk_MainDisplay == None) {
    Tcl_MutexUnlock(&GttkMutex);
    Tcl_Free((char *) wc_array[0]);
    Tcl_Free((char *) wc_array[1]);
    Tcl_Free((char *) wc_array);
    return NULL;
  }
#ifndef __WIN32__
  wc->gdkDisplay = Gttk_gdk_x11_lookup_xdisplay(wc->Gttk_MainDisplay);
#endif
  if (!wc->gdkDisplay) {
    wc->gdkDisplay = Gttk_gdk_display_get_default();
  }
  wc->gtkWindow = Gttk_gtk_window_new(GTK_WINDOW_POPUP);
  if (wc->gtkWindow) Gttk_gtk_widget_realize(wc->gtkWindow);
  wc->protoLayout = Gttk_gtk_fixed_new();
  Gttk_gtk_container_add((GtkContainer*)(wc->gtkWindow), wc->protoLayout);
  memcpy(wc_array[1], wc_array[0], sizeof(Gttk_WidgetCache));
  wc_array[0]->orientation    = TTK_ORIENT_HORIZONTAL;
  wc_array[1]->orientation    = TTK_ORIENT_VERTICAL;
  wc_array[0]->gtkOrientation = GTK_ORIENTATION_HORIZONTAL;
  wc_array[1]->gtkOrientation = GTK_ORIENTATION_VERTICAL;

#ifndef __WIN32__
  Tcl_MutexLock(&GttkMutex);
  if (!Gttk_xlib_rgb_initialised) {
    Gttk_xlib_rgb_init(wc->Gttk_MainDisplay,Tk_Screen(wc->Gttk_tkwin));
    Gttk_xlib_rgb_initialised = 1;
  }
  Tcl_MutexUnlock(&GttkMutex);
#endif
  return wc_array;
}; /* Gttk_CreateGtkApp */

void Gttk_DestroyGtkApp(void) {
  Tcl_MutexLock(&GttkMutex);
  if (Gttk_GtkInitialisedFlag) {
    // XSetErrorHandler(Gttk_TkXErrorHandler);
    Gttk_GtkInitialisedFlag = FALSE;
  }
  Tcl_MutexUnlock(&GttkMutex);
}; /* Gttk_DestroyGtkApp */

/*
 * Gttk_XErrorHandler:
 * This XError handler just prints some debug information and then calls
 * Tk's XError handler...
 */
static int Gttk_XErrorHandler(Display *displayPtr, XErrorEvent *errorPtr) {
#ifdef GTTK_VERBOSE_XERROR_HANDLER
  char buf[64];
  XGetErrorText (displayPtr, errorPtr->error_code, buf, 63);
  printf("===============================================================\n");
  printf("  Gttk_XErrorHandler:\n");
  printf("    error_code   = %s (%d)\n", buf, errorPtr->error_code);
  printf("    request_code = %d\n", errorPtr->request_code);
  printf("    minor_code   = %d\n", errorPtr->minor_code);
  printf("===============================================================\n");
#endif /* GTTK_VERBOSE_XERROR_HANDLER */
  return Gttk_TkXErrorHandler(displayPtr, errorPtr);
}; /* Gttk_XErrorHandler */

static int Gttk_XEventHandler(ClientData clientData, XEvent *eventPtr) {
  const char *tcl_callback;
  int status;
  if (eventPtr->type != ClientMessage) return 0;
  // Atom Gttk_KIPC_COMM_ATOM = XInternAtom(eventPtr->xclient.display,
  //                                          "KIPC_COMM_ATOM" , false);
  Atom Gttk_KIPC_COMM_ATOM = None;
  if (eventPtr->xclient.message_type != Gttk_KIPC_COMM_ATOM) return 0;
  /* The following data variable contains the type of the KIPC message,
   * As defined in gnomelibs/gnomecore/kipc.h:
   * PaletteChanged      = 0
   * StyleChanged        = 2
   * ToolbarStyleChanged = 6
   */
  switch (eventPtr->xclient.data.l[0]) {
    case 0:   /* PaletteChanged      */
      tcl_callback = "tile::theme::Gttk::gnomePaletteChangeNotification";
      break;
    case 2:   /* StyleChanged        */
    case 6: { /* ToolbarStyleChanged */
      tcl_callback = "tile::theme::Gttk::gnomeStyleChangeNotification";
      break;
    }
    default: {
      return 0;
    }
  }
  Tcl_Interp *interp = (Tcl_Interp *) clientData;
  if (interp == NULL) return 0;
  // printf("Gttk_XEventHandler: %p\n", interp); fflush(NULL);
  /* Notify the tile engine about the change... */
  status = Tcl_Eval(interp, tcl_callback);
  if (status != TCL_OK) Tcl_BackgroundError(interp);
  /* Do not remove True: As many interpreters may have registered this event
   * handler, allow Tk to call all of them! */
  return True;
} /* Gttk_XEventHandler */
