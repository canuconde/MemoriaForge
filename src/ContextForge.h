#pragma once

#include "llama.h"
#include <string>
#include <vector>

namespace CForge {

struct ChatMessage {
    std::string role;
    std::string content;
};

class LLMSession {
public:
    LLMSession(const std::string& model_path,
               int n_ctx = 2048,
               int n_gpu_layers = 99);

    ~LLMSession();

    bool load_state(const std::string& state_bin,
                    const std::string& messages_txt);

    void save_state(const std::string& state_bin,
                    const std::string& messages_txt);

    std::string chat(const std::string& user_input);

    void context_injector(const std::string& prompt);
private:
    llama_model* model = nullptr;
    llama_context* ctx = nullptr;
    llama_sampler* smpl = nullptr;

    const llama_vocab* vocab = nullptr;
    const char* tmpl = nullptr;

    std::vector<llama_chat_message> messages;
    std::vector<char> formatted;
    int prev_len = 0;

    std::string generate(const std::string& prompt);
    void init_sampler();
};

}
