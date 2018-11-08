cmake_minimum_required(VERSION 3.10)
project(MusicianAPP)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(MusicianAPP
        fft.cpp
        fft.h
        fftwave.cpp
        main.cpp
        makeunique.h
        wavread.cpp
        wavread.h)
