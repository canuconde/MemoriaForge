# /* LINUX */
```bash
cmake -B build  -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=ON 

camke --build build -j$(nproc)
```
# /* WINDWOS with Mingw32 for 64bits */
```bash
cmake -B build -G Ninja -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DLLAMA_OPENSSL=OFF

cmake --build build -j$(nproc)
```
# /* WINDWOS with Mingw32 for 32bits */
```bash
cmake -B build -G Ninja -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=i686-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=i686-w64-mingw32-g++ -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DLLAMA_OPENSSL=OFF

cmake --build build -j$(nproc)
```
