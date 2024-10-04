#pragma once

#include "defines.hpp"
#include "vulkan/vulkan_core.h"

class VulkanDevice;

class VulkanImage {
public:
    VulkanImage(VulkanDevice& device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                VkImageUsageFlags usageFlags, VkMemoryPropertyFlags memoryProperties);
    ~VulkanImage();

private:
    VkImage mHandle{nullptr};
    VkDeviceMemory mMemory{nullptr};
    VkImageView mImageView{nullptr};

    VulkanDevice* mDevice;

    uint32_t mWidth{0};
    uint32_t mHeight{0};

    void create_image_view(VkFormat format, VkImageAspectFlags aspectFlags);

    uint32_t find_memory_type_index(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};