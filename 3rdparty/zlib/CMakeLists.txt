
#
# local_zlib
#
if( WITH_LOCAL_LIBRARIES_zlib )
include( CheckFunctionExists )
include( CheckIncludeFile )

# variables
set( LOCAL_ZLIB_INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/include"  CACHE INTERNAL "" )
file( GLOB ZLIB_PUBLIC_HEADERS
	"${CMAKE_CURRENT_BINARY_DIR}/include/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
	)
file( GLOB ZLIB_PRIVATE_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	)
file( GLOB ZLIB_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
	)
check_include_file( unistd.h Z_HAVE_UNISTD_H )
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/zconf.h.cmakein"
	"${CMAKE_CURRENT_BINARY_DIR}/include/zconf.h"
	@ONLY )
check_function_exists( fseeko HAVE_FSEEKO )
set( INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" ${LOCAL_ZLIB_INCLUDE_DIRS} )

# target
include_directories( ${INCLUDE_DIRS} )
if( NOT HAVE_FSEEKO )
	add_definitions( -DNO_FSEEKO )
endif()
add_library( local_zlib STATIC EXCLUDE_FROM_ALL ${ZLIB_SOURCES} ${ZLIB_PUBLIC_HEADERS} ${ZLIB_PRIVATE_HEADERS} )

endif( WITH_LOCAL_LIBRARIES_zlib )
add_feature_info( local_zlib WITH_LOCAL_LIBRARIES_zlib "Local copy of libtarga? (unknown source of code)" )
