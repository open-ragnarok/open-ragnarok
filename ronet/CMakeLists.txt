
#
# ronet
#
if( HAVE_threads )
	option( BUILD_LIBRARY_RONET "build ronet library" ON )
	set( BUILD_LIBRARY_RONET_TYPE "STATIC"  CACHE STRING "build type: SHARED (dll/so) or STATIC (lib/a)" )
	set_property( CACHE BUILD_LIBRARY_RONET_TYPE  PROPERTY STRINGS "SHARED" "STATIC" )
	set( IGNORE_RONET_PACKETS "pkt_statinfo;pkt_statinfo2"  CACHE STRING "list of packets that are not compiled" )
else()
	unset( BUILD_LIBRARY_RONET CACHE )
	unset( BUILD_LIBRARY_RONET_TYPE CACHE )
	unset( RONET_IGNORE_PACKETS CACHE )
endif()
if( BUILD_LIBRARY_RONET )
message( STATUS "Creating target ronet..." )
message( STATUS "- type: ${BUILD_LIBRARY_RONET_TYPE} library" )

# variables
set( RONET_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" )
if( BUILD_LIBRARY_RONET_TYPE STREQUAL "SHARED" )
	set( RONET_DEFINITIONS "-DRONET_DLL" CACHE INTERNAL "" )
else()
	set( RONET_DEFINITIONS "-DRONET_STATIC" CACHE INTERNAL "" )
endif()
file( GLOB RONET_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ronet/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ronet/packets/*.h"
	)
file( GLOB RONET_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/packets/*.cc"
	)
foreach( _PACKET IN ITEMS ${IGNORE_RONET_PACKETS} )
	list( REMOVE_ITEM RONET_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/ronet/packets/${_PACKET}.h" )
	list( REMOVE_ITEM RONET_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/packets/${_PACKET}.cc" )
endforeach()
set( INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" ${RONET_INCLUDE_DIRS} ${CMAKE_THREAD_LIBS_INIT} )
set( DEFINITIONS ${RONET_DEFINITIONS} )
set( LIBRARIES )
set( DEPENDENCIES )
if( WIN32 )
	list( APPEND LIBRARIES wsock32.lib )
else( WIN32 )
	list( APPEND LIBRARIES ${PTHREAD_LIBRARY} )
endif( WIN32 )
if( HAVE_logsys )
	message( STATUS "- enabling logsys" )
	list( APPEND INCLUDE_DIRS ${LOGSYS_INCLUDE_DIRS} )
	list( APPEND DEFINITIONS ${LOGSYS_DEFINITIONS} )
	list( APPEND LIBRARIES logsys )
	list( APPEND DEPENDENCIES logsys )
else( HAVE_logsys )
	message( STATUS "- disabling logsys" )
endif( HAVE_logsys )

# target
include_directories( ${INCLUDE_DIRS} )
add_definitions( ${DEFINITIONS} )
add_library( ronet ${BUILD_LIBRARY_RONET_TYPE} ${RONET_SOURCES} ${RONET_HEADERS} )
if( LIBRARIES )
	target_link_libraries( ronet ${LIBRARIES} )
endif( LIBRARIES )
if( DEPENDENCIES )
	add_dependencies( ronet ${DEPENDENCIES} )
endif( DEPENDENCIES )

# install
if( INSTALL_LIBRARIES )
install( TARGETS ronet
	RUNTIME DESTINATION "${INSTALL_DIR_RUNTIME}"
	LIBRARY DESTINATION "${INSTALL_DIR_LIBRARIES}"
	ARCHIVE DESTINATION "${INSTALL_DIR_LIBRARIES}"
	)
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	DESTINATION "${INSTALL_DIR_INCLUDES}"
	FILES_MATCHING
	PATTERN "ronet/*.h"
	PATTERN "ronet/packets/*.h"
	)
endif( INSTALL_LIBRARIES )

set( TARGET_LIST ${TARGET_LIST} ronet  CACHE INTERNAL "" )
set( HAVE_ronet ON  CACHE INTERNAL "" )
message( STATUS "Creating target ronet - done" )
else( BUILD_LIBRARY_RONET )
message( STATUS "Skipping target ronet (requires threads)" )
unset( HAVE_ronet CACHE )
endif( BUILD_LIBRARY_RONET )
