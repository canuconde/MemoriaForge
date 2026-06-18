# MemoriaForge

A lightweight C++ wrapper around llama.cpp that adds persistent sessions and context caching for local GGUF models.

Instead of rebuilding large contexts every time an application starts, MemoriaForge allows applications to save and restore processed model state, reducing startup time for documentation assistants, knowledge bases, and long-running conversations.

![MemoriaForge_logo](https://github.com/canuconde/MemoriaForge/blob/main/res/banner_1280xshort.jpg?raw=true)

---

## Features

* Persistent sessions
* Context caching
* GGUF model support
* Simple chat interface
* Context injection
* Session restoration
* Built on top of llama.cpp
* Lightweight C++ API

---

## Quick Example

Load a model, inject context, and save the processed state:

```cpp
#include "MemoriaForge.h"

int main() {
    MemoriaForge::LLMSession llm("models/qwen.gguf");

    llm.context_injector("MemoriaForge is a lightweight C++ wrapper around llama.cpp.");

    llm.save_state("manual_session");

    return 0;
}
```

Later, restore the session instantly:

```cpp
#include "MemoriaForge.h"
#include <iostream>

int main() {
    MemoriaForge::LLMSession llm("models/qwen.gguf");

    llm.load_state("manual_session");

    std::cout << llm.chat("What is MemoriaForge?") << std::endl;

    return 0;
}
```

---

## Why MemoriaForge?

A common problem when building local AI applications is startup time.

Every time an application starts, large documents must be loaded, tokenized, and processed before the model can answer questions.

For large manuals, documentation, or knowledge bases, this process can become expensive and repetitive.

MemoriaForge allows applications to:

1. Load information once.
2. Inject it into the model context.
3. Save the resulting model state.
4. Restore it instantly later.

This avoids repeatedly processing the same information and significantly reduces startup time.

---

## Use Cases

* Documentation assistants
* Technical manuals
* Offline AI applications
* Knowledge base systems
* Persistent local chatbots
* Customer support tools
* Project-specific AI assistants
* Long-running conversations

---

## Requirements

* C++17
* Linux
* GGUF model
* llama.cpp

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

## Project Structure

```text
MemoriaForge/
├── build/
├── examples/
│   ├── SimpleChat.cpp
│   ├── FileToContext.cpp
│   └── PersistentChat.cpp
├── include/
│   └── MemoriaForge.h
├── src/
│   └── MemoriaForge.cpp
├── libs/
│   └── llama.cpp/
├── models/
├── FAQ.md
├── makefile
└── README.md
```

---

## Examples

### PersistentChat

Starts an interactive chat session with persistent memory.

```bash
./build/PersistentChat <path_to_model.gguf>
```

Demonstrates:

* Model loading
* Interactive conversations
* Saving sessions
* Restoring sessions

---

### FileToContext

Loads a text file into the model context and stores the resulting session.

```bash
./build/FileToContext <path_to_model.gguf> <path_to_text_file>
```

Demonstrates:

* Reading files
* Context injection
* Context caching
* Session restoration

---

## API Reference

### Create a Session

```cpp
MemoriaForge::LLMSession llm("models/model.gguf");
```

### Chat

```cpp
std::string response = llm.chat("Hello!");
```

### Inject Context

```cpp
std::string text = ReadFile("manual.txt");

llm.context_injector(text);
```

### Save State

```cpp
llm.save_state("manual_session");
```

### Load State

```cpp
llm.load_state("manual_session");
```
## Generation Parameters (NEW)

You can now configure the model’s sampling parameters directly from the wrapper.

```cpp
llm.set_generation_params(
                  0.02f,      //min_p
                  0.5f,       //temperature
                  1234        //seed
                  );
```
This allows you to control:

* min_p: minimum probability filtering
* temperature: model creativity
* seed: response reproducibility

---

## How It Works

Internally, MemoriaForge uses llama.cpp to:

1. Load a GGUF model.
2. Apply the model chat template.
3. Process user messages.
4. Inject external context.
5. Save model state.
6. Restore previous sessions.

This allows applications to resume work immediately without rebuilding context every time they start.

---

## Architecture

```text
Application
      │
      ▼
MemoriaForge
      │
      ▼
LLMSession
      │
      ▼
llama.cpp
      │
      ▼
GGUF Model
      │
      ▼
KV Cache
      │
 ┌────┴────┐
 ▼         ▼
chat()  save_state()
```

---

## Current Limitations

* Single session per instance
* Manual session management
* No automatic context overflow handling
* No embeddings support
* No vector database integration
* No RAG pipeline

---

## FAQ

For common questions, compatibility information, and implementation details, see:

```text
FAQ.md
```

---

## License

GPL-3.0

MemoriaForge depends on llama.cpp, which is distributed under the MIT License.
