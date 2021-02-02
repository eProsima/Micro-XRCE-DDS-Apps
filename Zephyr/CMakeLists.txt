cmake_minimum_required(VERSION 3.13.1)

list(APPEND ZEPHYR_EXTRA_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/modules/libmicroxrcedds)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(microxrcedds_sample_app)

target_sources(app PRIVATE src/main.c src/HelloWorld.c src/microxrce_transports.c)
