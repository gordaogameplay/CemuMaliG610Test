#pragma once

#include "VulkanRenderer.h"

// Funções específicas para otimização na Mali G610
void ApplyMaliG610Optimizations(VulkanRenderer* renderer);

// Extensão da classe VulkanRenderer para suporte à Mali G610
class VulkanRendererMaliG610 : public VulkanRenderer
{
public:
    VulkanRendererMaliG610();
    
    // Sobrescreve métodos específicos para otimização na Mali G610
    VkPipeline CreateGraphicsPipelineOptimized(const VkGraphicsPipelineCreateInfo& createInfo) override;
    
private:
    bool m_isMaliG610{false};
    bool m_useAsyncCompute{true};
    int m_optimalWorkgroupSize{128};
};
