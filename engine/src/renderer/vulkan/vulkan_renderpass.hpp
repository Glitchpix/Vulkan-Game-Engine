#pragma once
#include "defines.hpp"
#include "vulkan/vulkan_core.h"

class VulkanSwapchain;
class RenderPass {
public:
    RenderPass(VkDevice device, const VulkanSwapchain& swapchain, VkRect2D renderArea, VkClearColorValue clearColor,
               VkClearDepthStencilValue depthStencil);
    ~RenderPass();

    void begin(VkFramebuffer framebuffer, VkCommandBuffer commandBuffer);
    void end(VkCommandBuffer commandBuffer);

private:
    VkDevice mDevice{nullptr};
    VkRenderPass mRenderpass{nullptr};
    VkRect2D mRenderArea{};
    VkClearColorValue mClearColor{};
    VkClearDepthStencilValue mDepthStencil{};
};