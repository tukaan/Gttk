CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )

## ===========================================================================
##  Project Information
## ===========================================================================
PROJECT ( Gttk )
## Package version information.
SET ( PKG_NAME          ${PROJECT_NAME} )
SET ( PKG_MAJOR_VERSION 0 )
SET ( PKG_MINOR_VERSION 7 )
SET ( PKG_BUILD_VERSION 0 )
SET ( PKG_VERSION
     "${PKG_MAJOR_VERSION}.${PKG_MINOR_VERSION}" )
SET ( PKG_NAME_VERSION  ${PKG_NAME}-${PKG_VERSION} )
SET ( PKG_VENDOR        "rdbende" )
SET ( PKG_HOME_DIR      ${PROJECT_SOURCE_DIR} )

SET ( LOAD_GTK_DYNAMICALLY ON )
SET ( NO_MSVCRT            ON  )
SET ( USE_GNOME            OFF )
SET ( VERBOSE              ON  )

IF ( VERBOSE )
# SET ( CMAKE_VERBOSE_MAKEFILE ON )
  SET ( GTK2_LOGGING_ENABLED   ON )
ENDIF ( VERBOSE )

SET ( PKG_SOURCES 
                  src/Gttk_TreeView.cpp
                  src/Gttk_Separator.cpp
                  src/Gttk_Arrows.cpp )

SET ( PKG_SOURCES src/Gttk_Init.cpp
                  src/Gttk_Symbols.cpp
                  src/Gttk_GtkApp.cpp
                  src/Gttk_Utilities.cpp
                  src/Gttk_Background.cpp
                  src/Gttk_Button.cpp
                  src/Gttk_CheckButton.cpp
                  src/Gttk_RadioButton.cpp
                  src/Gttk_ToolButton.cpp
                  src/Gttk_Labelframe.cpp
                  src/Gttk_Entry.cpp
                  src/Gttk_Menubutton.cpp
                  src/Gttk_Scrollbar.cpp
                  src/Gttk_Scale.cpp
                  src/Gttk_Progress.cpp
                  src/Gttk_SizeGrip.cpp
                  src/Gttk_Paned.cpp
                  src/Gttk_Notebook.cpp
                  src/Gttk_Combobox.cpp
)

MESSAGE ( STATUS "===========================================================" )
MESSAGE ( STATUS " Welcome to the ${PKG_NAME} ${PKG_VERSION} build system!" )
MESSAGE ( STATUS "  * Selected generator:  ${CMAKE_GENERATOR}" )
MESSAGE ( STATUS "  * Operating System ID: "
        ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_VERSION}-${CMAKE_SYSTEM_PROCESSOR} )
MESSAGE ( STATUS "  * Installation Directory: " ${CMAKE_INSTALL_PREFIX} )
MESSAGE ( STATUS "===========================================================" )

## ===========================================================================
##  Locate Tcl/Tk
## ===========================================================================
MESSAGE ( STATUS "Searching for Tcl/Tk..." )
FIND_PACKAGE ( TCL REQUIRED )
FIND_PACKAGE ( TclStub REQUIRED )
MESSAGE ( STATUS "  TCL_TCLSH:               " ${TCL_TCLSH} )
MESSAGE ( STATUS "  TCL_INCLUDE_PATH:        " ${TCL_INCLUDE_PATH} )
MESSAGE ( STATUS "  TCL_STUB_LIBRARY:        " ${TCL_STUB_LIBRARY} )
MESSAGE ( STATUS "  TCL_LIBRARY:             " ${TCL_LIBRARY} )
MESSAGE ( STATUS "  TK_WISH:                 " ${TK_WISH} )
MESSAGE ( STATUS "  TK_INCLUDE_PATH:         " ${TK_INCLUDE_PATH} )
MESSAGE ( STATUS "  TK_STUB_LIBRARY:         " ${TK_STUB_LIBRARY} )
MESSAGE ( STATUS "  TTK_STUB_LIBRARY:        " ${TTK_STUB_LIBRARY} )
MESSAGE ( STATUS "  TK_LIBRARY:              " ${TK_LIBRARY} )
# STRING  ( REPLACE stub "" TCL_LIBRARY_STATIC   ${TCL_STUB_LIBRARY} )
# STRING  ( REPLACE stub "" TK_LIBRARY_STATIC    ${TK_STUB_LIBRARY} )
# MESSAGE ( STATUS "  TCL_LIBRARY_STATIC:      " ${TCL_LIBRARY_STATIC} )
# MESSAGE ( STATUS "  TK_LIBRARY_STATIC:       " ${TK_LIBRARY_STATIC} )

