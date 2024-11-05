#include "FakeBBInsertionPass.h"
#define DEBUG_TYPE "fakebbinsertionpass"

using namespace llvm;

PreservedAnalyses FakeBBInsertionPass::run(Function &F, FunctionAnalysisManager &FAM) {
    int NumFakeBBs = 5;
    // int NumFakeBBs = NumFakeBBsOpt;

    // random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 100000);

    for (BasicBlock &BB : F) {
        for (Instruction &I : BB) {
            if (auto *SI = dyn_cast<SwitchInst>(&I)) {
                LLVMContext &ctx = F.getContext();

                // 기존 switch의 모든 case 값을 수집
                std::set<int> existingCases;
                for (auto &Case : SI->cases()) {
                    existingCases.insert(Case.getCaseValue()->getSExtValue());
                }
                
                for (int i = 0; i < NumFakeBBs; i++) {
                    int randomValue;
                    do {
                        randomValue = distr(gen);
                    } while(existingCases.find(randomValue) != existingCases.end());

                    // Fake Basic Block 생성
                    BasicBlock *FakeBB = BasicBlock::Create(ctx, "fake_block", &F);

                    // Fake BB에 의미없는 명령어 삽입
                    IRBuilder<> Builder(FakeBB);
                    Builder.CreateAdd(ConstantInt::get(Type::getInt32Ty(ctx), 1), ConstantInt::get(Type::getInt32Ty(ctx), 1));
                    Builder.CreateBr(SI->getDefaultDest());

                    // Switch에 Fake BB를 새로운 case로 추가
                    SI->addCase(ConstantInt::get(Type::getInt32Ty(ctx), randomValue), FakeBB);
                }
            }
        }
    }

    return PreservedAnalyses::all();
}