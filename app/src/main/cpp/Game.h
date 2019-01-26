//
// Created by pux19 on 1/25/2019.
//

#ifndef GGJ_2019_GAME_H
#define GGJ_2019_GAME_H

#include <vulkan_wrapper.h>

struct android_app;

class Game {
public:
    Game(android_app* pApp);
    ~Game();

private:
    VkInstance _instance;
    VkSurfaceKHR _surface;
    VkPhysicalDevice _gpu;
    VkDevice _device;

    VkExtent2D _displaySize;
    VkSwapchainKHR _swapchain;
    uint32_t _swapchainLength;
};


#endif //GGJ_2019_GAME_H
