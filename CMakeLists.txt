# CMakeList.txt : CMake project for vulkan_guide, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
include(CMakePrintHelpers)
project("VulkanTest")

set(CMAKE_CXX_STANDARD 17)

set(GLFW_INCLUDE_DIR "C:/Users/Frost/Documents/Libraries/glfw-3.3.9.bin.WIN64/include")

set(GLFW_LIBRARY "C:/Users/Frost/Documents/Libraries/glfw-3.3.9.bin.WIN64/lib-vc2019/glfw3.lib")

set(GLM_LIBRARY "C:/Users/Frost/Documents/Libraries/glm")

set(STB_INCLUDE_PATH "C:/libs/stb")

set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(TEXTURES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/textures)
file(GLOB SHADERS ${SHADER_DIR}/*.vert ${SHADER_DIR}/*.frag)

add_executable(hello_cool main.cpp)

cmake_print_variables(CMAKE_BUILD_TYPE)

add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEXTURES_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/textures)

foreach(SHADER IN LISTS SHADERS)
    get_filename_component(FILENAME ${SHADER} LAST_EXT)
    string(REPLACE "." "" FILENAME ${FILENAME})
    add_custom_command(OUTPUT ${SHADER_DIR}/${FILENAME}.spv
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER} -o ${SHADER_DIR}/${FILENAME}.spv
        DEPENDS ${SHADER}
        COMMENT "Compiling ${FILENAME}")
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shaders/${FILENAME}.spv
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/shaders/${FILENAME}.spv ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shaders/${FILENAME}.spv
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/shaders/${FILENAME}.spv)
list(APPEND SPV_SHADERS ${SHADER_DIR}/${FILENAME}.spv)
list(APPEND SPV_SHADERS_COPY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shaders/${FILENAME}.spv)
endForeach()

add_custom_target(shaders ALL DEPENDS ${SPV_SHADERS})
add_custom_target(shaders_copy ALL DEPENDS ${SPV_SHADERS_COPY})
add_dependencies(hello_cool copy_resources shaders_copy shaders)
find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIR})
include_directories(${GLM_LIBRARY})
include_directories(${STB_INCLUDE_PATH})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

TARGET_LINK_LIBRARIES(hello_cool ${GLFW_LIBRARY} ${Vulkan_LIBRARIES})