#include "vulkan_swapchain.hpp"
#include "core/logger.hpp"
#include "vulkan/vulkan_core.h"
#include "vulkan_defines.inl"
#include "vulkan_device.hpp"
#include "vulkan_image.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>


VulkanSwapchain::VulkanSwapchain(VulkanDevice& device, uint32_t width, uint32_t height) : mDevice{&device} {
    const auto swapChainSupport = mDevice->get_swapchain_support_details();
    mImageFormat = choose_swap_surface_format(swapChainSupport.formats);
    mPresentMode = choose_swap_present_mode(swapChainSupport.presentModes);
    create(width, height);
};

void VulkanSwapchain::create(uint32_t width, uint32_t height) {
    const auto swapChainSupport = mDevice->get_swapchain_support_details();
    const VkDevice logicalDevice = mDevice->get_logical_device();
    mImageExtent = choose_swap_extent(swapChainSupport.capabilities, width, height);

    mImageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && mImageCount > swapChainSupport.capabilities.maxImageCount) {
        mImageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = mDevice->get_surface();

    createInfo.minImageCount = mImageCount;
    createInfo.imageFormat = mImageFormat.format;
    createInfo.imageColorSpace = mImageFormat.colorSpace;
    createInfo.imageExtent = mImageExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    const auto indices = mDevice->get_queue_families();
    std::array queueFamilyIndices{indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;  // Optional
        createInfo.pQueueFamilyIndices = nullptr;  // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = mPresentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VK_CHECK(vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &mHandle) != VK_SUCCESS);

    // Set swapchain images TODO: Refactor

    vkGetSwapchainImagesKHR(logicalDevice, mHandle, &mImageCount, nullptr);
    mImages.resize(mImageCount);
    mViews.resize(mImageCount);
    vkGetSwapchainImagesKHR(logicalDevice, mHandle, &mImageCount, mImages.data());

    for (size_t i = 0; i < mImages.size(); ++i) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = mImages[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = mImageFormat.format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        VK_CHECK(vkCreateImageView(mDevice->get_logical_device(), &viewInfo, nullptr, &mViews[i]));
    }

    // Depth resources
    VkFormat depthFormat = mDevice->get_depth_format();

    VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
    VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    mDepthAttachment = std::make_unique<VulkanImage>(*mDevice, mImageExtent.width, mImageExtent.height, depthFormat,
                                                     tiling, usageFlags, memoryFlags);
    MSG_INFO("[Vulkan] Swapchain: {:p} successfully created", static_cast<void*>(this));
}

VulkanSwapchain::~VulkanSwapchain() {
    destroy();
};

void VulkanSwapchain::destroy() {
    for (auto const& imageView : mViews) {
        vkDestroyImageView(mDevice->get_logical_device(), imageView, nullptr);
    }
    vkDestroySwapchainKHR(mDevice->get_logical_device(), mHandle, nullptr);
}

void VulkanSwapchain::recreate(uint32_t width, uint32_t height) {
    destroy();
    create(width, height);
};

bool VulkanSwapchain::acquire_next_image_index(size_t timeout_ns, VkSemaphore imageAvailable, VkFence imageFence,
                                               uint32_t& outImageIndex) {
    VkResult result = vkAcquireNextImageKHR(mDevice->get_logical_device(), mHandle, timeout_ns, imageAvailable,
                                            imageFence, &outImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        // recreate();   TODO: Implemement
        throw std::runtime_error("Recreate swap chain image not implemented!");
        return false;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        MSG_FATAL("[Vulkan] Failed to acquire swap chain image!");
        return false;
    }
    return true;
};

const VkFormat& VulkanSwapchain::get_image_depth_format() const {
    return mDepthAttachment->get_format();
}

void VulkanSwapchain::present(VkQueue presentQueue, uint32_t presentImageIndex, VkSemaphore renderComplete) {
    VkPresentInfoKHR presentInfo;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderComplete;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &mHandle;
    presentInfo.pImageIndices = &presentImageIndex;
    presentInfo.pResults = nullptr;

    VkResult resultImageAcquire = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (resultImageAcquire == VK_ERROR_OUT_OF_DATE_KHR || resultImageAcquire == VK_SUBOPTIMAL_KHR) {
        // recreate();   TODO: Implemement
        throw std::runtime_error("Recreate swap chain image not implemented!");
    } else if (resultImageAcquire != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }
};

VkSurfaceFormatKHR VulkanSwapchain::choose_swap_surface_format(std::vector<VkSurfaceFormatKHR> formats) {
    for (const auto& availableFormat : formats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return formats[0];
};
VkPresentModeKHR VulkanSwapchain::choose_swap_present_mode(std::vector<VkPresentModeKHR> presentModes) {
    for (const auto& availablePresentMode : presentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
};
VkExtent2D VulkanSwapchain::choose_swap_extent(VkSurfaceCapabilitiesKHR capabilities, uint32_t width, uint32_t height) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    VkExtent2D actualExtent = {width, height};

    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
};

VkImageView VulkanSwapchain::get_depth_view() const {
    return mDepthAttachment->get_view();
}