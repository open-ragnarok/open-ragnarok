
#
# tools list
#
if( OFF )
	set( _TOOLS_LIST act_dump rsm_dump rsw_dump spr_export )
else()
	file( GLOB _TOOLS_LIST RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cc" )
	set( _TMP )
	foreach( _NAME IN ITEMS ${_TOOLS_LIST} )
		string( REGEX REPLACE ".cc$" "" _NAME "${_NAME}" )
		list( APPEND _TMP "${_NAME}" )
	endforeach()
	set( _TOOLS_LIST ${_TMP} )
endif()


#
# tools
#
if( HAVE_roint AND _TOOLS_LIST )
	option( BUILD_EXECUTABLE_TOOLS "build tool executables [${_TOOLS_LIST}] (extra)" OFF )
else()
	unset( BUILD_EXECUTABLE_TOOLS CACHE )
endif()
if( BUILD_EXECUTABLE_TOOLS )
message( STATUS "Creating target tools..." )
message( STATUS "- type: group" )

# variables
set( DEPENDENCIES )
foreach( _NAME IN ITEMS ${_TOOLS_LIST} )
	option( BUILD_EXECUTABLE_TOOLS_${_NAME} "build ${_NAME} executable (extra)" ON )
	if( BUILD_EXECUTABLE_TOOLS_${_NAME} )
		list( APPEND DEPENDENCIES ${_NAME} )
	endif( BUILD_EXECUTABLE_TOOLS_${_NAME} )
endforeach()

# target
add_custom_target( tools ALL DEPENDS ${DEPENDENCIES} )

set( TARGET_LIST ${TARGET_LIST} tools  CACHE INTERNAL "" )
set( HAVE_tools ${DEPENDENCIES}  CACHE INTERNAL "" )
message( STATUS "Creating target tools - done" )
else( BUILD_EXECUTABLE_TOOLS )
message( STATUS "Skipping target tools (requires roint)" )
foreach( _NAME IN ITEMS ${HAVE_tools} )
	unset( BUILD_EXECUTABLE_TOOLS_${_NAME} CACHE )
	unset( HAVE_tools_${_NAME} CACHE )
endforeach()
unset( HAVE_tools CACHE )
endif( BUILD_EXECUTABLE_TOOLS )



#
# tools_<name>
#
function( _BUILD_TARGET _NAME )
if( BUILD_EXECUTABLE_TOOLS_${_NAME} )
	message( STATUS "Creating target tools_${_NAME}..." )
	message( STATUS "- type: executable" )

	# variables
	set( INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" ${ROINT_INCLUDE_DIRS} )
	set( DEFINITIONS ${ROINT_DEFINITIONS} )
	set( LIBRARIES roint )
	set( DEPENDENCIES roint )

	# target
	include_directories( ${INCLUDE_DIRS} )
	add_definitions( ${DEFINITIONS} )
	add_executable( tools_${_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/${_NAME}.cc" )
	set_target_properties( tools_${_NAME} PROPERTIES OUTPUT_NAME "${_NAME}" )
	target_link_libraries( tools_${_NAME} ${LIBRARIES} )
	add_dependencies( tools_${_NAME} ${DEPENDENCIES} )

	# install
	if( INSTALL_RUNTIME )
	install( TARGETS tools_${_NAME}
		RUNTIME DESTINATION "${INSTALL_DIR_RUNTIME}"
		)
	endif( INSTALL_RUNTIME )

	set( TARGET_LIST ${TARGET_LIST} tools_${_NAME}  CACHE INTERNAL "" )
	set( HAVE_tools_${_NAME} ON  CACHE INTERNAL "" )
	message( STATUS "Creating target tools_${_NAME} - done" )
else( BUILD_EXECUTABLE_TOOLS_${_NAME} )
	message( STATUS "Skipping target tools_${_NAME}" )
	unset( HAVE_tools_${_NAME} CACHE )
endif( BUILD_EXECUTABLE_TOOLS_${_NAME} )
endfunction( _BUILD_TARGET )
if( BUILD_EXECUTABLE_TOOLS )
	foreach( _NAME IN ITEMS ${_TOOLS_LIST} )
		_BUILD_TARGET( ${_NAME} )
	endforeach()
endif( BUILD_EXECUTABLE_TOOLS )
