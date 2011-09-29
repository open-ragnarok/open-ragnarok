#####################################################################
#
# http://open-ragnarok.org
#
# "Getting Started with CMake", a tutorial video by Eric Wing.
#   Part 1 of 6: http://www.youtube.com/watch?v=CLvZTyji_Uw
#   Part 2 of 6: http://www.youtube.com/watch?v=gUW-RrRQjEg
#   Part 3 of 6: http://www.youtube.com/watch?v=sz6cPhbuTk4
#   Part 4 of 6: http://www.youtube.com/watch?v=JICZOkyNXbg
#   Part 5 of 6: http://www.youtube.com/watch?v=lAiuLHy4dCk
#   Part 6 of 6: http://www.youtube.com/watch?v=fAtJNzDZdH8
#
# You can use notepad++ for syntax highlighting.
# Naming conventions:
#   WITH_*   : option to use an external package or not
#   ENABLE_* : option to use an internal feature/code or not
#   HAVE_*   : internal variable indicating if we have and are using something
#
#####################################################################
if( NOT DEFINED CMAKE_INSTALL_PREFIX )
	set( _ADJUST_INSTALL_PREFIX ON )# adjust the default install prefix
endif()
if( NOT DEFINED WITH_LOCAL_LIBRARIES )
	set( _AUTO_ENABLE_LOCAL_LIBRARIES ON )# auto-enable local libraries if missing from the system
endif()

#cmake_minimum_required( VERSION 2.8.5 )
# Functional changes from 2.8.4 to 2.8.5:
#   find_(library|package) has Linux multiarch support
#   Add CMAKE_ARGC and CMAKE_ARGV0..N-1 variables
#   Add file(UPLOAD)
#   Add a string(FIND) sub-command
#cmake_minimum_required( VERSION 2.8.4 )
# Functional changes from 2.8.3 to 2.8.4:
#   string(SUBSTRING) works with length -1 as "rest of string"
#   changes to some CPack generators
#   CYGWIN no longer defines WIN32
#   CMP0017: Prefer files from the CMake module directory when including from there.
set( CMAKE_LEGACY_CYGWIN_WIN32 0 )
cmake_minimum_required( VERSION 2.8.3 )
project( open-ragnarok CXX )
enable_language( C )
if( CYGWIN )
	unset( WIN32 )
endif( CYGWIN )
include( FeatureSummary )


#
# Global stuff
#
option( WITH_LOCAL_DEPENDENCIES "use 3rdparty/dependencies as a library source" ON )
option( WITH_LOCAL_LIBRARIES "use local 3rdparty libraries (libjpeg,libpng,zlib)" ON )
if( WITH_LOCAL_LIBRARIES )
	option( WITH_LOCAL_LIBRARIES_libjpeg "use 3rdparty/libjpeg instead of system library" OFF )
	option( WITH_LOCAL_LIBRARIES_libpng "uses 3rdparty/libpng instead of system library" OFF )
	option( WITH_LOCAL_LIBRARIES_zlib "uses 3rdparty/zlib instead of system library" OFF )
else( WITH_LOCAL_LIBRARIES )
	unset( WITH_LOCAL_LIBRARIES_libjpeg CACHE )
	unset( WITH_LOCAL_LIBRARIES_libpng CACHE )
	unset( WITH_LOCAL_LIBRARIES_zlib CACHE )
