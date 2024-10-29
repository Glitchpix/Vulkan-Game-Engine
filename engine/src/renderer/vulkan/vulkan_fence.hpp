#pragma once
#include "defines.hpp"
#include "vulkan/vulkan_core.h"

class VulkanFence {
public:
    VulkanFence(const VulkanFence&) = default;
    VulkanFence(VulkanFence&&) = default;
    VulkanFence& operator=(const VulkanFence&) = default;
    VulkanFence& operator=(VulkanFence&&) = default;
    VulkanFence(VkDevice device, bool signaled);
    ~VulkanFence();

    bool wait(size_t timeoutNs);

    void reset();

private:
    VkFence mHandle{};
    VkDevice mDevice{};
    bool mIsSignaled{};
};