# This script locates the austinhorn library
# ------------------------------------
#
# Usage
# -----
#
# You can enforce a specific version, one of either MAJOR.MINOR.REVISION,
# MAJOR.MINOR or only MAJOR. If nothing is specified, the version won't
# be checked i.e. any version will be accepted. austinhorn does not consist of
# multiple components, so specifying COMPONENTS is not required.
#
# Example:
#   find_package( austinhorn )       // no specific version
#   find_package( austinhorn 0.2 )   // any 0.2 version
#   find_package( austinhorn 0.2.3 ) // version 0.2.3 or greater
#
# By default, the dynamic version of austinhorn will be found. To find the static
# version instead, you must set the AUSTINHORN_STATIC_LIBRARIES variable to TRUE
# before calling find_package( austinhorn ). In that case, AUSTINHORN_STATIC will also be
# defined by this script.
#
# Example:
#   set( AUSTINHORN_STATIC_LIBRARIES TRUE )
#   find_package( austinhorn )
#
# Since you have to link all of austinhorn's dependencies when you link austinhorn
# statically, the variable AUSTINHORN_DEPENDENCIES is also defined. See below
# for a detailed description.
#
# On Mac OS X if AUSTINHORN_STATIC_LIBRARIES is not set to TRUE then by default CMake
# will search for frameworks unless CMAKE_FIND_FRAMEWORK is set to "NEVER".
# Please refer to CMake documentation for more details.
# Moreover, keep in mind that austinhorn frameworks are only available as release
# libraries unlike dylibs which are available for both release and debug modes.
#
# If austinhorn is not installed in a standard path, you can use the AUSTINHORN_ROOT
# CMake (or environment) variable to tell CMake where to look for austinhorn.
#
# Output
# ------
#
# This script defines the following variables:
#   - AUSTINHORN_LIBRARY_DEBUG:   the path to the debug library
#   - AUSTINHORN_LIBRARY_RELEASE: the path to the release library
#   - AUSTINHORN_LIBRARY:         the path to the library to link to
#   - AUSTINHORN_FOUND:           TRUE if the austinhorn library is found
#   - AUSTINHORN_INCLUDE_DIR:     the path where austinhorn headers are located (the directory containing the austinhorn/Config.hpp file)
#   - AUSTINHORN_DEPENDENCIES:    the list of libraries austinhorn depends on, in case of static linking
#
# Example (dynamic linking):
#   find_package( austinhorn REQUIRED )
#   include_directories( ${AUSTINHORN_INCLUDE_DIR} )
#   add_executable( myapp ... )
#   target_link_libraries( myapp ${AUSTINHORN_LIBRARY} ... )
#
# Example (static linking):
#   set( AUSTINHORN_STATIC_LIBRARIES TRUE )
#   find_package( austinhorn REQUIRED )
#   include_directories( ${AUSTINHORN_INCLUDE_DIR} )
#   add_executable( myapp ... )
#   target_link_libraries( myapp ${AUSTINHORN_LIBRARY} ${AUSTINHORN_DEPENDENCIES} ... )

if( AUSTINHORN_STATIC_LIBRARIES )
	add_definitions( -DAUSTINHORN_STATIC )
endif()

set(
	AUSTINHORN_SEARCH_PATHS
	"${AUSTINHORN_ROOT}"
	"$ENV{AUSTINHORN_ROOT}"
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw
	/opt/local
	/opt/csw
	/opt
)

find_path(
	AUSTINHORN_INCLUDE_DIR
	austinhorn/Config.hpp
	PATH_SUFFIXES
		include
	PATHS
		${AUSTINHORN_SEARCH_PATHS}
)

