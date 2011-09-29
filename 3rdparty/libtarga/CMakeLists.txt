
#
# local_libtarga
#
if( ON )

# variables
set( LOCAL_TARGA_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"  CACHE INTERNAL "" )
set( TARGA_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/libtarga.h"
	)
set( TARGA_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/libtarga.c"
	)
set( INCLUDE_DIRS ${LOCAL_TARGA_INCLUDE_DIRS} )

# target
include_directories( ${INCLUDE_DIRS} )
add_library( local_libtarga STATIC EXCLUDE_FROM_ALL ${TARGA_SOURCES} ${TARGA_HEADERS} )

endif( ON )
add_feature_info( local_libtarga ON "Local copy of libtarga? (unknown source of code)" )
