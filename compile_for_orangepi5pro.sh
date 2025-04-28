#!/bin/bash

# Script para compilar o CEMU modificado para Orange Pi 5 Pro
# com suporte ao processador RK3588s e GPU Mali G610

# Configurações com caminhos específicos
BUILD_TYPE="Debug"
ANDROID_SDK_PATH="/home/trebor/android-sdk"
ANDROID_NDK_PATH="/home/trebor/android-sdk/ndk/26.1.10909125"

# Verifica se os diretórios existem
if [ ! -d "$ANDROID_SDK_PATH" ]; then
    echo "Erro: Diretório do Android SDK não encontrado em $ANDROID_SDK_PATH"
    exit 1
fi

if [ ! -d "$ANDROID_NDK_PATH" ]; then
    echo "Erro: Diretório do Android NDK não encontrado em $ANDROID_NDK_PATH"
    echo "Verificando outras versões do NDK..."
    
    # Tenta encontrar outra versão do NDK
    ANDROID_NDK_PATH="/home/trebor/android-sdk/ndk/25.0.8775105"
    if [ ! -d "$ANDROID_NDK_PATH" ]; then
        echo "Erro: Nenhuma versão do NDK encontrada"
        exit 1
    else
        echo "Usando NDK em: $ANDROID_NDK_PATH"
    fi
else
    echo "Usando NDK em: $ANDROID_NDK_PATH"
fi

echo "Usando Android SDK em: $ANDROID_SDK_PATH"

# Exporta as variáveis de ambiente
export ANDROID_HOME="$ANDROID_SDK_PATH"
export ANDROID_NDK_HOME="$ANDROID_NDK_PATH"

# Diretório do projeto - usa o diretório atual
PROJECT_DIR="$(pwd)"
cd "$PROJECT_DIR" || exit 1

# Inicializa submódulos git necessários
echo "Inicializando submódulos git necessários..."
git submodule update --init --recursive || echo "Aviso: Falha ao inicializar submódulos git"

# Verifica e clona manualmente o repositório ImGui se necessário
if [ ! -f "$PROJECT_DIR/dependencies/imgui/imgui.cpp" ]; then
    echo "Arquivo imgui.cpp não encontrado. Clonando repositório ImGui manualmente..."
    rm -rf "$PROJECT_DIR/dependencies/imgui"
    cd "$PROJECT_DIR/dependencies" || exit 1
    git clone https://github.com/ocornut/imgui.git
    cd "$PROJECT_DIR" || exit 1
fi

# Corrige o problema com sanitizers no cubeb
echo "Corrigindo dependência sanitizers no cubeb..."

# Clona o repositório sanitizers-cmake se não existir
if [ ! -d "$PROJECT_DIR/dependencies/sanitizers-cmake" ]; then
    echo "Clonando repositório sanitizers-cmake..."
    cd "$PROJECT_DIR/dependencies" || exit 1
    git clone https://github.com/arsenm/sanitizers-cmake.git
    cd "$PROJECT_DIR" || exit 1
fi

# Modifica o CMakeLists.txt do cubeb para usar o sanitizers-cmake local
CUBEB_CMAKE="$PROJECT_DIR/dependencies/cubeb/CMakeLists.txt"
if [ -f "$CUBEB_CMAKE" ]; then
    # Faz backup do arquivo original
    cp "$CUBEB_CMAKE" "${CUBEB_CMAKE}.bak"
    
    # Verifica se a linha já existe para evitar duplicação
    if ! grep -q "CMAKE_MODULE_PATH.*sanitizers-cmake" "$CUBEB_CMAKE"; then
        # Adiciona a linha para incluir o caminho do sanitizers-cmake
        sed -i '34i list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../sanitizers-cmake/cmake")' "$CUBEB_CMAKE"
        echo "CMakeLists.txt do cubeb modificado para usar sanitizers-cmake local."
    else
        echo "CMakeLists.txt do cubeb já está configurado para usar sanitizers-cmake local."
    fi
else
    echo "Aviso: Arquivo CMakeLists.txt do cubeb não encontrado em $CUBEB_CMAKE"
fi

# Cria o arquivo local.properties com o caminho do SDK e NDK
echo "Criando arquivo local.properties..."
echo "sdk.dir=$ANDROID_SDK_PATH" > "$PROJECT_DIR/src/android/local.properties"
echo "ndk.dir=$ANDROID_NDK_PATH" >> "$PROJECT_DIR/src/android/local.properties"
echo "Arquivo local.properties criado com sucesso."

# Adiciona os novos arquivos ao CMakeLists.txt
echo "Atualizando CMakeLists.txt para incluir os novos arquivos..."

# Verifica se já existe a entrada para os arquivos Mali G610
if ! grep -q "gpu_features_mali" src/CMakeLists.txt; then
    # Adiciona os arquivos de detecção da GPU Mali
    sed -i '/Common\/cpu_features.cpp/a \ \ Common\/gpu_features_mali.cpp' src/CMakeLists.txt
    sed -i '/Common\/cpu_features.h/a \ \ Common\/gpu_features_mali.h' src/CMakeLists.txt
fi

# Verifica se já existe a entrada para os arquivos RK3588s
if ! grep -q "cpu_features_arm" src/CMakeLists.txt; then
    # Adiciona os arquivos de detecção do CPU ARM
    sed -i '/Common\/cpu_features.cpp/a \ \ Common\/cpu_features_arm.cpp' src/CMakeLists.txt
    sed -i '/Common\/cpu_features.h/a \ \ Common\/cpu_features_arm.h' src/CMakeLists.txt
