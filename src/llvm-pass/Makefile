PASS_VERSION=12
PLATFORMS:=x64
OPT_LEVEL:=O0 O1 O2 O3
PROGS:=cfg
OPT=opt-$(PASS_VERSION)
CPP=clang++-$(PASS_VERSION)
CC=clang-$(PASS_VERSION)
LLC=llc-$(PASS_VERSION)

LL_FILES=$(foreach prog, $(PROGS), $(foreach opt, $(OPT_LEVEL), $(prog).$(opt).ll))
OBJ_FILES=$(foreach prog, $(PROGS), $(foreach opt, $(OPT_LEVEL), $(prog).$(opt).o))
# SAMPLE_LL_FILES=$(foreach prog, $(PROGS), $(foreach opt, $(OPT_LEVEL), $(prog).$(opt).sample.ll))
SAMPLE_LL_FILES=$(foreach prog, $(PROGS), $(foreach opt, $(OPT_LEVEL), $(prog).$(opt).flatten.ll $(prog).$(opt).fake.ll $(prog).$(opt).harden.ll))
PASS_CPP_FILES=cfg-flattenPM/CFGFlattenPass.cpp cfg-flattenPM/FakeBBInsertionPass.cpp cfg-flattenPM/JunkCodeInsertionPass.cpp
# PASS_CPP_FILES=newPM/SamplePass.cpp

# Arch-specific
X64_SAMPLE_OBJ_FILES=$(foreach prog, $(PROGS), $(foreach opt, $(OPT_LEVEL), x64-$(prog).$(opt).o))

all: newPM/build-12/libSamplePass.so test

cfg-flattenPM/build-17/libObfuscation.so: $(PASS_CPP_FILES) | cfg-flattenPM/build-17
	cd cfg-flattenPM/build-17 && LLVM_VERSION=17 cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release && ninja

cfg-flattenPM/build-16/libObfuscation.so: $(PASS_CPP_FILES) | cfg-flattenPM/build-16
	cd cfg-flattenPM/build-16 && LLVM_VERSION=16 cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release && ninja

cfg-flattenPM/build-15/libObfuscation.so: $(PASS_CPP_FILES) | cfg-flattenPM/build-15
	cd cfg-flattenPM/build-15 && LLVM_VERSION=15 cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release && ninja

cfg-flattenPM/build-14/libObfuscation.so: $(PASS_CPP_FILES) | cfg-flattenPM/build-14
	cd cfg-flattenPM/build-14 && LLVM_VERSION=14 cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release && ninja

cfg-flattenPM/build-13/libObfuscation.so: $(PASS_CPP_FILES) | cfg-flattenPM/build-13
	cd cfg-flattenPM/build-13 && LLVM_VERSION=13 cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release && ninja

cfg-flattenPM/build-12/libObfuscation.so: $(PASS_CPP_FILES) | cfg-flattenPM/build-12
	cd cfg-flattenPM/build-12 && LLVM_VERSION=12 cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release && ninja

cfg-flattenPM/build-17:
	mkdir -p $@

cfg-flattenPM/build-16:
	mkdir -p $@

cfg-flattenPM/build-15:
	mkdir -p $@

cfg-flattenPM/build-14:
	mkdir -p $@

cfg-flattenPM/build-13:
	mkdir -p $@

cfg-flattenPM/build-12:
	mkdir -p $@

newPM/build-17/libSamplePass.so: $(PASS_CPP_FILES) | newPM/build-17
	cd newPM/build-17 && LLVM_VERSION=17 cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release && ninja

newPM/build-16/libSamplePass.so: $(PASS_CPP_FILES) | newPM/build-16
	cd newPM/build-16 && LLVM_VERSION=16 cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release && ninja

newPM/build-15/libSamplePass.so: $(PASS_CPP_FILES) | newPM/build-15
	cd newPM/build-15 && LLVM_VERSION=15 cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release && ninja

