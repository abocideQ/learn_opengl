cmake_minimum_required(VERSION 3.10.2)

include_directories(
        src/main/cpp/camera
        src/main/cpp/util
)

file(GLOB CAMERA_FILES
        src/main/cpp/*.cpp
        src/main/cpp/util/*.cpp
        src/main/cpp/camera/*.cpp
        )

add_library(CAMERA_LIB
        SHARED
        ${CAMERA_FILES}
        )

target_link_libraries(CAMERA_LIB
        EGL
        GLESv3
        log
        android)