endif( WITH_LOCAL_LIBRARIES )
if( MSVC )
	option( ENABLE_STATIC_RUNTIME_LIBRARY "link to the static runtime library" OFF )
	add_definitions( -D_CRT_SECURE_NO_WARNINGS )
	add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
	add_definitions( -D_CRT_NONSTDC_NO_DEPRECATE )
	add_definitions( /wd4251 )# disable warning C4251
	add_definitions( /wd4275 )# disable warning C4275
	foreach( _VAR IN ITEMS
		CMAKE_C_FLAGS  CMAKE_C_FLAGS_DEBUG  CMAKE_C_FLAGS_RELEASE
		CMAKE_C_FLAGS_MINSIZEREL  CMAKE_C_FLAGS_RELWITHDEBINFO
		CMAKE_CXX_FLAGS  CMAKE_CXX_FLAGS_DEBUG  CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_MINSIZEREL  CMAKE_CXX_FLAGS_RELWITHDEBINFO
		)
		get_property( _STR CACHE ${_VAR} PROPERTY VALUE )
		if( ENABLE_STATIC_RUNTIME_LIBRARY )
			string( REPLACE "/MD" "/MT" _STR "${_STR}" )
		else( ENABLE_STATIC_RUNTIME_LIBRARY )
			string( REPLACE "/MT" "/MD" _STR "${_STR}" )
		endif( ENABLE_STATIC_RUNTIME_LIBRARY )
		set_property( CACHE ${_VAR} PROPERTY VALUE "${_STR}" )
	endforeach()
endif( MSVC )
if( CMAKE_COMPILER_IS_GNUCXX )
	add_definitions( -D_GLIBCXX_USE_WCHAR_T )
	add_definitions( -D_GLIBCXX_USE_WSTRING )
endif( CMAKE_COMPILER_IS_GNUCXX )
if( CMAKE_BUILD_TYPE STREQUAL "" )
	set( CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE )
endif()
set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cmake"  CACHE INTERNAL "" )
set( TARGET_LIST CACHE INTERNAL "" )
include( CheckFunctionExists )
include( CheckIncludeFile )


#
# install/package stuff
#
option( INSTALL_RUNTIME "install runtime files" ON )
option( INSTALL_LIBRARIES "install library and include files" OFF )
option( INSTALL_PYTHON_SCRIPTS "install python scripts" OFF )
if( _ADJUST_INSTALL_PREFIX )
	if( UNIX )
		set_property( CACHE CMAKE_INSTALL_PREFIX  PROPERTY VALUE "${CMAKE_INSTALL_PREFIX}/etc/${CMAKE_PROJECT_NAME}" )
	endif( UNIX )
	set_property( CACHE CMAKE_INSTALL_PREFIX  PROPERTY VALUE "${CMAKE_CURRENT_BINARY_DIR}/install" )# XXX install to subdir while testing
endif( _ADJUST_INSTALL_PREFIX )
set( INSTALL_DIR_RUNTIME "${CMAKE_INSTALL_PREFIX}"  CACHE INTERNAL "" )
set( INSTALL_DIR_LIBRARIES "${CMAKE_INSTALL_PREFIX}/lib"  CACHE INTERNAL "" )
set( INSTALL_DIR_INCLUDES "${CMAKE_INSTALL_PREFIX}/include"  CACHE INTERNAL "" )


#
# python scripts
#
if( INSTALL_PYTHON_SCRIPTS )
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/blender/"
	DESTINATION "${INSTALL_DIR_RUNTIME}/python/blender"
	)
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pyroint/"
	DESTINATION "${INSTALL_DIR_RUNTIME}/python/pyroint"
	)
endif( INSTALL_PYTHON_SCRIPTS )


#
# dependencies
#
if( WITH_LOCAL_DEPENDENCIES )
if( INSTALL_RUNTIME )
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/dependencies/bin/"
	DESTINATION "${CMAKE_INSTALL_PREFIX}"
	USE_SOURCE_PERMISSIONS
	PATTERN ".gitignore" EXCLUDE
	)
