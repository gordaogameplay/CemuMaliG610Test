#pragma once

#include <string>

class CPUFeaturesARM
{
public:
    CPUFeaturesARM();
    std::string GetCPUName(); // empty if not available
    std::string GetCommaSeparatedExtensionList();
    
    struct
    {
        bool neon{ true };      // Assumido como padrão para ARM64
        bool fp16{ false };     // Suporte para operações de ponto flutuante de 16 bits
        bool dotprod{ false };  // Suporte para instruções de produto escalar
        bool sve{ false };      // Scalable Vector Extension
        bool sve2{ false };     // SVE versão 2
        bool rk3588s{ false };  // Flag específica para o processador RK3588s
    } arm;

private:
    void DetectRK3588s();
    std::string m_cpuBrandName;
};

extern CPUFeaturesARM g_CPUFeaturesARM;
