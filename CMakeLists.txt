cmake_minimum_required(VERSION 3.8)

project("iashow")

set(CMAKE_CXX_STANDARD 17)

set( LIBAV_INCLUDE_PATH "" CACHE PATH "The path containing libavcodec/avcodec.h")
set( LIBAV_LIBRARY_PATH "" CACHE PATH "The path containing avcodec.a or avcodec.lib")

set( LIBAV_LIBS avcodec avutil avformat swscale)

set( YAS_INCLUDE_PATH "" CACHE PATH "The path containing yas/serialize.hpp")

find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)

if(UNIX)
    set( MY_LIBS GL GLEW glfw atomic stdc++fs atomic )
    set( CMAKE_CXX_FLAGS -march=native )
else()
    set( MY_LIBS opengl32 glew32 glfw3)
endif()


include_directories(
    #"${LIBAV_INCLUDE_PATH}"
    "${OpenCV_INCLUDE_DIRS}"
        "${YAS_INCLUDE_PATH}"
)


link_directories(
    "${LIBAV_LIBRARY_PATH}"
)

enable_testing()


add_subdirectory("crazygl")
add_subdirectory("imgui")
add_subdirectory("libvideo")
add_subdirectory("yaia")