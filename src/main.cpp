#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <stdexcept>
#include <vector>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;


const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif


class VulkanApp {
public:
    VulkanApp() {



        initWindow();
        initVulkan();
    }

    ~VulkanApp() {
        vkDestroyPipeline(device, pipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyRenderPass(device, renderPass, nullptr);
        vkDestroyDevice(device, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void run() {
        while (true) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    return;
                }
            }
            drawFrame();
        }
    }

private:
    SDL_Window* window = nullptr;
    VkInstance instance;
    VkSurfaceKHR surface;
    VkDevice device;
    VkQueue graphicsQueue;
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
    VkSwapchainKHR swapChain;
    VkImageView swapChainImageView;
    VkFramebuffer framebuffer;

    // Add these declarations
    std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    bool enableValidationLayers = true;

    void initWindow() {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Vulkan Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_VULKAN);
        if (!window) {
            throw std::runtime_error("Failed to create SDL window");
        }
    }

    void initVulkan() {
        createInstance();
        createSurface();
        // Here you would normally create device, swapchain, render pass, etc.
        // For simplicity, this example will only initialize Vulkan instance and surface.
    }

    void createInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    std::vector<const char*> extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (enableValidationLayers) {
        if (!checkValidationLayerSupport()) {
            throw std::runtime_error("Validation layers requested, but not available!");
        }
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
}


    std::vector<const char*> getRequiredExtensions() {
        unsigned int extensionCount = 0;
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
        std::vector<const char*> extensions(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data());
        return extensions;
    }

    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    void createSurface() {
        if (!SDL_Vulkan_CreateSurface(window, instance, &surface)) {
            throw std::runtime_error("Failed to create Vulkan surface");
        }
    }

    void drawFrame() {
        // This is where you'd submit commands to Vulkan to render something
        // For this example, we'll just clear the screen with a color
    }
};

int main() {
    try {
        VulkanApp app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
