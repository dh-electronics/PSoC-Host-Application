cmake_minimum_required (VERSION 3.13)
project(psoc-drc02)
message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message("CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
include(GNUInstallDirs)

set(RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/copyright")
set(RESOURCE_FILE_CHANGELOG "${CMAKE_SOURCE_DIR}/changelog")

install(FILES ${RESOURCE_FILE_LICENSE} ${RESOURCE_FILE_CHANGELOG}
    DESTINATION /usr/share/doc/libpsoc-drc02/
    COMPONENT lib
)

install(FILES ${RESOURCE_FILE_LICENSE} ${RESOURCE_FILE_CHANGELOG}
    DESTINATION /usr/share/doc/psoc-drc02-utils/
    COMPONENT bin
)

install(FILES ${RESOURCE_FILE_LICENSE} ${RESOURCE_FILE_CHANGELOG}
    DESTINATION /usr/share/doc/libpsoc-drc02-dev/
    COMPONENT dev
)

include(CPackComponent)

add_subdirectory(Bootloader-Host) # executeable

add_subdirectory(C-API)           # shared library
add_subdirectory(Tool)            # executeable
add_subdirectory(Tests)           # executeable
add_subdirectory(Examples)        # executeable

include(PSOC_HOST_Utils.cmake)

# ----------------------------------------------------------------------------
#  Autodetect if we are in a GIT repository
# ----------------------------------------------------------------------------
find_host_package(Git QUIET)

if(NOT DEFINED PSOC_HOST_VCSVERSION AND GIT_FOUND)
  git_describe(PSOC_HOST_VCSVERSION "${psoc-drc02_SOURCE_DIR}")
elseif(NOT DEFINED PSOC_HOST_VCSVERSION)
  # We don't have git:
  set(PSOC_HOST_VCSVERSION "unknown")
endif()
if(PSOC_HOST_VCSVERSION)
  message("")
  message(${PROJECT_NAME} " vcs version: " ${PSOC_HOST_VCSVERSION})
endif()

set(CPACK_GENERATOR "DEB")
set(PSOC_HOST_PACKAGE_NAME "psoc-drc02")
include(PSOC_HOST_Packaging.cmake)
