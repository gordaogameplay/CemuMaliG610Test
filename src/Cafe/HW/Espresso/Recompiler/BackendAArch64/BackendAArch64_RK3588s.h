#pragma once

#include "BackendAArch64.h"

// Funções específicas para otimização no RK3588s
bool IsRK3588s();
void ApplyRK3588sOptimizations(AArch64GenContext_t* context);

// Configurações de otimização para o RK3588s
struct RK3588sConfig
{
    // Configurações de cache
    static constexpr int L1_CACHE_SIZE = 64 * 1024;      // 64KB L1 cache por núcleo A76
    static constexpr int L2_CACHE_SIZE = 512 * 1024;     // 512KB L2 cache por cluster
    static constexpr int L3_CACHE_SIZE = 2 * 1024 * 1024; // 2MB L3 cache compartilhado
    
    // Configurações de núcleos
    static constexpr int BIG_CORES = 4;    // 4 núcleos Cortex-A76
    static constexpr int LITTLE_CORES = 4; // 4 núcleos Cortex-A55
    
    // Frequências típicas
    static constexpr int BIG_CORE_FREQ_MHZ = 2400;    // 2.4 GHz para núcleos A76
    static constexpr int LITTLE_CORE_FREQ_MHZ = 1800; // 1.8 GHz para núcleos A55
    
    // Flags de otimização
    static constexpr bool USE_NEON_ACCELERATION = true;
    static constexpr bool OPTIMIZE_MEMORY_ACCESS = true;
    static constexpr bool USE_THREAD_AFFINITY = true;
};
