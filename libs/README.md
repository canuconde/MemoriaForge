# Third-Party Libraries

This directory contains third-party dependencies used by Context Forge.

## llama.cpp

Context Forge uses llama.cpp as its local Large Language Model (LLM) inference backend.

Repository:

https://github.com/ggml-org/llama.cpp

### Purpose

llama.cpp provides:

* GGUF model loading
* Tokenization
* Context management
* Sampling and text generation
* State serialization
* Embedding extraction
* CPU and GPU inference backends

Context Forge builds its own abstractions on top of llama.cpp through the `LLMSession` API.

### License

llama.cpp is distributed under the MIT License.

Copyright (c) The llama.cpp contributors.

See:

```text
llama.cpp/LICENSE
```

for the complete license text.

### Modifications

At the time of writing, Context Forge does not modify the llama.cpp source code directly.

The project links against llama.cpp and uses its public API.

### Updating

To update llama.cpp:

```bash
cd libs

rm -rf llama.cpp

git clone https://github.com/ggml-org/llama.cpp.git
```

Then rebuild the project.

### Notes

* llama.cpp is maintained independently from Context Forge.
* Issues related to llama.cpp internals should be reported to the upstream project.
* Context Forge only provides a higher-level wrapper around llama.cpp functionality.
