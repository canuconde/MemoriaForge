#include "MemoriaForge.h"
#include <fstream>
#include <iostream>
#include <cstring>

namespace MemoriaForge {

/*
 * Constructor
 *  model_path= direccion del modelo .gguf
 *  n_ctx= tamaño de contexto.
 */
LLMSession::LLMSession(const std::string& model_path, int n_ctx,int n_gpu_layers) {

    // only print errors
    llama_log_set([](enum ggml_log_level level, const char * text, void * /* user_data */) {
        if (level >= GGML_LOG_LEVEL_ERROR) {
            fprintf(stderr, "%s", text);
        }
    }, nullptr);

    ggml_backend_load_all();

    llama_model_params mparams = llama_model_default_params();
    mparams.n_gpu_layers = n_gpu_layers;

    model = llama_model_load_from_file(model_path.c_str(), mparams);
    if (!model) throw std::runtime_error("No se pudo cargar modelo");

    vocab = llama_model_get_vocab(model);

    llama_context_params cparams = llama_context_default_params();
    cparams.n_ctx = n_ctx;
    cparams.n_batch = n_ctx;

    ctx = llama_init_from_model(model, cparams);
    if (!ctx) throw std::runtime_error("No se pudo crear contexto");

    tmpl = llama_model_chat_template(model, nullptr);

    formatted.resize(n_ctx);

    init_sampler();
}

LLMSession::~LLMSession() {
    if (smpl) llama_sampler_free(smpl);
    if (ctx) llama_free(ctx);
    if (model) llama_model_free(model);

    for (auto &m : messages) {
        free((void*)m.content);
    }
}


void LLMSession::init_sampler() {

    smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());

    llama_sampler_chain_add(smpl, llama_sampler_init_temp(0.5f));

    llama_sampler_chain_add(smpl, llama_sampler_init_min_p(0.02f, 1));

    llama_sampler_chain_add(smpl, llama_sampler_init_dist(LLAMA_DEFAULT_SEED));
}


void LLMSession::save_state(const std::string& state_bin, const std::string& messages_txt) {

    std::vector<llama_token> buf(llama_n_ctx(ctx), 0);

    llama_state_save_file(ctx, state_bin.c_str(), buf.data(), buf.size());

    std::ofstream f(messages_txt);

    for (auto &m : messages) {
        f << m.role << ":" << m.content << "\n<---MSG_END--->\n";
    }
}



bool LLMSession::load_state(const std::string& state_bin, const std::string& messages_txt) {

    FILE* f = fopen(state_bin.c_str(), "rb");
    if (!f) return false;
    fclose(f);

    std::vector<llama_token> buf(llama_n_ctx(ctx));
    size_t out = 0;

    if (!llama_state_load_file(ctx, state_bin.c_str(), buf.data(), buf.size(), &out)) {
        return false;
    }

    std::ifstream file(messages_txt);
    if (!file.is_open()) return false;

    messages.clear();

    std::string line, role, content;
    bool reading = false;

    while (std::getline(file, line)) {
        if (line == "<---MSG_END--->") {

            // Usamos strdup para respetar la gestión de memoria dinámica de tu código original
            messages.push_back({strdup(role.c_str()), strdup(content.c_str())});

            // Mostrar en consola los mensajes recuperados para que el usuario vea el historial
            if (role == "user") {
                printf("\033[32m> \033[0m%s\n", content.c_str());
            } else if (role == "assistant") {
                printf("\033[33m%s\n\033[0m", content.c_str());
            }

            role.clear();
            content.clear();
            reading = false;

        } else {
            size_t sep = line.find(':');
            if (sep != std::string::npos && !reading) {
                role = line.substr(0, sep);
                content = line.substr(sep + 1) + "\n";
                reading = true;
            } else {
                content += line + "\n";
            }
        }
    }

    prev_len = llama_chat_apply_template( tmpl, messages.data(),  messages.size(), false,  nullptr,  0 );

    return true;
}
/*
 *  context_injector tokeniza el promt sin generar una respuesta
 *
 *  Util para pasarle reglas al modelo, guardarlas con save_state y usarlas luego con load_state
 */
void LLMSession::context_injector(const std::string& prompt) {

    bool first = llama_memory_seq_pos_max(
        llama_get_memory(ctx), 0) == -1;

        int n = -llama_tokenize(vocab, prompt.c_str(), prompt.size(),  nullptr,  0, first, true);

        std::vector<llama_token> tokens(n);

        llama_tokenize(vocab, prompt.c_str(), prompt.size(), tokens.data(), tokens.size(), first, true);

        llama_batch batch = llama_batch_get_one(tokens.data(), tokens.size());

        //SOLO PREFILL (NO GENERACIÓN)
        llama_decode(ctx, batch);

}

// -------------------------
// GENERATION CORE
// -------------------------

std::string LLMSession::generate(const std::string& prompt) {

    std::string out;

    bool first = llama_memory_seq_pos_max(
        llama_get_memory(ctx), 0) == -1;

    int n = -llama_tokenize(vocab,prompt.c_str(),prompt.size(),nullptr,0,first,true);

    std::vector<llama_token> tokens(n);

    llama_tokenize(vocab,prompt.c_str(),prompt.size(),tokens.data(),tokens.size(),first,true);

    llama_batch batch = llama_batch_get_one(tokens.data(), tokens.size());

    while (true) {

        llama_decode(ctx, batch);

        llama_token tok = llama_sampler_sample(smpl, ctx, -1);

        if (llama_vocab_is_eog(vocab, tok)) break;

        char buf[256];
        int len = llama_token_to_piece(vocab, tok,buf, sizeof(buf),0, true);

        std::string piece(buf, len);

        std::cout << piece << std::flush;

        out += piece;

        batch = llama_batch_get_one(&tok, 1);
    }

    return out;
}

// -------------------------
// CHAT PUBLIC API
// -------------------------

std::string LLMSession::chat(const std::string& user_input) {

// add the user input to the message list and format it
        messages.push_back({"user", strdup(user_input.c_str())});
    int new_len = llama_chat_apply_template(tmpl,messages.data(),messages.size(),true,formatted.data(),formatted.size());

    if (new_len > (int)formatted.size()) {
        formatted.resize(new_len);

        new_len = llama_chat_apply_template(tmpl,messages.data(),messages.size(),true,formatted.data(),formatted.size());
    }

    std::string prompt(formatted.begin() + prev_len, formatted.begin() + new_len);

    std::cout << "\033[33m";
    std::string response = generate(prompt);
    //std::cout<<response;

    std::cout << "\033[0m\n";

    // add the response to the messages
    messages.push_back({"assistant", strdup(response.c_str())});

    prev_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), false, nullptr,0);

    return response;
}

}
