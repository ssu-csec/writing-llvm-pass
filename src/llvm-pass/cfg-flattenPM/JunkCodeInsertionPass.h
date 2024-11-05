#pragma once

#define LLVM_3 (LLVM_VERSION_MAJOR == 3)
#define LLVM_4 (LLVM_VERSION_MAJOR == 4)
#define LLVM_5 (LLVM_VERSION_MAJOR == 5)
#define LLVM_6 (LLVM_VERSION_MAJOR == 6)
#define LLVM_7 (LLVM_VERSION_MAJOR == 7)
#define LLVM_8 (LLVM_VERSION_MAJOR == 8)
#define LLVM_9 (LLVM_VERSION_MAJOR == 9)
#define LLVM_10 (LLVM_VERSION_MAJOR == 10)
#define LLVM_11 (LLVM_VERSION_MAJOR == 11)
#define LLVM_12 (LLVM_VERSION_MAJOR == 12)
#define LLVM_13 (LLVM_VERSION_MAJOR == 13)
#define LLVM_14 (LLVM_VERSION_MAJOR == 14)
#define LLVM_15 (LLVM_VERSION_MAJOR == 15)
#define LLVM_16 (LLVM_VERSION_MAJOR == 16)
#define LLVM_17 (LLVM_VERSION_MAJOR == 17)
#define LLVM_18 (LLVM_VERSION_MAJOR == 18)

#define DEBUG_TYPE "junkcodeinsertionpass"

#include "llvm/ADT/Statistic.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Casting.h"

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/NoFolder.h"
#include "llvm/IR/GlobalValue.h"

#include <vector>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <string>

using namespace llvm;

namespace { }

struct JunkCodeInsertionPass : PassInfoMixin<JunkCodeInsertionPass> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
    static bool isRequired() { return true; }
};