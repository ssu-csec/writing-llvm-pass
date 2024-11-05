#include "llvm/Passes/PassPlugin.h"
#include "CFGFlattenPass.h"
#include "JunkCodeInsertionPass.h"
#include "FakeBBInsertionPass.h"

PassPluginLibraryInfo getTrainPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "Train", LLVM_VERSION_STRING,
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
          [](StringRef Name, FunctionPassManager &FPM,
            ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "CFGFlattenPass") {
            FPM.addPass(CFGFlattenPass());
            return true;
          }
          return false;
        });
      PB.registerPipelineParsingCallback(
          [](StringRef Name, FunctionPassManager &FPM,
            ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "JunkCodeInsertionPass") {
            FPM.addPass(JunkCodeInsertionPass());
            return true;
          }
          return false;
        });
      PB.registerPipelineParsingCallback(
          [](StringRef Name, FunctionPassManager &FPM,
            ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "FakeBBInsertionPass") {
            FPM.addPass(FakeBBInsertionPass());
            return true;
          }
          return false;
        });
    }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() { return getTrainPluginInfo(); }
