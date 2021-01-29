cmake_minimum_required(VERSION 3.5.1)
project(sdl2-opengl-pivot)

option(BUILD_DEBUG "Build a debug release" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(${BUILD_DEBUG})
	set(CMAKE_BUILD_TYPE Debug)
else()
	set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

#Add the include directories
include_directories("include")

#add the framework dependencies
add_library(lm SHARED IMPORTED)
add_library(tools SHARED IMPORTED)
add_library(ldtools SHARED IMPORTED)
add_library(dansdl2 SHARED IMPORTED)
add_library(dfw SHARED IMPORTED)

#executable
add_executable(pivot src/main.cpp ${SOURCE})
set_target_properties(pivot PROPERTIES RUNTIME_OUTPUT_DIRECTORY build)
add_custom_command(TARGET pivot POST_BUILD COMMAND cp -r ../data ./build)

IF(WIN32) 
	target_compile_definitions(pivot PUBLIC -DWINBUILD)
	
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../log/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../libdansdl2/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../libdansdl2-tools/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../tools/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../dfw/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../rapidjson/include/")

	set_target_properties(lm PROPERTIES IMPORTED_LOCATION "C:/Program Files (x86)/lm/lib/liblm.dll")
	set_target_properties(lm PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/lm/lib/liblm.dll")
	
	set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION "C:/Program Files (x86)/libdansdl2/lib/libdansdl2.dll")
	set_target_properties(dansdl2 PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/libdansdl2/lib/libdansdl2.dll")
	
	set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION "C:/Program Files (x86)/libldtools/lib/libldtools.dll")
	set_target_properties(ldtools PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/libldtools/lib/libldtools.dll")
	
	set_target_properties(tools PROPERTIES IMPORTED_LOCATION "C:/Program Files (x86)/tools/lib/libtools.dll")
	set_target_properties(tools PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/tools/lib/libtools.dll")
	
	set_target_properties(dfw PROPERTIES IMPORTED_LOCATION "C:/Program Files (x86)/dfw/lib/libdfw dll")
	set_target_properties(dfw PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/dfw/lib/libdfw.dll")

	target_link_libraries(pivot dfw tools dansdl2 ldtools tools lm SDL2 SDL2_ttf SDL2_mixer SDL2_image freeglut glew32 glu32 opengl32 stdc++fs)
	target_link_options(pivot PRIVATE -lmingw32 -lSDL2main -lSDL2 -mwindows)
ELSE()
	set_target_properties(tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libtools.so)
	set_target_properties(lm PROPERTIES IMPORTED_LOCATION /usr/local/lib/liblm.so)
	set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2.so)
	set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libldtools.so)
	set_target_properties(dfw PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdfw.so)

	target_link_libraries(pivot dfw tools dansdl2 ldtools tools lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs)
ENDIF()
