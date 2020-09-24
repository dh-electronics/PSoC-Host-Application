# PSoC Host Application


This project includes the following subprojects

| Subproject      | Type           | Hosted                                                                   |
| --------------- |:--------------:| ------------------------------------------------------------------------:|
| Bootloader-Host | executeable    | git@gitlab.dh-electronics.com:drc02/psoc-bootloader-host-application.git |
| C-API           | shared library | git@gitlab.dh-electronics.com:drc02/psoc-host-application.git            |
| UI-Tool         | executeable    | git@gitlab.dh-electronics.com:drc02/psoc-host-application.git            |
| Tests           | executeable    | git@gitlab.dh-electronics.com:drc02/psoc-host-application.git            |
| Examples        | executeable    | git@gitlab.dh-electronics.com:drc02/psoc-host-application.git            |

## Project structure
```bash
Root of the PSoC Host Application project (check 'root' CMakeLists.txt)
├──Bootloader-Tool
│   ├───include
│   ├───src
|   └───CMakeLists.txt
├───C-API
│   ├───include
│   ├───src
|   └───CMakeLists.txt
├───UI-Tool
│   ├───include
│   ├───src
|   └───CMakeLists.txt
├───Tests
│   ├───include
│   ├───src
|   └───CMakeLists.txt
├───Examples
│   ├───include
│   ├───src
|   └───CMakeLists.txt
├─README.md
└─CMakeLists.txt
```

## Latest Build

[![pipeline status](https://dhplgl01/sw0028/psoc-host-application/badges/drc02/pipeline.svg)](https://dhplgl01/sw0028/psoc-host-application/commits/drc02)

- [Download Debian Packages *psoc-drc02*_armhf.deb][ci-artifacts-build-armhf]

[ci-artifacts-build-armhf]: https://dhplgl01/sw0028/psoc-host-application/-/jobs/artifacts/drc02/download?job=build-psoc-host-application-arm32v7

## Build

* build directory should be out of source tree (when using Eclipse IDE)
* create build directory
```bash
devel@deb10:/work$ mkdir build_psoc_host
```
* assume the project source code (see above) is locate within directory _psoc-host-application_
* run cmake to create Makefiles
```bash
devel@deb10:/work/build_psoc_host$ cmake -D CMAKE_TOOLCHAIN_FILE=../psoc-host-application/arm-linux-gnueabihf-toolchain.CMakeCross.txt CMakeLists.txt ../psoc-host-application/
```
* build binaries
```bash
devel@deb10:/work/build_psoc_host$ make
```

* create Debian packages
```bash
devel@deb10:/work/build_psoc_host$ cpack
```

## Optional for using Eclipse IDE**

* create Eclipse project files/settings
```bash
devel@Stretch:/work/build_psoc_host$ cmake -D CMAKE_TOOLCHAIN_FILE=../psoc-host-application/arm-linux-gnueabihf-toolchain.CMakeCrossSysroot.txt CMakeLists.txt -G "Eclipse CDT4 - Unix Makefiles" ../psoc-host-application/
```
