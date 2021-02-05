# SPDX-License-Identifier: Apache-2.0

if(CONFIG_MICROXRCEDDSCLIENT)

zephyr_get_include_directories_for_lang_as_string(       C includes)
zephyr_get_system_include_directories_for_lang_as_string(C system_includes)
zephyr_get_compile_definitions_for_lang_as_string(       C definitions)
zephyr_get_compile_options_for_lang_as_string(           C options)

zephyr_get_include_directories_for_lang_as_string(       CXX includes_cxx)
zephyr_get_system_include_directories_for_lang_as_string(CXX system_includes_cxx)
zephyr_get_compile_definitions_for_lang_as_string(       CXX definitions_cxx)
zephyr_get_compile_options_for_lang_as_string(           CXX options_cxx)

set(external_project_cflags "${includes} -I${ZEPHYR_BASE}/include/posix ${definitions} ${options} ${system_includes}")
set(external_project_cxxflags "${includes_cxx} ${definitions_cxx} ${options_cxx} ${system_includes_cxx}" )

set(microxrceddsclient_src_dir   ${CMAKE_CURRENT_SOURCE_DIR}/../../../Micro-XRCE-DDS-Client)
set(microxrceddsclient_build_dir ${CMAKE_CURRENT_BINARY_DIR})

include(ExternalProject)

ExternalProject_Add(
    libmicroxrceddsclient_project
    PREFIX      ${microxrceddsclient_build_dir}
    SOURCE_DIR  ${microxrceddsclient_src_dir}
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
    CMAKE_CACHE_ARGS
        -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
        -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
        -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
        -DUCLIENT_PIC:BOOL=OFF
        -DUCLIENT_PROFILE_TCP:BOOL=OFF
        -DUCLIENT_PROFILE_UDP:BOOL=OFF
        -DUCLIENT_PROFILE_SERIAL:BOOL=OFF
        -DUCLIENT_PROFILE_DISCOVERY:BOOL=OFF
        -DUCLIENT_PROFILE_CUSTOM_TRANSPORT:BOOL=ON
        -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_CURRENT_SOURCE_DIR}/zephyr_toolchain.cmake
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}
        -DCMAKE_PREFIX_PATH:PATH=${CMAKE_CURRENT_BINARY_DIR}
    BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/lib/libmicroxrcedds_client.a
    BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/lib/libmicrocdr.a
    )

zephyr_library_import(libmicroxrceddsclient ${CMAKE_CURRENT_BINARY_DIR}/lib/libmicroxrcedds_client.a)
zephyr_library_import(libmicrocdr ${CMAKE_CURRENT_BINARY_DIR}/lib/libmicrocdr.a)

zephyr_interface_library_named(microxrceddsclient)
add_dependencies(microxrceddsclient libmicroxrceddsclient)
add_dependencies(microxrceddsclient libmicrocdr)
add_dependencies(microxrceddsclient libmicroxrceddsclient_project)
target_include_directories(microxrceddsclient INTERFACE ${microxrceddsclient_build_dir}/include)

endif()
