add_library(Engine_lib SHARED src/core/logger.cpp src/core/logger.hpp
                              src/core/asserts.hpp src/defines.hpp 
                              src/platform/platform.hpp src/platform/platform_win32.hpp src/platform/platform_win32.cpp
                              src/core/application.hpp src/core/application.cpp
                              src/entry.hpp src/game_types.hpp
                              src/core/e_memory.hpp src/core/e_memory.cpp
                              src/core/event.hpp src/core/event.cpp
                              src/core/input.hpp src/core/input.cpp
                              src/core/clock.hpp src/core/clock.cpp
                              src/renderer/renderer.hpp src/renderer/renderer.cpp
                              src/renderer/renderer_backend.hpp
                              src/renderer/vulkan/vulkan_defines.inl
                              src/renderer/vulkan/vulkan_backend.hpp src/renderer/vulkan/vulkan_backend.cpp
                              src/renderer/vulkan/vulkan_device.hpp src/renderer/vulkan/vulkan_device.cpp
                              src/renderer/vulkan/vulkan_swapchain.hpp src/renderer/vulkan/vulkan_swapchain.cpp
                              src/renderer/vulkan/vulkan_platform.hpp src/renderer/vulkan/vulkan_platform_win32.cpp
                              src/renderer/vulkan/vulkan_image.hpp src/renderer/vulkan/vulkan_image.cpp
                              src/renderer/vulkan/vulkan_renderpass.hpp src/renderer/vulkan/vulkan_renderpass.cpp
                              src/renderer/vulkan/vulkan_command_buffer.hpp src/renderer/vulkan/vulkan_command_buffer.cpp
                              src/renderer/vulkan/vulkan_framebuffer.hpp src/renderer/vulkan/vulkan_framebuffer.cpp
                              src/renderer/vulkan/vulkan_fence.hpp src/renderer/vulkan/vulkan_fence.cpp)
                              
target_include_directories(Engine_lib PRIVATE src/)

find_package(Vulkan REQUIRED)
target_include_directories(Engine_lib PRIVATE ${Vulkan_INCLUDE_DIR})
target_link_libraries(Engine_lib PRIVATE ${Vulkan_LIBRARIES})