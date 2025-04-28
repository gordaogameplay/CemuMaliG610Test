#include "VulkanRenderer.h"
#include "VulkanRendererMaliG610.h"
#include "VulkanAPIMaliG610.h"
#include "Common/gpu_features_mali.h"

// Modificação do arquivo principal do renderizador Vulkan para integrar o suporte à Mali G610

// Função para criar o renderizador apropriado com base no hardware detectado
std::unique_ptr<VulkanRenderer> CreateOptimizedVulkanRenderer()
{
    // Verifica se o dispositivo possui uma GPU Mali G610
    if (g_MaliGPUDetector.IsMaliG610())
    {
        // Cria o renderizador otimizado para Mali G610
        return std::make_unique<VulkanRendererMaliG610>();
    }
    else
    {
        // Cria o renderizador padrão para outras GPUs
        return std::make_unique<VulkanRenderer>();
    }
}

// Modificação da função de inicialização do Vulkan para integrar as otimizações para Mali G610
bool InitializeVulkanWithOptimizations(VkInstance instance, VkPhysicalDevice physicalDevice)
{
    // Inicialização padrão
    bool result = InitializeVulkan(instance, physicalDevice);
    
    if (!result)
        return false;
    
    // Aplica otimizações específicas para Mali G610 se disponível
    if (g_MaliGPUDetector.IsMaliG610())
    {
        // Configurar recursos específicos para Mali G610
        VkPhysicalDeviceFeatures features{};
        VkPhysicalDeviceVulkan12Features features12{};
        
        // Obter recursos disponíveis
        vkGetPhysicalDeviceFeatures(physicalDevice, &features);
        
        // Aplicar otimizações específicas para Mali G610
        ConfigureVulkanForMaliG610(&features, &features12);
        
        // Aplicar as configurações otimizadas
        // Nota: Na implementação real, isso seria feito durante a criação do dispositivo lógico
    }
    
    return true;
}
