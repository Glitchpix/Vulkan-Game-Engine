#pragma once
#include "defines.hpp"
#include "platform/platform.hpp"
#include "vulkan/vulkan_core.h"
#include <vector>

namespace vulkanplatform {
    std::vector<const char*> get_platform_extensions();

    VkSurfaceKHR create_platform_surface(Platform& platform, VkInstance vulkanInstance);
}
