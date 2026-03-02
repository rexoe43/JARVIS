#include <iostream>
#include <thread>
#include <atomic>
#include <csignal>
#include <memory>
#include <chrono>
#include "core/include/audio.h"
#include "core/include/socket_server.h"
#include "core/include/system_control.h"
#include "ui/include/interface.h"

std::atomic<bool> running(true);

void signalHandler(int signum) {
    std::cout << "\nSeñal " << signum << "recibida. Cerrando Jarvis..." << std::endl;
    running = false;
}

int main() {
    // Configurar el cierre para que se vea más limpio
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    std::cout << R"(
     ╔═══════════════════════════════════════════════════════════════╗
    ║                                                               ║
    ║     ██╗ █████╗ ██████╗ ██╗   ██╗██╗███████╗                  ║
    ║     ██║██╔══██╗██╔══██╗██║   ██║██║██╔════╝                  ║
    ║     ██║███████║██████╔╝██║   ██║██║███████╗                  ║
    ║     ██║██╔══██║██╔══██╗╚██╗ ██╔╝██║╚════██║                  ║
    ║     ██║██║  ██║██║  ██║ ╚████╔╝ ██║███████║                  ║
    ║     ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝  ╚═══╝  ╚═╝╚══════╝                  ║
    ║                                                               ║
    ║                    Asistente Personal por Voz                 ║
    ║                           v1.0                                ║
    ║                                                               ║
    ╚═══════════════════════════════════════════════════════════════╝
    )" << std::endl;

    std::cout <<"Inicializando módulos.." << std::endl;

    try {
        // Inicializar módulos
        auto audio = std::make_unique<AudioCapture>("default", 16000, 1);
        auto socketServer = std::make_unique<SocketServer>("/tmp/jarvis_socket");
        auto systemControl = std::make_unique<SystemControl>();
        auto ui = std::make_unique<UI>();
        

        // Inicializar servidor socket
        if(!socketServer.start()) {
            std::cerr << "Error: No se pudo iniciar el servidor socket" << std::endl;
            return 1;
        }

        std::cout << "Servidor socket iniciando en /tmp/jarvis_socket" << std::endl;
        std::cout << "Esperando conexión del cerebro Python..." << std::endl;

        //Esperar conexión del cerebro
        if (!socketServer.waitForConnection(5000)) { // Tiempo de 5 segundos
            std::cerr << "Error: No se pudo conectar con el cerebro de Python" << std::endl;
        return 1;
    }

    std::cout << "Cerebro Python conectado" << std::endl;
    ui.showReady();
    // Bucle principal
    while (running) {
        // Capturar audio
        std::cout <<"Escuchando.." << std::endl;
        std::string audioData = audio.capture(5); // Capturar 5 segundos máximo

        if(!audioData.empty()){
            // Enviar audio a Python
            std::cout << "Enviando audio a cerebro.." << std::endl;
            socketServer.send(audioData);

            // Recibir comando procesado
            std::cout <<"Esperando Respuesta.." << std::endl;
            std::string response = socketServer.receive(10000); // Tiempo de 10 segundos

            if (!response.empty()){
                std::cout <<"Comandos recibidos: " << response << std::endl;

                // Ejecutar acción
                std::string result = systemControl.execute(response);

                // Enviar resultado a Python para feedback de voz
                socketServer.send(result);
            }
        }

        // Pequeña pausa
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Limpieza
    socketServer.stop();
    std::cout << "Jarvis cerrado correctamente" << std::endl;
    }catch(const std::exception& e) {
        std::cerr << "Error fatal: " <<e.what() << std::endl;
        return 1;
    }
    return 0;
}