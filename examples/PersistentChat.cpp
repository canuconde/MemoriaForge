//                     MemoriaForge
//     Copyright (C) 2024 Sebastián 'Canu' Bergara Conde <canu.conde@gmail.com>
//
// This file is part of MemoriaForge.
//
// MemoriaForge is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// MemoriaForge is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with MemoriaForge.
// If not, see <https://www.gnu.org/licenses/>.

#include "MemoriaForge.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: "<< argv[0] << " <model.gguf> " << std::endl;
        return 1;
    }
    const std::string model_path = argv[1];

    //Load a GGUF model with 8192 context tokens and 99 cpu layers
    MemoriaForge::LLMSession llm(model_path, 8192, 99);
    //Set the model Temperature
    llm.set_sampling_params(0.02f, 0.9f, 1234);

    //Load previously saved state
    llm.load_state("state.bin");

    //Start a simple chat
    while (true) {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input.empty()) break;

        llm.chat(input);
    }

    //Save KV cache and the chat log
    llm.save_state("state.bin");

    return 0;
}
