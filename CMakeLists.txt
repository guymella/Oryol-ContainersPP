#
# project: oryol-ContainersPP
#
cmake_minimum_required(VERSION 2.8)
get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

include_directories(src) 
project(ContainersPP)
fips_setup()

fips_add_subdirectory(src)
fips_finish()


