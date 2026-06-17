# MemoriaForge

MemoriaForge is a lightweight C++ wrapper around llama.cpp focused on persistent sessions and context caching for local GGUF models.

The goal is simple: load a model, inject context, save the state, and restore it later without reprocessing everything from scratch.

This is especially useful for applications that work with large manuals, documentation, knowledge bases, or long conversations.

## Quick Example
```cpp
     MemoriaForge::LLMSession llm("models/qwen.gguf");

    llm.context_injector("MemoriaForge is a C++ library built on top of llama.cpp.");
    
    llm.save_state("manual_session");
```

And later...

```cpp
#include "MemoriaForge.h"

int main() {

    MemoriaForge::LLMSession llm("models/qwen.gguf");

    llm.load_state("manual_session");

    std::cout << llm.chat("What is MemoriaForge?") << std::endl;

    return 0;
}
```


---

## Why MemoriaForge?

A common issue when building local AI applications is startup time.

Every time an application starts, large documents must be tokenized and processed again before the model can answer questions about them.

MemoriaForge allows you to:

1. Load a document once.
2. Inject it into the model context.
3. Save the resulting model state.
4. Restore it instantly later.

This avoids repeatedly processing the same information and significantly reduces startup time.

---

## Features

### Persistent Sessions

Save and restore complete model sessions.

```cpp
llm.save_state("my_session");
llm.load_state("my_session");
```

Saved data includes:

- Model state
- KV cache
- Conversation history

---

### Context Injection

Load external text directly into the model context.

```cpp
std::ifstream file(Manual.txt);
std::stringstream buffer << file.rdbuf();

const std::string content = buffer.str();

llm.context_injector(content);
```

Once injected, the context can be saved and restored later.

---

### Simple Chat Interface

```cpp
std::string response = llm.chat("How do I create a customer account?");
```

No manual tokenization.

No prompt formatting.

No sampler configuration.

No direct llama.cpp API calls.

---

## Project Structure

```text
MemoriaForge/
├── build/
├── examples/
│   ├── SimpleChat.cpp
│   └── FileToContext.cpp
├── include/
│   └── MemoriaForge.h
├── src/
│   └── MemoriaForge.cpp
├── libs/
│   └── llama.cpp/
├── models/
├── makefile
└── README.md
```

---

## Requirements

- C++17
- llama.cpp
- GGUF model
- Linux

---

## Building llama.cpp

MemoriaForge depends on llama.cpp.

Follow the official build instructions from the llama.cpp repository:

https://github.com/ggml-org/llama.cpp

Static linking is supported and recommended for deployment builds.

---

## Building MemoriaForge

A Makefile is included.

Build all examples:

```bash
make
```

Generated binaries:

```text
build/SimpleChat
build/FileToContext
build/PersistentChat
```

Clean build artifacts:

```bash
make clean
```

---

## Examples

### SimpleChat

Starts an interactive chat session.

```bash
./build/PersitentChat <Path to model.gguf>
```

This example demonstrates:

- Model loading
- Interactive conversations
- Session persistence

---

### FileToContext

Loads a text file into the model context and saves the resulting state.

```bash
./build/FileToContext <Path to model.gguf> <Path to text file>
```

This example demonstrates:

- Reading external files
- Context injection
- State caching
- State restoration

---

## Basic Usage

### Create a Session

```cpp
#include "MemoriaForge.h"

MemoriaForge::LLMSession llm("models/model.gguf");
```

---

### Chat

```cpp
std::string response = llm.chat("Hello!");
```

---

### Inject Context

```cpp
std::string text = ReadFile("manual.txt");

llm.context_injector(text);
```

---

### Save State

```cpp
llm.save_state("manual_session");
```

---

### Load State

```cpp
llm.load_state("manual_session");
```

---

## How It Works

Internally, MemoriaForge uses llama.cpp to:

1. Load a GGUF model.
2. Apply the model's chat template.
3. Process user messages.
4. Inject external context into the KV cache.
5. Save model state to disk.
6. Restore previous sessions.

This allows applications to resume work immediately without rebuilding context every time they start.

---

## Current Limitations

- Single session per instance
- Manual session management
- No automatic context overflow handling
- No embeddings support
- No vector database integration
- No RAG pipeline

---


## License

GPL-3.0

This project depends on llama.cpp, which is licensed under the MIT License.