endif( INSTALL_RUNTIME )
set( CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/dependencies/include" )
set( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/dependencies/lib" )
endif( WITH_LOCAL_DEPENDENCIES )


#
# GnuWin32 (for WIN32 packages)
#
if( WIN32 )
option( WITH_GnuWin32 "search for and use packages in the GnuWin32 install dir" OFF )
if( WITH_GnuWin32 )
message( STATUS "Looking for GnuWin32..." )
find_package( GnuWin32 )
set_package_info( GnuWin32 "ports of tools and libraries for windows" "http://gnuwin32.sourceforge.net/" )
if( GNUWIN32_FOUND )
	set( CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${GNUWIN32_INCLUDE_DIR}" )
	set( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${GNUWIN32_LIBRARY_DIR}" )
endif( GNUWIN32_FOUND )
message( STATUS "Looking for GnuWin32 - done" )
endif( WITH_GnuWin32 )
endif( WIN32 )


#
# strnicmp/strncasecmp
#
message( STATUS "Looking for functions..." )
CHECK_FUNCTION_EXISTS( strnicmp HAVE_STRNICMP )
CHECK_FUNCTION_EXISTS( strncasecmp HAVE_STRNCASECMP )
foreach( _DEF  IN ITEMS  HAVE_STRNICMP HAVE_STRNCASECMP )
	if( ${${_DEF}} )
		add_definitions( -D${_DEF} )
	endif()
endforeach()
message( STATUS "Looking for functions - done" )


#
# zlib (has local)
#
message( STATUS "Looking for zlib..." )
if( NOT WITH_LOCAL_LIBRARIES_zlib )
	find_package( ZLIB )
	set_package_info( ZLIB "Lossless Data-Compression Library" "http://zlib.net/" )
	if( _AUTO_ENABLE_LOCAL_LIBRARIES AND NOT ZLIB_FOUND )
		message( STATUS "  not found, enabling local zlib" )
		set_property( CACHE WITH_LOCAL_LIBRARIES_zlib  PROPERTY VALUE ON )
	endif()
endif( NOT WITH_LOCAL_LIBRARIES_zlib )
if( WITH_LOCAL_LIBRARIES_zlib )
	add_subdirectory( 3rdparty/zlib )
	set( HAVE_zlib ON  CACHE INTERNAL "" )
	message( STATUS "Looking for zlib - local" )
else( WITH_LOCAL_LIBRARIES_zlib )
	if( ZLIB_FOUND )
		set( HAVE_zlib ON  CACHE INTERNAL "" )
		message( STATUS "Looking for zlib - found" )
	else( ZLIB_FOUND )
		message( STATUS "Looking for zlib - NOT FOUND" )
		unset( HAVE_zlib CACHE )
	endif( ZLIB_FOUND )
endif( WITH_LOCAL_LIBRARIES_zlib )


#
# png (has local)
#
message( STATUS "Looking for png..." )
if( NOT WITH_LOCAL_LIBRARIES_libpng )
	find_package( PNG )
	set_package_info( PNG "PNG Image Library" "http://www.libpng.org/pub/png/libpng.html" )
	if( _AUTO_ENABLE_LOCAL_LIBRARIES AND NOT PNG_FOUND )
		message( STATUS "  not found, enabling local libpng" )
		set_property( CACHE WITH_LOCAL_LIBRARIES_libpng  PROPERTY VALUE ON )
	endif()
endif( NOT WITH_LOCAL_LIBRARIES_libpng )
if( WITH_LOCAL_LIBRARIES_libpng )
	add_subdirectory( 3rdparty/libpng )# (zlib)
	set( HAVE_png ON  CACHE INTERNAL "" )
	message( STATUS "Looking for png - local" )
else( WITH_LOCAL_LIBRARIES_libpng )
	if( PNG_FOUND )
		set( HAVE_png ON  CACHE INTERNAL "" )
		message( STATUS "Looking for png - found" )
	else( PNG_FOUND )
		message( STATUS "Looking for png - NOT FOUND" )
		unset( HAVE_png CACHE )
	endif( PNG_FOUND )
endif( WITH_LOCAL_LIBRARIES_libpng )


#
# jpeg (has local)
#
message( STATUS "Looking for jpeg..." )
if( NOT WITH_LOCAL_LIBRARIES_libjpeg )
	find_package( JPEG )
	set_package_info( JPEG " JPEG Image Library" "http://www.ijg.org/" )
	if( _AUTO_ENABLE_LOCAL_LIBRARIES AND NOT JPEG_FOUND )
		message( STATUS "  not found, enabling local libjpeg" )
		set_property( CACHE WITH_LOCAL_LIBRARIES_libjpeg  PROPERTY VALUE ON )
	endif()
endif( NOT WITH_LOCAL_LIBRARIES_libjpeg )
if( WITH_LOCAL_LIBRARIES_libjpeg )
	add_subdirectory( 3rdparty/libjpeg )
	set( HAVE_jpeg ON  CACHE INTERNAL "" )
	message( STATUS "Looking for jpeg - local" )
else( WITH_LOCAL_LIBRARIES_libjpeg )
	if( JPEG_FOUND )
		set( HAVE_jpeg ON  CACHE INTERNAL "" )
		message( STATUS "Looking for jpeg - found" )
	else( JPEG_FOUND )
		message( STATUS "Looking for jpeg - NOT FOUND" )
		unset( HAVE_jpeg CACHE )
	endif( JPEG_FOUND )
endif( WITH_LOCAL_LIBRARIES_libjpeg )


#
# threads
#
message( STATUS "Looking for threads..." )
set( CMAKE_THREAD_PREFER_PTHREAD ON )
find_package( Threads )
set_package_info( Threads "System Threads" )
if( Threads_FOUND )
	set( HAVE_threads ON  CACHE INTERNAL "" )
	message( STATUS "Looking for threads - found" )
else( Threads_FOUND )
	message( STATUS "Looking for threads - NOT FOUND" )
	unset( HAVE_threads CACHE )
endif( Threads_FOUND )


#
# targa
#
message( STATUS "Looking for targa..." )
add_subdirectory( 3rdparty/libtarga )
set( HAVE_targa ON  CACHE INTERNAL "" )
message( STATUS "Looking for targa - local" )


#
# freetype
#
message( STATUS "Looking for freetype..." )
find_package( Freetype )
set_package_info( Freetype "Font Engine" "http://www.freetype.org/" )
if( FREETYPE_FOUND )
	set( HAVE_freetype ON  CACHE INTERNAL "" )
	message( STATUS "Looking for freetype - found" )
else( FREETYPE_FOUND )
	message( STATUS "Looking for freetype - NOT FOUND" )
	unset( HAVE_freetype CACHE )
endif( FREETYPE_FOUND )


#
# OpenGL
#
message( STATUS "Looking for OpenGL..." )
find_package( OpenGL )
set_package_info( OpenGL "High Performance Graphics Library" "http://www.opengl.org/" )
if( OPENGL_FOUND AND OPENGL_GLU_FOUND )
	set( HAVE_OpenGL ON  CACHE INTERNAL "" )
	message( STATUS "Looking for OpenGL - found" )
else( OPENGL_FOUND AND OPENGL_GLU_FOUND )
	message( STATUS "Looking for OpenGL - NOT FOUND" )
	unset( HAVE_OpenGL CACHE )
endif( OPENGL_FOUND AND OPENGL_GLU_FOUND )


#
# glew
#
message( STATUS "Looking for glew..." )
find_package( GLEW )
set_package_info( GLEW "OpenGL Extension Wrangler Library" "http://glew.sourceforge.net/" )
if( GLEW_FOUND )
	set( HAVE_glew ON  CACHE INTERNAL "" )
	message( STATUS "Looking for glew - found" )
else( GLEW_FOUND )
	message( STATUS "Looking for glew - NOT FOUND" )
	unset( HAVE_glew CACHE )
endif( GLEW_FOUND )


#
# sdl
#
message( STATUS "Looking for sdl..." )
set( SDL_BUILDING_LIBRARY 1 )
find_package( SDL )
set_package_info( SDL "Multimedia Library" "http://www.libsdl.org/" )
if( SDL_FOUND )
	mark_as_advanced( SDLMAIN_LIBRARY  SDL_INCLUDE_DIR  SDL_LIBRARY )
	set( HAVE_sdl ON  CACHE INTERNAL "" )
	message( STATUS "Looking for sdl - found" )
else( SDL_FOUND )
	message( STATUS "Looking for sdl - NOT FOUND" )
	unset( HAVE_sdl CACHE )
endif( SDL_FOUND )


#
# cAudio
#
message( STATUS "Looking for cAudio..." )
find_package( cAudio )
set_package_info( cAudio "Advanced OpenAL Wrapper" "http://caudio.deathtouchstudios.com/" )
if( CAUDIO_FOUND )
	set( HAVE_cAudio ON  CACHE INTERNAL "" )
	message( STATUS "Looking for cAudio - found" )
else( CAUDIO_FOUND )
	message( STATUS "Looking for cAudio - NOT FOUND" )
	unset( HAVE_cAudio CACHE )
endif( CAUDIO_FOUND )


#
# wxWidgets
#
message( STATUS "Looking for wxWidgets..." )
find_package( wxWidgets )
set_package_info( wxWidgets "GUI Library" "http://www.wxwidgets.org/" "disabled if wx/propgrid/propgrid.h is missing" )
if( wxWidgets_FOUND )
	mark_as_advanced( wxWidgets_CONFIG_EXECUTABLE  wxWidgets_wxrc_EXECUTABLE )
	find_file( HAVE_WX_PROPGRID_PROPGRID_H wx/propgrid/propgrid.h # wxWidgets 2.9?
		PATHS ${wxWidgets_INCLUDE_DIRS}
		NO_DEFAULT_PATHS
		)
	mark_as_advanced( HAVE_WX_PROPGRID_PROPGRID_H )
	if( HAVE_WX_PROPGRID_PROPGRID_H )
		set( HAVE_wxWidgets ON  CACHE INTERNAL "" )
		message( STATUS "Looking for wxWidgets - found" )
	else()
		message( STATUS "Looking for wxWidgets - incompatible (wx/propgrid/propgrid.h missing)" )
		unset( HAVE_wxWidgets CACHE )
	endif()
else( wxWidgets_FOUND )
	message( STATUS "Looking for wxWidgets - NOT FOUND" )
	unset( HAVE_wxWidgets CACHE )
endif( wxWidgets_FOUND )


#
# subdirectories
#
# libraries:
add_subdirectory( logsys )# [optional]
add_subdirectory( ronet )# (threads)
add_subdirectory( roint )# (zlib,png)
add_subdirectory( sdl_engine )# (zlib,png,targa,jpeg,freetype,OpenGL,sdl)
add_subdirectory( rogl )# roint,sdl_engine (OpenGL)
add_subdirectory( roengine )# ronet,roint,sdl_engine,rogl (png,freetype,OpenGL,glew,sdl,cAudio)
# runtime:
add_subdirectory( openro )# ronet,roint,sdl_engine,rogl,roengine (freetype,OpenGL,glew,sdl,cAudio)
# extras:
add_subdirectory( grf_explorer )# roint,wxWidgets
add_subdirectory( pktgen )
add_subdirectory( roconsole )
add_subdirectory( rsm_view )# roint,sdl_engine,rogl (OpenGL,sdl)
add_subdirectory( rsw_view )# roint,sdl_engine,rogl (OpenGL,sdl)
add_subdirectory( tools )# roint


# Print list of targets
PRINT_ENABLED_FEATURES()
PRINT_DISABLED_FEATURES()
if( TARGET_LIST )
	message( STATUS "Available targets:" )
	foreach( _TARGET IN ITEMS ${TARGET_LIST} )
		message( STATUS "\t${_TARGET}" )
	endforeach()
else( TARGET_LIST )
	message( WARNING "no targets available - nothing to build" )
endif( TARGET_LIST )
