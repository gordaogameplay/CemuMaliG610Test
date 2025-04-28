#pragma once

#include "VulkanAPI.h"

// Funções específicas para otimização na Mali G610
void ConfigureVulkanForMaliG610(VkPhysicalDeviceFeatures* features, VkPhysicalDeviceVulkan12Features* features12);
void OptimizeSwapchainForMaliG610(VkSwapchainCreateInfoKHR* createInfo);
void OptimizeRenderCommandsForMaliG610(VkCommandBuffer cmdBuffer);
