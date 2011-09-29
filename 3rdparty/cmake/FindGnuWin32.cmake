# - Try to find GnuWin32
# Once done, this will define
#
#  GNUWIN32_FOUND - system has GnuWin32
#  GNUWIN32_INCLUDE_DIR - the GnuWin32 include directory
#  GNUWIN32_LIBRARY_DIR - the GnuWin32 library directory
#  GNUWIN32_BINARY_DIR - the GnuWin32 binary directory


if( WIN32 )
include( FindPackageHandleStandardArgs REQUIRED )

find_path( GNUWIN32_DIR lib
	PATH
		"$ENV{GNUWIN32_DIR}"
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\GnuWin32;InstallPath]"
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\GnuWin32;InstallPath]"
		"$ENV{ProgramFiles}/GnuWin32"
		"$ENV{ProgramFiles(x86)}/GnuWin32"
		"$ENV{ProgramW6432}/GnuWin32"
	NO_DEFAULT_PATH
	)
mark_as_advanced( GNUWIN32_DIR )

if( GNUWIN32_DIR )
	set( GNUWIN32_INCLUDE_DIR "${GNUWIN32_DIR}/include" )
	set( GNUWIN32_LIBRARY_DIR "${GNUWIN32_DIR}/lib" )
	set( GNUWIN32_BINARY_DIR  "${GNUWIN32_DIR}/bin" )
endif( GNUWIN32_DIR )

# set GNUWIN32_FOUND and report message
FIND_PACKAGE_HANDLE_STANDARD_ARGS( GNUWIN32 DEFAULT_MSG  GNUWIN32_DIR )

endif( WIN32 )
