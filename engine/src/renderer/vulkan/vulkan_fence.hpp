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

    [[nodiscard]] const VkFence& get_handle() const {
        return mHandle;
    };

private:
    VkFence mHandle{};
    VkDevice mDevice{};
    bool mIsSignaled{};
};