FIND_PATH(ZLIB_INCLUDE_DIR zlib.h
	PATH
	    $ENV{ZLIB_ROOT}
	    ${ZLIB_ROOT}
    HINTS
        $ENV{ZLIB_ROOT}
        ${ZLIB_ROOT}
        $ENV{ZLIB_ROOT}/include
        ${ZLIB_ROOT}/include
)
MARK_AS_ADVANCED(ZLIB_INCLUDE_DIR)

FIND_LIBRARY(ZLIB_LIBRARY_DEBUG
    NAMES z zlibd zlibd.lib
    PATH
        $ENV{ZLIB_ROOT}
        ${ZLIB_ROOT}
    HINTS
	    $ENV{ZLIB_ROOT}/lib
	    ${ZLIB_ROOT}/lib
        $ENV{ZLIB_ROOT}/Debug
        ${ZLIB_ROOT}/Debug
)

FIND_LIBRARY(ZLIB_LIBRARY_RELEASE
    NAMES z zlib zlib.lib
    PATH
        $ENV{ZLIB_ROOT}
        ${ZLIB_ROOT}
    HINTS
		$ENV{ZLIB_ROOT}/lib
	    ${ZLIB_ROOT}/lib
        $ENV{ZLIB_ROOT}/Release
        ${ZLIB_ROOT}/Release
)
        
IF(ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY_DEBUG AND ZLIB_LIBRARY_RELEASE)
    SET(ZLIB_FOUND TRUE)
ELSE()
    message(${ZLIB_INCLUDE_DIR})
    message(${ZLIB_LIBRARY_DEBUG})
    message(${ZLIB_LIBRARY_RELEASE})
ENDIF()

IF(ZLIB_FOUND)
    IF (NOT ZLIB_FIND_QUIETLY)
        MESSAGE(STATUS "Found ZLIB")
    ENDIF()
ELSE()
    IF (ZLIB_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find ZLIB")
    ENDIF()
ENDIF()
