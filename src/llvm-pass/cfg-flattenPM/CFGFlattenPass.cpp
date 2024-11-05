#include "CFGFlattenPass.h"
#define DEBUG_TYPE "cfgflattenpass"

using namespace llvm;

namespace {
    static void splitEntryBlock(BasicBlock *entryBlock) {
        Instruction *term = entryBlock->getTerminator();
        BranchInst *br = dyn_cast<BranchInst>(term);

        if (br && br->isConditional()) {
            Value *cond = br->getCondition();

            if (Instruction *inst = dyn_cast<Instruction>(cond)) { // may be a constant
                entryBlock->splitBasicBlockBefore(inst, "newEntry");
            }
        }
    }

    static std::vector<int> createRandomArray(int count) {
        int min = 0;
        int max = 100000;
        if ((max - min + 1) < count) {
            llvm::report_fatal_error("Requested unique random count exceeds the range size.");
        }

        std::vector<int> numbers;
        for (int i = min; i <= max; ++i) {
            numbers.push_back(i);
        }

        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(numbers.begin(), numbers.end(), g);

        std::vector<int> randomArray(numbers.begin(), numbers.begin() + count);
        return randomArray;
    }

    /*
        Main function to flatten the control flow.
    */
    static LoadInst *flatten(Function &F, SmallVector<LoadInst *> oldLoads) {

        /*
            2. Creation of the dispatcher block and control variable
            - Create switch variable on the stack
        */
        LLVMContext &Context = F.getContext();
        Type *i32_type = Type::getInt32Ty(Context);
        AllocaInst *switchVar = new AllocaInst(
            i32_type,
            0,
            "switchVar",
            F.getEntryBlock().getFirstNonPHI() // PHI nodes must be at the beginning
        );

        // Create switch dispatcher block
        BasicBlock *dispatcher = BasicBlock::Create(Context, "dispatcher", &F);
        LoadInst *load = new LoadInst(i32_type, switchVar, "switchVar", dispatcher);
        SwitchInst *sw = SwitchInst::Create(load, dispatcher, 0, dispatcher);

        // Move older loadInsts to the new dispatcher block to avoid scope issues
        for (auto &oldLoad : oldLoads) {
            oldLoad->moveBefore(dispatcher->getFirstNonPHI());
        }

        /*
            1. Initialization
            - Split entry block "if necessary"
        */
        splitEntryBlock(&F.getEntryBlock()); // Add all non-terminating basic blocks to the switch

        /*
            3. Mapping original basic blocks
            - Assign unique IDs to each basic block

            4. Switch Statement Construction
            - Add cases to the switch statement for each basic block
        */
        std::vector<int> randomArr = createRandomArray(F.size());
        int idx = 0;
        for (auto &B : F) {
            if (&B == &F.getEntryBlock() || &B == dispatcher || B.isLandingPad())
                continue; // Skip entry and dispatcher blocks

            // Create case variable
            ConstantInt *swIdx = dyn_cast<ConstantInt>(ConstantInt::get(i32_type, randomArr[idx++]));
            sw->addCase(swIdx, &B);
        }

        /*
            5. Modifying basic blocks
            - Redirect the control flow to the dispatcher block
            - Update branches to set switchVar conditionally
            - Unconditionally branch to the dispatcher block
        */
        for (auto &B : F) {
            ConstantInt *caseValue = sw->findCaseDest(&B);
            if (!caseValue && &B != &F.getEntryBlock())
                continue; // Skips blocks not added except entry

            Instruction *term = B.getTerminator();

            if (term->getNumSuccessors() == 0)
                continue; // Return block

            if (term->getNumSuccessors() == 1) { // Unconditional jump
                BasicBlock *succ = term->getSuccessor(0);

                ConstantInt *idx = sw->findCaseDest(succ);
                new StoreInst(idx, switchVar, term);
            }
            else if (term->getNumSuccessors() == 2) { // Conditional jump
                ConstantInt *idxTrue = sw->findCaseDest(term->getSuccessor(0));
                ConstantInt *idxFalse = sw->findCaseDest(term->getSuccessor(1));

                BranchInst *br = cast<BranchInst>(term);
                SelectInst *sl = SelectInst::Create(br->getCondition(), idxTrue, idxFalse, "", term);
                new StoreInst(sl, switchVar, term); // switchVar = cond ? idxTrue : idxFalse
            }

            BranchInst::Create(dispatcher, &B);
            term->eraseFromParent();
        }

        // maybe fix stack here?
        // fixStack(F);

        return load;
    }    
}

PreservedAnalyses CFGFlattenPass::run(Function &F, FunctionAnalysisManager &FAM) {
    LowerSwitchPass *lower = new LowerSwitchPass();
    RegToMemPass *reg = new RegToMemPass();

    /*
        1. Initialization Step
        - skip function if it is a declaration or has less than two basic blocks
    */
    LLVMContext &Context = F.getContext();
    if (F.isDeclaration() || F.size() < 2) {
        return PreservedAnalyses::all();
    }
    for (auto &BB : F) {
        if (BB.isLandingPad()) {
            return PreservedAnalyses::all();
        }
    }

    /*
        6. Updating Control Variable
        - Flatten the control flow
    */
    int Iterations = 2; // opt option으로 인식되게 수정.
    // int Iterations = IterationsOpt;
    SmallVector<LoadInst *> oldLoads;
    for (int i = 0; i < Iterations; i++) {
        lower->run(F, FAM);
        reg->run(F, FAM);
        LoadInst *load = flatten(F, oldLoads); // flatten pass works here!

        oldLoads.push_back(load);
    }
        
    return PreservedAnalyses::none();
}