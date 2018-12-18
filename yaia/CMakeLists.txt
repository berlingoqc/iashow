
include_directories(
    "${PROJECT_SOURCE_DIR}/libvideo/include"
    "${PROJECT_SOURCE_DIR}/imgui"
    "${PROJECT_SOURCE_DIR}/crazygl"
)

add_executable(yaia main.cpp)


target_link_libraries( yaia imgui crazygl libvideo ${MY_LIBS} ${OpenCV_LIBS} ${LIBAV_LIBS})


file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