newPM/build-14/libSamplePass.so: $(PASS_CPP_FILES) | newPM/build-14
	cd newPM/build-14 && LLVM_VERSION=14 cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release && ninja

newPM/build-13/libSamplePass.so: $(PASS_CPP_FILES) | newPM/build-13
	cd newPM/build-13 && LLVM_VERSION=13 cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release && ninja

newPM/build-12/libSamplePass.so: $(PASS_CPP_FILES) | newPM/build-12
	cd newPM/build-12 && LLVM_VERSION=12 cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release && ninja

newPM/build-17:
	mkdir -p $@

newPM/build-16:
	mkdir -p $@

newPM/build-15:
	mkdir -p $@

newPM/build-14:
	mkdir -p $@

newPM/build-13:
	mkdir -p $@

newPM/build-12:
	mkdir -p $@

test: $(LL_FILES) $(SAMPLE_LL_FILES) # $(X64_SAMPLE_OBJ_FILES)

# llvm_pass_tool: cfg-flattenPM/main.cpp cfg-flattenPM/CommonOptions.cpp $(PASS_CPP_FILES) | cfg-flattenPM/build-12
# 	$(CPP) cfg-flattenPM/main.cpp cfg-flattenPM/CommonOptions.cpp $(PASS_CPP_FILES) -o cfg-flattenPM/build-12/llvm_pass_tool $(shell llvm-config-$(PASS_VERSION) --cxxflags --ldflags --system-libs --libs all)

%.O0.sample.ll: %.ll newPM/build-12/libSamplePass.so
	$(OPT) -load-pass-plugin newPM/build-12/libSamplePass.so -passes='SamplePass' $< -o $@ -S

%.O1.sample.ll: %.ll newPM/build-12/libSamplePass.so
	$(OPT) -load-pass-plugin newPM/build-12/libSamplePass.so -passes='SamplePass' $< -o $@ -S

%.O2.sample.ll: %.ll newPM/build-12/libSamplePass.so
	$(OPT) -load-pass-plugin newPM/build-12/libSamplePass.so -passes='SamplePass' $< -o $@ -S

%.O3.sample.ll: %.ll newPM/build-12/libSamplePass.so
	$(OPT) -load-pass-plugin newPM/build-12/libSamplePass.so -passes='SamplePass' $< -o $@ -S

%.O0.flatten.ll: %.ll cfg-flattenPM/build-12/libObfuscation.so
	$(OPT) -load-pass-plugin cfg-flattenPM/build-12/libObfuscation.so -passes='CFGFlattenPass' $< -o $@ -S

%.O1.flatten.ll: %.ll cfg-flattenPM/build-12/libObfuscation.so
	$(OPT) -load-pass-plugin cfg-flattenPM/build-12/libObfuscation.so -passes='CFGFlattenPass' $< -o $@ -S

%.O2.flatten.ll: %.ll cfg-flattenPM/build-12/libObfuscation.so
	$(OPT) -load-pass-plugin cfg-flattenPM/build-12/libObfuscation.so -passes='CFGFlattenPass' $< -o $@ -S

%.O3.flatten.ll: %.ll cfg-flattenPM/build-12/libObfuscation.so
	$(OPT) -load-pass-plugin cfg-flattenPM/build-12/libObfuscation.so -passes='CFGFlattenPass' $< -o $@ -S

%.O0.fake.ll: %.O0.flatten.ll cfg-flattenPM/build-12/libObfuscation.so
	$(OPT) -load-pass-plugin cfg-flattenPM/build-12/libObfuscation.so -passes='FakeBBInsertionPass' $< -o $@ -S

%.O1.fake.ll: %.O1.flatten.ll cfg-flattenPM/build-12/libObfuscation.so
	$(OPT) -load-pass-plugin cfg-flattenPM/build-12/libObfuscation.so -passes='FakeBBInsertionPass' $< -o $@ -S

