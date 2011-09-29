# - Try to find GLEW
# Once done, this will define
#
#  GLEW_FOUND - system has GLEW
#  GLEW_INCLUDE_DIRS - the GLEW include directories
#  GLEW_LIBRARIES - link these to use GLEW

include( LibFindMacros )

# Dependencies
#libfind_package( OPENGL OpenGL )

# Use pkg-config to get hints about paths
libfind_pkg_check_modules( GLEW_PKGCONF glew )

# Include dir
find_path( GLEW_INCLUDE_DIR
	NAMES GL/glew.h
	PATHS ${GLEW_PKGCONF_INCLUDE_DIRS}
	)

# Finally the library itself
find_library( GLEW_LIBRARY
	NAMES glew GLEW glew32 glew32s
	PATHS ${GLEW_PKGCONF_LIBRARY_DIRS}
	)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set( GLEW_PROCESS_INCLUDES GLEW_INCLUDE_DIR )#OPENGL_INCLUDE_DIRS )
set( GLEW_PROCESS_LIBS GLEW_LIBRARY )#OPENGL_LIBRARIES )
libfind_process( GLEW )
