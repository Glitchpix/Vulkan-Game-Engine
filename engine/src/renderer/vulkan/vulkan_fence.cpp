#include "vulkan_fence.hpp"
#include "core/logger.hpp"
#include "vulkan_defines.inl"


VulkanFence::VulkanFence(VkDevice device, bool signaled) : mDevice{device}, mIsSignaled{signaled} {
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    if (mIsSignaled) {
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }
    VK_CHECK(vkCreateFence(mDevice, &fenceCreateInfo, nullptr, &mHandle));
}
VulkanFence::~VulkanFence() {
    if (mHandle != nullptr) {
        vkDestroyFence(mDevice, mHandle, nullptr);
    }
}

bool VulkanFence::wait(size_t timeoutNs) {
    if (mIsSignaled) {
        return true;
    }

    VkResult result = vkWaitForFences(mDevice, 1, &mHandle, VK_TRUE, timeoutNs);
    switch (result) {
        case VK_SUCCESS:
            mIsSignaled = true;
            return true;
        case VK_TIMEOUT:
            MSG_WARN("vk_fence_wait - Timed out");
            break;
        case VK_ERROR_DEVICE_LOST:
            MSG_ERROR("vk_fence_wait - VK_ERROR_DEVICE_LOST.");
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            MSG_ERROR("vk_fence_wait - VK_ERROR_OUT_OF_HOST_MEMORY.");
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            MSG_ERROR("vk_fence_wait - VK_ERROR_OUT_OF_DEVICE_MEMORY.");
            break;
        default:
            MSG_ERROR("vk_fence_wait - An unknown error has occurred.");
            break;
    }
    return false;
}

void VulkanFence::reset() {
    if (mIsSignaled) {
        VK_CHECK(vkResetFences(mDevice, 1, &mHandle));
        mIsSignaled = false;
    }
}