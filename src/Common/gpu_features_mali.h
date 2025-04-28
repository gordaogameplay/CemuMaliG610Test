#pragma once

#include <string>
#include <vector>

// Classe para detecção e otimização da GPU Mali G610
class MaliGPUDetector 
{
public:
    MaliGPUDetector();
    
    // Verifica se o dispositivo possui uma GPU Mali G610
    bool IsMaliG610();
    
    // Obtém informações sobre a GPU
    std::string GetGPUName();
    std::string GetGPUVendor();
    int GetGPUCoreCount();
    
    // Recursos específicos da Mali G610
    struct {
        bool valhall_architecture{false};  // Arquitetura Valhall (usada na G610)
        bool mali_g610{false};            // Especificamente Mali G610
        bool vulkan_support{true};        // Suporte a Vulkan (assumido como padrão)
        bool opengl_es_support{true};     // Suporte a OpenGL ES (assumido como padrão)
    } features;
    
    // Configurações recomendadas para Mali G610
    struct {
        int optimal_workgroup_size{128};  // Tamanho ótimo de workgroup para shaders
        bool use_async_compute{true};     // Usar compute assíncrono quando disponível
        bool prefer_vulkan{true};         // Preferir Vulkan sobre OpenGL ES
    } config;
    
private:
    void DetectGPU();
    std::string m_gpuName;
    std::string m_gpuVendor;
    int m_coreCount{6};  // Valor padrão para Mali G610
};

extern MaliGPUDetector g_MaliGPUDetector;
