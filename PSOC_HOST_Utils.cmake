#
# CMake script hooks support
#
option(PSOC_HOST_DUMP_HOOKS_FLOW "Dump called DHCOM_HAL hooks" OFF)
macro(psoc_host_cmake_hook_append hook_name)
  set(__var_name "__PSOC_HOST_CMAKE_HOOKS_${hook_name}")
  set(__value "${${__var_name}}")
  message(STATUS "Registering hook '${hook_name}': ${ARGN}")
  list(APPEND __value ${ARGN})
  set(${__var_name} "${__value}" CACHE INTERNAL "")
endmacro()
macro(psoc_host_cmake_hook hook_name)
  set(__var_name "__PSOC_HOST_CMAKE_HOOKS_${hook_name}")
  if(PSOC_HOST_DUMP_HOOKS_FLOW)
    message(STATUS "Hook ${hook_name} ...")
  endif()
  foreach(__hook ${${__var_name}})
    #message(STATUS "Hook ${hook_name}: calling '${__hook}' ...")
    if(COMMAND "${__hook}")
      message(FATAL_ERROR "Indirect calling of CMake commands is not supported yet")
    else()
      include("${__hook}")
    endif()
  endforeach()
endmacro()
macro(psoc_host_cmake_reset_hooks)
  get_cmake_property(__variableNames VARIABLES)
  foreach(__variableName ${__variableNames})
    if(__variableName MATCHES "^__PSOC_HOST_CMAKE_HOOKS_")
      unset(${__variableName})
      unset(${__variableName} CACHE)
    endif()
  endforeach()
endmacro()
macro(psoc_host_cmake_hook_register_dir dir)
  file(GLOB hook_files RELATIVE "${dir}" "${dir}/*.cmake")
  foreach(f ${hook_files})
    if(f MATCHES "^(.+)\\.cmake$")
      set(hook_name "${CMAKE_MATCH_1}")
      psoc_host_cmake_hook_append(${hook_name} "${dir}/${f}")
    endif()
  endforeach()
endmacro()

# Search packages for the host system instead of packages for the target system
# in case of cross compilation these macros should be defined by the toolchain file
if(NOT COMMAND find_host_package)
  macro(find_host_package)
    find_package(${ARGN})
  endmacro()
endif()
if(NOT COMMAND find_host_program)
  macro(find_host_program)
    find_program(${ARGN})
  endmacro()
endif()

macro(git_describe var_name path)
  if(GIT_FOUND)
    execute_process(COMMAND "${GIT_EXECUTABLE}" describe --tags
      WORKING_DIRECTORY "${path}"
      OUTPUT_VARIABLE ${var_name}
      RESULT_VARIABLE GIT_RESULT
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT GIT_RESULT EQUAL 0)
      execute_process(COMMAND "${GIT_EXECUTABLE}" describe --always --dirty --long
        WORKING_DIRECTORY "${path}"
        OUTPUT_VARIABLE ${var_name}
        RESULT_VARIABLE GIT_RESULT
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      if(NOT GIT_RESULT EQUAL 0)  # --exclude is not supported by 'git'
        # match only tags with complete OpenCV versions (ignores -alpha/-beta/-rc suffixes)
        execute_process(COMMAND "${GIT_EXECUTABLE}" describe --all --always --dirty --match "[0-9].[0-9]*[0-9]"
          WORKING_DIRECTORY "${path}"
          OUTPUT_VARIABLE ${var_name}
          RESULT_VARIABLE GIT_RESULT
          ERROR_QUIET
          OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(NOT GIT_RESULT EQUAL 0)
          set(${var_name} "unknown")
        endif()
      endif()
    endif()
  else()
    set(${var_name} "unknown")
  endif()
endmacro()
