# Context Forge

**Persistent memory and session management for llama.cpp**

Context Forge is a lightweight C++ library built on top of llama.cpp that simplifies the development of local LLM applications by providing persistent sessions, conversation history management, context injection, and state restoration.

Instead of dealing directly with low-level llama.cpp APIs, Context Forge offers a clean session-based interface that allows developers to focus on building applications.

---

## Features

### Persistent Sessions

Save and restore complete conversations, including the model's internal state.

Context Forge preserves:

* Conversation history
* KV Cache
* Inference state
* Context position

This allows applications to resume conversations instantly without reprocessing the entire chat history.

---

### Context Injection

Load information into the model context without generating a response.

Useful for:

* Documentation
* User manuals
* Internal knowledge bases
* FAQs
* Product information
* Business data

Example:

```cpp
session.context_injector(
    "Context Forge is a C++ library for persistent LLM sessions."
);
```

The model can then answer questions using the injected information.

---

### Simple Chat Interface

Create a session and start chatting immediately.

```cpp
std::string response =
    session.chat("Hello!");
```

No manual prompt formatting required.

---

### Automatic Chat Template Support

Context Forge automatically uses the chat template embedded in the loaded model.

Compatible with:

* Qwen
* Llama
* Gemma
* Mistral
* Other GGUF models supported by llama.cpp

---

### Built on llama.cpp

Context Forge does not replace llama.cpp.

Instead, it acts as a higher-level abstraction layer while keeping the performance and portability of llama.cpp.

---

## Architecture

```text
Application
      │
      ▼
Context Forge
      │
      ├── Session Management
      ├── Conversation History
      ├── Context Injection
      ├── State Persistence
      ├── Sampling
      └── Chat Templates
              │
              ▼
          llama.cpp
              │
              ▼
            GGUF
```

---

## How It Works

### 1. Tokenization

Input text is converted into tokens.

```cpp
llama_tokenize(...)
```

---

### 2. Context Evaluation

Tokens are processed by the model.

```cpp
llama_decode(...)
```

---

### 3. Sampling

The next token is selected.

```cpp
llama_sampler_sample(...)
```

---

### 4. Text Generation

Tokens are converted back to text.

```cpp
llama_token_to_piece(...)
```

---

### 5. Completion

Generation stops when the model emits an end-of-generation token.

---

## State Persistence

Context Forge can save the entire conversation state.

```cpp
session.save_state();
```

This produces:

### state.bin

Contains:

* KV Cache
* Internal inference state
* Context position

Using:

```cpp
llama_state_save_file(...)
```

---

### messages.txt

Contains the conversation history.

Example:

```text
user:Hello
<---MSG_END--->

assistant:Hello! How can I help you?
<---MSG_END--->
```

---

## State Restoration

```cpp
session.load_state();
```

Restores:

* KV Cache
* Conversation history
* Context position

Allowing the model to continue exactly where it left off.

---

## Basic Example

```cpp
#include "context_forge.hpp"
#include <iostream>

int main() {

    forge::Session session(
        "models/qwen2.5-0.5b-instruct-q4.gguf"
    );

    std::cout
        << session.chat("Hello!")
        << std::endl;

    return 0;
}
```

---

## Knowledge Injection Example

```cpp
forge::Session assistant(
    "models/qwen2.5-0.5b-instruct-q4.gguf"
);

assistant.context_injector(
    "Context Forge provides persistent sessions "
    "for local language models."
);

std::cout
    << assistant.chat(
        "What is Context Forge?"
    )
    << std::endl;
```

---

## Why Context Forge?

Using llama.cpp directly often requires handling:

* Model loading
* Context management
* Conversation formatting
* State persistence
* Sampling configuration
* History tracking

Context Forge packages these responsibilities into a single reusable component.

---

## Current Features

Implemented:

* GGUF model loading
* Session-based chat
* Conversation history
* Context injection
* Session persistence
* Session restoration
* Chat template support
* Configurable sampling

---

## Roadmap

Planned features:

* Automatic context overflow handling
* Embeddings support
* Local RAG
* Tool calling
* Streaming generation
* HTTP API
* Multi-session management
* Semantic memory

---

## Requirements

* C++17 or newer
* llama.cpp
* GGUF compatible model

---

## Build

Example:

```bash
g++ src/context_forge.cpp example/main.cpp \
    -I llama.cpp/include \
    -L llama.cpp/build/bin \
    -lllama \
    -o example
```

---

## Philosophy

Context Forge focuses on a simple idea:

> Local LLMs should remember what they have already learned.

By combining session persistence and context management, Context Forge provides a foundation for building assistants, chatbots, and AI-powered applications that can continue working across multiple executions.

---

## License

GPL-3.0
