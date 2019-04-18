
include_directories(
        "${PROJECT_SOURCE_DIR}/libvideo/include"
)

add_executable(libvideo_test_serialization test_serialization.cpp)
target_link_libraries(libvideo_test_serialization libvideo Catch2::Catch2 stdc++fs ${OpenCV_LIBS} )


include(CTest)
include(Catch)
catch_discover_tests(libvideo_test_serialization)