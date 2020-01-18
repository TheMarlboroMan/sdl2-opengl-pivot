cmake_minimum_required(VERSION 3.5.1)
project(sdl2-opengl-pivot)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#includes
set(LOG_INC_DIR "${PROJECT_SOURCE_DIR}/../log/include/" CACHE STRING "Log include dir")
set(DANSDL2_INC_DIR "${PROJECT_SOURCE_DIR}/../libdansdl2/include/" CACHE STRING "dansdl2 include dir")
set(DANSDL2_TOOLS_INC_DIR "${PROJECT_SOURCE_DIR}/../libdansdl2-tools/include/" CACHE STRING "dansdl2-tools include dir")
set(TOOLS_INC_DIR "${PROJECT_SOURCE_DIR}/../tools/include/" CACHE STRING "tools include dir")
set(DFW_INC_DIR "${PROJECT_SOURCE_DIR}/../dfw/include/" CACHE STRING "dfw include dir") 
include_directories(${LOG_INC_DIR} ${DANSDL2_INC_DIR} ${DANSDL2_TOOLS_INC_DIR} ${TOOLS_INC_DIR} ${DFW_INC_DIR})

#source
add_subdirectory("${PROJECT_SOURCE_DIR}/src")

#libraries... added manually. Sorry, that's life.
set(LOG_LIB_LOCATION "${PROJECT_SOURCE_DIR}/../log/build/liblm.a" CACHE STRING "log static lib location")
add_library(lm STATIC IMPORTED)
set_target_properties(lm PROPERTIES IMPORTED_LOCATION ${LOG_LIB_LOCATION})

set(TOOLS_LIB_LOCATION "${PROJECT_SOURCE_DIR}/../tools/build/libtools.a" CACHE STRING "tools static lib location")
add_library(tools STATIC IMPORTED)
set_target_properties(tools PROPERTIES IMPORTED_LOCATION ${TOOLS_LIB_LOCATION})

set(DANSDL2_LIB_LOCATION "${PROJECT_SOURCE_DIR}/../libdansdl2/build/libdansdl2.a" CACHE STRING "dansdl2 static lib location")
add_library(dansdl2 STATIC IMPORTED)
add_dependencies(dansdl2 lm)
set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION ${DANSDL2_LIB_LOCATION})

set(DANSDL2_TOOLS_LIB_LOCATION "${PROJECT_SOURCE_DIR}/../libdansdl2-tools/build/libdansdl2-tools.a" CACHE STRING "dansdl2-tools static lib location")
add_library(dansdl2-tools STATIC IMPORTED)
add_dependencies(dansdl2-tools lm tools dansdl2)
set_target_properties(dansdl2-tools PROPERTIES IMPORTED_LOCATION ${DANSDL2_TOOLS_LIB_LOCATION})

set(DFW_LIB_LOCATION "${PROJECT_SOURCE_DIR}/../dfw/build/libdfw.a" CACHE STRING "dfw static lib location")
add_library(dfw STATIC IMPORTED)
add_dependencies(dfw lm tools dansdl2 dansdl2-tools)
set_target_properties(dfw PROPERTIES IMPORTED_LOCATION ${DFW_LIB_LOCATION})

#executable
add_executable(a src/main.cpp ${SOURCE})
target_link_libraries(a dfw lm tools dansdl2 dansdl2-tools SDL2 SDL2_ttf SDL2_mixer SDL2_image GL)
