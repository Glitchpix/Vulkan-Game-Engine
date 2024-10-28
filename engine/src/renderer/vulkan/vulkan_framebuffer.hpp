#pragma once
#include "defines.hpp"
#include "vulkan/vulkan_core.h"
#include "vulkan_renderpass.hpp"

class VulkanFramebuffer {
public:
    VulkanFramebuffer(const VulkanFramebuffer&) = default;
    VulkanFramebuffer(VulkanFramebuffer&&) = default;
    VulkanFramebuffer& operator=(const VulkanFramebuffer&) = default;
    VulkanFramebuffer& operator=(VulkanFramebuffer&&) = default;
    VulkanFramebuffer(RenderPass* renderpass, uint32_t width, uint32_t height, uint32_t attachmentCount,
                      VkImageView* attachments);
    ~VulkanFramebuffer();

private:
    VkFramebuffer mHandle{nullptr};
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mAttachmentCount;
    VkImageView* mAttachments;  // TODO: refactor this to C++ style
    RenderPass* mRenderpass;
};