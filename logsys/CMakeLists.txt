
#
# logsys
#
if( ON )
	option( BUILD_LIBRARY_LOGSYS "build logsys library (optional)" ON )
else()
	unset( BUILD_LIBRARY_LOGSYS CACHE )
endif()
if( BUILD_LIBRARY_LOGSYS )
message( STATUS "Creating target logsys..." )
message( STATUS "- type: STATIC library" )

# variables
set( LOGSYS_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"  CACHE INTERNAL "" )
set( LOGSYS_DEFINITIONS -DUSE_LOGSYS  CACHE INTERNAL "" )
file( GLOB LOGSYS_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/logsys/*.h"
	)
file( GLOB LOGSYS_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
	)
set( INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" ${LOGSYS_INCLUDE_DIRS} )
set( DEFINITIONS ${LOGSYS_DEFINITIONS} )
set( LIBRARIES )
set( DEPENDENCIES )

# target
include_directories( ${INCLUDE_DIRS} )
add_definitions( ${DEFINITIONS} )
add_library( logsys STATIC ${LOGSYS_SOURCES} ${LOGSYS_HEADERS} )
if( LIBRARIES )
	target_link_libraries( ronet ${LIBRARIES} )
endif( LIBRARIES )
if( DEPENDENCIES )
	add_dependencies( ronet ${DEPENDENCIES} )
endif( DEPENDENCIES )

# install
if( INSTALL_LIBRARIES )
install( TARGETS logsys
	RUNTIME DESTINATION "${INSTALL_DIR_RUNTIME}"
	LIBRARY DESTINATION "${INSTALL_DIR_LIBRARIES}"
	ARCHIVE DESTINATION "${INSTALL_DIR_LIBRARIES}"
	)
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	DESTINATION "${INSTALL_DIR_INCLUDES}"
	PATTERN "stdafx.h" EXCLUDE
	)
endif( INSTALL_LIBRARIES )

set( TARGET_LIST ${TARGET_LIST} logsys  CACHE INTERNAL "" )
set( HAVE_logsys ON  CACHE INTERNAL "" )
message( STATUS "Creating target logsys - done" )
else( BUILD_LIBRARY_LOGSYS )
message( STATUS "Skipping target logsys (optional)" )
unset( HAVE_logsys CACHE )
endif( BUILD_LIBRARY_LOGSYS )
