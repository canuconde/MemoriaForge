#pragma once
#include <string>
#include <memory>
#include <cstdint>

namespace MemoriaForge {
    //La API de llama.cpp queda aquí
    struct LlamaState;

    class LLMSession {
    public:
        LLMSession(const std::string& model_path,
                int n_ctx = 2048,
                int n_gpu_layers = 99);

        ~LLMSession();

        bool load_state(const std::string& state_bin,
                        const std::string& messages_txt = "chat_log.txt");

        void save_state(const std::string& state_bin,
                        const std::string& messages_txt = "chat_log.txt");

        std::string chat(const std::string& user_input);

        void context_injector(const std::string& prompt);

        void set_sampling_params(float min_p, float temp, uint32_t seed);

    private:
        std::unique_ptr<LlamaState> state;

        std::string generate(const std::string& prompt);
        void init_sampler();
    };

}
