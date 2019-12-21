cmake_minimum_required(VERSION 3.12)
project(PswDecrypter)

set(CMAKE_CXX_STANDARD 11)

add_executable(PswDecrypter main.cpp Decrypter.cpp Decrypter.h)