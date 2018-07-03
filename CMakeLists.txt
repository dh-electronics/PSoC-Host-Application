cmake_minimum_required (VERSION 2.6)
project(drc)

include(CPack)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Peter Kishalov <pkishalov@dh-electronics.de>" CACHE STRING "Maintainer of the Debian Package")
set(CPACK_DEBIAN_PACKAGE_VERSION "0.1" CACHE STRING "Version of the Debian Package")
set(CPACK_PACKAGE_NAME "drc-02" CACHE STRING "Package name")

add_subdirectory(C-API)
add_subdirectory(Tool)
add_subdirectory(Tests)
add_subdirectory(Examples)
