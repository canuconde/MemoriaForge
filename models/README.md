# Models

This directory contains the language models used by Context Forge.

## Included Example Model

The example model used during development and testing is:

```text
qwen2.5-coder-0.5b-instruct-q4_k_m.gguf
```

### About the Model

* Model Family: Qwen 2.5 Coder
* Parameters: 0.5 Billion
* Variant: Instruct
* Quantization: Q4_K_M
* Format: GGUF

This model is optimized for:

* Source code analysis
* Code generation
* Code explanation
* Software architecture discussions
* Development assistance

### Why This Model?

The 0.5B parameter version was chosen as an example because it:

* Runs efficiently on modest hardware.
* Has low memory requirements.
* Loads quickly.
* Provides acceptable coding assistance for small and medium-sized projects.

### Performance Notes

While lightweight and fast, the 0.5B model has limitations:

* Reduced reasoning capabilities compared to larger models.
* Smaller effective working memory.
* Less accurate analysis of large codebases.
* More likely to miss architectural details.

For more demanding tasks, larger models from the Qwen Coder family may provide better results.

### Model Availability

The example model is not distributed with Context Forge.

Users must download the model separately and place it inside this directory or provide its path at runtime.

Example:

```bash
./contextforge models/qwen2.5-coder-0.5b-instruct-q4_k_m.gguf source.cpp
```

### Supported Models

Context Forge is designed to work with GGUF-compatible models supported by llama.cpp, including:

* Qwen
* Qwen Coder
* Llama
* Mistral
* Gemma
* Other GGUF models compatible with llama.cpp

### Directory Structure

Example:

```text
models/
├── qwen2.5-coder-0.5b-instruct-q4_k_m.gguf
├── qwen2.5-coder-3b-q2_k.gguf
└── other-model.gguf
```

Only one model is required at runtime.