IF ( LOAD_GTK_DYNAMICALLY )
  MESSAGE ( STATUS "GTK+ will be loaded dynamically!" )
  ADD_DEFINITIONS   ( -DGTTK_LOAD_GTK_DYNAMICALLY )
ENDIF ( LOAD_GTK_DYNAMICALLY )
IF ( CMAKE_BUILD_TYPE STREQUAL Debug )
  ADD_DEFINITIONS   ( -DGTTK_SYNCHRONIZE )
ENDIF ( CMAKE_BUILD_TYPE STREQUAL Debug )
ADD_DEFINITIONS     ( -DGTTK_INSTALL_XERROR_HANDLER )
ADD_DEFINITIONS     ( -DUSE_TCL_STUBS )
ADD_DEFINITIONS     ( -DUSE_TK_STUBS )
ADD_DEFINITIONS     ( -DUSE_TTK_STUBS )
ADD_DEFINITIONS     ( -DPACKAGE_NAME="${PKG_NAME}" )
ADD_DEFINITIONS     ( -DPACKAGE_VERSION="${PKG_VERSION}" )
INCLUDE_DIRECTORIES ( ${TCL_INCLUDE_PATH} )
INCLUDE_DIRECTORIES ( ${TK_INCLUDE_PATH} )
INCLUDE_DIRECTORIES ( ${TK_INCLUDE_PATH}/tk-private/generic/ttk )
INCLUDE_DIRECTORIES ( ${PROJECT_SOURCE_DIR}/src )

## ===========================================================================
##  Locate GNOME2...
## ===========================================================================
IF ( USE_GNOME )
  MESSAGE ( STATUS "Searching for GNOME..." )
  INCLUDE ( cmake/Modules/FindGNOME2.cmake OPTIONAL )
  IF ( GNOME_FOUND )
      MESSAGE ( STATUS "  GNOME Found!" )
      ADD_DEFINITIONS     ( -DGTTK_ENABLE_GNOME )
      INCLUDE_DIRECTORIES ( ${GNOME_INCLUDE_DIR} )
      LINK_LIBRARIES      ( ${GNOME_LIBRARIES} )
  ENDIF ( GNOME_FOUND )
  MESSAGE ( STATUS "  GNOME_INCLUDE_DIR:" )
  FOREACH ( path ${GNOME_INCLUDE_DIR} )
    MESSAGE ( STATUS "   + ${path}" )
  ENDFOREACH ( path )
  MESSAGE ( STATUS "  GNOME_LIBRARIES:" )
  FOREACH ( path ${GNOME_LIBRARIES} )
    MESSAGE ( STATUS "   + ${path}" )
  ENDFOREACH ( path )
ENDIF ( USE_GNOME )

## ===========================================================================
##  Locate GTK...
## ===========================================================================
MESSAGE ( STATUS "Searching for GTK2..." )
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK "gtk+-2.0")
if (GTK_FOUND)
  LINK_LIBRARIES( ${GTK_LIBRARIES})
  add_definitions(${GTK_CFLAGS} ${GTK_CFLAGS_OTHER})

MESSAGE ( STATUS "  GTK 2.x Found!" )
endif()

