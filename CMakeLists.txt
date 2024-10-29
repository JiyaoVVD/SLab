cmake_minimum_required(VERSION 3.28)
project(slab)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

Set(EasyX_ROOT "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/VS")
#set(EasyX_ROOT "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS")
Set(EasyX_INCLUDE "${EasyX_ROOT}/include")
Set(EasyX_LIB "${EasyX_ROOT}/lib/x64")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

#find_package(TBB REQUIRED)

include(cmake/common.cmake)

set_common_project_settings(tbb)

include_directories(${EasyX_INCLUDE})
link_directories(${EasyX_LIB})

add_executable(slab main.cpp
        Camera.cpp
        Camera.h
        stype.h)

target_link_libraries(slab EasyXa EasyXw)
target_link_libraries(slab TBB::tbb Threads::Threads)
target_include_directories(slab PRIVATE ${CMAKE_SOURCE_DIR}/3rdParty/glm)
target_include_directories(slab PRIVATE ${CMAKE_SOURCE_DIR}/3rdParty/glm/gtc)

target_compile_options(slab PRIVATE ${TBB_CXX_STD_FLAG})