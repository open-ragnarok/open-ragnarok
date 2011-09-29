
#
# local_libjpeg
#
if( WITH_LOCAL_LIBRARIES_libjpeg )
include( CheckIncludeFile )
include( CheckTypeSize )

# variables
set( LOCAL_JPEG_INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/include"  CACHE INTERNAL "" )
file( GLOB JPEG_PUBLIC_HEADERS
	"${CMAKE_CURRENT_BINARY_DIR}/include/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
	)
file( GLOB JPEG_PRIVATE_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	)
file( GLOB JPEG_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
	)
check_include_file( stddef.h HAVE_STDDEF_H )
check_include_file( stdlib.h HAVE_STDLIB_H )
check_include_file( locale.h HAVE_LOCALE_H )
check_include_file( string.h HAVE_STRING_H )
check_include_file( sys/types.h HAVE_SYS_TYPES_H )
check_type_size( "unsigned char" UNSIGNED_CHAR BUILTIN_TYPES_ONLY )
check_type_size( "unsigned short" UNSIGNED_SHORT BUILTIN_TYPES_ONLY )
if( NOT HAVE_STRING_H )
	set( NEED_BSD_STRINGS 1 )
endif( NOT HAVE_STRING_H )
if( HAVE_SYS_TYPES_H )
	set( NEED_SYS_TYPES_H 1 )# might not be needed, but there should be no harm in including it
endif( HAVE_SYS_TYPES_H )
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/jconfig.h.cmakein"
	"${CMAKE_CURRENT_BINARY_DIR}/include/jconfig.h"
	@ONLY )
set( INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" ${LOCAL_JPEG_INCLUDE_DIRS} )

# target
include_directories( ${INCLUDE_DIRS} )
add_library( local_libjpeg STATIC EXCLUDE_FROM_ALL ${JPEG_SOURCES} ${JPEG_PUBLIC_HEADERS} ${JPEG_PRIVATE_HEADERS} )

endif( WITH_LOCAL_LIBRARIES_libjpeg )
add_feature_info( local_libjpeg WITH_LOCAL_LIBRARIES_libjpeg "Local copy of libjpeg" )
