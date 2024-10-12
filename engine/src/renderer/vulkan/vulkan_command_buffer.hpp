#pragma once

#include "vulkan/vulkan_core.h"
class VulkanCommandBuffer {
public:
    enum class State {
        COMMAND_BUFFER_STATE_READY,
        COMMAND_BUFFER_STATE_RECORDING,
        COMMAND_BUFFER_STATE_IN_RENDER_PASS,
        COMMAND_BUFFER_STATE_RECORDING_ENDED,
        COMMAND_BUFFER_STATE_SUBMITTED,
        COMMAND_BUFFER_STATE_NOT_ALLOCATED
    };
    VulkanCommandBuffer(VkDevice& device, VkCommandPool& pool, bool isPrimary);
    ~VulkanCommandBuffer();
    void begin();
    void end();

private:
    VkDevice mDevice;
    VkCommandPool mCommandPool;
    VkCommandBuffer mHandle;
    State mState;
};