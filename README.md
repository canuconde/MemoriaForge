# Context Forge

**Persistent memory and session management for llama.cpp**

Context Forge is a lightweight C++ wrapper for llama.cpp designed for applications that need:

* Persistent chat sessions
* Conversation history management
* Context injection
* Session save and restore
* Simplified GGUF model integration

Instead of dealing directly with low-level llama.cpp APIs, Context Forge provides a simple session-based interface for local LLM applications.

---

## Features

### Persistent Sessions

Save and restore:

* KV Cache
* Model inference state
* Conversation history
* Context position

This allows conversations to continue exactly where they left off without reprocessing the entire history.

---

### Context Injection

Load information into the model context without generating a response.

Ideal for:

* User manuals
* Documentation
* FAQs
* Knowledge bases
* Product information

Example:

```cpp
session.context_injector(
    "Context Forge is a C++ library built on top of llama.cpp."
);
```

The model can then answer questions using the injected information.

---

### High-Level Wrapper

Context Forge encapsulates much of the complexity of llama.cpp:

* Model loading
* Context management
* Chat templates
* Sampling
* History tracking
* State persistence

into a single class:

```cpp
forge::Session
```

---

### Automatic Chat Template Support

The library automatically uses the chat template embedded in the loaded model.

Compatible with:

* Qwen
* Llama
* Gemma
* Mistral
* Other GGUF models supported by llama.cpp

No manual prompt formatting required.

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

## Session Persistence

Save a session:

```cpp
session.save_state();
```

This stores:

* KV Cache
* Model state
* Conversation history

Restore it later:

```cpp
session.load_state();
```

The model will continue from the exact same point.

---

## Build Example

```bash
g++ src/context_forge.cpp example/main.cpp \
    -I llama.cpp/include \
    -L llama.cpp/build/bin \
    -lllama \
    -o example
```

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

## License

GPL-3.0