%.O2.fake.ll: %.O2.flatten.ll cfg-flattenPM/build-12/libObfuscation.so
	$(OPT) -load-pass-plugin cfg-flattenPM/build-12/libObfuscation.so -passes='FakeBBInsertionPass' $< -o $@ -S

%.O3.fake.ll: %.O3.flatten.ll cfg-flattenPM/build-12/libObfuscation.so
	$(OPT) -load-pass-plugin cfg-flattenPM/build-12/libObfuscation.so -passes='FakeBBInsertionPass' $< -o $@ -S

%.O0.harden.ll: %.O0.fake.ll cfg-flattenPM/build-12/libObfuscation.so
	$(OPT) -load-pass-plugin cfg-flattenPM/build-12/libObfuscation.so -passes='JunkCodeInsertionPass' $< -o $@ -S

%.O1.harden.ll: %.O1.fake.ll cfg-flattenPM/build-12/libObfuscation.so
	$(OPT) -load-pass-plugin cfg-flattenPM/build-12/libObfuscation.so -passes='JunkCodeInsertionPass' $< -o $@ -S

%.O2.harden.ll: %.O2.fake.ll cfg-flattenPM/build-12/libObfuscation.so
	$(OPT) -load-pass-plugin cfg-flattenPM/build-12/libObfuscation.so -passes='JunkCodeInsertionPass' $< -o $@ -S

%.O3.harden.ll: %.O3.fake.ll cfg-flattenPM/build-12/libObfuscation.so
	$(OPT) -load-pass-plugin cfg-flattenPM/build-12/libObfuscation.so -passes='JunkCodeInsertionPass' $< -o $@ -S

# %.O0.harden.ll: %.O0.ll | cfg-flattenPM/build-12/llvm_pass_tool
# 	cfg-flattenPM/build-12/llvm_pass_tool -flatten-iterations=3 -num-fake-bb=20 $< -o $@

# %.O1.harden.ll: %.O1.ll | cfg-flattenPM/build-12/llvm_pass_tool
# 	cfg-flattenPM/build-12/llvm_pass_tool -flatten-iterations=3 -num-fake-bb=20 $< -o $@

# %.O2.harden.ll: %.O2.ll | cfg-flattenPM/build-12/llvm_pass_tool
# 	cfg-flattenPM/build-12/llvm_pass_tool -flatten-iterations=3 -num-fake-bb=20 $< -o $@

# %.O3.harden.ll: %.O3.ll | cfg-flattenPM/build-12/llvm_pass_tool
# 	cfg-flattenPM/build-12/llvm_pass_tool -flatten-iterations=3 -num-fake-bb=20 $< -o $@

x64-%.O0.o: %.O0.ll
	$(LLC) -O0 $^ -o $@ --filetype=obj --march=x86-64 --relocation-model=pic

x64-%.O1.o: %.O1.ll
	$(LLC) -O1 $^ -o $@ --filetype=obj --march=x86-64 --relocation-model=pic

x64-%.O2.o: %.O2.ll
	$(LLC) -O2 $^ -o $@ --filetype=obj --march=x86-64 --relocation-model=pic

x64-%.O3.o: %.O3.ll
	$(LLC) -O3 $^ -o $@ --filetype=obj --march=x86-64 --relocation-model=pic

%.ll: %.cpp
	$(CPP) -emit-llvm -S -O -o $@ $^ -g -Xclang -disable-llvm-passes

%.ll: %.c
	$(CC) -emit-llvm -S -O -o $@ $^ -g -Xclang -disable-llvm-passes

%.O0.ll: %.ll
	$(OPT) -S -o $@ $^ -O0

%.O1.ll: %.ll
	$(OPT) -S -o $@ $^ -O1

%.O2.ll: %.O0.ll
	$(OPT) -S -o $@ $^ -O2

%.O3.ll: %.O0.ll
	$(OPT) -S -o $@ $^ -O3

.PHONY: test clean

clean:
	$(RM) *.ll *.o *.bc
