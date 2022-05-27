cmake_minimum_required(VERSION 3.14)
project(dynBufferTrial)

add_executable(bufTrial bufTrial.cpp)
target_compile_options(bufTrial PRIVATE -fsanitize=address)
target_link_options(bufTrial PRIVATE -fsanitize=address)