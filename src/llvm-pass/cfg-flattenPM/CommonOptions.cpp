#include "CommonOptions.h"

llvm::cl::opt<int> NumFakeBBsOpt(
    "num-fake-bb",
    llvm::cl::desc("Specify the number of fake basic blocks to insert"),
    llvm::cl::value_desc("number"),
    llvm::cl::init(5)
);

llvm::cl::opt<int> IterationsOpt(
    "flatten-iterations",
    llvm::cl::desc("Specify the number of iterations for the flatten pass"),
    llvm::cl::init(2)
);

