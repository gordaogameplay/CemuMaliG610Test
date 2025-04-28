# Guia de Instalação do CEMU para Orange Pi 5 Pro

Este guia descreve como instalar e executar o CEMU modificado no Orange Pi 5 Pro com processador RK3588s e GPU Mali G610.

## Requisitos

- Orange Pi 5 Pro com Android instalado
- Acesso root (opcional, mas recomendado para melhor desempenho)
- Pelo menos 4GB de espaço livre em armazenamento
- Controlador USB ou Bluetooth (opcional, para jogos que requerem controle)

## Instalação

### Método 1: Instalação Direta do APK

1. Transfira o arquivo APK compilado (`app-debug.apk`) para o seu Orange Pi 5 Pro
2. No Orange Pi 5 Pro, navegue até o local onde o APK foi salvo
3. Toque no arquivo APK para iniciar a instalação
4. Se solicitado, permita a instalação de aplicativos de fontes desconhecidas
5. Aguarde a conclusão da instalação

### Método 2: Instalação via ADB

1. Conecte o Orange Pi 5 Pro ao computador via USB
2. Certifique-se de que o ADB está instalado no computador
3. Ative a Depuração USB nas configurações do desenvolvedor no Orange Pi 5 Pro
4. Abra um terminal no computador e execute:
   ```
   adb install caminho/para/app-debug.apk
   ```
5. Aguarde a conclusão da instalação

## Configuração Inicial

Ao iniciar o CEMU pela primeira vez, você precisará configurar:

1. **Diretório de Jogos**: Configure o local onde seus jogos de Wii U estão armazenados
2. **Controladores**: Configure os controles para emular o GamePad do Wii U
3. **Configurações Gráficas**: As configurações otimizadas para Mali G610 serão aplicadas automaticamente

## Otimizações de Desempenho

Para obter o melhor desempenho no Orange Pi 5 Pro:

1. **Modo de Alto Desempenho**: Se disponível, ative o modo de alto desempenho nas configurações do sistema
2. **Feche Aplicativos em Segundo Plano**: Libere memória fechando aplicativos desnecessários
3. **Temperatura**: Certifique-se de que o dispositivo tem ventilação adequada para evitar throttling térmico
4. **Configurações de Energia**: Conecte o dispositivo à energia para evitar limitações de desempenho por economia de bateria

## Solução de Problemas

### Problemas de Desempenho

- **Baixo FPS**: Tente reduzir a resolução nas configurações gráficas do CEMU
- **Travamentos**: Verifique se há atualizações de firmware disponíveis para o Orange Pi 5 Pro
- **Aquecimento Excessivo**: Considere adicionar refrigeração externa se o dispositivo estiver muito quente

### Problemas de Compatibilidade

- **Jogos Não Iniciam**: Verifique se o formato do jogo é compatível e se os arquivos estão completos
- **Controles Não Funcionam**: Reconfigure os controles nas configurações do CEMU
- **Erros Gráficos**: Experimente alternar entre os modos de renderização Vulkan e OpenGL

## Contato e Suporte

Se encontrar problemas específicos com esta versão modificada do CEMU para Orange Pi 5 Pro, entre em contato com o desenvolvedor para obter suporte.

---

Nota: Este guia é específico para a versão modificada do CEMU otimizada para o Orange Pi 5 Pro com processador RK3588s e GPU Mali G610.
