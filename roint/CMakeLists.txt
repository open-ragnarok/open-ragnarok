
#
# roint
#
if( HAVE_zlib AND HAVE_png )
	option( BUILD_LIBRARY_ROINT "build roint library" ON )
	set( BUILD_LIBRARY_ROINT_TYPE "STATIC" CACHE STRING "build type: SHARED (dll/so) or STATIC (lib/a)" )
	set_property( CACHE BUILD_LIBRARY_ROINT_TYPE  PROPERTY STRINGS "SHARED" "STATIC" )
else()
	unset( BUILD_LIBRARY_ROINT CACHE )
	unset( BUILD_LIBRARY_ROINT_TYPE CACHE )
endif()
if( BUILD_LIBRARY_ROINT )
message( STATUS "Creating target roint..." )
message( STATUS "- type: ${BUILD_LIBRARY_ROINT_TYPE} library" )

# variables
set( ROINT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" )
if( BUILD_LIBRARY_ROINT_TYPE STREQUAL "SHARED" )
	set( ROINT_DEFINITIONS "-DROINT_DLL" CACHE INTERNAL "" )
else()
	set( ROINT_DEFINITIONS "-DROINT_STATIC" CACHE INTERNAL "" )
endif()
file( GLOB ROINT_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ro/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ro/python/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ro/types/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/tinyxml/*.h"
	)
file( GLOB ROINT_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/python/*.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/*.cc"
	)
set( INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" ${ROINT_INCLUDE_DIRS} )
set( DEFINITIONS ${ROINT_DEFINITIONS} )
set( LIBRARIES )
set( DEPENDENCIES )
if( WITH_LOCAL_LIBRARIES_zlib )
	set( INCLUDE_DIRS ${LOCAL_ZLIB_INCLUDE_DIRS} ${INCLUDE_DIRS} )# local first
	list( APPEND LIBRARIES local_zlib )
	list( APPEND DEPENDENCIES local_zlib )
else( WITH_LOCAL_LIBRARIES_zlib )
	list( APPEND INCLUDE_DIRS ${ZLIB_INCLUDE_DIR} )
	list( APPEND LIBRARIES ${ZLIB_LIBRARY} )
endif( WITH_LOCAL_LIBRARIES_zlib )
if( WITH_LOCAL_LIBRARIES_libpng )
	set( INCLUDE_DIRS ${LOCAL_PNG_INCLUDE_DIRS} ${INCLUDE_DIRS} )# local first
	list( APPEND LIBRARIES local_libpng )
	list( APPEND DEPENDENCIES local_libpng )
else( WITH_LOCAL_LIBRARIES_libpng )
	list( APPEND INCLUDE_DIRS ${PNG_PNG_INCLUDE_DIR} )
	list( APPEND LIBRARIES ${PNG_LIBRARY} )
endif( WITH_LOCAL_LIBRARIES_libpng )
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
add_library( roint ${BUILD_LIBRARY_ROINT_TYPE} ${ROINT_SOURCES} ${ROINT_HEADERS} )
if( LIBRARIES )
	target_link_libraries( roint ${LIBRARIES} )
endif( LIBRARIES )
if( DEPENDENCIES )
	add_dependencies( roint ${DEPENDENCIES} )
endif( DEPENDENCIES )

# install
if( INSTALL_LIBRARIES )
install( TARGETS roint
	RUNTIME DESTINATION "${INSTALL_DIR_RUNTIME}"
	LIBRARY DESTINATION "${INSTALL_DIR_LIBRARIES}"
	ARCHIVE DESTINATION "${INSTALL_DIR_LIBRARIES}"
	)
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	DESTINATION "${INSTALL_DIR_INCLUDES}"
	PATTERN "cp949.h" EXCLUDE
	PATTERN "log.h" EXCLUDE
	PATTERN "stdafx.h" EXCLUDE
	)
endif( INSTALL_LIBRARIES )

set( TARGET_LIST ${TARGET_LIST} roint  CACHE INTERNAL "" )
set( HAVE_roint ON  CACHE INTERNAL "" )
message( STATUS "Creating target roint - done" )
else( BUILD_LIBRARY_ROINT )
message( STATUS "Skipping target roint (requires zlib,png)" )
unset( HAVE_roint CACHE )
endif( BUILD_LIBRARY_ROINT )