fi

# Verifica se já existe a entrada para os arquivos de otimização do Vulkan para Mali G610
if ! grep -q "VulkanRendererMaliG610" src/CMakeLists.txt; then
    # Adiciona os arquivos de otimização do Vulkan para Mali G610
    sed -i '/Cafe\/HW\/Latte\/Renderer\/Vulkan\/VulkanRenderer.cpp/a \ \ Cafe\/HW\/Latte\/Renderer\/Vulkan\/VulkanRendererMaliG610.cpp' src/CMakeLists.txt
    sed -i '/Cafe\/HW\/Latte\/Renderer\/Vulkan\/VulkanRenderer.h/a \ \ Cafe\/HW\/Latte\/Renderer\/Vulkan\/VulkanRendererMaliG610.h' src/CMakeLists.txt
    sed -i '/Cafe\/HW\/Latte\/Renderer\/Vulkan\/VulkanAPI.cpp/a \ \ Cafe\/HW\/Latte\/Renderer\/Vulkan\/VulkanAPIMaliG610.cpp' src/CMakeLists.txt
    sed -i '/Cafe\/HW\/Latte\/Renderer\/Vulkan\/VulkanAPI.h/a \ \ Cafe\/HW\/Latte\/Renderer\/Vulkan\/VulkanAPIMaliG610.h' src/CMakeLists.txt
    sed -i '/Cafe\/HW\/Latte\/Renderer\/Vulkan\/VulkanRenderer.cpp/a \ \ Cafe\/HW\/Latte\/Renderer\/Vulkan\/VulkanRendererIntegration.cpp' src/CMakeLists.txt
    sed -i '/Cafe\/HW\/Latte\/Renderer\/Vulkan\/VulkanRenderer.h/a \ \ Cafe\/HW\/Latte\/Renderer\/Vulkan\/VulkanRendererIntegration.h' src/CMakeLists.txt
fi

# Verifica se já existe a entrada para os arquivos de otimização do backend AArch64 para RK3588s
if ! grep -q "BackendAArch64_RK3588s" src/CMakeLists.txt; then
    # Adiciona os arquivos de otimização do backend AArch64 para RK3588s
    sed -i '/Cafe\/HW\/Espresso\/Recompiler\/BackendAArch64\/BackendAArch64.cpp/a \ \ Cafe\/HW\/Espresso\/Recompiler\/BackendAArch64\/BackendAArch64_RK3588s.cpp' src/CMakeLists.txt
    sed -i '/Cafe\/HW\/Espresso\/Recompiler\/BackendAArch64\/BackendAArch64.h/a \ \ Cafe\/HW\/Espresso\/Recompiler\/BackendAArch64\/BackendAArch64_RK3588s.h' src/CMakeLists.txt
fi

# Modifica o build.gradle.kts para incluir otimizações para RK3588s e Mali G610
echo "Atualizando build.gradle.kts para otimizações específicas..."

# Diretório do arquivo build.gradle.kts
GRADLE_FILE="$PROJECT_DIR/src/android/app/build.gradle.kts"

# Adiciona flags de compilação específicas para RK3588s e Mali G610
if ! grep -q "RK3588S" "$GRADLE_FILE"; then
    # Adiciona as flags de compilação para RK3588s e Mali G610
    sed -i '/abiFilters("arm64-v8a")/a \                cppFlags.addAll(listOf("-DRK3588S_OPTIMIZATIONS=ON", "-DMALI_G610_OPTIMIZATIONS=ON"))' "$GRADLE_FILE"
fi

# Atualiza a referência ao NDK no build.gradle.kts
if grep -q "ndkVersion" "$GRADLE_FILE"; then
    # Extrai a versão do NDK do caminho
    NDK_VERSION=$(basename "$ANDROID_NDK_PATH")
    # Atualiza a versão do NDK no build.gradle.kts
    sed -i "s/ndkVersion = \".*\"/ndkVersion = \"$NDK_VERSION\"/" "$GRADLE_FILE"
    echo "Versão do NDK atualizada para $NDK_VERSION no build.gradle.kts"
fi

# Compila o projeto para Android
echo "Compilando o projeto para Android..."
cd "$PROJECT_DIR/src/android" || exit 1

# Verifica se o Gradle está disponível
if [ ! -f "./gradlew" ]; then
    echo "Erro: Gradle wrapper não encontrado"
    echo "Tentando criar o wrapper do Gradle..."
    
    # Tenta criar o wrapper do Gradle
    if command -v gradle &> /dev/null; then
        gradle wrapper
    else
        echo "Gradle não está instalado. Por favor, instale o Gradle ou use o Android Studio para abrir e compilar o projeto."
        exit 1
    fi
fi

# Torna o gradlew executável
chmod +x ./gradlew

# Compila o projeto
./gradlew assembleDebug

# Verifica se a compilação foi bem-sucedida
if [ $? -eq 0 ]; then
    echo "Compilação concluída com sucesso!"
    echo "APK gerado em: $PROJECT_DIR/src/android/app/build/outputs/apk/debug/app-debug.apk"
else
    echo "Erro durante a compilação"
    exit 1
fi

echo "Processo de compilação concluído!"
