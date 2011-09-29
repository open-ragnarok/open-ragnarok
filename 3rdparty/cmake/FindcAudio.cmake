# - Try to find cAudio
# Once done, this will define
#
#  CAUDIO_FOUND - system has cAudio
#  CAUDIO_INCLUDE_DIRS - the cAudio include directories
#  CAUDIO_LIBRARIES - link these to use cAudio

include( LibFindMacros )

# Dependencies
#libfind_package( OPENAL OpenAL )
#libfind_package( OGG ogg )
#libfind_package( VORBIS vorbis )

# Use pkg-config to get hints about paths
libfind_pkg_check_modules( CAUDIO_PKGCONF cAudio )

# Include dir
find_path( CAUDIO_INCLUDE_DIR
	NAMES cAudio.h
	PATHS ${CAUDIO_PKGCONF_INCLUDE_DIRS}
	PATH_SUFFIXES cAudio
	)

# Finally the library itself
find_library( CAUDIO_LIBRARY
	NAMES cAudio
	PATHS ${CAUDIO_PKGCONF_LIBRARY_DIRS}
	)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set( CAUDIO_PROCESS_INCLUDES CAUDIO_INCLUDE_DIR )#OPENGAL_INCLUDE_DIRS OGG_INCLUDE_DIRS VORBIS_INCLUDE_DIRS )
set( CAUDIO_PROCESS_LIBS CAUDIO_LIBRARY )#OPENAL_LIBRARIES OGG_LIBRARIES VORBIS_LIBRARIES )
libfind_process( CAUDIO )
