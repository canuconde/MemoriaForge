#include "MemoriaForge.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdint>
#include <vector>
#include "llama.h"

namespace MemoriaForge {
    //Implementamos
    struct LlamaState {
            llama_model* model = nullptr;
            llama_context* ctx = nullptr;
            llama_sampler* smpl = nullptr;

            const llama_vocab* vocab = nullptr;
            const char* tmpl = nullptr;

            std::vector<llama_chat_message> messages;
            std::vector<char> formatted;

            int prev_len = 0;

            float temp = 0.7f;
            float min_p = 0.05f;
            uint32_t seed = LLAMA_DEFAULT_SEED;
    };
    /*
    * Constructor
    *  model_path= direccion del modelo .gguf
    *  n_ctx= tamaño de contexto.
    */
    LLMSession::LLMSession(const std::string& model_path, int n_ctx,int n_gpu_layers) {

        state = std::make_unique<LlamaState>();
        // only print errors
        llama_log_set([](enum ggml_log_level level, const char * text, void * /* user_data */) {
            if (level >= GGML_LOG_LEVEL_ERROR) {
                fprintf(stderr, "%s", text);
            }
        }, nullptr);

        ggml_backend_load_all();

        llama_model_params mparams = llama_model_default_params();
        mparams.n_gpu_layers = n_gpu_layers;

        state->model = llama_model_load_from_file(model_path.c_str(), mparams);
        if (!state->model) throw std::runtime_error("No se pudo cargar modelo");

        state->vocab = llama_model_get_vocab(state->model);

        llama_context_params cparams = llama_context_default_params();
        cparams.n_ctx = n_ctx;
        cparams.n_batch = n_ctx;

        state->ctx = llama_init_from_model(state->model, cparams);
        if (!state->ctx) throw std::runtime_error("No se pudo crear contexto");

        state->tmpl = llama_model_chat_template(state->model, nullptr);

        state->formatted.resize(n_ctx);

        init_sampler();
    }

    LLMSession::~LLMSession() {
        if (state->smpl) llama_sampler_free(state->smpl);
        if (state->ctx) llama_free(state->ctx);
        if (state->model) llama_model_free(state->model);

        //ESTO fue reemplazado, no necesitamos hacer free
        //Creo (?) Verificar posbiles memory leaks
        // for (auto &m : state->messages) {
        //     free((void*)m.content);
        //     free((void*)m.role);
        // }
    }


    void LLMSession::init_sampler() {

        state->smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());


        llama_sampler_chain_add(state->smpl, llama_sampler_init_temp(state->temp));

        llama_sampler_chain_add(state->smpl, llama_sampler_init_min_p(state->min_p, 1));

