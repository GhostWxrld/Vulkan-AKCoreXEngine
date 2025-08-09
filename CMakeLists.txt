cmake_minimum_required(VERSION 3.15)
project("Vulkan AKCoreXEngine")

set(CMAKE_CXX_STANDARD 23)

include_directories(Headers)
include_directories("E:/VulkanSDK/Include")
include_directories("D:/VULKAN DEV ENVIRONMENT/GLFW64/glfw-3.3.9.bin.WIN64/include")
include_directories("D:/VULKAN DEV ENVIRONMENT/glm-1.0.1/glm-1.0.1")

link_directories("E:/VulkanSDK/Lib")
link_directories("D:/VULKAN DEV ENVIRONMENT/GLFW64/glfw-3.3.9.bin.WIN64/lib-mingw-w64")

file(GLOB SRC_FILES "Source/*.cpp")

add_executable(AKCoreXEngine ${SRC_FILES})
target_link_libraries(AKCoreXEngine vulkan-1)



