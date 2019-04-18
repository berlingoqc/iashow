cmake_minimum_required(VERSION 3.8)

project("iashow")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-config)

set(CMAKE_CXX_STANDARD 17)

set( LIBAV_INCLUDE_PATH "" CACHE PATH "The path containing libavcodec/avcodec.h")
set( LIBAV_LIBRARY_PATH "" CACHE PATH "The path containing avcodec.a or avcodec.lib")

set( LIBAV_LIBS avcodec avutil avformat swscale)

set( YAS_INCLUDE_PATH "" CACHE PATH "The path containing yas/serialize.hpp")

set (JSON_INCLUDE_PATH "" CACHE PATH "The path containing nlohmann/json.hpp")

find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)

if(UNIX)
    set( MY_LIBS GL GLEW glfw atomic stdc++fs atomic )
    set( CMAKE_CXX_FLAGS -march=native )
else()
    set( MY_LIBS opengl32 glew32 glfw3)
endif()

find_package(Catch2 REQUIRED)

find_package(PythonInterp)

if (PYTHONINTERP_FOUND)
    if(UNIX AND NOT APPLE)
        if (PYTHON_VERSION_MAJOR EQUAL 3)
            find_package(Boost COMPONENTS python3 numpy3)
            find_package(PythonInterp 3)
            find_package(PythonLibs 3 REQUIRED)
        endif()
    endif()
endif()

message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")


include_directories(
        "${Boost_INCLUDE_DIR}"
        "${PYTHON_INCLUDE_DIRS}"
        "${JSON_INCLUDE_PATH}"
)

link_libraries(
        "${Boost_LIBRARIES}"
        "${PYTHON_LIBRARIES}"
)

include_directories(
    #"${LIBAV_INCLUDE_PATH}"
    "${OpenCV_INCLUDE_DIRS}"
        "${YAS_INCLUDE_PATH}"
)


link_directories(
    "${LIBAV_LIBRARY_PATH}"
)

enable_testing()


add_subdirectory("bsl")
add_subdirectory("libvideo")
add_subdirectory("ia")
add_subdirectory("yaia")