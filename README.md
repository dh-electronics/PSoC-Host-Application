PSoC Host Application
=====================

This project includes the following subprojects

| Subproject      | Type           | Hosted                                                         |
| --------------- |:--------------:| --------------------------------------------------------------:|
| DHCOM_HAL       | shared library | SVN at file:///X:/prj/dhelectronics/DHCOM/sw/Repos             |
| Bootloader-Host | executeable    | SVN at file:///X:/prj/dhelectronics/DRCXX/sw/Repos             |
| C-API           | shared library | SVN at file:///X:/prj/dhelectronics/DRCXX/sw/Repos             |
| UI-Tool         | executeable    | SVN at file:///X:/prj/dhelectronics/DRCXX/sw/Repos             |
| Tests           | executeable    | SVN at file:///X:/prj/dhelectronics/DRCXX/sw/Repos             |
| Examples        | executeable    | SVN at file:///X:/prj/dhelectronics/DRCXX/sw/Repos             |

Build
-----

**Project structure**
```bash
Root of the PSoC Host Application project
├──"Bootloader-Tool"
│   ├───include
│   ├───src
|   └───CMakeLists.txt
├───"DHCOM_HAL"
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

**Build Steps**

* build directory should be out of source tree (when using Eclipse IDE)
* create build directory
```bash
devel@Stretch:/work$ mkdir build_psoc_host
```
* assume the project source code (see above) is locate within directory _psoc-host-application_
* run cmake to create Makefiles
```bash
devel@Stretch:/work/build_psoc_host$ cmake -D CMAKE_TOOLCHAIN_FILE=../psoc-host-application/arm-linux-gnueabihf-toolchain.CMakeCrossSysroot.txt CMakeLists.txt ../psoc-host-application/
```
* build binaries
```bash
devel@Stretch:/work/build_psoc_host$ make
```

**Optional for using Eclipse IDE**

* create Eclipse project files/settings
```bash
devel@Stretch:/work/build_psoc_host$ cmake -D CMAKE_TOOLCHAIN_FILE=../psoc-host-application/arm-linux-gnueabihf-toolchain.CMakeCrossSysroot.txt CMakeLists.txt -G "Eclipse CDT4 - Unix Makefiles" ../psoc-host-application/
```
