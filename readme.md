# Esp_Health para ESP32

`Esp_health` é uma biblioteca Arduino para ESP32 que permite monitorar a "saúde" do dispositivo, fornecendo informações detalhadas sobre o uso de RAM, flash, SPIFFS e Wi-Fi. Além disso, a biblioteca oferece funções para verificar se esses recursos estão dentro de limites aceitáveis e gerar relatórios completos sobre o sistema.

## Funcionalidades

- **Monitoramento de RAM**: Verifica a quantidade de RAM total e disponível.
- **Monitoramento de Flash**: Verifica o espaço total da memória flash e o espaço disponível para OTA.
- **Monitoramento de SPIFFS**: verifica os arquivos armazenados, seus tamanhos e analisa entao o espaço disponível e usado.
- **Monitoramento de Wi-Fi**: Obtém informações sobre o estado da conexão Wi-Fi, incluindo RSSI, IP local e MAC address.
- **Relatórios**: Gera relatórios detalhados sobre o estado do sistema.

## Por Que Usar

- **Manutenção Preventiva**: Monitore recursos críticos do ESP32 para evitar problemas de desempenho e falhas.
- **Diagnóstico Remoto**: Gere relatórios detalhados que podem ser enviados remotamente para análise.
- **Facilidade de Uso**: Interface simples e intuitiva para verificar rapidamente o estado do dispositivo.

## Instalação

1. Clone este repositório ou baixe os arquivos.
2. Inclua os arquivos na sua IDE Arduino ou Extensão PlatformIo.

## Exemplo de Uso

### Configuração Básica

```cpp
#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include "esp_health.h"

// Configurações da rede Wi-Fi
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";

esp_health hl;

void setup() {
    // Inicializa a comunicação serial
    Serial.begin(115200);

    // Conecta à rede Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
    }
    Serial.println("Conectado ao WiFi");

    // Inicializa o SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("Falha ao montar o sistema de arquivos SPIFFS");
        return;
    }
}

void loop() {
    // Verifica a saúde do sistema usando todas as funções disponíveis
    bool ram_ok = hl.ram(); // Verifica se a RAM livre está dentro do limite
    bool ota_ok = hl.ota(); // Verifica se o espaço livre para OTA está dentro do limite
    bool spiffs_ok = hl.spiffs(); // Verifica se o espaço livre no SPIFFS está dentro do limite

    // Imprime o status da saúde do sistema
    Serial.println("Status da Saúde do Sistema:");
    Serial.print("RAM OK: ");
    Serial.println(ram_ok ? "Sim" : "Não");
    Serial.print("OTA OK: ");
    Serial.println(ota_ok ? "Sim" : "Não");
    Serial.print("SPIFFS OK: ");
    Serial.println(spiffs_ok ? "Sim" : "Não");

    // Imprime o relatório completo do sistema
    String report = hl.report(); // Gera um relatório completo do sistema
    Serial.println("Relatório Completo do Sistema:");
    Serial.println(report);

    // Espera 30 segundos antes de realizar a próxima verificação
    delay(30000);
}
```

## API

### Construtor

```cpp
esp_health();
```

### Funções

- **report()**: Retorna uma string formatada com os dados do sistema.

  ```cpp
  String report();
  ```

- **ram(uint32_t size = _limit_ram_free)**: Verifica se a RAM livre está dentro do limite especificado.

  ```cpp
  bool ram(uint32_t size = _limit_ram_free);
  ```

- **ota(uint32_t size = _limit_ota_free)**: Verifica se o espaço livre para OTA está dentro do limite especificado.

  ```cpp
  bool ota(uint32_t size = _limit_ota_free);
  ```

- **spiffs(uint32_t size = _limit_spiffs_free)**: Verifica se o espaço livre no SPIFFS está dentro do limite especificado.

  ```cpp
  bool spiffs(uint32_t size = _limit_spiffs_free);
  ```

## Limites Padrão

- **RAM**: 4000 KB
- **OTA**: 1000000 KB (1 MB)
- **SPIFFS**: 100000 KB

Esses limites podem ser ajustados dentro dos arquivos conforme necessário para atender às necessidades específicas do seu projeto.

## Contribuição

Contribuições são bem-vindas! Sinta-se à vontade para abrir issues e pull requests para melhorias e correções de bugs.

futuras melhorias 
- [ ] monitoramento de tensão da placa
- [ ] tornar menos dependente de libs externas como spiffs e wifi, pois tem caso que pode nao ser usado
- [ ] organizar melhor funções e documentação
- [ ] separar e formatar de maneira correta o documento esp_health.h que está so indexando a lib

## Licença

Este projeto está licenciado sob a MIT License. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.