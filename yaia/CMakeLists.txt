
include_directories(
        "${PROJECT_SOURCE_DIR}/libvideo/include"
        "${PROJECT_SOURCE_DIR}/bsl/3dparty/imgui"
        "${PROJECT_SOURCE_DIR}/bsl/ui/include"
        "${PROJECT_SOURCE_DIR}/bsl/gl/include"
)

add_executable(yaia main.cpp home.h app.h video_extractor_window.h new_project.h)

target_link_libraries( yaia imgui bsl_gl bsl_ui ${OpenCV_LIBS} ${LIBAV_LIBS})
