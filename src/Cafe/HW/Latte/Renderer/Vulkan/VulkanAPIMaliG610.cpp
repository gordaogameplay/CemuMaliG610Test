#include "VulkanAPI.h"
#include "gpu_features_mali.h"

// Modificações específicas para Mali G610 nas configurações do Vulkan API
void ConfigureVulkanForMaliG610(VkPhysicalDeviceFeatures* features, VkPhysicalDeviceVulkan12Features* features12)
{
    // Verifica se o dispositivo possui uma GPU Mali G610
    if (!g_MaliGPUDetector.IsMaliG610())
        return;
    
    // Configurações otimizadas para Mali G610
    
    // 1. Habilitar recursos específicos que funcionam bem na Mali G610
    if (features) {
        // A Mali G610 tem bom suporte para shaders de geometria
        features->geometryShader = VK_TRUE;
        
        // Otimizações para texturas
        features->textureCompressionASTC_LDR = VK_TRUE;
        
        // A Mali G610 tem bom suporte para cálculos de ponto flutuante
        features->shaderFloat64 = VK_FALSE; // Desabilitar para melhor desempenho
        
        // Otimizações de fillrate
        features->largePoints = VK_TRUE;
        features->wideLines = VK_TRUE;
        
        // Otimizações de memória
        features->robustBufferAccess = VK_TRUE;
    }
    
    // 2. Configurações Vulkan 1.2 específicas para Mali G610
    if (features12) {
        // A Mali G610 tem bom suporte para shaders de computação
        features12->shaderInt8 = VK_TRUE;
        
        // Desabilitar recursos que podem ser problemáticos na Mali G610
        features12->bufferDeviceAddress = VK_FALSE;
        
        // Otimizações de memória para Mali G610
        features12->hostQueryReset = VK_TRUE;
    }
}

// Ajustes específicos para Mali G610 na criação de swapchain
void OptimizeSwapchainForMaliG610(VkSwapchainCreateInfoKHR* createInfo)
{
    // Verifica se o dispositivo possui uma GPU Mali G610
    if (!g_MaliGPUDetector.IsMaliG610())
        return;
    
    // A Mali G610 tem melhor desempenho com certos formatos de cor
    // Preferir formatos que são nativos para a GPU
    
    // Ajustar modo de apresentação para melhor desempenho na Mali G610
    // FIFO é geralmente mais estável em GPUs móveis
    createInfo->presentMode = VK_PRESENT_MODE_FIFO_KHR;
    
    // Ajustar número de imagens no swapchain
    // Mali G610 funciona bem com triplo buffering
    createInfo->minImageCount = 3;
}

// Otimizações para comandos de renderização na Mali G610
void OptimizeRenderCommandsForMaliG610(VkCommandBuffer cmdBuffer)
{
    // Verifica se o dispositivo possui uma GPU Mali G610
    if (!g_MaliGPUDetector.IsMaliG610())
        return;
    
    // Inserir marcadores de desempenho específicos para Mali
    // Isso ajuda o driver a otimizar o processamento
    
    // Ajustar parâmetros de renderização para Mali G610
    // Por exemplo, configurações de viewport e scissor otimizadas
}
