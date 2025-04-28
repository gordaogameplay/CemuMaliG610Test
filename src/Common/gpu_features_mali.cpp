#include "gpu_features_mali.h"
#include <fstream>
#include <regex>
#include <string>
#include <cstdlib>

MaliGPUDetector g_MaliGPUDetector;

MaliGPUDetector::MaliGPUDetector()
{
    // Inicializa com valores padrão
    features.vulkan_support = true;
    features.opengl_es_support = true;
    
    // Detecta a GPU
    DetectGPU();
}

void MaliGPUDetector::DetectGPU()
{
    // Em sistemas Android, podemos usar propriedades do sistema ou OpenGL ES para detectar a GPU
    // Aqui vamos implementar uma detecção baseada em arquivos do sistema
    
    // Tenta ler informações do driver de GPU
    std::ifstream gpuinfo("/sys/class/graphics/fb0/name");
    if (gpuinfo.is_open()) {
        std::getline(gpuinfo, m_gpuName);
    }
    
    // Verifica se é uma Mali G610 pelo nome
    if (m_gpuName.find("Mali-G610") != std::string::npos) {
        features.mali_g610 = true;
        features.valhall_architecture = true;
        m_gpuVendor = "ARM";
        m_coreCount = 6; // Mali G610 MC6 (padrão no RK3588s)
        return;
    }
    
    // Método alternativo: verificar através de comandos do sistema
    // Nota: em um ambiente real, isso seria implementado usando APIs Android/Linux
    FILE* pipe = popen("dumpsys SurfaceFlinger | grep GLES", "r");
    if (pipe) {
        char buffer[128];
        std::string result = "";
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
        pclose(pipe);
        
        // Procura por Mali G610 na saída
        if (result.find("Mali-G610") != std::string::npos) {
            m_gpuName = "Mali-G610";
            features.mali_g610 = true;
            features.valhall_architecture = true;
            m_gpuVendor = "ARM";
            m_coreCount = 6;
        }
    }
    
    // Se ainda não detectou, tenta outro método específico para o RK3588s
    if (!features.mali_g610) {
        std::ifstream dtmodel("/proc/device-tree/model");
        std::string model;
        if (dtmodel.is_open()) {
            std::getline(dtmodel, model);
            if (model.find("RK3588") != std::string::npos) {
                // RK3588s normalmente vem com Mali G610 MC6
                m_gpuName = "Mali-G610 MC6";
                features.mali_g610 = true;
                features.valhall_architecture = true;
                m_gpuVendor = "ARM";
                m_coreCount = 6;
            }
        }
    }
}

bool MaliGPUDetector::IsMaliG610()
{
    return features.mali_g610;
}

std::string MaliGPUDetector::GetGPUName()
{
    return m_gpuName;
}

std::string MaliGPUDetector::GetGPUVendor()
{
    return m_gpuVendor;
}

int MaliGPUDetector::GetGPUCoreCount()
{
    return m_coreCount;
}
