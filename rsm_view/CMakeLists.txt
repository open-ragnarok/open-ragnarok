
#
# rsm_view
#
if( HAVE_roint AND HAVE_sdl_engine AND HAVE_rogl AND HAVE_OpenGL AND HAVE_sdl )
	option( BUILD_EXECUTABLE_RSM_VIEW "build rsm_view executable (extra)" OFF )
else()
	unset( BUILD_EXECUTABLE_RSM_VIEW CACHE )
endif()
if( BUILD_EXECUTABLE_RSM_VIEW )
message( STATUS "Creating target rsm_view..." )
message( STATUS "- type: executable" )

# variables
file( GLOB RSM_VIEW_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	)
file( GLOB RSM_VIEW_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
	)
set( INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" ${ROINT_INCLUDE_DIRS} ${SDL_ENGINE_INCLUDE_DIRS} ${ROGL_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${SDL_INCLUDE_DIR} )
set( DEFINITIONS ${ROINT_DEFINITIONS} ${SDL_ENGINE_DEFINITIONS} ${ROGL_DEFINITIONS} )
set( LIBRARIES roint sdl_engine rogl ${OPENGL_LIBRARIES} ${SDL_LIBRARY} )
set( DEPENDENCIES roint sdl_engine rogl )

# target
include_directories( ${INCLUDE_DIRS} )
add_definitions( ${DEFINITIONS} )
add_executable( rsm_view ${RSM_VIEW_SOURCES} ${RSM_VIEW_HEADERS} )
target_link_libraries( rsm_view ${LIBRARIES} )
add_dependencies( rsm_view ${DEPENDENCIES} )

# install
if( INSTALL_RUNTIME )
install( TARGETS rsm_view
	RUNTIME DESTINATION "${INSTALL_DIR_RUNTIME}"
	)
endif( INSTALL_RUNTIME )

set( TARGET_LIST ${TARGET_LIST} rsm_view  CACHE INTERNAL "" )
set( HAVE_rsm_view ON  CACHE INTERNAL "" )
message( STATUS "Creating target rsm_view - done" )
else( BUILD_EXECUTABLE_RSM_VIEW )
message( STATUS "Skipping target rsm_view (requires roint,sdl_engine,rogl,OpenGL,sdl)" )
unset( HAVE_rsm_view CACHE )
endif( BUILD_EXECUTABLE_RSM_VIEW )
