
#
# local_libpng
#
if( WITH_LOCAL_LIBRARIES_libpng )

# variables
set( LOCAL_PNG_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"  CACHE INTERNAL "" )
set( PNG_PUBLIC_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/png.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/pngconf.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/pnglibconf.h"
)
set( PNG_PRIVATE_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/pngdebug.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/pnginfo.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngpriv.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngstruct.h"
	)
set( PNG_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/png.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngerror.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngget.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngmem.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngpread.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngread.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngrio.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngrtran.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngrutil.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngset.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngtrans.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngwio.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngwrite.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngwtran.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/pngwutil.c"
	)
set( INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" ${LOCAL_PNG_INCLUDE_DIRS} )
set( LIBRARIES )# TODO m library for floating point support?
set( DEPENDENCIES )
if( WITH_LOCAL_LIBRARIES_zlib )
	set( INCLUDE_DIRS ${LOCAL_ZLIB_INCLUDE_DIRS} ${INCLUDE_DIRS} )# local first
	list( APPEND LIBRARIES local_zlib )
	list( APPEND DEPENDENCIES local_zlib )
else( WITH_LOCAL_LIBRARIES_zlib )
	list( APPEND INCLUDE_DIRS ${ZLIB_INCLUDE_DIR} )
	list( APPEND LIBRARIES ${ZLIB_LIBRARY} )
endif( WITH_LOCAL_LIBRARIES_zlib )

# target
include_directories( ${INCLUDE_DIRS} )
add_library( local_libpng STATIC EXCLUDE_FROM_ALL ${PNG_SOURCES} ${PNG_PUBLIC_HEADERS} ${PNG_PRIVATE_HEADERS} )
if( LIBRARIES )
	target_link_libraries( local_libpng ${LIBRARIES} )
endif( LIBRARIES )
if( DEPENDENCIES )
	add_dependencies( local_libpng ${DEPENDENCIES} )
endif( DEPENDENCIES )

endif( WITH_LOCAL_LIBRARIES_libpng )
add_feature_info( local_libpng WITH_LOCAL_LIBRARIES_libpng "Local copy of libpng" )
