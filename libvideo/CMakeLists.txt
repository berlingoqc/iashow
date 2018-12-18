

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

set ( SRC src/video_parser.cpp)

add_library( libvideo STATIC ${SRC})