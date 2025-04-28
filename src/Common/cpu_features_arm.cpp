#include "cpu_features_arm.h"
#include <fstream>
#include <regex>
#include <string>

CPUFeaturesARM g_CPUFeaturesARM;

CPUFeaturesARM::CPUFeaturesARM()
{
    // Inicializa com valores padrão para ARM64
    arm.neon = true;
    
    // Detecta recursos específicos do processador
    DetectRK3588s();
    
    // Lê o nome do processador do sistema
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    std::regex model_regex("^model name\\s*:\\s*(.*)$");
    std::smatch match;
    
    while (std::getline(cpuinfo, line)) {
        if (std::regex_search(line, match, model_regex) && match.size() > 1) {
            m_cpuBrandName = match[1].str();
            break;
        }
    }
    
    // Se não encontrou o nome do modelo, tenta o campo "Hardware"
    if (m_cpuBrandName.empty()) {
        cpuinfo.clear();
        cpuinfo.seekg(0, std::ios::beg);
        std::regex hardware_regex("^Hardware\\s*:\\s*(.*)$");
        
        while (std::getline(cpuinfo, line)) {
            if (std::regex_search(line, match, hardware_regex) && match.size() > 1) {
                m_cpuBrandName = match[1].str();
                break;
            }
        }
    }
    
    // Detecta recursos específicos com base no /proc/cpuinfo
    cpuinfo.clear();
    cpuinfo.seekg(0, std::ios::beg);
    std::regex features_regex("^Features\\s*:\\s*(.*)$");
    
    while (std::getline(cpuinfo, line)) {
        if (std::regex_search(line, match, features_regex) && match.size() > 1) {
            std::string features = match[1].str();
            
            // Verifica recursos específicos
            arm.fp16 = features.find("fphp") != std::string::npos || 
                       features.find("fp16") != std::string::npos;
            arm.dotprod = features.find("asimddp") != std::string::npos;
            arm.sve = features.find("sve") != std::string::npos;
            arm.sve2 = features.find("sve2") != std::string::npos;
            
            break;
        }
    }
}

void CPUFeaturesARM::DetectRK3588s()
{
    // Verifica se o processador é um RK3588s
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    bool foundRK3588s = false;
    
    // Procura por indicadores do RK3588s
    while (std::getline(cpuinfo, line)) {
        // O RK3588s geralmente tem 8 núcleos (4x Cortex-A76 + 4x Cortex-A55)
        if (line.find("Rockchip") != std::string::npos && line.find("RK3588") != std::string::npos) {
            foundRK3588s = true;
            break;
        }
    }
    
    // Verifica o dispositivo através do device tree
    if (!foundRK3588s) {
        std::ifstream dtmodel("/proc/device-tree/model");
        std::string model;
        if (dtmodel.is_open()) {
            std::getline(dtmodel, model);
            if (model.find("RK3588") != std::string::npos) {
                foundRK3588s = true;
            }
        }
    }
    
    arm.rk3588s = foundRK3588s;
}

std::string CPUFeaturesARM::GetCPUName()
{
    return m_cpuBrandName;
}

std::string CPUFeaturesARM::GetCommaSeparatedExtensionList()
{
    std::string result = "neon";
    
    if (arm.fp16)
        result += ", fp16";
    if (arm.dotprod)
        result += ", dotprod";
    if (arm.sve)
        result += ", sve";
    if (arm.sve2)
        result += ", sve2";
    if (arm.rk3588s)
        result += ", rk3588s";
    
    return result;
}
