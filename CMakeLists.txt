cmake_minimum_required (VERSION 2.6)
project(drc)

include(CPack)
set(CPACK_PACKAGE_NAME "drc-02" CACHE STRING "Package name")
set(CPACK_DEBIAN_PACKAGE_VERSION "0.1" CACHE STRING "Version of the debian package")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libpocofoundation9, dhcom-hal" CACHE STRING "Debian package dependencies")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Peter Kishalov <pkishalov@dh-electronics.de>" CACHE STRING "Maintainer of the debian package")

add_subdirectory(C-API)
add_subdirectory(Tool)
add_subdirectory(Tests)
add_subdirectory(Examples)

find_file(BOOTLOADER_HOST bootloader_host DOC "bootloader-tool bin compiled with DHCOM_HAL library")
install(PROGRAMS 
	${BOOTLOADER_HOST} 
	Program-PSOC/program-psoc.sh
	DESTINATION bin
)

