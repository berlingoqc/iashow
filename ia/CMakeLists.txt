include_directories(
        "${PROJECT_SOURCE_DIR}/libvideo/include"
)

set ( SRC src/py_wrap.cpp src/python_mat.cpp)

PYTHON_ADD_MODULE(pyvideo ${SRC})

set ( PY_IA "${CMAKE_CURRENT_SOURCE_DIR}/ia")

configure_file(${PY_IA}/showia.py ${CMAKE_CURRENT_BINARY_DIR}/showia.py COPYONLY)

target_link_libraries(pyvideo
        ${Boost_LIBRARIES}
        ${OpenCV_LIBRARIES}
        ${PYTHON_LIBRARIES}
        stdc++fs
        )