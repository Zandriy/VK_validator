cmake_minimum_required (VERSION 3.1)

set(PRJ VK_validator)
project (${PRJ})

set(VULKAN_SDK_PATH $ENV{VULKAN_SDK_PATH})
link_directories(${VULKAN_SDK_PATH}/lib)

add_executable(${PRJ}
 main.cpp
 Info.h
 Info.cpp
)
target_include_directories(${PRJ} PRIVATE ${VULKAN_SDK_PATH}/include)
target_link_libraries(${PRJ} PRIVATE glfw vulkan)
set_target_properties(${PRJ} PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            CXX_STANDARD_REQUIRED ON
            )