## ===========================================================================
##  Target: Gttk_Symbols.[h|cpp]
## ===========================================================================
ADD_CUSTOM_COMMAND ( OUTPUT ${PROJECT_SOURCE_DIR}/src/Gttk_Symbols.cpp
                            ${PROJECT_SOURCE_DIR}/src/Gttk_Symbols.h
    DEPENDS src/GenerateSymbols.tcl
            src/Gttk_Init.cpp
            src/Gttk_GtkApp.cpp
            src/Gttk_Utilities.h
            src/Gttk_Utilities.cpp
    COMMAND ${TCL_TCLSH} GenerateSymbols.tcl
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src )
SET_SOURCE_FILES_PROPERTIES ( ${PROJECT_SOURCE_DIR}/src/Gttk_Symbols.cpp
                              PROPERTIES GENERATED true )
SET_SOURCE_FILES_PROPERTIES ( ${PROJECT_SOURCE_DIR}/src/Gttk_Symbols.h
                              PROPERTIES GENERATED true )

SET ( PKG_TARGET_LIB_NAME ${PKG_NAME}${PKG_VERSION} )
## ===========================================================================
##  Target: Gttk shared library
## ===========================================================================
MESSAGE ( STATUS "===========================================================" )
MESSAGE ( STATUS "   ++++ Shared Library: ${PKG_TARGET_LIB_NAME} ++++" )
MESSAGE ( STATUS "===========================================================" )
ADD_LIBRARY ( ${PKG_TARGET_LIB_NAME} SHARED ${PKG_SOURCES} )
TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${TCL_STUB_LIBRARY} )
TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${TK_STUB_LIBRARY} )

find_package(X11 REQUIRED)
TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${X11_X11_LIB} )
IF ( TTK_STUB_LIBRARY )
  TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${TTK_STUB_LIBRARY} )
ENDIF ( TTK_STUB_LIBRARY )
# IF ( TCL_LIBRARY_STATIC )
#   TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${TCL_LIBRARY_STATIC} )
# ENDIF ( TCL_LIBRARY_STATIC )
# IF ( TK_LIBRARY_STATIC )
#   TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${TK_LIBRARY_STATIC} )
# ENDIF ( TK_LIBRARY_STATIC )
IF ( LOAD_GTK_DYNAMICALLY )
  IF ( GTK2_GMODULE_LIBRARY )
    TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${GTK2_GMODULE_LIBRARY} )
  ELSE ( GTK2_GMODULE_LIBRARY )
    TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${GTK2_LIBRARIES} )
  ENDIF ( GTK2_GMODULE_LIBRARY )
  IF ( UNIX )
    TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} dl )
  ENDIF ( UNIX )
ELSE ( LOAD_GTK_DYNAMICALLY )
  TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${GTK2_LIBRARIES} )
  IF ( GTK2_gdk_LIBRARY )
    TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${GTK2_gdk_LIBRARY} )
  ENDIF ( GTK2_gdk_LIBRARY )
  IF ( GTK2_gobject_LIBRARY )
    TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} ${GTK2_gobject_LIBRARY} )
  ENDIF ( GTK2_gobject_LIBRARY )
  IF ( UNIX )
    TARGET_LINK_LIBRARIES ( ${PKG_TARGET_LIB_NAME} gdk_pixbuf_xlib-2.0 )
  ENDIF ( UNIX )
ENDIF ( LOAD_GTK_DYNAMICALLY )

file(WRITE src/pkgIndex.tcl 
"package ifneeded ttk::theme::Gttk ${PKG_VERSION}  [list apply { dir {
  package require Ttk

  source [file join $dir Gttk.tcl]
  load [file join $dir lib${PKG_NAME}${PKG_VERSION}[info sharedlibextension]] Gttkpkg
  
  package provide ttk::theme::Gttk 0.7

}} $dir]" )


file(COPY src/pkgIndex.tcl DESTINATION pkg)
file(COPY src/Gttk.tcl DESTINATION pkg)
