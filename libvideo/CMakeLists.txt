
include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_subdirectory(test)

set ( HEAD include/video_parser.h include/tvshow_info.h)
set ( SRC src/cv_video_traveler.cpp)

add_library( libvideo STATIC ${SRC} ${HEAD})


