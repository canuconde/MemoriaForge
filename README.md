# Sigma LLM

Wrapper moderno para **llama.cpp** orientado a aplicaciones C++ que necesitan integrar modelos LLM locales de forma sencilla, con soporte para persistencia de sesiones, historial de conversación y carga de contexto dinámico.

## Características

### Wrapper de alto nivel para llama.cpp

Sigma LLM encapsula gran parte de la complejidad de la API de llama.cpp en una interfaz simple basada en sesiones.

En lugar de gestionar manualmente:

* Modelos
* Contextos
* Samplers
* Tokenización
* Historial de mensajes
* Plantillas de chat

puedes utilizar una única clase:

```cpp
sigma::LLMSession
```

---

### Persistencia de sesiones

Permite guardar y restaurar conversaciones completas.

Se almacena:

* Estado interno del modelo (KV Cache)
* Historial de mensajes
* Contexto de inferencia

Esto permite continuar una conversación sin tener que reprocesar nuevamente todo el historial.

```cpp
session.save_state("session");
session.load_state("session");
```

---

### Context Injection

Permite inyectar información en el contexto del modelo sin generar una respuesta.

Ideal para:

* Manuales de usuario
* Documentación técnica
* FAQs
* Bases de conocimiento
* Contexto empresarial

Ejemplo:

```cpp
session.context_injector(
    "Sigma ERP es un sistema de gestión empresarial..."
);
```

Posteriormente:

```cpp
session.chat("¿Qué es Sigma ERP?");
```

El modelo responderá utilizando la información previamente cargada.

---

### Compatibilidad con múltiples modelos

Sigma LLM utiliza automáticamente la plantilla de conversación definida por cada modelo.

Compatible con modelos basados en:

* Qwen
* Llama
* Gemma
* Mistral
* Otros modelos GGUF compatibles con llama.cpp

No es necesario escribir formatos especiales de prompt.

---

### Configuración de generación

Incluye un sampler preconfigurado orientado a asistentes y aplicaciones empresariales:

* Baja temperatura
* Salidas más consistentes
* Menor nivel de alucinaciones
* Respuestas más deterministas

---

## Arquitectura

```text
Usuario
   │
   ▼
LLMSession
   │
   ├── Gestión de historial
   ├── Chat Templates
   ├── Context Injection
   ├── Persistencia
   └── Generación
          │
          ▼
      llama.cpp
          │
          ▼
        GGUF
```

---

## Flujo de generación

### 1. Tokenización

El texto de entrada se convierte en tokens utilizando:

```cpp
llama_tokenize()
```

### 2. Prefill

Los tokens son procesados por el modelo:

```cpp
llama_decode()
```

### 3. Sampling

Se selecciona el siguiente token:

```cpp
llama_sampler_sample()
```

### 4. Conversión a texto

```cpp
llama_token_to_piece()
```

### 5. Finalización

La generación termina cuando se detecta:

```cpp
llama_vocab_is_eog()
```

---

## Persistencia de Estado

Cuando se ejecuta:

```cpp
session.save_state();
```

se generan dos archivos.

### state.bin

Contiene:

* KV Cache
* Estado interno del modelo
* Contexto de inferencia

Utiliza:

```cpp
llama_state_save_file()
```

---

### messages.txt

Contiene el historial completo de la conversación.

Ejemplo:

```text
user:Hola
<---MSG_END--->

assistant:Hola, ¿en qué puedo ayudarte?
<---MSG_END--->
```

---

## Restauración

```cpp
session.load_state();
```

Recupera:

* KV Cache
* Historial
* Longitud previa del contexto

permitiendo continuar la conversación exactamente desde el punto donde fue guardada.

---

## Ejemplo básico

```cpp
#include "sigma_llm.hpp"

int main() {

    sigma::LLMSession llm(
        "models/qwen2.5-0.5b-instruct-q4.gguf"
    );

    std::string respuesta =
        llm.chat("Hola");

    std::cout << respuesta << std::endl;

    return 0;
}
```

---

## Caso de uso: Asistente para Sigma ERP

```cpp
sigma::LLMSession assistant(model);

assistant.context_injector(
    manual_sigma
);

assistant.load_state();

assistant.chat(
    "¿Cómo genero una factura?"
);

assistant.save_state();
```

Beneficios:

* Respuestas basadas en documentación propia.
* Conversaciones persistentes.
* Sin dependencia de servicios externos.
* Ejecución completamente local.

---

## Dependencias

* C++17 o superior
* llama.cpp
* Modelo GGUF compatible

---

## Objetivos del Proyecto

Sigma LLM busca proporcionar:

* Una API simple para integrar LLMs locales.
* Persistencia completa de sesiones.
* Carga eficiente de conocimiento contextual.
* Base para asistentes empresariales offline.
* Integración sencilla con aplicaciones C++.

---

## Estado Actual

Implementado:

* Carga de modelos GGUF
* Chat interactivo
* Historial de mensajes
* Persistencia de sesiones
* Restauración de contexto
* Context Injection
* Soporte para chat templates

Próximamente:

* Gestión automática de contexto lleno
* Embeddings
* RAG local
* Herramientas (Tools)
* Streaming de tokens
* API HTTP opcional

---

## Licencia

GPL v3

Este proyecto utiliza llama.cpp como dependencia externa. Consulta las licencias correspondientes antes de distribuir binarios o versiones modificadas.
