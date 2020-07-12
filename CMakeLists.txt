cmake_minimum_required(VERSION 3.0.0)
project(MyTinyRenderer VERSION 0.1.0)

set(MY_TINY_RENDERER_SRCS src/main
src/tgaimage.cpp
src/tgaimage
)

add_executable(MyTinyRenderer ${MY_TINY_RENDERER_SRCS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
