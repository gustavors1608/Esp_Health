#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include "health_lib.h" // Inclui a biblioteca de monitoramento de saúde

// Configurações da rede Wi-Fi
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";

health_lib hl; // Cria uma instância da classe health_lib

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
