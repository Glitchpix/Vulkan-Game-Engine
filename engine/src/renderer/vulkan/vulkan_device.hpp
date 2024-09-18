#pragma once
#include "defines.hpp"

#include <vulkan/vulkan.h>

class VulkanDevice {
public:
    VulkanDevice(VkInstance instance);
    ~VulkanDevice();

private:
    VkInstance mInstance;
    VkPhysicalDevice mPhysicalDevice;
    VkDevice mDevice;
};