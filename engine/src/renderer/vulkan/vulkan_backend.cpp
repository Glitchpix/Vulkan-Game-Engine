#include "vulkan_backend.hpp"
#include "core/logger.hpp"
#include "vulkan_command_buffer.hpp"
#include "vulkan_defines.inl"
#include "vulkan_device.hpp"
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

    create_command_buffers();

    MSG_TRACE("[Vulkan] Vulkan Renderer: {:p} initialized", static_cast<void*>(this));
};

VulkanRenderer::~VulkanRenderer() {
    MSG_DEBUG("Vulkan renderer: {:p} destructor called", static_cast<void*>(this));
    // Reset pointer to members here since otherwise the destructors will be called in the wrong order (as expected by Vulkan)
    destroy_command_buffers();
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
    //TODO
    return true;
}
bool VulkanRenderer::end_frame(f64 /*deltaTime*/) {
    //TODO
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
}

void VulkanRenderer::destroy_command_buffers() {
    mCommandBuffers.clear();
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