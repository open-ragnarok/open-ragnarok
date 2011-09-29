
#
# rsw_view
#
if( HAVE_roint AND HAVE_sdl_engine AND HAVE_rogl AND HAVE_OpenGL AND HAVE_sdl )
	option( BUILD_EXECUTABLE_RSW_VIEW "build rsw_view executable (extra)" OFF )
else()
	unset( BUILD_EXECUTABLE_RSW_VIEW CACHE )
endif()
if( BUILD_EXECUTABLE_RSW_VIEW )
message( STATUS "Creating target rsw_view..." )
message( STATUS "- type: executable" )
message( WARNING "!!! files are missing from rsw_view, it will fail to build !!!" )# XXX remove this when fixed

# variables
file( GLOB RSW_VIEW_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	)
file( GLOB RSW_VIEW_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
	)
set( INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" ${ROINT_INCLUDE_DIRS} ${SDL_ENGINE_INCLUDE_DIRS} ${ROGL_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${SDL_INCLUDE_DIR} )
set( DEFINITIONS ${ROINT_DEFINITIONS} ${SDL_ENGINE_DEFINITIONS} ${ROGL_DEFINITIONS} )
set( LIBRARIES roint sdl_engine rogl ${OPENGL_LIBRARIES} ${SDL_LIBRARY} )
set( DEPENDENCIES roint sdl_engine rogl )

# target
include_directories( ${INCLUDE_DIRS} )
add_definitions( ${DEFINITIONS} )
add_executable( rsw_view ${RSW_VIEW_SOURCES} ${RSW_VIEW_HEADERS} )
target_link_libraries( rsw_view ${LIBRARIES} )
add_dependencies( rsw_view ${DEPENDENCIES} )

# install
if( INSTALL_RUNTIME )
install( TARGETS rsw_view
	RUNTIME DESTINATION "${INSTALL_DIR_RUNTIME}"
	)
endif( INSTALL_RUNTIME )

set( TARGET_LIST ${TARGET_LIST} rsw_view  CACHE INTERNAL "" )
set( HAVE_rsw_view ON  CACHE INTERNAL "" )
message( STATUS "Creating target rsw_view - done" )
else( BUILD_EXECUTABLE_RSW_VIEW )
message( STATUS "Skipping target rsw_view (requires roint,sdl_engine,rogl,OpenGL,sdl)" )
unset( HAVE_rsw_view CACHE )
endif( BUILD_EXECUTABLE_RSW_VIEW )
