# writing-llvm-pass

## FOR OUR PROJECT
- **Let's try to use Ubuntu-20.04.**

## Step 1: Setting up the environments
- Install the LLVM compiler, version 11 or higher-feel free to choose whichever version you want.
```bash
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
# sudo ./llvm.sh <version number> # 11, 12, 13, 14
sudo ./llvm.sh 12
```

## Step 2: Compile the pass
This repository includes a total of one pass:
- **[SamplePass](src/llvm-pass/newPM/SamplePass.cpp)**

Build the pass using the [Makefile](src/llvm-pass/Makefile)

```bash
cd src/llvm-pass
# make newPM/build-<version number>/libSample.so
make newPM/build-12/libSamplePass.so
```

## Step 3: Generate a bitcode using the LLVM pass
The built pass will likely be located at the `src/llvm-pass/newPM/build-<version>`

```bash
# Run our SamplenPass here
opt -debug-pass-manager -load-pass-plugin=src/llvm-pass/src/newPM/build-<version>/libSample.so -passes=SamplePass <input file> -o <output file>
```

## Step 4: Analyze the bitcode on your own