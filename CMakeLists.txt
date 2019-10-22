cmake_minimum_required (VERSION 3.0)
project(psoc-capi-drc02)

include(GNUInstallDirs)
include(CPackComponent)

#add_subdirectory(DHCOM_HAL)      # shared library
                                  # (uncomment if your sysroot does NOT
                                  # contain a DHCOM_HAL)

add_subdirectory(Bootloader-Host) # executeable
add_subdirectory(C-API)           # shared library
add_subdirectory(Tool)            # executeable
add_subdirectory(Tests)           # executeable
add_subdirectory(Examples)        # executeable

cpack_add_component(Libraries)
cpack_add_component(Namelinks GROUP Development DEPENDS Libraries)
cpack_add_component(Headers GROUP Development DEPENDS Libraries)

cpack_add_component_group(Development)

set(CPACK_PACKAGE_VERSION 9.5)

set(CPACK_GENERATOR "DEB")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Peter Kishalov <pkishalov@dh-electronics.com>, Ludwig Zenz <lzenz@dh-electronics.com>" CACHE STRING "Maintainer of the debian package")
set(CPACK_DEBIAN_PACKAGE_VERSION 9.5)
set(CPACK_DEBIAN_PACKAGE_ARCHITECURE armhf)
set(CPACK_PACKAGE_NAME "psoc-capi-drc02" CACHE STRING "Package name")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "PSoC Host Application, C-API: Library for psoc access")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>=2.19-18), dhcom-hal(>=1.1-2)" CACHE STRING "Debian package dependencies")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/DEBIAN_PACKAGE/changelog;${CMAKE_CURRENT_SOURCE_DIR}/DEBIAN_PACKAGE/preinst;${CMAKE_CURRENT_SOURCE_DIR}/DEBIAN_PACKAGE/postinst")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://www.dh-electronics.com")
include(CPack)
