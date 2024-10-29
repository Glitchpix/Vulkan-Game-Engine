#include "vulkan_backend.hpp"
#include "core/logger.hpp"
#include "vulkan_command_buffer.hpp"
#include "vulkan_defines.inl"
#include "vulkan_device.hpp"
#include "vulkan_fence.hpp"
#include "vulkan_framebuffer.hpp"
#include "vulkan_platform.hpp"
#include "vulkan_renderpass.hpp"
#include "vulkan_swapchain.hpp"


#include <cstddef>
#include <cstdint>
#include <memory>
#include <vulkan/vulkan_core.h>


VulkanRenderer::VulkanRenderer(std::string applicationName, Platform* platform, uint32_t width, uint32_t height)
    : RendererBackend(platform, RendererBackend::BackendType::RENDERER_BACKEND_TYPE_VULKAN, width, height) {
#if defined(_DEBUG)
    mEnableValidationLayers = true;
#endif
    MSG_TRACE("[Vulkan] Vulkan Renderer: {:p} created", static_cast<void*>(this));
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = applicationName.c_str();
    appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
    appInfo.pEngineName = "Vulkan Test Engine";
    appInfo.engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    const auto requiredExtensions = get_required_extensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    MSG_DEBUG("[Vulkan] {} validation layer", mEnableValidationLayers);
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (mEnableValidationLayers) {
        mValidationLayers.emplace_back("VK_LAYER_KHRONOS_validation");

        // Verify validation layers can be used
        if (!check_validation_layer_support()) {
            MSG_ERROR("[Vulkan] Support for all validation layers failed, disabling validation support.");
            mEnableValidationLayers = false;
            mValidationLayers.clear();
        } else {
            MSG_INFO("[Vulkan] All required validation layers are supported");
        }
        populate_debug_messenger_create_info(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    size_t requiredValidationLayerCount = mValidationLayers.size();

    createInfo.enabledLayerCount = requiredValidationLayerCount;
    createInfo.ppEnabledLayerNames = mValidationLayers.data();  // TODO: Check this, might be unsafe
    createInfo.pNext = nullptr;

    //TODO: custom allocator
    VkResult result = vkCreateInstance(&createInfo, nullptr, &mInstance);

    if (result != VK_SUCCESS) {
        MSG_FATAL("[Vulkan] Failed to create Vulkan renderer: {:p}", static_cast<void*>(this));
        throw std::runtime_error("");
    }
    setup_debug_messenger();

    mSurface = vulkanplatform::create_platform_surface(*mPlatform, mInstance);
    mDevice = std::make_unique<VulkanDevice>(mInstance, mSurface, mValidationLayers);
    mSwapchain = std::make_unique<VulkanSwapchain>(*mDevice, mWidth, mHeight);


    // TODO: Temp values
    // Change width & height to framebuffer height and width
    VkRect2D renderArea{.offset = VkOffset2D{.x = 0, .y = 0}, .extent = VkExtent2D{.width = width, .height = height}};
    VkClearColorValue clearColor{.float32{1.0F, 0.0F, 0.0F, 1.0F}};
    VkClearDepthStencilValue depthStencil{.depth = 1.0F, .stencil = 0};

    mRenderpass =
        std::make_unique<RenderPass>(mDevice->get_logical_device(), *mSwapchain, renderArea, clearColor, depthStencil);

    create_framebuffers();

    create_command_buffers();

    create_sync_objects();

    MSG_TRACE("[Vulkan] Vulkan Renderer: {:p} initialized", static_cast<void*>(this));
};

VulkanRenderer::~VulkanRenderer() {
    MSG_DEBUG("Vulkan renderer: {:p} destructor called", static_cast<void*>(this));
    vkDeviceWaitIdle(mDevice->get_logical_device());
    // Reset pointer to members here since otherwise the destructors will be called in the wrong order (as expected by Vulkan)
    destroy_sync_objects();
    destroy_command_buffers();
    destroy_framebuffers();
    mRenderpass.reset();
    mSwapchain.reset();
    mDevice.reset();
    if (mEnableValidationLayers) {
        auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT"));
        if (func != nullptr) {
            func(mInstance, mDebugMessenger, nullptr);
        }
    }
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
    vkDestroyInstance(mInstance, nullptr);
}

void VulkanRenderer::resized(uint32_t /*width*/, uint32_t /*height*/) {
    //TODO
}

bool VulkanRenderer::begin_frame(f64 /*deltaTime*/) {
    MSG_TRACE("[Vulkan] begin frame called");
    constexpr auto timeout = UINT64_MAX;

    auto& currentInFlightFence = mInFlightFences[mCurrentFrame];
    auto& currentImageAvailableSemaphore = mImageAvailableSemaphore[mCurrentFrame];


    if (!currentInFlightFence.wait(timeout)) {
        MSG_WARN("[Vulkan] In-flight fence wait failure!");
        return false;
    }

    if (!mSwapchain->acquire_next_image_index(timeout, currentImageAvailableSemaphore, VK_NULL_HANDLE, mImageIndex)) {
        MSG_WARN("[Vulkan] Failed to acquire next image index!");
        return false;
    }

    currentInFlightFence.reset();

    auto& currentCommandBuffer = mCommandBuffers[mImageIndex];
    currentCommandBuffer.reset_state();
    currentCommandBuffer.begin_multiple_use();

    auto& currentFrameBuffer = mFrameBuffers[mImageIndex];

    const auto viewPortExtent = currentFrameBuffer.get_image_extent();
    const auto viewPortWidth = static_cast<float>(viewPortExtent.width);
    const auto viewPortHeight = static_cast<float>(viewPortExtent.height);

    VkViewport viewport;
    viewport.x = 0.0F;
    viewport.y = viewPortHeight;
    viewport.width = viewPortWidth;
    viewport.height = -viewPortHeight;  // OpenGL-like offset
    viewport.minDepth = 0.0F;
    viewport.maxDepth = 1.0F;


    VkRect2D scissor;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent = viewPortExtent;

    vkCmdSetViewport(currentCommandBuffer.get_handle(), 0, 1, &viewport);
    vkCmdSetScissor(currentCommandBuffer.get_handle(), 0, 1, &scissor);

    const auto renderExtent = viewPortExtent;
    mRenderpass->set_render_area_extent(renderExtent);

    mRenderpass->begin(currentFrameBuffer.get_handle(), currentCommandBuffer.get_handle());

    return true;
}
bool VulkanRenderer::end_frame(f64 /*deltaTime*/) {
    MSG_TRACE("[Vulkan] end frame called");
    auto& currentInFlightFence = mInFlightFences[mCurrentFrame];
    auto& currentImageAvailableSemaphore = mImageAvailableSemaphore[mCurrentFrame];
    auto& currentRenderFinishedSemaphore = mRenderFinishedSemaphore[mCurrentFrame];
    auto& currentCommandBuffer = mCommandBuffers[mImageIndex];

    mRenderpass->end(currentCommandBuffer.get_handle());
    currentCommandBuffer.end();

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &currentCommandBuffer.get_handle();
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &currentRenderFinishedSemaphore;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &currentImageAvailableSemaphore;

    VkPipelineStageFlags waitStages[1] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submit_info.pWaitDstStageMask = waitStages;

    VkResult result = vkQueueSubmit(mDevice->get_graphics_queue(), 1, &submit_info, currentInFlightFence.get_handle());
    if (result != VK_SUCCESS) {
        MSG_ERROR("[Vulkan] vkQueueSubmit failed with result");
        return false;
    }
    currentCommandBuffer.update_submitted();

    mSwapchain->present(mDevice->get_present_queue(), mImageIndex, currentRenderFinishedSemaphore);

    mCurrentFrame = (mCurrentFrame + 1) % mSwapchain->get_max_frames_inflight();
    return true;
}

std::vector<const char*> VulkanRenderer::get_required_extensions() {
    std::vector<const char*> extensions{};

    extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);  // Generic surface

    std::vector<const char*> requiredPlatformExtensions = vulkanplatform::get_platform_extensions();

    for (const auto& extension : requiredPlatformExtensions) {
        extensions.emplace_back(extension);
    }

    if (mEnableValidationLayers) {
        extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        MSG_DEBUG("[Vulkan] Required extensions:");
        for (const auto& extension : extensions) {
            MSG_DEBUG("{}", extension);
        }
    }

    return extensions;
}

bool VulkanRenderer::check_validation_layer_support() {
    uint32_t layerCount{};
    VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

    std::vector<VkLayerProperties> availableLayers(layerCount);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()));

    MSG_TRACE("[Vulkan] Verifying validation layers...");
    for (const char* layerName : mValidationLayers) {
        MSG_TRACE("[Vulkan] Checking validation layer: {}", layerName);
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                MSG_TRACE("[Vulkan] Found validation layer: {}", layerName);
                break;
            }
        }

        if (!layerFound) {
            MSG_WARN("[Vulkan] Missing validation layer: {}", layerName);
            return false;
        }
    }

    return true;
}

