#pragma once
#include "defines.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

class VulkanDevice;

class VulkanSwapchain {
public:
    VulkanSwapchain(VulkanDevice& device, i16 width, i16 height);
    ~VulkanSwapchain();

    VulkanSwapchain(const VulkanSwapchain&) = default;
    VulkanSwapchain(VulkanSwapchain&&) = delete;
    VulkanSwapchain& operator=(const VulkanSwapchain&) = default;
    VulkanSwapchain& operator=(VulkanSwapchain&&) = delete;

    void recreate(i16 width, i16 height);

    bool acquire_next_image_index();

    void present();

private:
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
    VulkanDevice* mDevice{nullptr};
    i16 mWidth{0};
    i16 mHeight{0};
    VkSwapchainKHR mHandle{nullptr};


    size_t mMaxFramesInFlight{0};
    size_t mImageCount{0};
#pragma clang diagnostic pop
    std::vector<VkImage> mImages;
    VkFormat mImageFormat{};
    VkExtent2D mImageExtent{};
    std::vector<VkImageView> mViews;


    VkSurfaceFormatKHR choose_swap_surface_format(std::vector<VkSurfaceFormatKHR> formats);
    VkPresentModeKHR choose_swap_present_mode(std::vector<VkPresentModeKHR> presentModes);
    VkExtent2D choose_swap_extent(VkSurfaceCapabilitiesKHR capabilities);
};