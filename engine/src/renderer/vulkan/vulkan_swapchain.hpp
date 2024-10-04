#pragma once
#include "defines.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

class VulkanDevice;
class VulkanImage;
class VulkanSwapchain {
public:
    VulkanSwapchain(VulkanDevice& device, uint32_t width, uint32_t height);
    ~VulkanSwapchain();

    VulkanSwapchain(const VulkanSwapchain&) = delete;
    VulkanSwapchain(VulkanSwapchain&&) = delete;
    VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;
    VulkanSwapchain& operator=(VulkanSwapchain&&) = delete;

    void recreate(uint32_t width, uint32_t height);

    [[nodiscard]] bool acquire_next_image_index(size_t timeout_ns, VkSemaphore imageAvailable, VkFence imageFence,
                                                uint32_t& outImageIndex);

    void present(VkQueue presentQueue, uint32_t presentImageIndex, VkSemaphore renderComplete);

private:
    VulkanDevice* mDevice{nullptr};
    VkSwapchainKHR mHandle{nullptr};

    VkExtent2D mImageExtent{};
    VkSurfaceFormatKHR mImageFormat{};

    uint32_t mImageCount{0};
    std::vector<VkImage> mImages;
    std::vector<VkImageView> mViews;

    size_t mMaxFramesInFlight{2};
    VkPresentModeKHR mPresentMode{};

    std::unique_ptr<VulkanImage> mDepthAttachment{nullptr};

    void create(uint32_t width, uint32_t height);
    void destroy();
    VkSurfaceFormatKHR choose_swap_surface_format(std::vector<VkSurfaceFormatKHR> formats);
    VkPresentModeKHR choose_swap_present_mode(std::vector<VkPresentModeKHR> presentModes);
    VkExtent2D choose_swap_extent(VkSurfaceCapabilitiesKHR capabilities, uint32_t width, uint32_t height);
};