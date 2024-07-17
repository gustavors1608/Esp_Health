#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "health_lib.h"

// Configurações da rede Wi-Fi
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";

// Configurações do email
const char* smtp_server = "smtp.seuemail.com";
const int smtp_port = 465;
const char* email_user = "seu_email@dominio.com";
const char* email_password = "sua_senha";
const char* recipient = "destinatario@dominio.com";

health_lib hl;

void sendEmail(String subject, String body) {
    WiFiClientSecure client;
    client.setInsecure();
    
    if (!client.connect(smtp_server, smtp_port)) {
        Serial.println("Conexão ao servidor SMTP falhou.");
        return;
    }

    client.println("EHLO " + String(smtp_server));
    client.println("AUTH LOGIN");
    client.println(base64::encode(email_user));
    client.println(base64::encode(email_password));
    client.println("MAIL FROM: <" + String(email_user) + ">");
    client.println("RCPT TO: <" + String(recipient) + ">");
    client.println("DATA");
    client.println("Subject: " + subject);
    client.println(body);
    client.println(".");
    client.println("QUIT");
    
    client.stop();
}

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
    // Verifica a saúde do sistema
    bool ram_ok = hl.ram();
    bool ota_ok = hl.ota();
    bool spiffs_ok = hl.spiffs();

    // Se algum dos parâmetros estiver fora dos limites, envia um email
    if (!ram_ok || !ota_ok || !spiffs_ok) {
        String subject = "Alerta de Saúde do Sistema ESP32";
        String body = hl.report();
        sendEmail(subject, body);
        Serial.println("Alerta enviado por email.");
    } else {
        Serial.println("Sistema em bom estado.");
    }

    // Espera 60 segundos antes de realizar a próxima verificação
    delay(60000);
}
