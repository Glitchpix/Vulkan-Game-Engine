
#include "vulkan_platform.hpp"

#if ENGINE_PLATFORM_WINDOWS
#include "platform/platform_win32.hpp"
#include "vulkan/vulkan_win32.h"

std::vector<const char*> vulkanplatform::get_platform_extensions() {
    std::vector<const char*> requiredExtensions{"VK_KHR_win32_surface"};
    return requiredExtensions;
}

VkSurfaceKHR vulkanplatform::create_platform_surface(Platform& platform, VkInstance instance) {
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hinstance = dynamic_cast<WindowsState*>(platform.getState())->h_instance;
    surfaceCreateInfo.hwnd = dynamic_cast<WindowsState*>(platform.getState())->hwnd;

    VkSurfaceKHR surface{nullptr};

    vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);
    return surface;
}

#endif