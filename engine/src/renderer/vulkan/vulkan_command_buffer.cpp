#include "vulkan_command_buffer.hpp"
#include "vulkan/vulkan_core.h"
#include "vulkan_defines.inl"

VulkanCommandBuffer::VulkanCommandBuffer(const VkDevice& device, const VkCommandPool& pool, bool isPrimary)
    : mDevice{device}, mCommandPool{pool}, mState(State::COMMAND_BUFFER_STATE_READY), mPrimary{isPrimary} {
    VkCommandBufferAllocateInfo allocateInfo;
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = mCommandPool;
    allocateInfo.level = mPrimary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    allocateInfo.commandBufferCount = 1;
    allocateInfo.pNext = nullptr;

    VK_CHECK(vkAllocateCommandBuffers(mDevice, &allocateInfo, &mHandle));
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
    vkFreeCommandBuffers(mDevice, mCommandPool, 1, &mHandle);
}

void VulkanCommandBuffer::begin(bool singleUse, bool continueRenderpass, bool simultaneous) {
    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    if (singleUse) {
        beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }
    if (continueRenderpass) {
        beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    }
    if (simultaneous) {
        beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    }

    VK_CHECK(vkBeginCommandBuffer(mHandle, &beginInfo));
    mState = State::COMMAND_BUFFER_STATE_RECORDING;
}

//TODO: Check valid states for these
void VulkanCommandBuffer::end() {

    VK_CHECK(vkEndCommandBuffer(mHandle));
    mState = State::COMMAND_BUFFER_STATE_RECORDING_ENDED;
}

void VulkanCommandBuffer::update_submitted() {
    mState = State::COMMAND_BUFFER_STATE_SUBMITTED;
}

void VulkanCommandBuffer::reset_state() {
    VkCommandBufferResetFlags resetFlags = 0;
    vkResetCommandBuffer(mHandle, resetFlags);
    mState = State::COMMAND_BUFFER_STATE_READY;
}
// END TODO
void VulkanCommandBuffer::begin_single_use() {
    begin(true, false, false);
}

void VulkanCommandBuffer::end_single_use(VkQueue queue) {
    end();

    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &mHandle;

    VK_CHECK(vkQueueSubmit(queue, 1, &submitInfo, nullptr));

    VK_CHECK(vkQueueWaitIdle(queue));
    reset_state();
}