set( AUSTINHORN_VERSION_OK true )
if( AUSTINHORN_FIND_VERSION AND AUSTINHORN_INCLUDE_DIR )
	file( READ "${AUSTINHORN_INCLUDE_DIR}/austinhorn/Config.hpp" AUSTINHORN_CONFIG_HPP )
	
	string( REGEX MATCH ".*#define AUSTINHORN_MAJOR_VERSION ([0-9]+).*#define AUSTINHORN_MINOR_VERSION ([0-9]+).*#define AUSTINHORN_REVISION_VERSION ([0-9]+).*" AUSTINHORN_CONFIG_HPP "${AUSTINHORN_CONFIG_HPP}" )
	string( REGEX REPLACE ".*#define AUSTINHORN_MAJOR_VERSION ([0-9]+).*" "\\1" AUSTINHORN_VERSION_MAJOR "${AUSTINHORN_CONFIG_HPP}" )
	string( REGEX REPLACE ".*#define AUSTINHORN_MINOR_VERSION ([0-9]+).*" "\\1" AUSTINHORN_VERSION_MINOR "${AUSTINHORN_CONFIG_HPP}" )
	string( REGEX REPLACE ".*#define AUSTINHORN_REVISION_VERSION ([0-9]+).*" "\\1" AUSTINHORN_VERSION_PATCH "${AUSTINHORN_CONFIG_HPP}" )
	
	math( EXPR AUSTINHORN_REQUESTED_VERSION "${AUSTINHORN_FIND_VERSION_MAJOR} * 10000 + ${AUSTINHORN_FIND_VERSION_MINOR} * 100 + ${AUSTINHORN_FIND_VERSION_PATCH}" )

	if( AUSTINHORN_VERSION_MAJOR OR AUSTINHORN_VERSION_MINOR OR AUSTINHORN_VERSION_PATCH )
		math( EXPR AUSTINHORN_VERSION "${AUSTINHORN_VERSION_MAJOR} * 10000 + ${AUSTINHORN_VERSION_MINOR} * 100 + ${AUSTINHORN_VERSION_PATCH}" )

		if( AUSTINHORN_VERSION LESS AUSTINHORN_REQUESTED_VERSION )
			set( AUSTINHORN_VERSION_OK false )
		endif()
	else()
		# AUSTINHORN version is < 0.3.0
		if( AUSTINHORN_REQUESTED_VERSION GREATER 300 )
			set( AUSTINHORN_VERSION_OK false )
			set( AUSTINHORN_VERSION_MAJOR 0 )
			set( AUSTINHORN_VERSION_MINOR x )
			set( AUSTINHORN_VERSION_PATCH y )
		endif()
	endif()
endif()

find_library(
	AUSTINHORN_LIBRARY_DYNAMIC_RELEASE
	NAMES
		austinhorn
		AUSTINHORN
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		${AUSTINHORN_SEARCH_PATHS}
)

find_library(
	AUSTINHORN_LIBRARY_DYNAMIC_DEBUG
	NAMES
		austinhorn-d
		AUSTINHORN-d
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		${AUSTINHORN_SEARCH_PATHS}
)

find_library(
	AUSTINHORN_LIBRARY_STATIC_RELEASE
	NAMES
		austinhorn-s
		AUSTINHORN-s
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		${AUSTINHORN_SEARCH_PATHS}
)

find_library(
	AUSTINHORN_LIBRARY_STATIC_DEBUG
	NAMES
		austinhorn-s-d
		AUSTINHORN-s-d
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		${AUSTINHORN_SEARCH_PATHS}
)

if( AUSTINHORN_STATIC_LIBRARIES )
	if( AUSTINHORN_LIBRARY_STATIC_RELEASE )
		set( AUSTINHORN_LIBRARY_RELEASE "${AUSTINHORN_LIBRARY_STATIC_RELEASE}" )
	endif()
	if( AUSTINHORN_LIBRARY_STATIC_DEBUG )
		set( AUSTINHORN_LIBRARY_DEBUG "${AUSTINHORN_LIBRARY_STATIC_DEBUG}" )
	endif()
else()
	if( AUSTINHORN_LIBRARY_DYNAMIC_RELEASE )
		set( AUSTINHORN_LIBRARY_RELEASE "${AUSTINHORN_LIBRARY_DYNAMIC_RELEASE}" )
	endif()
	if( AUSTINHORN_LIBRARY_DYNAMIC_DEBUG )
		set( AUSTINHORN_LIBRARY_DEBUG "${AUSTINHORN_LIBRARY_DYNAMIC_DEBUG}" )
	endif()
endif()

mark_as_advanced(
	AUSTINHORN_LIBRARY_STATIC_RELEASE
	AUSTINHORN_LIBRARY_STATIC_DEBUG
	AUSTINHORN_LIBRARY_DYNAMIC_RELEASE
	AUSTINHORN_LIBRARY_DYNAMIC_DEBUG
)