void VulkanRenderer::populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debug_callback;
}

void VulkanRenderer::setup_debug_messenger() {
    if (!mEnableValidationLayers) {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    populate_debug_messenger_create_info(createInfo);

    VK_CHECK(CreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr, &mDebugMessenger));
    MSG_DEBUG("[Vulkan] Debug messenger created");
}

void VulkanRenderer::create_command_buffers() {
    size_t swapChainImageCount = mSwapchain->get_image_count();
    mCommandBuffers.reserve(swapChainImageCount);

    for (size_t i = 0; i < swapChainImageCount; ++i) {
        mCommandBuffers.emplace_back(mDevice->get_logical_device(), mDevice->get_graphics_commandpool(), true);
    }
    MSG_INFO("[Vulkan] All command buffers successfully created by: {:p}", static_cast<void*>(this));
}

void VulkanRenderer::destroy_command_buffers() {
    mCommandBuffers.clear();
    MSG_INFO("[Vulkan] All command buffers successfully destroyed by: {:p}", static_cast<void*>(this));
}

void VulkanRenderer::create_framebuffers() {
    MSG_INFO("[Vulkan] Create framebuffers called by: {:p}", static_cast<void*>(this));
    size_t swapChainImageCount = mSwapchain->get_image_count();
    mFrameBuffers.reserve(swapChainImageCount);

    // TODO Finish this section, call and implement fences
    for (size_t i = 0; i < swapChainImageCount; ++i) {
        auto extent = mSwapchain->get_image_extent();
        uint32_t attachmentCount = 2;  // TODO: Get this from Swapchain
        VkImageView attachments[] = {mSwapchain->get_image_view(i), mSwapchain->get_depth_view()};
        mFrameBuffers.emplace_back(mRenderpass.get(), extent, attachmentCount, attachments);
    }
    MSG_INFO("[Vulkan] All framebuffers successfully created by: {:p}", static_cast<void*>(this));
}

