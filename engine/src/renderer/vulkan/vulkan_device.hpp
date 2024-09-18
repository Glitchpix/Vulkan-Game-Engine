#pragma once
#include "defines.hpp"

#include <vulkan/vulkan.h>

class VulkanDevice {
public:
    VulkanDevice();
    ~VulkanDevice();

private:
    VkPhysicalDevice mPhysicalDevice;
    VkDevice mDevice;
};