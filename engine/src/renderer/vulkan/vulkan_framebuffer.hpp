#pragma once
#include "defines.hpp"
#include "vulkan/vulkan_core.h"
#include "vulkan_renderpass.hpp"

class VulkanFramebuffer {
public:
    VulkanFramebuffer(RenderPass* renderpass, int width, int height, int attachmentCount, VkImageView attachments);
    ~VulkanFramebuffer();

private:
    VkFramebuffer mHandle;
    int mWidth;
    int mHeight;
    int mAttachmentCount;
    VkImageView mAttachments;
    RenderPass* mRenderpass;
};