cmake_minimum_required(VERSION 3.0.0)
project(MyTinyRenderer VERSION 0.1.0)

set(MY_TINY_RENDERER_SRCS 
src/main.cpp
src/tgaimage.cpp
src/tgaimage.h
src/geometry.h
src/model.h
src/model.cpp
)

add_executable(MyTinyRenderer ${MY_TINY_RENDERER_SRCS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
