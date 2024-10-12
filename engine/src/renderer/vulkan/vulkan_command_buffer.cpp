#include "vulkan_command_buffer.hpp"
#include "vulkan/vulkan_core.h"
#include "vulkan_defines.inl"

VulkanCommandBuffer::VulkanCommandBuffer(VkDevice& device, VkCommandPool& pool, bool isPrimary)
    : mDevice{device}, mCommandPool{pool}, mState(State::COMMAND_BUFFER_STATE_READY) {
    VkCommandBufferAllocateInfo allocateInfo;
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = mCommandPool;
    allocateInfo.level = isPrimary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    allocateInfo.commandBufferCount = 1;
    allocateInfo.pNext = nullptr;

    VK_CHECK(vkAllocateCommandBuffers(mDevice, &allocateInfo, &mHandle));
}

VulkanCommandBuffer::~VulkanCommandBuffer() { vkFreeCommandBuffers(mDevice, mCommandPool, 1, &mHandle); }