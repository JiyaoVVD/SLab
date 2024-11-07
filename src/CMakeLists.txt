set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/glfw/include")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/libs/glfw/lib-vc2022")


# opengl
find_package(OpenGL REQUIRED)

include_directories(${GLFW_INCLUDE_DIR})

# Intel OneApi TBB https://github.com/oneapi-src/oneTBB
#include(${CMAKE_SOURCE_DIR}/cmake/common.cmake)
#set_common_project_settings(tbb)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(slab
        camera.cpp
        renderer.cpp
        main.cpp
        camera.h
        renderer.h
        framebuffer.cpp
        framebuffer.cpp
        vertex.cpp
        vertex.h
        vertex.h
)

target_link_libraries(slab OpenGL::GL)
#target_link_libraries(slab TBB::tbb Threads::Threads)
target_link_libraries(slab ${GLFW_LIB_DIR}/glfw3.lib)
target_include_directories(slab PRIVATE ${CMAKE_SOURCE_DIR}/libs/glm)
target_include_directories(slab PRIVATE ${CMAKE_SOURCE_DIR}/libs/glm/gtc)
target_include_directories(slab PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_options(slab PRIVATE ${TBB_CXX_STD_FLAG})