project(Rack)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard
set(CMAKE_CXX_FLAGS "-std=c++1y -stdlib=libc++ -Wall")


include_directories(/Applications/Xcode.app/Contents/Developer/usr/include)
include_directories(./include)
include_directories(./include/app)
include_directories(./include/dsp)
include_directories(./include/engine)
include_directories(./include/plugin)
include_directories(./include/simd)
include_directories(./include/ui)
include_directories(./include/widget)


include_directories(./dep/include)
include_directories(./dep/jansson-2.10/src)
include_directories(./dep/glew-2.1.0/include)
include_directories(./dep/libzip-1.2.0/lib)
include_directories(./dep/glfw/include)
include_directories(./ext/nanovg/src)

file(GLOB SOURCE_FILES
        ./plugins/Fundamental/src/*.cpp
        ./plugins/BogaudioModules/src/*.cpp
        ./plugins/BogaudioModules/src/dsp/*.cpp
        ./plugins/ImpromptuModular/src/*.cpp
        ./plugins/ImpromptuModular/src/*/*.cpp
        ./plugins/ImpromptuModular/src/*/*/*.cpp
        ./plugins/LOGinstruments/src/*.cpp
        ./plugins/LOGinstruments/src/kiss_fft130/*.c
        ./src/*.cpp
        ./src/*/*.cpp ./src/*/*/*.cpp
        ./src/*/*/*/*.cpp
        ./src/*.c ./src/*/*.c
        ./src/*/*/*.c ./src/*/*/*/*.c)

add_executable(Rack ${SOURCE_FILES} include/timer.hpp src/blendish.cpp)

target_compile_definitions(Rack PRIVATE ARCH_MAC=1)
