# writing-llvm-pass

## Development Environment
**Let's try to use Ubuntu-22.04.**

In order to build llvm-your-own-pass you will need:
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