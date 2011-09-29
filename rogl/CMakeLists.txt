
#
# rogl
#
if( HAVE_roint AND HAVE_sdl_engine AND HAVE_OpenGL )
	option( BUILD_LIBRARY_ROGL "build rogl library" ON )
	set( BUILD_LIBRARY_ROGL_TYPE "STATIC" CACHE STRING "build type: SHARED (dll/so) or STATIC (lib/a)" )
	set_property( CACHE BUILD_LIBRARY_ROGL_TYPE  PROPERTY STRINGS "SHARED" "STATIC" )
else()
	unset( BUILD_LIBRARY_ROGL CACHE )
	unset( BUILD_LIBRARY_ROGL_TYPE CACHE )
endif()
if( BUILD_LIBRARY_ROGL )
message( STATUS "Creating target rogl" )
message( STATUS "- type: ${BUILD_LIBRARY_ROGL_TYPE} library" )

# variables
set( ROGL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" )
if( BUILD_LIBRARY_ROGL_TYPE STREQUAL "SHARED" )
	set( ROGL_DEFINITIONS "-DROGL_DLL" CACHE INTERNAL "" )
else()
	set( ROGL_DEFINITIONS "-DROGL_STATIC" CACHE INTERNAL "" )
endif()
file( GLOB ROGL_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/rogl/*.h"
	)
file( GLOB ROGL_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	)
set( INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" ${ROGL_INCLUDE_DIRS} ${ROINT_INCLUDE_DIRS} ${SDL_ENGINE_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} )
set( DEFINITIONS ${ROGL_DEFINITIONS} ${ROINT_DEFINITIONS} ${SDL_ENGINE_DEFINITIONS} )
set( LIBRARIES roint sdl_engine ${OPENGL_LIBRARIES} )
set( DEPENDENCIES roint sdl_engine )
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
add_library( rogl ${BUILD_LIBRARY_ROGL_TYPE} ${ROGL_SOURCES} ${ROGL_HEADERS} )
target_link_libraries( rogl ${LIBRARIES} )
add_dependencies( rogl ${DEPENDENCIES} )

# install
if( INSTALL_LIBRARIES )
install( TARGETS rogl
	RUNTIME DESTINATION "${INSTALL_DIR_RUNTIME}"
	LIBRARY DESTINATION "${INSTALL_DIR_LIBRARIES}"
	ARCHIVE DESTINATION "${INSTALL_DIR_LIBRARIES}"
	)
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	DESTINATION "${INSTALL_DIR_INCLUDES}"
	PATTERN "stdafx.h" EXCLUDE
	)
endif( INSTALL_LIBRARIES )

set( TARGET_LIST ${TARGET_LIST} rogl  CACHE INTERNAL "" )
set( HAVE_rogl ON  CACHE INTERNAL "" )
message( STATUS "Creating target rogl - done" )
else( BUILD_LIBRARY_ROGL )
message( STATUS "Skipping target rogl (requires roint,sdl_engine,OpenGL)" )
unset( HAVE_rogl CACHE )
endif( BUILD_LIBRARY_ROGL )
