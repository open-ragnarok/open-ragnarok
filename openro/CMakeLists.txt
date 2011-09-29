
#
# openro
#
if( HAVE_ronet AND HAVE_roint AND HAVE_sdl_engine AND HAVE_rogl AND HAVE_roengine AND HAVE_freetype AND HAVE_OpenGL AND HAVE_glew AND HAVE_sdl AND HAVE_cAudio )
	option( BUILD_EXECUTABLE_OPENRO "build openro executable" ON )
else()
	unset( BUILD_EXECUTABLE_OPENRO CACHE )
endif()
if( BUILD_EXECUTABLE_OPENRO )
message( STATUS "Creating target openro..." )
message( STATUS "- type: executable" )

# variables
file( GLOB OPENRO_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/gui/*.h"
	
	)
file( GLOB OPENRO_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/gui/*.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/msvc/openro.rc"
	)
set( INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" ${RONET_INCLUDE_DIRS} ${ROINT_INCLUDE_DIRS} ${SDL_ENGINE_INCLUDE_DIRS} ${ROGL_INCLUDE_DIRS} ${ROENGINE_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${SDL_INCLUDE_DIR} ${CAUDIO_INCLUDE_DIRS} )
set( DEFINITIONS ${RONET_DEFINITIONS} ${ROINT_DEFINITIONS} ${SDL_ENGINE_DEFINITIONS} ${ROGL_DEFINITIONS} ${ROENGINE_DEFINITIONS} )
set( LIBRARIES ronet roint sdl_engine rogl roengine ${FREETYPE_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL_LIBRARY} ${CAUDIO_LIBRARIES} )
set( DEPENDENCIES ronet roint sdl_engine rogl roengine )
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
add_executable( openro ${OPENRO_SOURCES} ${OPENRO_HEADERS} )
target_link_libraries( openro ${LIBRARIES} )
add_dependencies( openro ${DEPENDENCIES} )

# install
if( INSTALL_RUNTIME )
install( TARGETS openro
	RUNTIME DESTINATION "${INSTALL_DIR_RUNTIME}"
	)
install( FILES "${CMAKE_CURRENT_SOURCE_DIR}/data.ini.sample"
	DESTINATION "${INSTALL_DIR_RUNTIME}"
	)
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../data/"
	DESTINATION "${INSTALL_DIR_RUNTIME}/data"
	)
endif( INSTALL_RUNTIME )

set( TARGET_LIST ${TARGET_LIST} openro  CACHE INTERNAL "" )
set( HAVE_openro ON  CACHE INTERNAL "" )
message( STATUS "Creating target openro - done" )
else( BUILD_EXECUTABLE_OPENRO )
message( STATUS "Skipping target openro (requires ronet,roint,sdl_engine,rogl,roengine,freetype,OpenGL,glew,sdl,cAudio)" )
unset( HAVE_openro CACHE )
endif( BUILD_EXECUTABLE_OPENRO )
