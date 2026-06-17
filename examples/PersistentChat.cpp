#include "MemoriaForge.h"
#include <iostream>

int main(int argc, char* argv[]) {

    // Verificar cantidad de argumentos
    // argv[0] = nombre del ejecutable
    // argv[1] = ruta al modelo GGUF
    if (argc != 2) {
        std::cerr << "Usage: "<< argv[0] << " <model.gguf> " << std::endl;
        return 1;
    }
    const std::string model_path = argv[1];

    MemoriaForge::LLMSession llm(model_path, 8192, 99);
    //Cargamos el estado
    llm.load_state("state.bin", "messages_meta.txt");

    //Iniciamos el chat
    while (true) {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input.empty()) break;

        llm.chat(input);
    }

    //Guardamos el estado
    llm.save_state("state.bin", "messages_meta.txt");

    return 0;
}
