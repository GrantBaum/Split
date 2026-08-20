# Split
A coherent and simple language that can be used for ultra specific file parsing (mainly for plaintext, txt, md, xml, csv) without huge blocks of regular code. split reads a data file, splits each row into fields according to rules you define, filters and transforms the rows, and prints results. The parsing rules are part of the language, so awkward formats are described rather than worked around.

Requirements
A C++17 compiler (g++ 9+ or clang 10+)
CMake 3.16 or newer

On Debian/Ubuntu (including WSL):

bash
sudo apt update
sudo apt install cmake build-essential

Verify:

bash
cmake --version
g++ --version
Building

From the project root (the folder containing CMakeLists.txt):

bash
cmake -B build          # configure — only needed when CMakeLists.txt changes
cmake --build build     # compile — run this after every code edit

The binary lands at build/splt.

Running
bash
./build/splt examples/test.split

Right now this prints the token stream produced by the lexer — one line per token, showing its type and text.

Rebuilding from scratch

Not usually necessary; CMake recompiles only what changed. If the build gets into a confused state (files moved, CMakeLists restructured):

bash
rm -rf build
cmake -B build
cmake --build build
