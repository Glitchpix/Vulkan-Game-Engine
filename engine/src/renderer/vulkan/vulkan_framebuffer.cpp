#include "vulkan_framebuffer.hpp"
#include "core/logger.hpp"
#include "vulkan_defines.inl"

VulkanFramebuffer::VulkanFramebuffer(RenderPass* renderpass, int width, int height, int attachmentCount,
                                     VkImageView attachments)
    : mWidth{width}, mHeight{height}, mAttachmentCount{attachmentCount}, mAttachments{attachments},
      mRenderpass{renderpass} {

    // Creation info
    VkFramebufferCreateInfo framebuffer_create_info;
    framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_create_info.renderPass = mRenderpass->get_handle();
    framebuffer_create_info.attachmentCount = mAttachmentCount;
    framebuffer_create_info.pAttachments = &mAttachments;
    framebuffer_create_info.width = mWidth;
    framebuffer_create_info.height = mHeight;
    framebuffer_create_info.layers = 1;
    VK_CHECK(vkCreateFramebuffer(mRenderpass->get_device(), &framebuffer_create_info, nullptr, &mHandle));

    MSG_TRACE("[Vulkan] Vulkan Framebuffer: {:p} initialized", static_cast<void*>(this));
}

VulkanFramebuffer::~VulkanFramebuffer() {
    vkDestroyFramebuffer(mRenderpass->get_device(), mHandle, nullptr);
    MSG_TRACE("[Vulkan] Vulkan Framebuffer: {:p} destroyed", static_cast<void*>(this));
}