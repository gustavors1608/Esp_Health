// lib para monitorar os dados de "saude" do esp32 e gerar textos de relatorio do mesmo para ser acompanhado a execucao a distancia.

// mas oque seria a saude de um esp32? entre os itens monitorados estao: 
// - ram        -> total e usado
// - flash      -> total
// - spiffs     -> arquivos, tamanhos e total do spiffs
// - wifi       -> modo, conectado, rssi, ip global e local

// entre esses dados, tambem tem fucnoes que retornam se o mesmo está ok ou nao exemplo:
// check.ram() que retorna true ou false, informando se tem pouca ram disponivel etc
// check.report() que retorna uma string contendo os dados sobre o sistema

#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>

//abaixo é definido os limites pelo dev, onde aparir destes pode ser gerado alertas etc
#define _limit_ram_free  4000 //kb
#define _limit_ota_free  1000000 //kb ou deve deixar 1mb livre no minimo para atualizar o firmware via ota
#define _limit_spiffs_free  100000 //kb livre para salvar um novo arquivo

class health_lib{
    private:
        // Função auxiliar para comparar valores com limites definidos
        bool calc(uint32_t value, uint32_t limit);
    public:
        uint32_t free_ram = ESP.getFreeHeap(); // Memória RAM livre
        uint32_t total_ram = ESP.getHeapSize(); // Memória RAM total
        uint32_t freq = ESP.getCpuFreqMHz(); // Frequência da CPU
        const char * model_esp = "esp32"; // Modelo do ESP32
        uint8_t revision_esp = ESP.getChipRevision(); // Revisão do chip ESP32
        uint32_t total_flash = ESP.getFlashChipSize(); // Tamanho total da memória flash
        uint32_t free_ota_flash = ESP.getFreeSketchSpace(); // Espaço livre para OTA
        uint32_t firmware_size = ESP.getSketchSize(); // Tamanho do firmware

        // Construtor da classe health_lib
        health_lib(/* args */);
        // Destrutor da classe health_lib
        ~health_lib();

        // Função que retorna uma string formatada com os dados do sistema
        String report();

        // Funções para verificar se os recursos estão dentro dos limites
        bool ram(uint32_t size);
        bool ota(uint32_t size);
        bool spiffs(uint32_t size);
};

// Implementação do construtor da classe health_lib
health_lib::health_lib(/* args */){
}

// Implementação do destrutor da classe health_lib
health_lib::~health_lib(){
    ESP.~EspClass();
}

// Implementação da função auxiliar calc
bool health_lib::calc(uint32_t value, uint32_t limit){
    if (value < limit){
        return true;
    }else{
        return false;
    }
}

// Implementação da função report que gera um relatório do sistema
String health_lib::report(){
    uint8_t num_files = 0; // Contador de arquivos no SPIFFS
    String nomes_arquivos; // String para armazenar nomes e tamanhos de arquivos

    // Inicia o SPIFFS
    if (!SPIFFS.begin(true)) {
        return String();
    }

    static uint32_t size = SPIFFS.totalBytes() - SPIFFS.usedBytes();

    // Abre o diretório raiz do SPIFFS
    File root = SPIFFS.open("/");

    // Itera sobre os arquivos no diretório raiz
    File file = root.openNextFile();
    while(file){
        nomes_arquivos += "f=" + String(num_files) + " " + file.name() + " s=" + String(file.size()) + " \n";
        num_files++;
        file = root.openNextFile();
    }

    // Monta a string de retorno com informações do sistema
    String return_content = "t_spiffs= " + String(SPIFFS.totalBytes()) + " kb\n" +
    "u_spiffs= " + String(SPIFFS.usedBytes()) + " kb\n" + nomes_arquivos +
    "\nt_ram= " + String(this->total_ram) + " kb\n" +
    "f_ram= " + String(ESP.getFreeHeap()) + " kb\n" +
    "\nf_ota= " + String(this->free_ota_flash) + " kb\n" +
    "t_firm= " + String(this->firmware_size) + " kb\n" +
    "\nmodel= " + String(this->model_esp) + "\n" +
    "f_cpu= " + String(this->freq) + " mhz\n" +
    "\nrssi= " + String(WiFi.RSSI()) + "\n" +
    "local_ip= " + String(WiFi.localIP()) + "\n" +
    "mac= " + String(WiFi.macAddress()) + "\n";

    // Encerra o SPIFFS
    SPIFFS.end();

    return return_content;
}

// Implementação da função ram para verificar a memória RAM livre
bool health_lib::ram(uint32_t size = _limit_ram_free){
    free_ram = ESP.getFreeHeap();
    return calc(this->free_ram, size);
}

// Implementação da função ota para verificar o espaço livre para OTA
bool health_lib::ota(uint32_t size = _limit_ota_free){
    free_ota_flash = ESP.getFreeSketchSpace();
    return calc(this->free_ota_flash, size);
}

// Implementação da função spiffs para verificar o espaço livre no SPIFFS
bool health_lib::spiffs(uint32_t size = _limit_spiffs_free){
    if (!SPIFFS.begin(true)) {
        return String();
    }

    static uint32_t size_livre = SPIFFS.totalBytes() - SPIFFS.usedBytes();
    SPIFFS.end();

    return calc(size_livre, size);
}
