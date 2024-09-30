#include "vulkan_swapchain.hpp"
#include "vulkan_device.hpp"
#include <array>
#include <stdexcept>


VulkanSwapchain::VulkanSwapchain(VulkanDevice& device, i16 width, i16 height) : mDevice{&device}, mWidth(width), mHeight{height} {
    const auto swapChainSupport = mDevice->get_swapchain_support_details();
    const VkDevice logicalDevice = mDevice->get_logical_device();

    VkSurfaceFormatKHR surfaceFormat = choose_swap_surface_format(swapChainSupport.formats);
    VkPresentModeKHR presentMode = choose_swap_present_mode(swapChainSupport.presentModes);
    VkExtent2D extent = choose_swap_extent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = mDevice->get_surface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
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

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &mHandle) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(logicalDevice, mHandle, &imageCount, nullptr);
    mImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicalDevice, mHandle, &imageCount, mImages.data());

    mImageFormat = surfaceFormat.format;
    mImageExtent = extent;
};

VulkanSwapchain::~VulkanSwapchain(){
    //TODO
};

void VulkanSwapchain::recreate(i16 width, i16 height) {
    //TODO
    (void)width;
    (void)height;
};

bool VulkanSwapchain::acquire_next_image_index() {
    //TODO
    return false;
};

void VulkanSwapchain::present() {
    //TODO
};

VkSurfaceFormatKHR VulkanSwapchain::choose_swap_surface_format(std::vector<VkSurfaceFormatKHR> formats) {
    //TODO
    (void)formats;
    return VkSurfaceFormatKHR{};
};
VkPresentModeKHR VulkanSwapchain::choose_swap_present_mode(std::vector<VkPresentModeKHR> presentModes) {
    //TODO
    (void)presentModes;
    return VkPresentModeKHR{};
};
VkExtent2D VulkanSwapchain::choose_swap_extent(VkSurfaceCapabilitiesKHR capabilities) {
    //TODO
    (void)capabilities;
    return VkExtent2D{};
};