psoc_host_cmake_hook(INIT_CPACK)
if(NOT EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
  message(STATUS "CPack is not found. SKIP")
  return()
endif()

set(CPACK_set_DESTDIR "on")

if(NOT PSOC_HOST_CUSTOM_PACKAGE_INFO)
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "DHCOM Hardware Abstraction Library")
  set(CPACK_PACKAGE_DESCRIPTION
"The DHCOM Hardware Abstraction Library provides access to commen hardware interfaces. The aim is to
provide a interface that is useable on multiple DHCOM boards.")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "DH electronics GmbH <embedded@dh-electronics.com>" CACHE STRING "Maintainer of the Debian Package")
  set(CPACK_PACKAGE_VERSION "${PSOC_HOST_VCSVERSION}")
endif(NOT PSOC_HOST_CUSTOM_PACKAGE_INFO)

#arch
if(X86)
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
elseif(X86_64)
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
elseif(ARM)
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "armhf")
elseif(AARCH64)
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "arm64")
else()
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
endif()

message("Package cpu architecture: " ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})

if(CPACK_GENERATOR STREQUAL "DEB")
  set(PSOC_HOST_PACKAGE_ARCH_SUFFIX ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})
else()
  set(PSOC_HOST_PACKAGE_ARCH_SUFFIX ${CMAKE_SYSTEM_PROCESSOR})
endif()

set(PSOC_HOST_PACKAGE_ARCH_SUFFIX "armhf")
message("WARNING: Target CPU arch is hardcoded (fails on ELBE SDK)")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "armhf")
set(CPACK_DEBIAN_LIB_PACKAGE_ARCHITECTURE "armhf")
set(CPACK_DEBIAN_DEV_PACKAGE_ARCHITECTURE "all")
set(CPACK_DEBIAN_BIN_PACKAGE_ARCHITECTURE "armhf")

#deb options
set(CPACK_DEB_COMPONENT_INSTALL   TRUE)
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SECTION  "libs")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://www.dh-electronics.com")

#display names
set(CPACK_COMPONENT_DEV_DISPLAY_NAME     "Development files")
set(CPACK_COMPONENT_LIB_DISPLAY_NAME     "Library")
set(CPACK_COMPONENT_BIN_DISPLAY_NAME     "Utilities")

#depencencies
# set(CPACK_DEBIAN_LIB_PACKAGE_SHLIBDEPS TRUE) # fails because of libdhcom-hal
set(CPACK_COMPONENT_LIB_REQUIRED TRUE)
set(CPACK_COMPONENT_BIN_DEPENDS lib)
set(CPACK_COMPONENT_DEV_DEPENDS lib)

set(CPACK_COMPONENT_LIB_CONFLICTS "lib${PSOC_HOST_PACKAGE_NAME}")
set(CPACK_COMPONENT_LIB_PROVIDES  "lib${PSOC_HOST_PACKAGE_NAME}")
set(CPACK_COMPONENT_LIB_REPLACES  "lib${PSOC_HOST_PACKAGE_NAME}")

set(CPACK_COMPONENT_DEV_CONFLICTS "lib${PSOC_HOST_PACKAGE_NAME}-dev")
set(CPACK_COMPONENT_DEV_PROVIDES  "lib${PSOC_HOST_PACKAGE_NAME}-dev")
set(CPACK_COMPONENT_DEV_REPLACES  "lib${PSOC_HOST_PACKAGE_NAME}-dev")

set(CPACK_COMPONENT_BIN_CONFLICTS "${PSOC_HOST_PACKAGE_NAME}-utils")
set(CPACK_COMPONENT_BIN_PROVIDES  "${PSOC_HOST_PACKAGE_NAME}-utils")
set(CPACK_COMPONENT_BIN_REPLACES  "${PSOC_HOST_PACKAGE_NAME}-utils")

set(CPACK_COMPONENT_LIB_DESCRIPTION "PSOC-C-API: Library and utility for PSoC access an DHCOM DRC02")
set(CPACK_DEBIAN_COMPONENT_LIB_NAME "lib${PSOC_HOST_PACKAGE_NAME}")
set(CPACK_DEBIAN_LIB_PACKAGE_NAME "${CPACK_DEBIAN_COMPONENT_LIB_NAME}")
set(CPACK_DEBIAN_LIB_FILE_NAME "${CPACK_DEBIAN_COMPONENT_LIB_NAME}_${PSOC_HOST_VCSVERSION}_${PSOC_HOST_PACKAGE_ARCH_SUFFIX}.deb")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.4), libgcc1 (>= 1:3.5), libgpiod2 (>= 1.1), libstdc++6 (>= 5.2), libdhcom-hal (>= 1.4)")

set(CPACK_COMPONENT_BIN_DESCRIPTION "PSOC-C-API: Utilities for PSoC access an DHCOM DRC02")
set(CPACK_DEBIAN_COMPONENT_BIN_NAME "${PSOC_HOST_PACKAGE_NAME}-utils")
set(CPACK_DEBIAN_BIN_PACKAGE_SECTION  "utils")
set(CPACK_DEBIAN_BIN_PACKAGE_NAME "${CPACK_DEBIAN_COMPONENT_BIN_NAME}")
set(CPACK_DEBIAN_BIN_FILE_NAME "${PSOC_HOST_PACKAGE_NAME}-utils_${PSOC_HOST_VCSVERSION}_${PSOC_HOST_PACKAGE_ARCH_SUFFIX}.deb")
set(CPACK_DEBIAN_BIN_PACKAGE_DEPENDS "${CPACK_DEBIAN_LIB_PACKAGE_NAME} (= ${CPACK_PACKAGE_VERSION})")

set(CPACK_COMPONENT_DEV_DESCRIPTION "Development files PSOC-C-API: Library and utility for psoc access")
set(CPACK_DEBIAN_COMPONENT_DEV_NAME "lib${PSOC_HOST_PACKAGE_NAME}-dev")
set(CPACK_DEBIAN_DEV_PACKAGE_SECTION  "libdevel")
set(CPACK_DEBIAN_DEV_PACKAGE_NAME "${CPACK_DEBIAN_COMPONENT_DEV_NAME}")
set(CPACK_DEBIAN_DEV_FILE_NAME "${PSOC_HOST_PACKAGE_NAME}-dev_${PSOC_HOST_VCSVERSION}_${PSOC_HOST_PACKAGE_ARCH_SUFFIX}.deb")
set(CPACK_DEBIAN_DEV_PACKAGE_DEPENDS "${CPACK_DEBIAN_LIB_PACKAGE_NAME} (= ${CPACK_PACKAGE_VERSION})")

set(CPACK_COMPONENTS_ALL lib bin dev)

psoc_host_cmake_hook(PRE_CPACK)
include(CPack)
psoc_host_cmake_hook(POST_CPACK)
