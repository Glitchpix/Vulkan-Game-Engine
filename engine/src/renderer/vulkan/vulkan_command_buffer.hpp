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
    VulkanCommandBuffer(const VkDevice& device, const VkCommandPool& pool, bool isPrimary);
    ~VulkanCommandBuffer();

    void update_submitted();
    void reset_state();
    void begin_single_use();
    void end_single_use(VkQueue queue);

private:
    const VkDevice mDevice;
    const VkCommandPool mCommandPool;
    VkCommandBuffer mHandle;
    State mState;
    bool mPrimary;

    void begin(bool singleUse, bool continueRenderpass, bool simultaneous);
    void end();
};