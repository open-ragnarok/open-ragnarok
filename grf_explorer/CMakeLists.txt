
#
# grf_explorer
#
if( HAVE_roint AND HAVE_wxWidgets )
	option( BUILD_EXECUTABLE_GRF_EXPLORER "build grf_explorer executable (extra)" OFF )
else()
	unset( BUILD_EXECUTABLE_GRF_EXPLORER CACHE )
endif()
if( BUILD_EXECUTABLE_GRF_EXPLORER )
message( STATUS "Creating target grf_explorer..." )
message( STATUS "- type: executable" )

# variables
file( GLOB GRF_EXPLORER_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	)
file( GLOB GRF_EXPLORER_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
	)
set( INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" ${ROINT_INCLUDE_DIRS} )
set( DEFINITIONS ${ROINT_DEFINITIONS} )
set( LIBRARIES roint ${wxWidgets_LIBRARIES} )
set( DEPENDENCIES roint )

# target
include( "${wxWidgets_USE_FILE}" )# doesn't handle libraries
include_directories( ${INCLUDE_DIRS} )
add_definitions( ${DEFINITIONS} )
add_executable( grf_explorer ${GRF_EXPLORER_SOURCES} ${GRF_EXPLORER_HEADERS} )
target_link_libraries( grf_explorer ${LIBRARIES} )
if( DEPENDENCIES )
	add_dependencies( grf_explorer ${DEPENDENCIES} )
endif( DEPENDENCIES )

# install
if( INSTALL_RUNTIME )
install( TARGETS grf_explorer
	RUNTIME DESTINATION "${INSTALL_DIR_RUNTIME}"
	)
endif( INSTALL_RUNTIME )

set( TARGET_LIST ${TARGET_LIST} grf_explorer  CACHE INTERNAL "" )
set( HAVE_grf_explorer ON  CACHE INTERNAL "" )
message( STATUS "Creating target grf_explorer - done" )
else( BUILD_EXECUTABLE_GRF_EXPLORER )
message( STATUS "Skipping target grf_explorer (requires roint,wxWidgets)" )
unset( HAVE_grf_explorer CACHE )
endif( BUILD_EXECUTABLE_GRF_EXPLORER )
