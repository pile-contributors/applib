
# enable/disable cmake debug messages related to this pile
set (APPLIB_DEBUG_MSG ON)

# make sure support code is present; no harm
# in including it twice; the user, however, should have used
# pileInclude() from pile_support.cmake module.
include(pile_support)

# initialize this module
macro    (applibInit
          ref_cnt_use_mode)

    # default name
    if (NOT APPLIB_INIT_NAME)
        set(APPLIB_INIT_NAME "AppLib")
    endif ()

    # compose the list of headers and sources
    set(APPLIB_HEADERS
        "applib-util.h"
        "applib.h")
    set(APPLIB_SOURCES
        "applib.cc")
    set(APPLIB_QT_MODS
        "Core"
        "Widgets"
        "Gui")

    pileSetSources(
        "${APPLIB_INIT_NAME}"
        "${APPLIB_HEADERS}"
        "${APPLIB_SOURCES}")

    pileSetCommon(
        "${APPLIB_INIT_NAME}"
        "0;0;1;d"
        "ON"
        "${ref_cnt_use_mode}"
        "Translate"
        "category1"
        "tag1;tag2")

endmacro ()