if( AUSTINHORN_LIBRARY_RELEASE OR AUSTINHORN_LIBRARY_DEBUG )
	if( AUSTINHORN_LIBRARY_RELEASE AND AUSTINHORN_LIBRARY_DEBUG )
		set( AUSTINHORN_LIBRARY debug "${AUSTINHORN_LIBRARY_DEBUG}" optimized "${AUSTINHORN_LIBRARY_RELEASE}" )
	elseif( AUSTINHORN_LIBRARY_DEBUG AND NOT AUSTINHORN_LIBRARY_RELEASE )
		set( AUSTINHORN_LIBRARY_RELEASE "${AUSTINHORN_LIBRARY_DEBUG}" )
		set( AUSTINHORN_LIBRARY "${AUSTINHORN_LIBRARY_DEBUG}" )
	elseif( AUSTINHORN_LIBRARY_RELEASE AND NOT AUSTINHORN_LIBRARY_DEBUG )
		set( AUSTINHORN_LIBRARY_DEBUG "${AUSTINHORN_LIBRARY_RELEASE}" )
		set( AUSTINHORN_LIBRARY "${AUSTINHORN_LIBRARY_RELEASE}" )
	endif()

	set( AUSTINHORN_FOUND true )
else()
	set( AUSTINHORN_LIBRARY "" )
	set( AUSTINHORN_FOUND false )
endif()

mark_as_advanced(
	AUSTINHORN_LIBRARY
	AUSTINHORN_LIBRARY_RELEASE
	AUSTINHORN_LIBRARY_DEBUG
)

if( AUSTINHORN_STATIC_LIBRARIES )
	set( AUSTINHORN_DEPENDENCIES )
	set( AUSTINHORN_MISSING_DEPENDENCIES )

	if( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux" )
		find_library( X11_LIBRARY NAMES X11 PATHS ${AUSTINHORN_SEARCH_PATHS} PATH_SUFFIXES lib )
		
		if( ${X11_LIBRARY} STREQUAL "X11_LIBRARY-NOTFOUND" )
			unset( X11_LIBRARY )
			set( AUSTINHORN_MISSING_DEPENDENCIES "${AUSTINHORN_MISSING_DEPENDENCIES} X11" )
		endif()
	endif()

	if( "${CMAKE_SYSTEM_NAME}" MATCHES "Windows" )
		set( AUSTINHORN_DEPENDENCIES ${AUSTINHORN_DEPENDENCIES} "opengl32" )
	elseif( ( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux" ) OR ( "${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD" ) )
		set( AUSTINHORN_DEPENDENCIES ${AUSTINHORN_DEPENDENCIES} "GL" ${X11_LIBRARY} )
	elseif( "${CMAKE_SYSTEM_NAME}" MATCHES "Darwin" )
		set( AUSTINHORN_DEPENDENCIES ${AUSTINHORN_DEPENDENCIES} "-framework OpenGL -framework Foundation" )
	endif()
endif()

if( NOT AUSTINHORN_INCLUDE_DIR OR NOT AUSTINHORN_LIBRARY )
	set( AUSTINHORN_FOUND false )
endif()

if( NOT AUSTINHORN_FOUND )
	set( FIND_AUSTINHORN_ERROR "AUSTINHORN not found." )
elseif( NOT AUSTINHORN_VERSION_OK )
	set( FIND_AUSTINHORN_ERROR "AUSTINHORN found but version too low, requested: ${AUSTINHORN_FIND_VERSION}, found: ${AUSTINHORN_VERSION_MAJOR}.${AUSTINHORN_VERSION_MINOR}.${AUSTINHORN_VERSION_PATCH}" )
	set( AUSTINHORN_FOUND false )
elseif( AUSTINHORN_STATIC_LIBRARIES AND AUSTINHORN_MISSING_DEPENDENCIES )
	set( FIND_AUSTINHORN_ERROR "AUSTINHORN found but some of its dependencies are missing: ${AUSTINHORN_MISSING_DEPENDENCIES}" )
	set( AUSTINHORN_FOUND false )
endif()

if( NOT AUSTINHORN_FOUND )
	if( AUSTINHORN_FIND_REQUIRED )
		message( FATAL_ERROR "${FIND_AUSTINHORN_ERROR}" )
	elseif( NOT AUSTINHORN_FIND_QUIETLY )
		message( "${FIND_AUSTINHORN_ERROR}" )
	endif()
else()
	if( AUSTINHORN_FIND_VERSION )
		message( STATUS "Found AUSTINHORN version ${AUSTINHORN_VERSION_MAJOR}.${AUSTINHORN_VERSION_MINOR}.${AUSTINHORN_VERSION_PATCH} in ${AUSTINHORN_INCLUDE_DIR}" )
	else()
		message( STATUS "Found AUSTINHORN in ${AUSTINHORN_INCLUDE_DIR}" )
	endif()
endif()
