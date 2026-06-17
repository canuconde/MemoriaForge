CXX = g++
CXXFLAGS = -Ilibs/llama.cpp/include \
           -Ilibs/llama.cpp/ggml/include

LDFLAGS = -Llibs/llama.cpp/build/src \
          -Llibs/llama.cpp/build/ggml/src

LIBS = -lllama -lggml -lggml-base -lggml-cpu -lgomp

all: main leerarchivo

main:
	$(CXX) src/sigma_llm.cpp src/Main.cpp \
	$(CXXFLAGS) $(LDFLAGS) $(LIBS) -o main

leerarchivo:
	$(CXX) src/sigma_llm.cpp src/LeerArchivo.cpp \
	$(CXXFLAGS) $(LDFLAGS) $(LIBS) -o test

clean:
	rm -f main test