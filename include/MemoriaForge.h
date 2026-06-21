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

#pragma once
#include <string>
#include <memory>
#include <cstdint>

namespace MemoriaForge {
    //Llama API encapsulation
    struct LlamaState;

    /**
     * @brief Large Language Model session.
     *
     * Provides a simplified interface for loading GGUF models,
     * maintaining conversation history, saving/restoring state,
     * and generating responses through llama.cpp.
     */
    class LLMSession {
    public:
        /**
         * @brief Creates a new LLM session.
         *
         * Loads the specified GGUF model and initializes the
         * inference context.
         *
         * @param model_path Path to the GGUF model file.
         * @param n_ctx Context window size in tokens.
         * @param n_gpu_layers Number of layers to offload to the GPU.
         */
        LLMSession(const std::string& model_path,
                int n_ctx = 2048,
                int n_gpu_layers = 99);

        ~LLMSession();

        /**
         * @brief Restores a previously saved session state.
         *
         * Loads both the model state and the conversation history.
         *
         * @param state_bin Path to the binary state file.
         * @param messages_txt Path to the conversation history file.
         * @return true if the state was successfully loaded.
         * @return false otherwise.
         */
        bool load_state(const std::string& state_bin,
                        const std::string& messages_txt = "chat_log.txt");

        /**
         * @brief Saves the current session state.
         *
         * Stores the model state and conversation history so the
         * session can be restored later.
         *
         * @param state_bin Output binary state file.
         * @param messages_txt Output conversation history file.
         */
        void save_state(const std::string& state_bin,
                        const std::string& messages_txt = "chat_log.txt");

        /**
         * @brief Sends a user message and generates a response.
         *
         * The conversation history is automatically preserved.
         *
         * @param user_input User message.
         * @return Model response.
         */
        std::string chat(const std::string& user_input);

        /**
         * @brief Injects additional information into the context.
         *
         * Useful for memory systems, document retrieval,
         * or custom context augmentation.
         *
         * @param prompt Text to inject into the conversation context.
         */
        void context_injector(const std::string& prompt);

        /**
         * @brief Configures text generation parameters.
         *
         * @param min_p Minimum probability threshold.
         * @param temp Sampling temperature.
         * @param seed Random seed used during generation.
         */
        void set_sampling_params(float min_p, float temp, uint32_t seed);

    private:
        std::unique_ptr<LlamaState> state;
        std::string generate(const std::string& prompt);
        void init_sampler();
    };

}
