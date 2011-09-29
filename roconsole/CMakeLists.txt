
#
# roconsole
#
if( ON )
	option( BUILD_EXECUTABLE_ROCONSOLE "build roconsole executable (extra)" OFF )
else()
	unset( BUILD_EXECUTABLE_ROCONSOLE CACHE )
endif()
if( BUILD_EXECUTABLE_ROCONSOLE )
message( STATUS "Creating target roconsole..." )
message( STATUS "- type: executable" )

# variables
file( GLOB ROCONSOLE_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	)
file( GLOB ROCONSOLE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
	)

# target
add_executable( roconsole ${ROCONSOLE_SOURCES} ${ROCONSOLE_HEADERS} )

# install
if( INSTALL_RUNTIME )
install( TARGETS roconsole
	RUNTIME DESTINATION "${INSTALL_DIR_RUNTIME}"
	)
endif( INSTALL_RUNTIME )

set( TARGET_LIST ${TARGET_LIST} roconsole  CACHE INTERNAL "" )
set( HAVE_roconsole ON  CACHE INTERNAL "" )
message( STATUS "Creating target roconsole - done" )
else( BUILD_EXECUTABLE_ROCONSOLE )
message( STATUS "Skipping target roconsole" )
unset( HAVE_roconsole CACHE )
endif( BUILD_EXECUTABLE_ROCONSOLE )
