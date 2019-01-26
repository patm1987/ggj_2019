//
// Created by pux19 on 1/25/2019.
//

#include <vector>
#include <sstream>
#include <android_native_app_glue.h>
#include "Game.h"
#include "Logger.h"

#define CALL_VK(func)\
    if (VK_SUCCESS != (func)) {\
        std::stringstream errorStream;\
        errorStream << "Vulkan Error - File: " << __FILE__ << " :: Line: " << __LINE__;\
        Logger::Error(errorStream.str());\
        assert(false);\
    }

Game::Game(android_app *pApp) {
    InitVulkan();
    Logger::Info("Game Created!");

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pApplicationName = "GGJ 2019";
    appInfo.pEngineName = "Ninja Engine of Doom!";

    // prepare the extensions
    std::vector<const char *> instanceExt, deviceExt;
    instanceExt.push_back("VK_KHR_surface");
    instanceExt.push_back("VK_KHR_android_surface");
    deviceExt.push_back("VK_KHR_swapchain");

    // create a vulkan instance
    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExt.size());
    instanceCreateInfo.ppEnabledExtensionNames = instanceExt.data();
    instanceCreateInfo.enabledLayerCount = 0;
    instanceCreateInfo.ppEnabledLayerNames = nullptr;
    CALL_VK(vkCreateInstance(&instanceCreateInfo, nullptr, &_instance));

    // create a surface
    VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.window = pApp->window;
    CALL_VK(vkCreateAndroidSurfaceKHR(_instance, &surfaceCreateInfo, nullptr, &_surface));

    // find a GPU, just use the first one because: Android
    uint32_t gpuCount = 0;
    CALL_VK(vkEnumeratePhysicalDevices(_instance, &gpuCount, nullptr));
    VkPhysicalDevice tmpGpus[gpuCount];
    CALL_VK(vkEnumeratePhysicalDevices(_instance, &gpuCount, tmpGpus));
    _gpu = tmpGpus[0];

    // get the vulkan info for this device
    VkPhysicalDeviceProperties gpuProperties;
    vkGetPhysicalDeviceProperties(_gpu, &gpuProperties);
    std::stringstream deviceName;
    deviceName << "Vulkan Physical Device Name: " << gpuProperties.deviceName;
    Logger::Info(deviceName.str());
    std::stringstream deviceInfo;
    deviceInfo
            << "Vulkan Physical Device Info:" << std::endl
            << "\tapiVersion: " << std::hex << gpuProperties.apiVersion << std::endl
            << "\tdriverVersion: " << std::hex << gpuProperties.driverVersion;
    Logger::Info(deviceInfo.str());
    std::stringstream apiVersion;
    apiVersion
            << "API Version Supported: "
            << VK_VERSION_MAJOR(gpuProperties.apiVersion) << "."
            << VK_VERSION_MINOR(gpuProperties.apiVersion) << "."
            << VK_VERSION_PATCH(gpuProperties.apiVersion);
    Logger::Info(apiVersion.str());

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_gpu, _surface, &surfaceCapabilities);
    std::stringstream surfaceCapabilitiesLog;
    surfaceCapabilitiesLog
            << "Vulkan Surface Capabilities" << std::endl
            << "\timage count: " << surfaceCapabilities.minImageCount << " - "
            << surfaceCapabilities.maxImageCount << std::endl
            << "\tarray layers: " << surfaceCapabilities.maxImageArrayLayers << std::endl
            << "\timage size (now): " << surfaceCapabilities.currentExtent.width << "x"
            << surfaceCapabilities.currentExtent.height << std::endl
            << "\timage size (extent): " << surfaceCapabilities.minImageExtent.width << "x"
            << surfaceCapabilities.minImageExtent.height << " - "
            << surfaceCapabilities.maxImageExtent.width << "x"
            << surfaceCapabilities.maxImageExtent.height << std::endl
            << "\tusage: " << std::hex << surfaceCapabilities.supportedUsageFlags << std::endl
            << "\tcurrent transform: " << surfaceCapabilities.currentTransform << std::endl
            << "\tallowed transforms: " << std::hex << surfaceCapabilities.supportedTransforms
            << std::endl
            << "\tcomposite alpha flags: " << surfaceCapabilities.currentTransform;
    Logger::Info(surfaceCapabilitiesLog.str());

    // Find a GFX queue family
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &queueFamilyCount, nullptr);
    assert(queueFamilyCount);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &queueFamilyCount, queueFamilyProperties.data());
    uint32_t queueFamilyIndex;
    for (queueFamilyIndex = 0; queueFamilyIndex < queueFamilyCount; queueFamilyIndex++) {
        if (queueFamilyProperties[queueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            break;
        }
    }
    assert(queueFamilyIndex < queueFamilyCount);

    // create a logical device from the GPU
    float priorities[] = {1.0f,};
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.pQueuePriorities = priorities;

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = nullptr;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExt.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExt.data();
    deviceCreateInfo.pEnabledFeatures = nullptr;
    CALL_VK(vkCreateDevice(_gpu, &deviceCreateInfo, nullptr, &_device));
}

Game::~Game() {
    vkDestroySurfaceKHR(_instance, _surface, nullptr);
    vkDestroyDevice(_device, nullptr);
    vkDestroyInstance(_instance, nullptr);
    vkDestroyInstance(_instance, nullptr);

    Logger::Info("Game Destroyed!");
}
