#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/PassManager.h"

#include "CommonOptions.h"
#include "FakeBBInsertionPass.h"
#include "CFGFlattenPass.h"
#include "JunkCodeInsertionPass.h"

using namespace llvm;

cl::opt<std::string> InputFilename(cl::Positional, cl::desc("<input IR file>"), cl::init("-"));
cl::opt<std::string> OutputFilename("o", cl::desc("Specify output filename"), cl::value_desc("filename"));

int main(int argc, char **argv) {
    InitLLVM X(argc, argv);
    cl::ParseCommandLineOptions(argc, argv, "CSEC LLVM Obfuscation Tool\n");

    LLVMContext Context;
    SMDiagnostic Err;

    // 모듈 로드
    std::unique_ptr<Module> M = parseIRFile(InputFilename, Err, Context);
    if (!M) {
        Err.print(argv[0], errs());
        return 1;
    }

    // PassManager와 FunctionAnalysisManager 설정
    FunctionPassManager FPM;
    FunctionAnalysisManager FAM;

    // 기본 분석 등록
    PassBuilder PB;
    PB.registerFunctionAnalyses(FAM);

    // Pass들을 FunctionPassManager에 추가
    FPM.addPass(CFGFlattenPass(IterationsOpt));
    FPM.addPass(FakeBBInsertionPass(NumFakeBBsOpt));
    FPM.addPass(JunkCodeInsertionPass());

    // 모듈 내 모든 함수에 대해 Pass 실행
    for (Function &F : *M) {
        FPM.run(F, FAM);
    }

    // 결과를 파일에 출력
    std::error_code EC;
    raw_fd_ostream OS(OutputFilename, EC, sys::fs::OF_None);
    if (EC) {
        errs() << "Could not open output file: " << EC.message() << "\n";
        return 1;
    }
    M->print(OS, nullptr);

    return 0;
}