void VulkanRenderer::destroy_framebuffers() {
    mFrameBuffers.clear();
    MSG_INFO("[Vulkan] All framebuffers successfully destroyed by: {:p}", static_cast<void*>(this));
}

void VulkanRenderer::create_sync_objects() {
    const auto& maxFramesInFlight = mSwapchain->get_max_frames_inflight();
    const auto logicalDevice = mDevice->get_logical_device();
    mImageAvailableSemaphore.resize(maxFramesInFlight);
    mRenderFinishedSemaphore.resize(maxFramesInFlight);

    mInFlightFences.reserve(maxFramesInFlight);

    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    for (size_t i = 0; i < maxFramesInFlight; ++i) {
        // TODO: Extract semaphores into class
        VK_CHECK(vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &mImageAvailableSemaphore[i]));
        VK_CHECK(vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &mRenderFinishedSemaphore[i]));

        mInFlightFences.emplace_back(logicalDevice, true);
    }
}

void VulkanRenderer::destroy_sync_objects() {
    const auto& maxFramesInFlight = mSwapchain->get_max_frames_inflight();
    for (size_t i = 0; i < maxFramesInFlight; ++i) {
        vkDestroySemaphore(mDevice->get_logical_device(), mImageAvailableSemaphore[i], nullptr);
        vkDestroySemaphore(mDevice->get_logical_device(), mRenderFinishedSemaphore[i], nullptr);
    }
    mInFlightFences.clear();
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderer::debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                              VkDebugUtilsMessageTypeFlagsEXT /*unused*/,
                                                              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                              void* /*unused*/) {

    switch (messageSeverity) {
        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            MSG_ERROR("{}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            MSG_WARN("{}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            MSG_INFO("{}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            MSG_TRACE("{}", pCallbackData->pMessage);
            break;
    }

    return VK_FALSE;
}

VkResult VulkanRenderer::CreateDebugUtilsMessengerEXT(VkInstance instance,
                                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                      const VkAllocationCallbacks* pAllocator,
                                                      VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}