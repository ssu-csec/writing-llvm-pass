# writing-llvm-pass

## Development Environment
**Let's try to use Ubuntu-22.04.**

In order to build **your-own-llvm-pass** you will need:
- LLVM 12 or higher
- C++ compiler that supports C++14
- CMake 3.13.4 or higher

In order to run the passes, you will need:
- **clang-12** (to generate input LLVM files)
- **opt-12** (to run the passes)

## Installing LLVM 12 on Ubuntu
- Install the LLVM compiler, version 12 or higher-feel free to choose whichever version you want.
- This repository is based on **LLVM-12**.
```bash
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
# sudo ./llvm.sh <version number> # 11, 12, 13, 14
sudo ./llvm.sh 12
```

## Building LLVM-12 from Sources
Building from sources can be slow and tricky to debug. **It is not necessary**, but might be your preferred way of obtaining LLVM. The following steps will work on Linux.
```bash
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
git checkout release/12.x
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD=host -DLLVM_ENABLE_PROJECTS=clang <llvm-project/root/dir>/llvm/
cmake --build .
```
For more details read the [official documentation](https://llvm.org/docs/CMake.html).

# LLVM Toolchain Commands
This section describes how to use LLVM toolchain commands to work with `.c` source files, `.ll` LLVM IR code, and `.bc` LLVM bitcode. Optimization levels `-O0`, `-O1`, and `-O2` can be applied depending on the required stage of optimization.

## Generate LLVM IR (.ll) from a C Source Code
To compile a `.c` source file into LLVM IR (.ll) with a specified optimization level, use:

```bash
clang -S -emit-llvm -O0 <sample.c> -o <sample.ll>
```

Replace -O0 with -O1 or -O2 to apply higher levels of optimization.

## Convert LLVM IR (.ll) to LLVM Bitcode (.bc)
To convert an LLVM IR file (.ll) to LLVM bitcode (.bc):
```bash
llvm-as <sample.ll> -o <sample.bc>
```

## Convert LLVM Bitcode (.bc) to LLVM IR (.ll)
To revert an LLVM bitcode file (.bc) back to LLVM IR (.ll):
```bash
llvm-dis <sample.bc> -o <sample.ll>
```

## Generate Control Flow Graph (CFG) PDF from LLVM IR
To visualize the control flow graph, follow these steps:
1. Install **Graphviz** (if not already installed):
    ```bash
    sudo apt install graphviz
    ```
2. Generate **.dot** Files for Functions in the LLVM IR:

    Use the **opt** command to create **.dot** files for each function’s CFG:
    ```bash
    opt -dot-cfg -enable-new-pm=0 <sample.ll>
    ```
    This command generates files like **./.func1.dot**, **./.func2.dot**, and **./.func3.dot**.
3. Convert **.dot** Files to PDF:
    Convert each .dot file to a PDF:
    ```bash
    dot -Tpdf <./.func1.dot> -o <func.pdf>
    ```
    This command will output a PDF visualizing the control flow for the function specified in the .dot file.

# Building & Testing
## Building
This repository includes a total of one pass:
- **[SamplePass](src/llvm-pass/newPM/SamplePass.cpp)**

You can build llvm-pass (and all the provided pass plugins) as follows:
```bash
cd <your-own-llvm-pass-workingdir>
mkdir build && cd build
cmake ..
make
```

## Testing
In order to run <your-own-llvm-pass>:
```bash
# Run our SamplenPass here
opt -debug-pass-manager -load-pass-plugin=src/llvm-pass/src/newPM/build-<version>/libSample.so -passes=SamplePass <input file> -o <output file>
```


## Build and Test using Makefile
To build and test the llvm-pass, use the [Makefile](src/llvm-pass/Makefile)

```bash
cd src/llvm-pass
# make newPM/build-<version number>/libSample.so
make newPM/build-12/libSamplePass.so
```

One the build is complete, you can test the project with:
```bash
make test
```

This command will first build the project and then run the tests automatically.
```bash
make all
```