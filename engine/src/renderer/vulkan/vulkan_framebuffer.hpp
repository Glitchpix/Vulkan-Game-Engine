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
    VulkanFramebuffer(RenderPass* renderpass, VkExtent2D extent, uint32_t attachmentCount, VkImageView* attachments);
    ~VulkanFramebuffer();

    [[nodiscard]] VkExtent2D get_image_extent() const {
        return mImageExtent;
    };


    [[nodiscard]] const VkFramebuffer& get_handle() const {
        return mHandle;
    };

private:
    VkFramebuffer mHandle{nullptr};
    VkExtent2D mImageExtent{};
    uint32_t mAttachmentCount;
    VkImageView* mAttachments;  // TODO: refactor this to C++ style
    RenderPass* mRenderpass;
};