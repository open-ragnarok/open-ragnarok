
#
# pktgen
#
if( ON )
	option( BUILD_EXECUTABLE_PKTGEN "build pktgen executable (extra)" OFF )
else()
	unset( BUILD_EXECUTABLE_PKTGEN CACHE )
endif()
if( BUILD_EXECUTABLE_PKTGEN )
message( STATUS "Creating target pktgen..." )
message( STATUS "- type: executable" )

# variables
file( GLOB PKTGEN_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/*.h"
	)
file( GLOB PKTGEN_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/*.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/msvc/pktgen.rc"
	)
set( INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" )

# target
include_directories( ${INCLUDE_DIRS} )
add_executable( pktgen ${PKTGEN_SOURCES} ${PKTGEN_HEADERS} )

# install
if( INSTALL_RUNTIME )
install( TARGETS pktgen
	RUNTIME DESTINATION "${INSTALL_DIR_RUNTIME}"
	)
endif( INSTALL_RUNTIME )

set( TARGET_LIST ${TARGET_LIST} pktgen  CACHE INTERNAL "" )
set( HAVE_pktgen ON  CACHE INTERNAL "" )
message( STATUS "Creating target pktgen - done" )
else( BUILD_EXECUTABLE_PKTGEN )
message( STATUS "Skipping target pktgen" )
unset( HAVE_pktgen CACHE )
endif( BUILD_EXECUTABLE_PKTGEN )
