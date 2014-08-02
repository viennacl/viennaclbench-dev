find_path(
    zlibstatic_INCLUDE_DIR
    zlib.h
    HINTS "${PROJECT_SOURCE_DIR}/external/zlib/"
)

set(zlibstatic_LIB_SEARCH_PATH
     ${zlibstatic_LIB_SEARCH_PATH}
 "${PROJECT_SOURCE_DIR}/external/zlib/"
 "${PROJECT_SOURCE_DIR}/external/zlib/Release"
 "${PROJECT_SOURCE_DIR}/external/zlib/Debug"
)

find_library(
    zlibstatic_LIBRARY
    NAMES zlibstatic libzlib zlib
    HINTS 
 "${PROJECT_SOURCE_DIR}/external/zlib/"
"${PROJECT_SOURCE_DIR}/external/zlib/Release"
"${PROJECT_SOURCE_DIR}/external/zlib/Debug"
    )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  zlibstatic
  DEFAULT_MSG
  zlibstatic_LIBRARY zlibstatic_INCLUDE_DIR
  )

set(zlibstatic_INCLUDE_DIRS ${zlibstatic_INCLUDE_DIR})

if(zlibstatic_FOUND)
  set(zlibstatic_INCLUDE_DIRS  ${zlibstatic_INCLUDE_DIR})
  set(zlibstatic_LIBRARIES ${zlibstatic_LIBRARY})
else(zlibstatic_FOUND)
  set(zlibstatic_INCLUDE_DIRS)
  set(zlibstatic_LIBRARIES)
endif(zlibstatic_FOUND)

mark_as_advanced(zlibstatic_INCLUDE_DIRS)
