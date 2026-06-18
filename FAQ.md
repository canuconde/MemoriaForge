# FAQ

Frequently Asked Questions about MemoriaForge.

---

## What is MemoriaForge?

MemoriaForge is a lightweight C++ wrapper around llama.cpp focused on persistent sessions and context caching for local GGUF models.

It allows applications to save and restore model sessions without rebuilding the entire context every time they start.

---

## Why was MemoriaForge created?

When working with large documents, manuals, or knowledge bases, the model must process the same information every time the application starts.

MemoriaForge allows that processed context to be saved and restored later, reducing startup time and avoiding repeated computation.

---

## Does MemoriaForge replace RAG?

No.

MemoriaForge and RAG solve different problems.

RAG retrieves information dynamically from external sources.

MemoriaForge restores previously processed context from a saved session.

Both approaches can be combined.

---

## Does MemoriaForge support any GGUF model?

Yes.

Any GGUF model supported by llama.cpp should work.

Model quality, speed, and memory requirements depend on the selected model.

---

## Does MemoriaForge require llama.cpp?

Yes.

MemoriaForge is built on top of llama.cpp and depends on it for model loading, inference, and state management.

---

## What does `save_state()` store?

A saved session contains:

* Model state
* KV cache
* Previously processed context
* Conversation history

This allows the session to continue later without rebuilding the context.

---

## What does `load_state()` restore?

`load_state()` restores the previously saved model state and conversation data.

After loading, the model can continue working immediately from the same context.

---

## Can I share saved sessions with other machines?

Not always.

Session compatibility generally requires:

* The same GGUF model
* The same model quantization
* A compatible llama.cpp version
* A compatible runtime environment

Loading sessions created under different conditions may fail.

---

## Does session persistence reduce RAM usage?

No.

Session persistence reduces preprocessing time.

The memory requirements of the model remain essentially the same.

---

## Does MemoriaForge make inference faster?

No.

Token generation speed remains unchanged.

MemoriaForge only avoids reprocessing previously loaded information.

---

## What is Context Injection?

Context Injection allows external text to be processed and inserted into the model context before starting a conversation.

Example:

```cpp
std::string manual = ReadFile("manual.txt");

llm.context_injector(manual);
```

This information becomes part of the session and can later be saved.

---

## Is Context Injection the same as a System Prompt?

Not exactly.

A system prompt is usually a short instruction.

Context Injection is intended for large amounts of information such as:

* Documentation
* Manuals
* Knowledge bases
* Project descriptions
* Technical references

---

## Can I load multiple documents?

Yes.

Multiple documents can be injected before saving the session.

The total amount of information is limited by the available context window.

---

## What happens if the context exceeds `n_ctx`?

The behavior depends on the underlying llama.cpp implementation and the selected model.

MemoriaForge currently does not provide automatic context overflow management.

Managing large contexts remains the responsibility of the application.

---

## Does MemoriaForge support embeddings?

No.

Embeddings are currently outside the scope of the project.

---

## Does MemoriaForge include a vector database?

No.

Vector databases are not included.

The project focuses on session persistence rather than retrieval systems.

---

## Does MemoriaForge include a RAG pipeline?

No.

Document retrieval, indexing, chunking, and vector search must be implemented separately.

---

## Does MemoriaForge support streaming output?

Support depends on the current implementation.

Check the latest API documentation and examples included with the project.

---

## Is GPU acceleration supported?

Yes.

GPU offloading is controlled through the `n_gpu_layers` parameter and depends on the capabilities provided by llama.cpp.

---

## Which operating systems are supported?

The project is currently developed and tested on Linux.

Support for other operating systems depends on llama.cpp compatibility and the build environment.

---

## Is MemoriaForge production-ready?

MemoriaForge is designed as a lightweight wrapper focused on persistent sessions.

Before production deployment, users should evaluate:

* Error handling
* Session management strategy
* Context size requirements
* Model compatibility
* Resource constraints

---

## Is MemoriaForge a replacement for llama.cpp?

No.

MemoriaForge is a convenience layer built on top of llama.cpp.

It simplifies common workflows while continuing to use llama.cpp as the inference engine.

---

## Where can I learn more about llama.cpp?

Official repository:

https://github.com/ggml-org/llama.cpp

Refer to the llama.cpp documentation for model support, hardware acceleration, build instructions, and low-level API details.
