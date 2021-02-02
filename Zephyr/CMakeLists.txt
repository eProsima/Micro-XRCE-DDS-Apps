# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(microxrceddsapp C CXX)

FILE(GLOB app_sources src/*.c)

target_sources(app PRIVATE ${app_sources})

zephyr_get_include_directories_for_lang_as_string(       C includes)
zephyr_get_system_include_directories_for_lang_as_string(C system_includes)
zephyr_get_compile_definitions_for_lang_as_string(       C definitions)
zephyr_get_compile_options_for_lang_as_string(           C options)

zephyr_get_include_directories_for_lang_as_string(       CXX includes_cxx)
zephyr_get_system_include_directories_for_lang_as_string(CXX system_includes_cxx)
zephyr_get_compile_definitions_for_lang_as_string(       CXX definitions_cxx)
zephyr_get_compile_options_for_lang_as_string(           CXX options_cxx)

set(external_project_cflags
    "${includes} ${definitions} ${options} ${system_includes}"
    )

set(external_project_cxxflags
    "${includes_cxx} ${definitions_cxx} ${options_cxx} ${system_includes_cxx}" 
    )

include(ExternalProject)

set(microxrceclient_src_dir   ${CMAKE_CURRENT_SOURCE_DIR}/../Micro-XRCE-DDS-Client)
set(microxrceclient_build_dir ${CMAKE_CURRENT_BINARY_DIR}/microxrceclient_extensions)

if(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
set(submake "$(MAKE)")
else()
set(submake "make")
endif()

ExternalProject_Add(
    libmicroxrceclient_project
    PREFIX      ${microxrceclient_build_dir}
    SOURCE_DIR  ${microxrceclient_src_dir}
    INSTALL_DIR ${microxrceclient_build_dir}/temp_install
    CMAKE_CACHE_ARGS  
        -DCMAKE_INSTALL_PREFIX:PATH=${microxrceclient_build_dir}
        -DUCLIENT_PROFILE_TCP=OFF
        -DUCLIENT_PROFILE_SERIAL=OFF
    )

file (MAKE_DIRECTORY ${microxrceclient_build_dir}/include)
add_library (libmicroxrceclient STATIC IMPORTED GLOBAL)
set_target_properties (libmicroxrceclient PROPERTIES
  IMPORTED_LOCATION ${microxrceclient_build_dir}/lib/libserial.a
  INTERFACE_INCLUDE_DIRECTORIES ${microxrceclient_build_dir}/include)
add_dependencies(libmicroxrceclient libmicroxrceclient_project)

# zephyr_interface_library_named(microxrceclient)
# zephyr_library_import(libmicroxrceclient ${microxrceclient_build_dir}/lib/libmicroros.a)
# add_dependencies(microxrceclient libmicroxrceclient)
# add_dependencies(microxrceclient libmicroxrceclient_project)
# target_include_directories(microxrceclient INTERFACE ${microxrceclient_build_dir}/include)

target_link_libraries(app PUBLIC libmicroxrceclient)