        llama_sampler_chain_add(state->smpl, llama_sampler_init_dist(state->seed));
    }

    void LLMSession::set_sampling_params(float mp, float t, uint32_t s) {
        state->temp = t;
        state->min_p = mp;
        state->seed = s;

        //Tenemos que recrear el sampler si ya fue creado
        if (state->smpl) { llama_sampler_free(state->smpl);
            state->smpl = nullptr;
        }

        state->smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());

        //Temperatura
        llama_sampler_chain_add(state->smpl,llama_sampler_init_temp(state->temp));

        //Min-p
        llama_sampler_chain_add(state->smpl,llama_sampler_init_min_p(state->min_p, 1));

        //Semilla
        llama_sampler_chain_add(state->smpl,llama_sampler_init_dist(state->seed));
    }

    void LLMSession::save_state(const std::string& state_bin, const std::string& messages_txt) {

        std::vector<llama_token> buf(llama_n_ctx(state->ctx), 0);

        llama_state_save_file(state->ctx, state_bin.c_str(), buf.data(), buf.size());

        std::ofstream f(messages_txt);

        for (auto &m : state->messages) {
            f << m.role << ":" << m.content << "\n<---MSG_END--->\n";
        }
    }



    bool LLMSession::load_state(const std::string& state_bin, const std::string& messages_txt) {

        FILE* f = fopen(state_bin.c_str(), "rb");
        if (!f) return false;
        fclose(f);

        std::vector<llama_token> buf(llama_n_ctx(state->ctx));
        size_t out = 0;

        if (!llama_state_load_file(state->ctx, state_bin.c_str(), buf.data(), buf.size(), &out)) {
            return false;
        }

        std::ifstream file(messages_txt);
        if (!file.is_open()) return false;

        state->messages.clear();

        std::string line, role, content;
        bool reading = false;

        while (std::getline(file, line)) {
            if (line == "<---MSG_END--->") {

                // Usamos strdup para respetar la gestión de memoria dinámica de tu código original
                state->messages.push_back({strdup(role.c_str()), strdup(content.c_str())});

                // Mostrar en consola los mensajes recuperados para que el usuario vea el historial
                if (role == "user") {
                    printf("\033[32m> \033[0m%s\n", content.c_str()); // <-- Esto no se ve bien en windows
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

        state->prev_len = llama_chat_apply_template( state->tmpl, state->messages.data(),  state->messages.size(), false,  nullptr,  0 );

        return true;
    }
    /*
    *  context_injector tokeniza el promt sin generar una respuesta
    *
    *  Util para pasarle reglas al modelo, guardarlas con save_state y usarlas luego con load_state
    */
    void LLMSession::context_injector(const std::string& prompt) {

        bool first = llama_memory_seq_pos_max(
            llama_get_memory(state->ctx), 0) == -1;

            int n = -llama_tokenize(state->vocab, prompt.c_str(), prompt.size(),  nullptr,  0, first, true);

            std::vector<llama_token> tokens(n);

            llama_tokenize(state->vocab, prompt.c_str(), prompt.size(), tokens.data(), tokens.size(), first, true);

            llama_batch batch = llama_batch_get_one(tokens.data(), tokens.size());

            //SOLO PREFILL (NO GENERACIÓN)
            llama_decode(state->ctx, batch);

    }

    // -------------------------
    // GENERATION CORE
    // -------------------------

    std::string LLMSession::generate(const std::string& prompt) {

        std::string out;

        bool first = llama_memory_seq_pos_max(
            llama_get_memory(state->ctx), 0) == -1;

        int n = -llama_tokenize(state->vocab,prompt.c_str(),prompt.size(),nullptr,0,first,true);

        std::vector<llama_token> tokens(n);

        llama_tokenize(state->vocab,prompt.c_str(),prompt.size(),tokens.data(),tokens.size(),first,true);

        llama_batch batch = llama_batch_get_one(tokens.data(), tokens.size());

        while (true) {

            llama_decode(state->ctx, batch);

            llama_token tok = llama_sampler_sample(state->smpl, state->ctx, -1);

            if (llama_vocab_is_eog(state->vocab, tok)) break;

            char buf[256];
            int len = llama_token_to_piece(state->vocab, tok,buf, sizeof(buf),0, true);

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
        state->messages.push_back({"user", strdup(user_input.c_str())});

        // Inyección manual: Forzamos al modelo a creer que ya terminó de pensar
        // Al abrir el rol de asistente e inmediatamente cerrar el tag </think>,
        // el modelo se ve obligado a continuar con la respuesta final.
        // state->messages.push_back({"assistant", "</think>"});


        int new_len = llama_chat_apply_template(state->tmpl,state->messages.data(),state->messages.size(),true,state->formatted.data(),state->formatted.size());

        if (new_len > (int)state->formatted.size()) {
            state->formatted.resize(new_len);

            new_len = llama_chat_apply_template(state->tmpl,state->messages.data(),state->messages.size(),true,state->formatted.data(),state->formatted.size());
        }

        std::string prompt(state->formatted.begin() + state->prev_len, state->formatted.begin() + new_len);

        std::cout << "\033[33m";
        std::string response = generate(prompt);
        //std::cout<<response;

        std::cout << "\033[0m\n";

        // add the response to the messages
        state->messages.push_back({"assistant", strdup(response.c_str())});

        state->prev_len = llama_chat_apply_template(state->tmpl, state->messages.data(), state->messages.size(), false, nullptr,0);

        return response;
    }

}
