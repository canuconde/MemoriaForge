CXX = g++
CXXFLAGS = -Iinclude \
	-Ilibs/llama.cpp/include \
           -Ilibs/llama.cpp/ggml/include

LDFLAGS = -Llibs/llama.cpp/build/src \
          -Llibs/llama.cpp/build/ggml/src

LIBS = -lllama -lggml -lggml-base -lggml-cpu -lgomp

PREFIX ?= /usr/local

all: SimpleChat FileToContext PersistentChat

shared:
	mkdir -p build
	$(CXX) -shared src/MemoriaForge.cpp \
	$(CXXFLAGS) $(LDFLAGS) $(LIBS) \
	-o build/libMemoriaForge.so -fPIC

shared-windows:
	mkdir -p build

	$(CXX) -shared src/MemoriaForge.cpp \
	$(CXXFLAGS) $(LDFLAGS) $(LIBS) \
	-Wl,--out-implib,build/libMemoriaForge.a \
	-o build/MemoriaForge.dll

SimpleChat:
	$(CXX) src/MemoriaForge.cpp examples/SimpleChat.cpp \
	$(CXXFLAGS) $(LDFLAGS) $(LIBS) -o build/SimpleChat

PersistentChat:
	$(CXX) src/MemoriaForge.cpp examples/PersistentChat.cpp \
	$(CXXFLAGS) $(LDFLAGS) $(LIBS) -o build/PersistentChat


FileToContext:
	$(CXX) src/MemoriaForge.cpp examples/FileToContext.cpp \
	$(CXXFLAGS) $(LDFLAGS) $(LIBS) -o build/FileToContext

clean:
	rm -rf build

install: shared
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/lib

	cp include/MemoriaForge.h $(PREFIX)/include/
	cp build/libMemoriaForge.so $(PREFIX)/lib/

	ldconfig
uninstall:
	rm -f $(PREFIX)/include/MemoriaForge.h
	rm -f $(PREFIX)/lib/libMemoriaForge.so

	ldconfig
