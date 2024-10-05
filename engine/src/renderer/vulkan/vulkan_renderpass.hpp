#pragma once
#include "defines.hpp"
#include "vulkan/vulkan_core.h"

class RenderPass {
public:
    RenderPass(VkDevice device, VkFormat imageColorFormat, VkFormat imageDepthFormat);
    ~RenderPass();

private:
    VkDevice mDevice{nullptr};
    VkRenderPass mRenderpass{nullptr};
};