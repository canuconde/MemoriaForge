CXX = g++
CXXFLAGS = -Iinclude \
	-Ilibs/llama.cpp/include \
           -Ilibs/llama.cpp/ggml/include

LDFLAGS = -Llibs/llama.cpp/build/src \
          -Llibs/llama.cpp/build/ggml/src

LIBS = -lllama -lggml -lggml-base -lggml-cpu -lgomp

all: SimpleChat FileToContext

SimpleChat:
	$(CXX) src/MemoriaForge.cpp examples/SimpleChat.cpp \
	$(CXXFLAGS) $(LDFLAGS) $(LIBS) -o build/SimpleChat

FileToContext:
	$(CXX) src/MemoriaForge.cpp examples/FileToContext.cpp \
	$(CXXFLAGS) $(LDFLAGS) $(LIBS) -o build/FileToContext

clean:
	rm -rf build/*
