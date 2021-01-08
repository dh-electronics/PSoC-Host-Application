# PSoC Host Application

## Latest Build

[![pipeline status](https://dhplgl01/sw0028/psoc-host-application/badges/dev/drc02/master/pipeline.svg)](https://dhplgl01/sw0028/psoc-host-application/commits/dev/drc02/master)

- [Download Debian Packages *psoc-drc02*_armhf.deb][ci-artifacts-build-armhf]

[ci-artifacts-build-armhf]: https://dhplgl01/sw0028/psoc-host-application/-/jobs/artifacts/dev/drc02/master/download?job=build-psoc-host-application-arm32v7

## Documentation

[DOC_psoc-host-application.pdf (drc02)][ci-artifacts-build-doc]

[ci-artifacts-build-doc]: https://dhplgl01/sw0028/psoc-host-application/-/jobs/artifacts/dev/drc02/master/raw/DOC_psoc-host-application.pdf?job=build-latex-doc

## Git Submodules

This project includes the following subproject(s):

| Subproject           | Type           | Hosted at                               |
| -------------------- |:--------------:| ---------------------------------------:|
| PSoC-Bootloader-Host | executeable    | ../psoc-bootloader-host-application.git |


## Project structure
```bash
Root of the PSoC Host Application project (check 'root' CMakeLists.txt)
├──Bootloader-Tool (psoc-bootloader-tool) [git submodule]
│   ├───include
│   ├───src
|   └───CMakeLists.txt
├───C-API (libpsoc-drc02)
│   ├───include
│   ├───src
|   └───CMakeLists.txt
├───UI-Tool (psoc-ui-tool)
│   ├───include
│   ├───src
|   └───CMakeLists.txt
├───Tests (psoc-tests)
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

## Optional for using Eclipse IDE

* create Eclipse project files/settings
```bash
devel@Stretch:/work/build_psoc_host$ cmake -D CMAKE_TOOLCHAIN_FILE=../psoc-host-application/arm-linux-gnueabihf-toolchain.CMakeCross.txt CMakeLists.txt -G "Eclipse CDT4 - Unix Makefiles" ../psoc-host-application/
```
