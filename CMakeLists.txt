if (NOT APPLIB_BUILD_MODE)
    set (APPLIB_BUILD_MODE STATIC)
endif ()

include(pile_support)
pileInclude (AppLib)
applibInit(${APPLIB_BUILD_MODE})
