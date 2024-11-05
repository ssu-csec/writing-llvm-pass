#include "JunkCodeInsertionPass.h"
#define DEBUG_TYPE "junkcodeinsertionpass"

using namespace llvm;

namespace { }

PreservedAnalyses JunkCodeInsertionPass::run(Function &F, FunctionAnalysisManager &FAM) {
    /*
        1. Initialization
        - Find the maximum basic block size among switch case dest destinations
    */
    unsigned maxBBSize = 0;
    for (auto &BB : F) {
        for (auto &I : BB) {
            if (auto *SI = dyn_cast<SwitchInst>(&I)) {
                for (auto Case : SI->cases()) {
                    BasicBlock *CaseDest = Case.getCaseSuccessor();
                    unsigned caseBBSize = CaseDest->size();
                    if (caseBBSize > maxBBSize) {
                        maxBBSize = caseBBSize;
                    }
                }
            }
        }
    }

    /*
        2. Junk Code Generation
        - Create a global variable to store random values
    */
    Module &M = *F.getParent();
    LLVMContext &ctxM = M.getContext();
    IntegerType *I32Ty = Type::getInt32Ty(ctxM);
    ConstantInt *init = ConstantInt::get(I32Ty, 0);
    GlobalVariable *global = new GlobalVariable(
        M,
        init->getType(),
        false,
        GlobalVariable::PrivateLinkage,
        init,
        "RandomVariable"
    );

    global->setAlignment(MaybeAlign(4));

    /*
        3. Basic Block Selection
        - Iterate through the basic blocks and insert junk code
    */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1,100000);

    for (auto &BB : F) {
        LLVMContext &ctxF = F.getContext();
        for (auto &I : BB) {
            if (auto *SI = dyn_cast<SwitchInst>(&I)) {
                for (auto Case : SI->cases()) {
                    BasicBlock *CaseDest = Case.getCaseSuccessor();

                    // if (CaseDest->size() == 1 && isa<BranchInst>(CaseDest->front())) {
                    //     continue;
                    // }

                    int insertPosCount = CaseDest->size();
                    if (insertPosCount > 0) {
                        int insertCount = std::max(0, (int)(maxBBSize * 2 - CaseDest->size()));
                        if (insertCount <= 0) continue;
                        else {
                            for (int i = 0; i < insertCount; ++i) {
                                int randPos = distr(gen) % insertPosCount;

                                auto iter = CaseDest->begin();
                                std::advance(iter, randPos);

                                if (iter != CaseDest->end()) {
                                    IRBuilder<NoFolder> builder(CaseDest);
                                    builder.SetInsertPoint(&*iter);

                                    /*
                                        4. Junk Code Insertion
                                        - Generate and insert random arithmetic operations
                                    */

                                    Value *Lef = ConstantInt::get(Type::getInt32Ty(ctxF), distr(gen));
                                    Value *Rig = ConstantInt::get(Type::getInt32Ty(ctxF), distr(gen));

                                    if (isa<ConstantInt>(Rig) && cast<ConstantInt>(Rig)->getValue() == 0) {
                                        Rig = ConstantInt::get(Type::getInt32Ty(ctxF), 1);
                                    }

                                    Value *Result;
                                    int choice = rand() % 7;
                                    switch (choice) {
                                        case 0: Result = builder.CreateAdd(Lef, Rig);   break;
                                        case 1: Result = builder.CreateSub(Lef, Rig);   break;
                                        case 2: Result = builder.CreateMul(Lef, Rig);   break;
                                        case 3: Result = builder.CreateUDiv(Lef, Rig);  break;
                                        case 4: Result = builder.CreateXor(Lef, Rig);   break;
                                        case 5: Result = builder.CreateNot(Lef, "not"); break;
                                        case 6: Result = builder.CreateOr(Lef, Rig);    break;
                                        default:                                        break;
                                    }

                                    if (isa<ReturnInst>(&I)) {
                                        Value *store = builder.CreateStore(Result, global, true);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return PreservedAnalyses::none();
}