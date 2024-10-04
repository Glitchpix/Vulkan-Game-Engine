#include "vulkan_image.hpp"
#include "core/logger.hpp"
#include "vulkan/vulkan_core.h"
#include "vulkan_defines.inl"
#include "vulkan_device.hpp"
#include <stdexcept>

VulkanImage::VulkanImage(VulkanDevice& device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                         VkImageUsageFlags usageFlags, VkMemoryPropertyFlags memoryProperties)
    : mDevice{&device}, mWidth{width}, mHeight{height} {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = mWidth;
    imageInfo.extent.height = mHeight;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usageFlags;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;

    VK_CHECK(vkCreateImage(mDevice->get_logical_device(), &imageInfo, nullptr, &mHandle));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(mDevice->get_logical_device(), mHandle, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = find_memory_type_index(memRequirements.memoryTypeBits, memoryProperties);

    VK_CHECK(vkAllocateMemory(mDevice->get_logical_device(), &allocInfo, nullptr, &mMemory));

    VK_CHECK(vkBindImageMemory(mDevice->get_logical_device(), mHandle, mMemory, 0));

    create_image_view(format, VK_IMAGE_ASPECT_DEPTH_BIT);
}

VulkanImage::~VulkanImage() {
    if (mImageView != nullptr) {
        vkDestroyImageView(mDevice->get_logical_device(), mImageView, nullptr);
    }
    if (mMemory != nullptr) {
        vkFreeMemory(mDevice->get_logical_device(), mMemory, nullptr);
    }
    if (mHandle != nullptr) {
        vkDestroyImage(mDevice->get_logical_device(), mHandle, nullptr);
    }
}

void VulkanImage::create_image_view(VkFormat format, VkImageAspectFlags aspectFlags) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = mHandle;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    VK_CHECK(vkCreateImageView(mDevice->get_logical_device(), &viewInfo, nullptr, &mImageView));
}

uint32_t VulkanImage::find_memory_type_index(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties = mDevice->get_memory_properties();
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    MSG_FATAL("[Vulkan] Failed to find suitable image format!");
    throw std::runtime_error("Failed to find suitable memory type!");
}