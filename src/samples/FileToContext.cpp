#include <iostream>
#include <fstream>
#include <sstream>
#include "ContextForge.h"

int main(int argc, char* argv[]) {

    // Verificar cantidad de argumentos
    // argv[0] = nombre del ejecutable
    // argv[1] = ruta al modelo GGUF
    // argv[2] = archivo a cargar en el contexto
    if (argc != 3) {
        std::cerr << "Usage: "<< argv[0] << " <model.gguf> <file_name>" << std::endl;
        return 1;
    }

    // Obtener parámetros de línea de comandos
    const std::string model_path = argv[1];
    const std::string file_name  = argv[2];

    // Abrir archivo de entrada
    std::ifstream file(file_name);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << file_name << std::endl;
        return 1;
    }

    // Leer el archivo completo preservando
    // espacios, tabs y saltos de línea
    std::stringstream buffer;
    buffer << file.rdbuf();

    const std::string content = buffer.str();

    // Mostrar información básica
    std::cout << "Loaded file: "<< file_name<< std::endl;

    std::cout << "Size: "<< content.size()<< " bytes"<< std::endl;

    // Crear una sesión LLM
    //
    // Parámetros:
    // - Ruta del modelo
    // - Tamaño de contexto
    // - Seed para generación reproducible
    sigma::LLMSession llm(model_path,8192,99);

    // Inyectar el contenido del archivo
    // dentro del contexto del modelo
    llm.context_injector(content);

    // Guardar el estado completo de la sesión
    //
    // state.bin:
    //     Estado interno del contexto LLM
    //
    // messages_meta.txt:
    //     Metadatos de mensajes almacenados
    llm.save_state("state.bin","messages_meta.txt");

    std::cout << "Context saved successfully."<< std::endl;

    return 0;

}
