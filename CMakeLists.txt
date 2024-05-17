cmake_minimum_required(VERSION 3.28)
project(slab)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

Set(EasyX_ROOT "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/VS")
Set(EasyX_INCLUDE "${EasyX_ROOT}/include")
Set(EasyX_LIB "${EasyX_ROOT}/lib/x64")

include_directories(${EasyX_INCLUDE})
link_directories(${EasyX_LIB})

add_subdirectory(math_utils)

add_executable(slab WIN32 main.cpp)

target_link_libraries(slab math_utils)
target_link_libraries(slab EasyXa EasyXw)
