#pragma once

#include "VulkanRenderer.h"
#include "VulkanRendererMaliG610.h"

// Funções para integração das otimizações para Mali G610
std::unique_ptr<VulkanRenderer> CreateOptimizedVulkanRenderer();
bool InitializeVulkanWithOptimizations(VkInstance instance, VkPhysicalDevice physicalDevice);
