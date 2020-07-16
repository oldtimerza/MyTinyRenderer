cmake_minimum_required(VERSION 3.0.0)
project(MyTinyRenderer VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(MY_TINY_RENDERER_SRCS 
src/main.cpp
src/file/tgaimage.cpp
src/file/tgaimage.h
src/render/buffer.h
src/render/buffer.cpp
src/render/geometry.h
src/render/geometry.cpp
src/render/model.h
src/render/model.cpp
src/render/transform.h
)

add_executable(MyTinyRenderer ${MY_TINY_RENDERER_SRCS})

set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 99 CXX_STANDARD 11)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
    ${SDL2_LIBRARY}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
