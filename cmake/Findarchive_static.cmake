find_path(
    archive_static_INCLUDE_DIR
    libarchive/archive_entry.h
    HINTS "${PROJECT_SOURCE_DIR}/external/libarchive/"
)

set(archive_static_LIB_SEARCH_PATH
     ${archive_static_LIB_SEARCH_PATH}
 "${PROJECT_SOURCE_DIR}/external/libarchive/"
 "${PROJECT_SOURCE_DIR}/external/libarchive/Release"
 "${PROJECT_SOURCE_DIR}/external/libarchive/Debug"
"${PROJECT_SOURCE_DIR}/external/libarchive/libarchive/Release"
"${PROJECT_SOURCE_DIR}/external/libarchive/libarchive/Debug"
)

find_library(
    archive_static_LIBRARY
    NAMES archive_static libarchive_static
    HINTS 
 "${PROJECT_SOURCE_DIR}/external/libarchive/"
"${PROJECT_SOURCE_DIR}/external/libarchive/libarchive/Release"
"${PROJECT_SOURCE_DIR}/external/libarchive/libarchive/Debug"
    )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  archive_static
  DEFAULT_MSG
  archive_static_LIBRARY archive_static_INCLUDE_DIR
  )

set(archive_static_INCLUDE_DIRS ${archive_static_INCLUDE_DIR})

if(archive_static_FOUND)
  set(archive_static_INCLUDE_DIRS 
       ${archive_static_INCLUDE_DIR})
  set(archive_static_LIBRARIES ${archive_static_LIBRARY})
else(archive_static_FOUND)
  set(archive_static_INCLUDE_DIRS)
  set(archive_static_LIBRARIES)
endif(archive_static_FOUND)

mark_as_advanced(archive_static_INCLUDE_DIRS)
