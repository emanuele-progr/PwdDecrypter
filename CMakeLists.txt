cmake_minimum_required(VERSION 3.12)
project(PswDecrypter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -lcrypt")
add_executable(PswDecrypter main.cpp Decrypter.cpp Decrypter.h)
target_link_libraries(PswDecrypter crypt)