#pragma once
#include "defines.hpp"
#include "vulkan/vulkan_core.h"

class VulkanFence {
public:
    VulkanFence(const VulkanFence&) = delete;
    VulkanFence(VulkanFence&&) = default;
    VulkanFence& operator=(const VulkanFence&) = delete;
    VulkanFence& operator=(VulkanFence&&) = delete;
    VulkanFence(VkDevice device, bool signaled);
    ~VulkanFence();

    bool wait(size_t timeoutNs);

    void reset();

    [[nodiscard]] const VkFence& get_handle() const {
        return mHandle;
    };

private:
    VkFence mHandle{nullptr};
    VkDevice mDevice{nullptr};
    bool mIsSignaled{};
};